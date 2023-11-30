/*
 *  Copyright 2023 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include "HnRenderPass.hpp"
#include "HnRenderPassState.hpp"
#include "HnRenderDelegate.hpp"
#include "HnMesh.hpp"
#include "HnMaterial.hpp"
#include "HnDrawItem.hpp"
#include "HnTypeConversions.hpp"
#include "HnRenderParam.hpp"

#include <array>

#include "pxr/imaging/hd/renderIndex.h"

#include "USD_Renderer.hpp"
#include "MapHelper.hpp"

namespace Diligent
{

namespace HLSL
{

#include "Shaders/Common/public/BasicStructures.fxh"
#include "Shaders/PBR/public/PBR_Structures.fxh"
#include "Shaders/PBR/private/RenderPBR_Structures.fxh"

} // namespace HLSL

namespace USD
{

pxr::HdRenderPassSharedPtr HnRenderPass::Create(pxr::HdRenderIndex*           pIndex,
                                                const pxr::HdRprimCollection& Collection)
{
    return pxr::HdRenderPassSharedPtr{new HnRenderPass{pIndex, Collection}};
}

HnRenderPass::HnRenderPass(pxr::HdRenderIndex*           pIndex,
                           const pxr::HdRprimCollection& Collection) :
    pxr::HdRenderPass{pIndex, Collection}
{
}

struct HnRenderPass::RenderState
{
    const HnRenderPass&       RenderPass;
    const HnRenderPassState&  RPState;
    const pxr::HdRenderIndex& RenderIndex;
    const HnRenderDelegate&   RenderDelegate;
    USD_Renderer&             USDRenderer;

    IDeviceContext* const pCtx;

    const USD_Renderer::ALPHA_MODE AlphaMode;

    const Uint32 PrimitiveAttribsAlignedOffset;

    RenderState(const HnRenderPass&      _RenderPass,
                const HnRenderPassState& _RPState) :
        RenderPass{_RenderPass},
        RPState{_RPState},
        RenderIndex{*RenderPass.GetRenderIndex()},
        RenderDelegate{*static_cast<HnRenderDelegate*>(RenderIndex.GetRenderDelegate())},
        USDRenderer{*RenderDelegate.GetUSDRenderer()},
        pCtx{RenderDelegate.GetDeviceContext()},
        AlphaMode{MaterialTagToPbrAlphaMode(RenderPass.m_MaterialTag)},
        PrimitiveAttribsAlignedOffset{RenderDelegate.GetPrimitiveAttribsAlignedOffset()}
    {
    }

    void SetPipelineState(IPipelineState* pNewPSO)
    {
        VERIFY_EXPR(pNewPSO != nullptr);
        if (pNewPSO == nullptr || pNewPSO == pPSO)
            return;

        pCtx->SetPipelineState(pNewPSO);
        pPSO = pNewPSO;
    }

    void CommitShaderResources(IShaderResourceBinding* pNewSRB)
    {
        VERIFY_EXPR(pNewSRB != nullptr);
        if (pNewSRB == nullptr || pNewSRB == this->pSRB)
            return;

        pCtx->CommitShaderResources(pNewSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        pSRB = pNewSRB;
    }

    void SetIndexBuffer(IBuffer* pNewIndexBuffer)
    {
        if (pNewIndexBuffer == nullptr || pNewIndexBuffer == pIndexBuffer)
            return;

        pIndexBuffer = pNewIndexBuffer;
        pCtx->SetIndexBuffer(pIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void SetVertexBuffers(IBuffer* const* ppBuffers, Uint32 NumBuffers)
    {
        VERIFY_EXPR(NumBuffers <= 4);
        bool SetBuffers = false;
        for (Uint32 i = 0; i < NumBuffers; ++i)
        {
            if (ppBuffers[i] != nullptr && ppBuffers[i] != ppVertexBuffers[i])
            {
                SetBuffers         = true;
                ppVertexBuffers[i] = ppBuffers[i];
            }
        }

        if (SetBuffers)
        {
            pCtx->SetVertexBuffers(0, NumBuffers, ppBuffers, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        }
    }

    USD_Renderer::PsoCacheAccessor& GePsoCache()
    {
        if (!PsoCache)
        {
            GraphicsPipelineDesc GraphicsDesc = RenderPass.GetGraphicsDesc(RPState);

            PsoCache = USDRenderer.GetPsoCacheAccessor(GraphicsDesc);
            VERIFY_EXPR(PsoCache);
        }

        return PsoCache;
    }

private:
    IPipelineState*         pPSO = nullptr;
    IShaderResourceBinding* pSRB = nullptr;

    IBuffer*                pIndexBuffer    = nullptr;
    std::array<IBuffer*, 4> ppVertexBuffers = {};

    USD_Renderer::PsoCacheAccessor PsoCache;
};

GraphicsPipelineDesc HnRenderPass::GetGraphicsDesc(const HnRenderPassState& RPState) const
{
    GraphicsPipelineDesc GraphicsDesc = RPState.GetGraphicsPipelineDesc();
    if ((m_Params.UsdPsoFlags & USD_Renderer::USD_PSO_FLAG_ENABLE_ALL_OUTPUTS) == 0)
    {
        for (Uint32 i = 0; i < GraphicsDesc.NumRenderTargets; ++i)
            GraphicsDesc.RTVFormats[i] = TEX_FORMAT_UNKNOWN;
        GraphicsDesc.NumRenderTargets = 0;
    }

    switch (m_RenderParams.RenderMode)
    {
        case HN_RENDER_MODE_SOLID:
            GraphicsDesc.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            break;

        case HN_RENDER_MODE_MESH_EDGES:
            GraphicsDesc.PrimitiveTopology = PRIMITIVE_TOPOLOGY_LINE_LIST;
            break;

        case HN_RENDER_MODE_POINTS:
            GraphicsDesc.PrimitiveTopology = PRIMITIVE_TOPOLOGY_POINT_LIST;
            break;

        default:
            UNEXPECTED("Unexpected render mode");
    }
    static_assert(HN_RENDER_MODE_COUNT == 3, "Please handle the new render mode in the switch above");

    return GraphicsDesc;
}

void HnRenderPass::_Execute(const pxr::HdRenderPassStateSharedPtr& RPState,
                            const pxr::TfTokenVector&              Tags)
{
    UpdateDrawList(Tags);
    if (m_DrawList.empty())
        return;

    // Render pass state is initialized by HnBeginFrameTask, and
    // passed from the render Rprims task.
    if (!RPState)
    {
        UNEXPECTED("Render pass state should not be null");
        return;
    }

    RenderState State{*this, *static_cast<const HnRenderPassState*>(RPState.get())};

    IBuffer* const pPrimitiveAttribsCB = State.RenderDelegate.GetPrimitiveAttribsCB();
    VERIFY_EXPR(pPrimitiveAttribsCB != nullptr);

    const auto& Desc                 = pPrimitiveAttribsCB->GetDesc();
    const auto  MaxDrawItemsInBuffer = Desc.Size / State.PrimitiveAttribsAlignedOffset;
    const auto  NumDrawItems         = m_DrawList.size();

    m_PendingDrawItems.clear();
    HLSL::PBRPrimitiveAttribs* pCurrPrimitive = nullptr;
    for (size_t DrawItemIdx = 0; DrawItemIdx < NumDrawItems; ++DrawItemIdx)
    {
        DrawListItem&     ListItem = m_DrawList[DrawItemIdx];
        const HnDrawItem& DrawItem = ListItem.DrawItem;

        const HnMesh&     Mesh      = DrawItem.GetMesh();
        const HnMaterial* pMaterial = DrawItem.GetMaterial();
        if (pMaterial == nullptr)
            continue;

        auto DrawItemGPUResDirtyFlags = m_DrawListItemsDirtyFlags;
        if (ListItem.Version != Mesh.GetVersion())
            DrawItemGPUResDirtyFlags |= DRAW_LIST_ITEM_DIRTY_FLAG_PSO | DRAW_LIST_ITEM_DIRTY_FLAG_MESH_DATA;
        if (DrawItemGPUResDirtyFlags != DRAW_LIST_ITEM_DIRTY_FLAG_NONE)
        {
            UpdateDrawListItemGPUResources(ListItem, State, DrawItemGPUResDirtyFlags);
        }

        // Make sure we update all items before skipping any since we will clear the
        // m_DrawListItemsDirtyFlags at the end of the function.
        if (!ListItem || !DrawItem.GetVisible())
            continue;

        if (pCurrPrimitive == nullptr)
        {
            State.pCtx->MapBuffer(pPrimitiveAttribsCB, MAP_WRITE, MAP_FLAG_DISCARD, reinterpret_cast<PVoid&>(pCurrPrimitive));
            if (pCurrPrimitive == nullptr)
            {
                UNEXPECTED("Failed to map the primitive attributes buffer");
                break;
            }
        }

        // Write current primitive attributes
        float4 CustomData{
            static_cast<float>(Mesh.GetUID()),
            m_Params.Selection == HnRenderPassParams::SelectionType::Selected ? 1.f : 0.f,
            0,
            0,
        };

        State.USDRenderer.WritePBRPrimitiveShaderAttribs(
            pCurrPrimitive,
            Mesh.GetTransform() * m_RenderParams.Transform,
            0,
            pMaterial->GetBasicShaderAttribs(),
            pMaterial->GetShaderTextureAttribs(),
            pMaterial->GetNumShaderTextureAttribs(),
            &CustomData,
            sizeof(CustomData));

        if (DrawItem.GetIsFallbackMaterial())
        {
            pCurrPrimitive->Material.Basic.BaseColorFactor = Mesh.GetDisplayColor();
        }

        pCurrPrimitive = reinterpret_cast<HLSL::PBRPrimitiveAttribs*>(reinterpret_cast<Uint8*>(pCurrPrimitive) + State.PrimitiveAttribsAlignedOffset);
        m_PendingDrawItems.push_back(&ListItem);

        if (m_PendingDrawItems.size() == MaxDrawItemsInBuffer)
        {
            // Either the buffer is full. Render the pending items and start
            // filling the buffer from the beginning.
            State.pCtx->UnmapBuffer(pPrimitiveAttribsCB, MAP_WRITE);
            pCurrPrimitive = nullptr;
            RenderPendingDrawItems(State);
            VERIFY_EXPR(m_PendingDrawItems.empty());
        }
    }
    if (pCurrPrimitive != nullptr)
    {
        State.pCtx->UnmapBuffer(pPrimitiveAttribsCB, MAP_WRITE);
        RenderPendingDrawItems(State);
    }

    m_DrawListItemsDirtyFlags = DRAW_LIST_ITEM_DIRTY_FLAG_NONE;
}

void HnRenderPass::_MarkCollectionDirty()
{
    // Force any cached data based on collection to be refreshed.
    m_CollectionVersion = ~0u;
}

void HnRenderPass::SetMeshRenderParams(const HnMeshRenderParams& Params)
{
    if (m_RenderParams.SelectedPrimId != Params.SelectedPrimId)
        _MarkCollectionDirty();

    if (m_RenderParams.RenderMode != Params.RenderMode)
        m_DrawListItemsDirtyFlags |= DRAW_LIST_ITEM_DIRTY_FLAG_PSO | DRAW_LIST_ITEM_DIRTY_FLAG_MESH_DATA;

    if (m_RenderParams.DebugViewMode != Params.DebugViewMode)
        m_DrawListItemsDirtyFlags |= DRAW_LIST_ITEM_DIRTY_FLAG_PSO;

    m_RenderParams = Params;
}

void HnRenderPass::SetParams(const HnRenderPassParams& Params)
{
    if (m_Params.UsdPsoFlags != Params.UsdPsoFlags)
        m_DrawListItemsDirtyFlags |= DRAW_LIST_ITEM_DIRTY_FLAG_PSO;

    m_Params = Params;
}

void HnRenderPass::UpdateDrawList(const pxr::TfTokenVector& RenderTags)
{
    pxr::HdRenderIndex* pRenderIndex = GetRenderIndex();
    //HnRenderDelegate*   pRenderDelegate = static_cast<HnRenderDelegate*>(pRenderIndex->GetRenderDelegate());

    const pxr::HdRprimCollection& Collection  = GetRprimCollection();
    const pxr::HdChangeTracker&   Tracker     = pRenderIndex->GetChangeTracker();
    const pxr::TfToken&           MaterialTag = Collection.GetMaterialTag();

    const unsigned int CollectionVersion     = Tracker.GetCollectionVersion(Collection.GetName());
    const unsigned int RprimRenderTagVersion = Tracker.GetRenderTagVersion();
    const unsigned int TaskRenderTagsVersion = Tracker.GetTaskRenderTagsVersion();
    //const unsigned int GeomSubsetDrawItemsVersion = GetGeomSubsetDrawItemsVersion(pRenderIndex);

    const bool CollectionChanged     = (m_CollectionVersion != CollectionVersion);
    const bool RprimRenderTagChanged = (m_RprimRenderTagVersion != RprimRenderTagVersion);
    const bool MaterialTagChanged    = (m_MaterialTag != MaterialTag);
    //const bool GeomSubsetDrawItemsChanged = (m_GeomSubsetDrawItemsVersion != GeomSubsetDrawItemsVersion);

    bool TaskRenderTagsChanged = false;
    if (m_TaskRenderTagsVersion != TaskRenderTagsVersion)
    {
        m_TaskRenderTagsVersion = TaskRenderTagsVersion;
        if (m_RenderTags != RenderTags)
        {
            m_RenderTags          = RenderTags;
            TaskRenderTagsChanged = true;
        }
    }

    if (CollectionChanged ||
        RprimRenderTagChanged ||
        MaterialTagChanged ||
        //GeomSubsetDrawItemsChanged ||
        TaskRenderTagsChanged)
    {
        m_DrawList.clear();
        //const HnRenderParam* const RenderParam = static_cast<HnRenderParam*>(pRenderIndex->GetRenderDelegate()->GetRenderParam());
        //if (RenderParam->HasMaterialTag(Collection.GetMaterialTag()))
        {
            pxr::HdRenderIndex::HdDrawItemPtrVector DrawItems = GetRenderIndex()->GetDrawItems(Collection, RenderTags);
            // GetDrawItems() uses multithreading, so the order of draw items is not deterministic.
            std::sort(DrawItems.begin(), DrawItems.end());

            for (const pxr::HdDrawItem* pDrawItem : DrawItems)
            {
                if (pDrawItem == nullptr)
                    continue;

                const pxr::SdfPath& RPrimID = pDrawItem->GetRprimID();
                const pxr::HdRprim* pRPrim  = pRenderIndex->GetRprim(RPrimID);
                if (pRPrim == nullptr)
                    continue;

                const bool IsSelected = RPrimID.HasPrefix(m_RenderParams.SelectedPrimId);
                if ((m_Params.Selection == HnRenderPassParams::SelectionType::All) ||
                    (m_Params.Selection == HnRenderPassParams::SelectionType::Selected && IsSelected) ||
                    (m_Params.Selection == HnRenderPassParams::SelectionType::Unselected && !IsSelected))
                {
                    const HnDrawItem& DrawItem = static_cast<const HnDrawItem&>(*pDrawItem);
                    if (DrawItem.IsValid())
                        m_DrawList.push_back(DrawListItem{DrawItem});
                }
            }

            m_DrawListItemsDirtyFlags = DRAW_LIST_ITEM_DIRTY_FLAG_ALL;
        }
        //else
        //{
        //    // There is no prim with the desired material tag.
        //    m_DrawItems.clear()
        //}
    }

    m_CollectionVersion     = CollectionVersion;
    m_RprimRenderTagVersion = RprimRenderTagVersion;
    m_MaterialTag           = MaterialTag;
}

HnRenderPass::SupportedVertexInputsSetType HnRenderPass::GetSupportedVertexInputs(const HnMaterial* Material)
{
    SupportedVertexInputsSetType SupportedInputs{{pxr::HdTokens->points, pxr::HdTokens->normals}};
    if (Material != nullptr)
    {
        const auto& TexCoordSets = Material->GetTextureCoordinateSets();
        for (const auto& TexCoordSet : TexCoordSets)
        {
            if (!TexCoordSet.PrimVarName.IsEmpty())
                SupportedInputs.emplace(TexCoordSet.PrimVarName);
        }
    }

    return SupportedInputs;
}

void HnRenderPass::UpdateDrawListItemGPUResources(DrawListItem& ListItem, RenderState& State, DRAW_LIST_ITEM_DIRTY_FLAGS DirtyFlags)
{
    const HnDrawItem& DrawItem = ListItem.DrawItem;
    if (DirtyFlags & DRAW_LIST_ITEM_DIRTY_FLAG_PSO)
    {
        ListItem.pPSO = nullptr;

        auto& PsoCache = State.GePsoCache();
        VERIFY_EXPR(PsoCache);

        auto PSOFlags = static_cast<PBR_Renderer::PSO_FLAGS>(m_Params.UsdPsoFlags);

        const auto& Geo = DrawItem.GetGeometryData();

        if (m_RenderParams.RenderMode == HN_RENDER_MODE_SOLID)
        {
            if (Geo.Normals != nullptr && (m_Params.UsdPsoFlags & USD_Renderer::USD_PSO_FLAG_ENABLE_COLOR_OUTPUT) != 0)
                PSOFlags |= PBR_Renderer::PSO_FLAG_USE_VERTEX_NORMALS;
            if (Geo.TexCoords[0] != nullptr)
                PSOFlags |= PBR_Renderer::PSO_FLAG_USE_TEXCOORD0;
            if (Geo.TexCoords[1] != nullptr)
                PSOFlags |= PBR_Renderer::PSO_FLAG_USE_TEXCOORD1;

            const HnMaterial* pMaterial = DrawItem.GetMaterial();
            if (pMaterial != nullptr)
            {
                for (Uint32 i = 0; i < pMaterial->GetNumShaderTextureAttribs(); ++i)
                {
                    const auto& TexAttrib = pMaterial->GetShaderTextureAttrib(i);
                    if (TexAttrib.UVScaleAndRotation != float4{1, 0, 0, 1} ||
                        TexAttrib.UBias != 0 ||
                        TexAttrib.VBias != 0)
                    {
                        PSOFlags |= PBR_Renderer::PSO_FLAG_ENABLE_TEXCOORD_TRANSFORM;
                        break;
                    }
                }
            }

            PSOFlags |= PBR_Renderer::PSO_FLAG_USE_COLOR_MAP;
            if (m_Params.UsdPsoFlags & USD_Renderer::USD_PSO_FLAG_ENABLE_COLOR_OUTPUT)
            {
                PSOFlags |=
                    PBR_Renderer::PSO_FLAG_USE_NORMAL_MAP |
                    PBR_Renderer::PSO_FLAG_USE_METALLIC_MAP |
                    PBR_Renderer::PSO_FLAG_USE_ROUGHNESS_MAP |
                    PBR_Renderer::PSO_FLAG_USE_AO_MAP |
                    PBR_Renderer::PSO_FLAG_USE_EMISSIVE_MAP |
                    PBR_Renderer::PSO_FLAG_USE_IBL;
            }

            if (static_cast<const HnRenderParam*>(State.RenderDelegate.GetRenderParam())->GetUseTextureAtlas())
                PSOFlags |= PBR_Renderer::PSO_FLAG_USE_TEXTURE_ATLAS;

            VERIFY(pMaterial->GetBasicShaderAttribs().AlphaMode == State.AlphaMode || DrawItem.GetIsFallbackMaterial(),
                   "Alpha mode derived from the material tag is not consistent with the alpha mode in the shader attributes. "
                   "This may indicate an issue in how alpha mode is determined in the material, or (less likely) an issue in Rprim sorting by Hydra.");
            ListItem.pPSO = PsoCache.Get({PSOFlags, static_cast<PBR_Renderer::ALPHA_MODE>(State.AlphaMode), /*DoubleSided = */ false, static_cast<PBR_Renderer::DebugViewType>(m_RenderParams.DebugViewMode)}, true);
        }
        else if (m_RenderParams.RenderMode == HN_RENDER_MODE_MESH_EDGES ||
                 m_RenderParams.RenderMode == HN_RENDER_MODE_POINTS)
        {
            PSOFlags |= PBR_Renderer::PSO_FLAG_UNSHADED;
            ListItem.pPSO = PsoCache.Get({PSOFlags, /*DoubleSided = */ false, static_cast<PBR_Renderer::DebugViewType>(m_RenderParams.DebugViewMode)}, true);
        }
        else
        {
            UNEXPECTED("Unexpected render mode");
        }

        VERIFY_EXPR(ListItem.pPSO != nullptr);
    }

    if (DirtyFlags & DRAW_LIST_ITEM_DIRTY_FLAG_MESH_DATA)
    {
        const HnDrawItem::GeometryData& Geo = DrawItem.GetGeometryData();

        ListItem.VertexBuffers = {Geo.Positions, Geo.Normals, Geo.TexCoords[0], Geo.TexCoords[1]};

        const HnDrawItem::TopologyData* Topology = nullptr;
        switch (m_RenderParams.RenderMode)
        {
            case HN_RENDER_MODE_SOLID:
                Topology                  = &DrawItem.GetFaces();
                ListItem.NumVertexBuffers = 4;
                break;

            case HN_RENDER_MODE_MESH_EDGES:
                Topology                  = &DrawItem.GetEdges();
                ListItem.NumVertexBuffers = 1; // Only positions are used
                break;

            case HN_RENDER_MODE_POINTS:
                Topology                  = &DrawItem.GetPoints();
                ListItem.NumVertexBuffers = 1; // Only positions are used
                break;

            default:
                UNEXPECTED("Unexpected render mode");
        }

        if (Topology != nullptr)
        {
            ListItem.IndexBuffer = Topology->IndexBuffer;
            ListItem.StartIndex  = Topology->StartIndex;
            ListItem.NumVertices = Topology->NumVertices;
        }
        else
        {
            ListItem.IndexBuffer = nullptr;
            ListItem.StartIndex  = 0;
            ListItem.NumVertices = 0;
        }
    }

    ListItem.Version = DrawItem.GetMesh().GetVersion();
}

