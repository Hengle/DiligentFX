"// PBR shader based on the Khronos WebGL PBR implementation\n"
"// See https://github.com/KhronosGroup/glTF-WebGL-PBR\n"
"// Supports both metallic roughness and specular glossiness inputs\n"
"\n"
"#include \"BasicStructures.fxh\"\n"
"#include \"PBR_Shading.fxh\"\n"
"#include \"RenderPBR_Structures.fxh\"\n"
"\n"
"#if ENABLE_TONE_MAPPING\n"
"#   include \"ToneMapping.fxh\"\n"
"#endif\n"
"\n"
"#if ENABLE_IRIDESCENCE\n"
"#   include \"Iridescence.fxh\"\n"
"#endif\n"
"\n"
"#include \"VSOutputStruct.generated\"\n"
"// struct VSOutput\n"
"// {\n"
"//     float4 ClipPos  : SV_Position;\n"
"//     float3 WorldPos : WORLD_POS;\n"
"//     float4 Color    : COLOR;\n"
"//     float3 Normal   : NORMAL;\n"
"//     float2 UV0      : UV0;\n"
"//     float2 UV1      : UV1;\n"
"// };\n"
"\n"
"#ifndef USE_TEXTURE_ATLAS\n"
"#   define USE_TEXTURE_ATLAS 0\n"
"#endif\n"
"\n"
"#ifndef ALLOW_DEBUG_VIEW\n"
"#   define ALLOW_DEBUG_VIEW 0\n"
"#endif\n"
"\n"
"#include \"PBR_Textures.fxh\"\n"
"\n"
"cbuffer cbFrameAttribs\n"
"{\n"
"    PBRFrameAttribs g_Frame;\n"
"}\n"
"\n"
"cbuffer cbPrimitiveAttribs\n"
"{\n"
"    PBRPrimitiveAttribs g_Primitive;\n"
"}\n"
"\n"
"struct NormalMapUVInfo\n"
"{\n"
"    float  Selector;\n"
"    float2 UV;\n"
"    float2 SmoothUV;\n"
"    float2 dUV_dx;\n"
"    float2 dUV_dy;\n"
"};\n"
"\n"
"NormalMapUVInfo GetNormalMapUVInfo(in VSOutput VSOut, in PBRMaterialTextureAttribs TexAttribs)\n"
"{\n"
"    NormalMapUVInfo UVInfo;\n"
"\n"
"    UVInfo.Selector = TexAttribs.UVSelector;\n"
"    UVInfo.UV       = SelectUV(VSOut, TexAttribs.UVSelector);\n"
"\n"
"#if ENABLE_TEXCOORD_TRANSFORM\n"
"    UVInfo.UV = TransformUV(UVInfo.UV, TexAttribs);\n"
"#endif\n"
"\n"
"    UVInfo.SmoothUV = UVInfo.UV;\n"
"    UVInfo.dUV_dx   = ddx(UVInfo.UV);\n"
"    UVInfo.dUV_dy   = ddy(UVInfo.UV);\n"
"#if USE_TEXTURE_ATLAS\n"
"    {\n"
"        UVInfo.UV = frac(UVInfo.UV) * TexAttribs.AtlasUVScaleAndBias.xy + TexAttribs.AtlasUVScaleAndBias.zw;\n"
"        UVInfo.SmoothUV *= TexAttribs.AtlasUVScaleAndBias.xy;\n"
"        UVInfo.dUV_dx   *= TexAttribs.AtlasUVScaleAndBias.xy;\n"
"        UVInfo.dUV_dy   *= TexAttribs.AtlasUVScaleAndBias.xy;\n"
"    }\n"
"#endif\n"
"\n"
"    return UVInfo;\n"
"}\n"
"\n"
"BaseLayerShadingInfo ReadBaseLayerProperties(in VSOutput          VSOut,\n"
"                                             in float4            BaseColor,\n"
"                                             in PerturbNormalInfo NormalInfo,\n"
"                                             in NormalMapUVInfo   NMUVInfo,\n"
"                                             in float3            View,\n"
"                                             in bool              IsFrontFace)\n"
"{\n"
"    BaseLayerShadingInfo Base;\n"
"\n"
"    float3 TSNormal     = GetMicroNormal(g_Primitive.Material, NMUVInfo.UV, NMUVInfo.SmoothUV, NMUVInfo.dUV_dx, NMUVInfo.dUV_dy);\n"
"    float4 PhysicalDesc = GetPhysicalDesc(VSOut, g_Primitive.Material);\n"
"\n"
"    PBRMaterialBasicAttribs BasicAttribs = g_Primitive.Material.Basic;\n"
"    if (BasicAttribs.Workflow == PBR_WORKFLOW_SPECULAR_GLOSINESS)\n"
"    {\n"
"        PhysicalDesc.rgb = TO_LINEAR(PhysicalDesc.rgb) * BasicAttribs.SpecularFactor.rgb;\n"
"        const float u_GlossinessFactor = 1.0;\n"
"        PhysicalDesc.a *= u_GlossinessFactor;\n"
"    }\n"
"    else if (BasicAttribs.Workflow == PBR_WORKFLOW_METALLIC_ROUGHNESS)\n"
"    {\n"
"        // PhysicalDesc should already be in linear space\n"
"        PhysicalDesc.g = saturate(PhysicalDesc.g * BasicAttribs.RoughnessFactor);\n"
"        PhysicalDesc.b = saturate(PhysicalDesc.b * BasicAttribs.MetallicFactor);\n"
"    }\n"
"    Base.Metallic = 0.0;\n"
"    Base.Srf = GetSurfaceReflectance(BasicAttribs.Workflow, BaseColor, PhysicalDesc, Base.Metallic);\n"
"\n"
"    Base.Normal = PerturbNormal(NormalInfo,\n"
"                                NMUVInfo.dUV_dx,\n"
"                                NMUVInfo.dUV_dy,\n"
"                                TSNormal,\n"
"                                NMUVInfo.Selector >= 0.0,\n"
"                                IsFrontFace);\n"
"\n"
"    Base.NdotV = saturate(dot(Base.Normal, View));\n"
"\n"
"    return Base;\n"
"}\n"
"\n"
"#if ENABLE_CLEAR_COAT\n"
"ClearcoatShadingInfo ReadClearcoatLayerProperties(in VSOutput          VSOut,\n"
"                                                  in PerturbNormalInfo NormalInfo,\n"
"                                                  in NormalMapUVInfo   NMUVInfo,\n"
"                                                  in bool              IsFrontFace)\n"
"{\n"
"    ClearcoatShadingInfo Clearcoat;\n"
"\n"
"    Clearcoat.Factor  = GetClearcoatFactor(VSOut, g_Primitive.Material);\n"
"\n"
"    float  ClearCoatRoughness = GetClearcoatRoughness(VSOut, g_Primitive.Material);\n"
"    float3 ClearCoatNormal    = GetClearcoatNormal(g_Primitive.Material, NMUVInfo.UV, NMUVInfo.SmoothUV, NMUVInfo.dUV_dx, NMUVInfo.dUV_dy);\n"
"\n"
"    float IOR = 1.5;\n"
"    Clearcoat.Srf = GetSurfaceReflectanceClearCoat(ClearCoatRoughness, IOR);\n"
"\n"
"    Clearcoat.Normal = NormalInfo.Normal;\n"
"#   if USE_CLEAR_COAT_NORMAL_MAP\n"
"    {\n"
"        // If clearcoatNormalTexture is not given, no normal mapping is applied to the clear coat layer,\n"
"        // even if normal mapping is applied to the base material.\n"
"        if (NMUVInfo.Selector >= 0)\n"
"        {\n"
"            Clearcoat.Normal = PerturbNormal(NormalInfo,\n"
"                                             NMUVInfo.dUV_dx,\n"
"                                             NMUVInfo.dUV_dy,\n"
"                                             ClearCoatNormal,\n"
"                                             true,\n"
"                                             IsFrontFace);\n"
"        }\n"
"    }\n"
"#   endif\n"
"\n"
"    return Clearcoat;\n"
"}\n"
"#endif\n"
"\n"
"#if ENABLE_SHEEN\n"
"SheenShadingInfo ReadSheenLayerProperties(in VSOutput VSOut)\n"
"{\n"
"    SheenShadingInfo Sheen;\n"
"\n"
"    Sheen.Color     = GetSheenColor(VSOut, g_Primitive.Material);\n"
"    Sheen.Roughness = GetSheenRoughness(VSOut, g_Primitive.Material);\n"
"\n"
"    return Sheen;\n"
"}\n"
"#endif\n"
"\n"
"#if ENABLE_IRIDESCENCE\n"
"IridescenceShadingInfo ReadIridescenceProperties(in VSOutput VSOut, BaseLayerShadingInfo BaseLayer)\n"
"{\n"
"    IridescenceShadingInfo Iridescence;\n"
"\n"
"    Iridescence.Factor    = GetIridescence(VSOut, g_Primitive.Material);\n"
"    Iridescence.Thickness = GetIridescenceThickness(VSOut, g_Primitive.Material);\n"
"\n"
"    Iridescence.Fresnel = EvalIridescence(1.0, g_Primitive.Material.Iridescence.IOR, BaseLayer.NdotV, Iridescence.Thickness, BaseLayer.Srf.Reflectance0);\n"
"    Iridescence.F0      = SchlickToF0(BaseLayer.NdotV, Iridescence.Fresnel, float3(1.0, 1.0, 1.0));\n"
"\n"
"    if (Iridescence.Thickness == 0.0)\n"
"        Iridescence.Factor = 0.0;\n"
"\n"
"    return Iridescence;\n"
"}\n"
"#endif\n"
"\n"
"float4 ComputePbrSurfaceColor(in VSOutput VSOut,\n"
"                              in bool     IsFrontFace)\n"
"{\n"
"    float4 BaseColor = GetBaseColor(VSOut, g_Primitive.Material);\n"
"\n"
"#if USE_VERTEX_NORMALS\n"
"    float3 MeshNormal = VSOut.Normal;\n"
"#else\n"
"    // PerturbNormal can handle zero-length mesh normals.\n"
"    float3 MeshNormal = float3(0.0, 0.0, 0.0);\n"
"#endif\n"
"    // We have to compute gradients in uniform flow control to avoid issues with perturbed normal\n"
"    PerturbNormalInfo NormalInfo = GetPerturbNormalInfo(VSOut.WorldPos, MeshNormal);\n"
"\n"
"    PBRMaterialTextureAttribs NormalTexAttribs = g_Primitive.Material.Textures[NormalTextureAttribId];\n"
"    NormalMapUVInfo           NMUVInfo         = GetNormalMapUVInfo(VSOut, NormalTexAttribs);\n"
"\n"
"#if USE_CLEAR_COAT_NORMAL_MAP\n"
"    PBRMaterialTextureAttribs ClearCoatNormalTexAttribs = g_Primitive.Material.Textures[ClearCoatNormalTextureAttribId];\n"
"    NormalMapUVInfo           ClearCoatNMUVInfo         = GetNormalMapUVInfo(VSOut, ClearCoatNormalTexAttribs);\n"
"#endif\n"
"\n"
"    PBRMaterialBasicAttribs BasicAttribs = g_Primitive.Material.Basic;\n"
"    if (BasicAttribs.AlphaMode == PBR_ALPHA_MODE_MASK && BaseColor.a < BasicAttribs.AlphaMaskCutoff)\n"
"    {\n"
"        discard;\n"
"    }\n"
"\n"
"    SurfaceShadingInfo Shading;\n"
"    Shading.View      = normalize(g_Frame.Camera.f4Position.xyz - VSOut.WorldPos.xyz); // Direction from surface point to camera\n"
"    Shading.BaseLayer = ReadBaseLayerProperties(VSOut, BaseColor, NormalInfo, NMUVInfo, Shading.View, IsFrontFace);\n"
"    Shading.Occlusion = GetOcclusion(VSOut, g_Primitive.Material);\n"
"    Shading.Emissive  = GetEmissive(VSOut, g_Primitive.Material);\n"
"\n"
"    Shading.IBLScale          = g_Frame.Renderer.IBLScale;\n"
"    Shading.OcclusionStrength = g_Frame.Renderer.OcclusionStrength;\n"
"    Shading.EmissionScale     = g_Frame.Renderer.EmissionScale;\n"
"\n"
"#   if ENABLE_CLEAR_COAT\n"
"    {\n"
"        Shading.Clearcoat = ReadClearcoatLayerProperties(VSOut, NormalInfo, ClearCoatNMUVInfo, IsFrontFace);\n"
"    }\n"
"#   endif\n"
"\n"
"#if ENABLE_SHEEN\n"
"    {\n"
"        Shading.Sheen = ReadSheenLayerProperties(VSOut);\n"
"    }\n"
"#   endif\n"
"\n"
"#if ENABLE_ANISOTROPY\n"
"    {\n"
"        Shading.Anisotropy = GetAnisotropy(VSOut, g_Primitive.Material);\n"
"    }\n"
"#   endif\n"
"\n"
"#   if ENABLE_IRIDESCENCE\n"
"    {\n"
"        Shading.Iridescence = ReadIridescenceProperties(VSOut, Shading.BaseLayer);\n"
"\n"
"        Shading.BaseLayer.Srf.Reflectance0 = lerp(Shading.BaseLayer.Srf.Reflectance0,\n"
"                                                  Shading.Iridescence.F0,\n"
"                                                  float3(Shading.Iridescence.Factor, Shading.Iridescence.Factor, Shading.Iridescence.Factor));\n"
"    }\n"
"#   endif\n"
"\n"
"#   if ENABLE_TRANSMISSION\n"
"    {\n"
"        Shading.Transmission = GetTransmission(VSOut, g_Primitive.Material);\n"
"    }\n"
"#   endif\n"
"\n"
"#if ENABLE_VOLUME\n"
"    {\n"
"        Shading.VolumeThickness = GetVolumeThickness(VSOut, g_Primitive.Material);\n"
"    }\n"
"#endif\n"
"\n"
"    SurfaceLightingInfo SrfLighting = GetDefaultSurfaceLightingInfo();\n"
"\n"
"    if (BasicAttribs.Workflow != PBR_WORKFLOW_UNLIT)\n"
"    {\n"
"        ApplyPunctualLights(\n"
"            Shading,\n"
"            g_Frame.Light,\n"
"#if     ENABLE_SHEEN\n"
"            g_SheenAlbedoScalingLUT,\n"
"            g_SheenAlbedoScalingLUT_sampler,\n"
"#endif\n"
"            SrfLighting);\n"
"\n"
"#       if USE_IBL\n"
"        {\n"
"            ApplyIBL(Shading, float(g_Frame.Renderer.PrefilteredCubeMipLevels),\n"
"                     g_PreintegratedGGX,  g_PreintegratedGGX_sampler,\n"
"                     g_IrradianceMap,     g_IrradianceMap_sampler,\n"
"                     g_PrefilteredEnvMap, g_PrefilteredEnvMap_sampler,\n"
"#                    if ENABLE_SHEEN\n"
"                         g_PreintegratedCharlie, g_PreintegratedCharlie_sampler,\n"
"#                    endif\n"
"                     SrfLighting);\n"
"        }\n"
"#       endif\n"
"    }\n"
"\n"
"    float4 OutColor;\n"
"    OutColor.rgb = ResolveLighting(Shading, SrfLighting);\n"
"    OutColor.a   = BaseColor.a;\n"
"\n"
"#if ENABLE_TONE_MAPPING\n"
"    {\n"
"        // Perform tone mapping\n"
"        ToneMappingAttribs TMAttribs;\n"
"        TMAttribs.iToneMappingMode     = TONE_MAPPING_MODE;\n"
"        TMAttribs.bAutoExposure        = false;\n"
"        TMAttribs.fMiddleGray          = g_Frame.Renderer.MiddleGray;\n"
"        TMAttribs.bLightAdaptation     = false;\n"
"        TMAttribs.fWhitePoint          = g_Frame.Renderer.WhitePoint;\n"
"        TMAttribs.fLuminanceSaturation = 1.0;\n"
"        OutColor.rgb = ToneMap(OutColor.rgb, TMAttribs, g_Frame.Renderer.AverageLogLum);\n"
"    }\n"
"#endif\n"
"\n"
"    // Add highlight color\n"
"    OutColor.rgb = lerp(OutColor.rgb, g_Frame.Renderer.HighlightColor.rgb, g_Frame.Renderer.HighlightColor.a);\n"
"\n"
"\n"
"    // Shader inputs debug visualization\n"
"#if (DEBUG_VIEW == DEBUG_VIEW_BASE_COLOR)\n"
"    {\n"
"        OutColor.rgba = BaseColor;\n"
"    }\n"
"#elif (DEBUG_VIEW == DEBUG_VIEW_TEXCOORD0 && USE_TEXCOORD0)\n"
"    {\n"
"        OutColor.rgb = float3(VSOut.UV0, 0.0);\n"
"    }\n"
"#elif (DEBUG_VIEW == DEBUG_VIEW_TEXCOORD1 && USE_TEXCOORD1)\n"
"    {\n"
"        OutColor.rgb = float3(VSOut.UV1, 0.0);\n"
"    }\n"
"#elif (DEBUG_VIEW == DEBUG_VIEW_TRANSPARENCY)\n"
"    {\n"
"        OutColor.rgba = float4(BaseColor.a, BaseColor.a, BaseColor.a, 1.0);\n"
"    }\n"
"#elif (DEBUG_VIEW == DEBUG_VIEW_NORMAL_MAP)\n"
"    {\n"
"        //OutColor.rgb = TSNormal.xyz;\n"
"    }\n"
"#elif (DEBUG_VIEW == DEBUG_VIEW_MESH_NORMAL)\n"
"    {\n"
"        OutColor.rgb  = MeshNormal * float3(0.5, 0.5, 0.5) + float3(0.5, 0.5, 0.5);\n"
"    }\n"
"#elif (DEBUG_VIEW != DEBUG_VIEW_NONE)\n"
"    {\n"
"        OutColor.rgb = GetDebugColor(Shading, SrfLighting);\n"
"    }\n"
"#endif\n"
"\n"
"\n"
"#if CONVERT_OUTPUT_TO_SRGB\n"
"    {\n"
"        OutColor.rgb = FastLinearToSRGB(OutColor.rgb);\n"
"    }\n"
"#endif\n"
"\n"
"    return OutColor;\n"
"}\n"
"\n"
"\n"
"#include \"PSMainGenerated.generated\"\n"
"// struct PSOutput\n"
"// {\n"
"//     float4 Color      : SV_Target0;\n"
"//     float4 CustomData : SV_Target1;\n"
"// };\n"
"//\n"
"// void main(in VSOutput VSOut,\n"
"//           in bool IsFrontFace : SV_IsFrontFace,\n"
"//           out PSOutput PSOut)\n"
"// {\n"
"//     PSOut.Color = ComputePbrSurfaceColor(VSOut, IsFrontFace);\n"
"//\n"
"// #if ENABLE_CUSTOM_DATA_OUTPUT\n"
"//     {\n"
"//         PSOut.CustomData = g_Primitive.CustomData;\n"
"//     }\n"
"// #endif\n"
"// }\n"
