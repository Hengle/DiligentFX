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

#pragma once

#include <array>

#include "pxr/imaging/hd/types.h"
#include "pxr/imaging/hd/drawItem.h"

#include "Buffer.h"
#include "PipelineState.h"
#include "ShaderResourceBinding.h"
#include "RefCntAutoPtr.hpp"

namespace Diligent
{

namespace USD
{

class HnMesh;
class HnMaterial;

class HnDrawItem final
{
public:
    explicit HnDrawItem(const pxr::HdDrawItem& HdDrawItem, const HnMesh& Mesh) noexcept;
    ~HnDrawItem();

    const pxr::HdDrawItem& GetHdDrawItem() const { return m_HdDrawItem; }
    const HnMesh&          GetMesh() const { return m_Mesh; }

    struct GeometryData
    {
        GeometryData() noexcept {}
        GeometryData(const HnMaterial& _Material, bool _IsFallbackMaterial) noexcept :
            pMaterial{&_Material},
            IsFallbackMaterial{_IsFallbackMaterial}
        {}

        constexpr explicit operator bool() const
        {
            return pMaterial != nullptr;
        }

        const HnMaterial* pMaterial          = nullptr;
        bool              IsFallbackMaterial = false;

        RefCntAutoPtr<IBuffer> Positions;
        RefCntAutoPtr<IBuffer> Normals;

        std::array<RefCntAutoPtr<IBuffer>, 2> TexCoords;
    };

    void SetGeometryData(GeometryData&& Data) { m_GeometryData = std::move(Data); }
    void SetPSO(IPipelineState* pPSO) { m_PSO = pPSO; }

    const GeometryData& GetGeometryData() const { return m_GeometryData; }
    IPipelineState*     GetPSO() const { return m_PSO; }

    bool IsValid() const { return m_PSO; }

private:
    const pxr::HdDrawItem& m_HdDrawItem;
    const HnMesh&          m_Mesh;

    GeometryData                  m_GeometryData;
    RefCntAutoPtr<IPipelineState> m_PSO;
};

} // namespace USD

} // namespace Diligent