void HnRenderPass::RenderPendingDrawItems(RenderState& State)
{
    for (size_t i = 0; i < m_PendingDrawItems.size(); ++i)
    {
        const DrawListItem& ListItem = *m_PendingDrawItems[i];
        const HnDrawItem&   DrawItem = ListItem.DrawItem;

        State.SetPipelineState(ListItem.pPSO);

        const HnDrawItem::GeometryData& Geo = DrawItem.GetGeometryData();

        IShaderResourceBinding* pSRB = DrawItem.GetMaterial()->GetSRB(static_cast<Uint32>(i * State.PrimitiveAttribsAlignedOffset));
        VERIFY(pSRB != nullptr, "Material SRB is null. This may happen if UpdateSRB was not called for this material.");
        State.CommitShaderResources(pSRB);

        State.SetIndexBuffer(ListItem.IndexBuffer);
        State.SetVertexBuffers(ListItem.VertexBuffers.data(), ListItem.NumVertexBuffers);

        if (ListItem.IndexBuffer != nullptr)
        {
            constexpr Uint32 NumInstances = 1;
            State.pCtx->DrawIndexed({ListItem.NumVertices, VT_UINT32, DRAW_FLAG_VERIFY_ALL, NumInstances, ListItem.StartIndex});
        }
        else
        {
            State.pCtx->Draw({ListItem.NumVertices, DRAW_FLAG_VERIFY_ALL});
        }
    }

    m_PendingDrawItems.clear();
}

} // namespace USD

} // namespace Diligent
