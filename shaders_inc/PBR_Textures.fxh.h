"#ifndef _PBR_TEXTURES_FXH_\n"
"#define _PBR_TEXTURES_FXH_\n"
"\n"
"#if USE_TEXTURE_ATLAS\n"
"#   include \"AtlasSampling.fxh\"\n"
"#endif\n"
"\n"
"#ifndef BaseColorTextureAttribId\n"
"#   define BaseColorTextureAttribId 0\n"
"#endif\n"
"\n"
"#ifndef PhysicalDescriptorTextureAttribId\n"
"#   define PhysicalDescriptorTextureAttribId 1\n"
"#endif\n"
"\n"
"#ifndef NormalTextureAttribId\n"
"#   define NormalTextureAttribId 2\n"
"#endif\n"
"\n"
"#ifndef OcclusionTextureAttribId\n"
"#   define OcclusionTextureAttribId 3\n"
"#endif\n"
"\n"
"#ifndef EmissiveTextureAttribId\n"
"#   define EmissiveTextureAttribId 4\n"
"#endif\n"
"\n"
"#if !defined(USE_TEXCOORD0) && !defined(USE_TEXCOORD1)\n"
"#   undef USE_COLOR_MAP\n"
"#   define USE_COLOR_MAP 0\n"
"\n"
"#   undef USE_METALLIC_MAP\n"
"#   define USE_METALLIC_MAP 0\n"
"\n"
"#   undef USE_ROUGHNESS_MAP\n"
"#   define USE_ROUGHNESS_MAP 0\n"
"\n"
"#   undef USE_PHYS_DESC_MAP\n"
"#   define USE_PHYS_DESC_MAP 0\n"
"\n"
"#   undef USE_NORMAL_MAP\n"
"#   define USE_NORMAL_MAP 0\n"
"\n"
"#   undef USE_AO_MAP\n"
"#   define USE_AO_MAP 0\n"
"\n"
"#   undef USE_EMISSIVE_MAP\n"
"#   define USE_EMISSIVE_MAP 0\n"
"\n"
"#   undef USE_CLEAR_COAT_MAP\n"
"#   define USE_CLEAR_COAT_MAP 0\n"
"\n"
"#   undef USE_CLEAR_COAT_ROUGHNESS_MAP\n"
"#   define USE_CLEAR_COAT_ROUGHNESS_MAP 0\n"
"\n"
"#   undef USE_CLEAR_COAT_NORMAL_MAP\n"
"#   define USE_CLEAR_COAT_NORMAL_MAP 0\n"
"\n"
"#   undef USE_SHEEN_COLOR_MAP\n"
"#   define USE_SHEEN_COLOR_MAP 0\n"
"\n"
"#   undef USE_SHEEN_ROUGHNESS_MAP\n"
"#   define USE_SHEEN_ROUGHNESS_MAP 0\n"
"\n"
"#   undef USE_ANISOTROPY_MAP\n"
"#   define USE_ANISOTROPY_MAP 0\n"
"\n"
"#   undef USE_IRIDESCENCE_MAP\n"
"#   define USE_IRIDESCENCE_MAP 0\n"
"\n"
"#   undef USE_IRIDESCENCE_THICKNESS_MAP\n"
"#   define USE_IRIDESCENCE_THICKNESS_MAP 0\n"
"\n"
"#   undef USE_TRANSMISSION_MAP\n"
"#   define USE_TRANSMISSION_MAP 0\n"
"\n"
"#   undef USE_THICKNESS_MAP\n"
"#   define USE_THICKNESS_MAP 0\n"
"#endif\n"
"\n"
"#if USE_IBL\n"
"    TextureCube  g_IrradianceMap;\n"
"    SamplerState g_IrradianceMap_sampler;\n"
"\n"
"    TextureCube  g_PrefilteredEnvMap;\n"
"    SamplerState g_PrefilteredEnvMap_sampler;\n"
"\n"
"    Texture2D     g_BRDF_LUT;\n"
"    SamplerState  g_BRDF_LUT_sampler;\n"
"#endif\n"
"\n"
"#if USE_COLOR_MAP\n"
"    Texture2DArray g_ColorMap;\n"
"    SamplerState   g_ColorMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_METALLIC_MAP\n"
"    Texture2DArray g_MetallicMap;\n"
"    SamplerState   g_MetallicMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_ROUGHNESS_MAP\n"
"    Texture2DArray g_RoughnessMap;\n"
"    SamplerState   g_RoughnessMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_PHYS_DESC_MAP\n"
"    Texture2DArray g_PhysicalDescriptorMap;\n"
"    SamplerState   g_PhysicalDescriptorMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_NORMAL_MAP\n"
"    Texture2DArray g_NormalMap;\n"
"    SamplerState   g_NormalMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_AO_MAP\n"
"    Texture2DArray g_AOMap;\n"
"    SamplerState   g_AOMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_EMISSIVE_MAP\n"
"    Texture2DArray g_EmissiveMap;\n"
"    SamplerState   g_EmissiveMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_CLEAR_COAT_MAP\n"
"    Texture2DArray g_ClearCoatMap;\n"
"    SamplerState   g_ClearCoatMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_CLEAR_COAT_ROUGHNESS_MAP\n"
"    Texture2DArray g_ClearCoatRoughnessMap;\n"
"    SamplerState   g_ClearCoatRoughnessMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_CLEAR_COAT_NORMAL_MAP\n"
"    Texture2DArray g_ClearCoatNormalMap;\n"
"    SamplerState   g_ClearCoatNormalMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_SHEEN_COLOR_MAP\n"
"    Texture2DArray g_SheenColorMap;\n"
"    SamplerState   g_SheenColorMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_SHEEN_ROUGHNESS_MAP\n"
"    Texture2DArray g_SheenRoughnessMap;\n"
"    SamplerState   g_SheenRoughnessMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_ANISOTROPY_MAP\n"
"    Texture2DArray g_AnisotropyMap;\n"
"    SamplerState   g_AnisotropyMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_IRIDESCENCE_MAP\n"
"    Texture2DArray g_IridescenceMap;\n"
"    SamplerState   g_IridescenceMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_IRIDESCENCE_THICKNESS_MAP\n"
"    Texture2DArray g_IridescenceThicknessMap;\n"
"    SamplerState   g_IridescenceThicknessMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_TRANSMISSION_MAP\n"
"    Texture2DArray g_TransmissionMap;\n"
"    SamplerState   g_TransmissionMap_sampler;\n"
"#endif\n"
"\n"
"#if USE_THICKNESS_MAP\n"
"    Texture2DArray g_ThicknessMap;\n"
"    SamplerState   g_ThicknessMap_sampler;\n"
"#endif\n"
"\n"
"float2 SelectUV(VSOutput VSOut, float Selector)\n"
"{\n"
"#if USE_TEXCOORD0 && USE_TEXCOORD1\n"
"    return lerp(VSOut.UV0, VSOut.UV1, Selector);\n"
"#elif USE_TEXCOORD0\n"
"    return VSOut.UV0;\n"
"#elif USE_TEXCOORD1\n"
"    return VSOut.UV1;\n"
"#else\n"
"    return float2(0.0, 0.0);\n"
"#endif\n"
"}\n"
"\n"
"float2 TransformUV(float2 UV, PBRMaterialTextureAttribs TexAttribs)\n"
"{\n"
"    return mul(UV, MatrixFromRows(TexAttribs.UVScaleAndRotation.xy, TexAttribs.UVScaleAndRotation.zw)) + float2(TexAttribs.UBias, TexAttribs.VBias);\n"
"}\n"
"\n"
"float4 SampleTexture(Texture2DArray            Tex,\n"
"                     SamplerState              Tex_sampler,\n"
"                     VSOutput                  VSOut,\n"
"                     PBRMaterialTextureAttribs TexAttribs,\n"
"                     float4                    DefaultValue)\n"
"{\n"
"#   if USE_TEXCOORD0 || USE_TEXCOORD1\n"
"    {\n"
"        float2 UV = SelectUV(VSOut, TexAttribs.UVSelector);\n"
"#       if ENABLE_TEXCOORD_TRANSFORM\n"
"        {\n"
"            UV = TransformUV(UV, TexAttribs);\n"
"        }\n"
"#       endif\n"
"\n"
"#       if USE_TEXTURE_ATLAS\n"
"        {\n"
"            if (TexAttribs.UVSelector < 0.0)\n"
"            {\n"
"                return DefaultValue;\n"
"            }\n"
"            else\n"
"            {\n"
"                SampleTextureAtlasAttribs SampleAttribs;\n"
"                SampleAttribs.f2UV                   = frac(UV) * TexAttribs.AtlasUVScaleAndBias.xy + TexAttribs.AtlasUVScaleAndBias.zw;\n"
"                SampleAttribs.f2SmoothUV             = UV      * TexAttribs.AtlasUVScaleAndBias.xy;\n"
"                SampleAttribs.f2dSmoothUV_dx         = ddx(UV) * TexAttribs.AtlasUVScaleAndBias.xy;\n"
"                SampleAttribs.f2dSmoothUV_dy         = ddy(UV) * TexAttribs.AtlasUVScaleAndBias.xy;\n"
"                SampleAttribs.fSlice                 = TexAttribs.TextureSlice;\n"
"                SampleAttribs.f4UVRegion             = TexAttribs.AtlasUVScaleAndBias;\n"
"                SampleAttribs.fSmallestValidLevelDim = 4.0;\n"
"                SampleAttribs.IsNonFilterable        = false;\n"
"                SampleAttribs.fMaxAnisotropy         = 1.0; // Only used on GLES\n"
"                return SampleTextureAtlas(Tex, Tex_sampler, SampleAttribs);\n"
"            }\n"
"        }\n"
"#       else\n"
"        {\n"
"            return Tex.Sample(Tex_sampler, float3(UV, TexAttribs.TextureSlice));\n"
"        }\n"
"#       endif\n"
"    }\n"
"#   else\n"
"    {\n"
"        return DefaultValue;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"float4 GetBaseColor(VSOutput              VSOut,\n"
"                    PBRMaterialShaderInfo Material)\n"
"{\n"
"    float4 BaseColor = float4(1.0, 1.0, 1.0, 1.0);\n"
"\n"
"#   if USE_COLOR_MAP\n"
"    {\n"
"        BaseColor = SampleTexture(g_ColorMap,\n"
"                                  g_ColorMap_sampler,\n"
"                                  VSOut,\n"
"                                  Material.Textures[BaseColorTextureAttribId],\n"
"                                  float4(1.0, 1.0, 1.0, 1.0));\n"
"        BaseColor = float4(TO_LINEAR(BaseColor.rgb), BaseColor.a);\n"
"    }\n"
"#   endif\n"
"\n"
"#   if USE_VERTEX_COLORS\n"
"    {\n"
"        BaseColor *= VSOut.Color;\n"
"    }\n"
"#   endif\n"
"    return BaseColor * Material.Basic.BaseColorFactor;\n"
"}\n"
"\n"
"float3 SampleNormalTexture(PBRMaterialTextureAttribs TexAttribs,\n"
"                           Texture2DArray            NormalMap,\n"
"                           SamplerState              NormalMap_sampler,\n"
"                           float2                    NormalMapUV,\n"
"                           float2                    SmoothNormalMapUV,\n"
"                           float2                    dNormalMapUV_dx,\n"
"                           float2                    dNormalMapUV_dy)\n"
"{\n"
"#   if USE_TEXTURE_ATLAS\n"
"    {\n"
"        if (TexAttribs.UVSelector >= 0.0)\n"
"        {\n"
"            SampleTextureAtlasAttribs SampleAttribs;\n"
"            SampleAttribs.f2UV                   = NormalMapUV;\n"
"            SampleAttribs.f2SmoothUV             = SmoothNormalMapUV;\n"
"            SampleAttribs.f2dSmoothUV_dx         = dNormalMapUV_dx;\n"
"            SampleAttribs.f2dSmoothUV_dy         = dNormalMapUV_dy;\n"
"            SampleAttribs.fSlice                 = TexAttribs.TextureSlice;\n"
"            SampleAttribs.f4UVRegion             = TexAttribs.AtlasUVScaleAndBias;\n"
"            SampleAttribs.fSmallestValidLevelDim = 4.0;\n"
"            SampleAttribs.IsNonFilterable        = false;\n"
"            SampleAttribs.fMaxAnisotropy         = 1.0; // Only used on GLES\n"
"            return SampleTextureAtlas(NormalMap, NormalMap_sampler, SampleAttribs).xyz;\n"
"        }\n"
"    }\n"
"#   else\n"
"    {\n"
"        return NormalMap.Sample(NormalMap_sampler, float3(NormalMapUV, TexAttribs.TextureSlice)).xyz;\n"
"    }\n"
"#   endif\n"
"\n"
"    return float3(0.5, 0.5, 1.0);\n"
"}\n"
"\n"
"float3 GetMicroNormal(PBRMaterialShaderInfo Material,\n"
"                      float2                NormalMapUV,\n"
"                      float2                SmoothNormalMapUV,\n"
"                      float2                dNormalMapUV_dx,\n"
"                      float2                dNormalMapUV_dy)\n"
"{\n"
"    float3 MicroNormal = float3(0.5, 0.5, 1.0);\n"
"\n"
"#   if USE_NORMAL_MAP && (USE_TEXCOORD0 || USE_TEXCOORD1)\n"
"    {\n"
"        MicroNormal = SampleNormalTexture(Material.Textures[NormalTextureAttribId],\n"
"                                          g_NormalMap,\n"
"                                          g_NormalMap_sampler,\n"
"                                          NormalMapUV,\n"
"                                          SmoothNormalMapUV,\n"
"                                          dNormalMapUV_dx,\n"
"                                          dNormalMapUV_dy);\n"
"    }\n"
"#endif\n"
"\n"
"    return MicroNormal * float3(2.0, 2.0, 2.0) - float3(1.0, 1.0, 1.0);\n"
"}\n"
"\n"
"float GetOcclusion(VSOutput              VSOut,\n"
"                   PBRMaterialShaderInfo Material)\n"
"{\n"
"    float Occlusion = 1.0;\n"
"#   if USE_AO_MAP\n"
"    {\n"
"        Occlusion = SampleTexture(g_AOMap,\n"
"                                  g_AOMap_sampler,\n"
"                                  VSOut,\n"
"                                  Material.Textures[OcclusionTextureAttribId],\n"
"                                  float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"    }\n"
"#   endif\n"
"    return Occlusion * Material.Basic.OcclusionFactor;\n"
"}\n"
"\n"
"float3 GetEmissive(VSOutput              VSOut,\n"
"                   PBRMaterialShaderInfo Material)\n"
"{\n"
"    float3 Emissive = float3(1.0, 1.0, 1.0);\n"
"\n"
"#   if USE_EMISSIVE_MAP\n"
"    {\n"
"        Emissive = SampleTexture(g_EmissiveMap,\n"
"                                 g_EmissiveMap_sampler,\n"
"                                 VSOut,\n"
"                                 Material.Textures[EmissiveTextureAttribId],\n"
"                                 float4(1.0, 1.0, 1.0, 1.0)).rgb;\n"
"        Emissive = TO_LINEAR(Emissive);\n"
"    }\n"
"#   endif\n"
"    return Emissive * Material.Basic.EmissiveFactor.rgb;\n"
"}\n"
"\n"
"float4 GetPhysicalDesc(VSOutput              VSOut,\n"
"                       PBRMaterialShaderInfo Material)\n"
"{\n"
"    // Set defaults to 1 so that if the textures are not available, the values\n"
"    // are controlled by the metallic/roughness scale factors.\n"
"    float4 PhysicalDesc = float4(1.0, 1.0, 1.0, 1.0);\n"
"#   if USE_PHYS_DESC_MAP\n"
"    {\n"
"        PhysicalDesc = SampleTexture(g_PhysicalDescriptorMap,\n"
"                                     g_PhysicalDescriptorMap_sampler,\n"
"                                     VSOut,\n"
"                                     Material.Textures[PhysicalDescriptorTextureAttribId],\n"
"                                     float4(1.0, 1.0, 1.0, 1.0));\n"
"    }\n"
"#   else\n"
"    {\n"
"#       if USE_METALLIC_MAP\n"
"        {\n"
"            PhysicalDesc.b = SampleTexture(g_MetallicMap,\n"
"                                           g_MetallicMap_sampler,\n"
"                                           VSOut,\n"
"                                           Material.Textures[MetallicTextureAttribId],\n"
"                                           float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"        }\n"
"#       endif\n"
"\n"
"#       if USE_ROUGHNESS_MAP\n"
"        {\n"
"            PhysicalDesc.g = SampleTexture(g_RoughnessMap,\n"
"                                           g_RoughnessMap_sampler,\n"
"                                           VSOut,\n"
"                                           Material.Textures[RoughnessTextureAttribId],\n"
"                                           float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"\n"
"        }\n"
"#       endif\n"
"    }\n"
"#endif\n"
"\n"
"    return PhysicalDesc;\n"
"}\n"
"\n"
"\n"
"// Extensions\n"
"\n"
"// Clear coat\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_clearcoat\n"
"\n"
"float GetClearcoatFactor(VSOutput              VSOut,\n"
"                         PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_CLEAR_COAT\n"
"    {\n"
"        float Cearcoat = 1.0;\n"
"#       if USE_CLEAR_COAT_MAP\n"
"        {\n"
"            Cearcoat = SampleTexture(g_ClearCoatMap,\n"
"                                     g_ClearCoatMap_sampler,\n"
"                                     VSOut,\n"
"                                     Material.Textures[ClearCoatTextureAttribId],\n"
"                                     float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"        }\n"
"#       endif\n"
"        return Cearcoat * Material.Basic.ClearcoatFactor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"float GetClearcoatRoughness(VSOutput              VSOut,\n"
"                            PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_CLEAR_COAT\n"
"    {\n"
"        float CearcoatRoughness = 1.0;\n"
"#       if USE_CLEAR_COAT_ROUGHNESS_MAP\n"
"        {\n"
"            CearcoatRoughness = SampleTexture(g_ClearCoatRoughnessMap,\n"
"                                              g_ClearCoatRoughnessMap_sampler,\n"
"                                              VSOut,\n"
"                                              Material.Textures[ClearCoatRoughnessTextureAttribId],\n"
"                                              float4(1.0, 1.0, 1.0, 1.0)).g;\n"
"        }\n"
"#       endif\n"
"        return CearcoatRoughness * Material.Basic.ClearcoatRoughnessFactor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"float3 GetClearcoatNormal(PBRMaterialShaderInfo Material,\n"
"                          float2                NormalMapUV,\n"
"                          float2                SmoothNormalMapUV,\n"
"                          float2                dNormalMapUV_dx,\n"
"                          float2                dNormalMapUV_dy)\n"
"{\n"
"    float3 ClearcoatNormal = float3(0.5, 0.5, 1.0);\n"
"#   if ENABLE_CLEAR_COAT\n"
"    {\n"
"#       if USE_CLEAR_COAT_NORMAL_MAP\n"
"        {\n"
"            ClearcoatNormal =\n"
"                SampleNormalTexture(Material.Textures[ClearCoatNormalTextureAttribId],\n"
"                                    g_ClearCoatNormalMap,\n"
"                                    g_ClearCoatNormalMap_sampler,\n"
"                                    NormalMapUV,\n"
"                                    SmoothNormalMapUV,\n"
"                                    dNormalMapUV_dx,\n"
"                                    dNormalMapUV_dy);\n"
"        }\n"
"#       endif\n"
"    }\n"
"#endif\n"
"    return ClearcoatNormal * float3(2.0, 2.0, 2.0) - float3(1.0, 1.0, 1.0);\n"
"}\n"
"\n"
"\n"
"// Sheen\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_sheen\n"
"\n"
"float3 GetSheenColor(VSOutput              VSOut,\n"
"                     PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_SHEEN\n"
"    {\n"
"        float3 SheenColor = float3(1.0, 1.0, 1.0);\n"
"#       if USE_SHEEN_COLOR_MAP\n"
"        {\n"
"            SheenColor = SampleTexture(g_SheenColorMap,\n"
"                                       g_SheenColorMap_sampler,\n"
"                                       VSOut,\n"
"                                       Material.Textures[SheenColorTextureAttribId],\n"
"                                       float4(1.0, 1.0, 1.0, 1.0)).rgb;\n"
"            SheenColor = TO_LINEAR(SheenColor);\n"
"        }\n"
"#       endif\n"
"        return SheenColor * float3(Material.Sheen.ColorFactorR, Material.Sheen.ColorFactorG, Material.Sheen.ColorFactorB);\n"
"    }\n"
"#   else\n"
"    {\n"
"        return float3(0.0, 0.0, 0.0);\n"
"    }\n"
"#endif\n"
"}\n"
"\n"
"float GetSheenRoughness(VSOutput              VSOut,\n"
"                        PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_SHEEN\n"
"    {\n"
"        float SheenRoughness = 1.0;\n"
"#       if USE_SHEEN_ROUGHNESS_MAP\n"
"        {\n"
"            SheenRoughness = SampleTexture(g_SheenRoughnessMap,\n"
"                                           g_SheenRoughnessMap_sampler,\n"
"                                           VSOut,\n"
"                                           Material.Textures[SheenRoughnessTextureAttribId],\n"
"                                           float4(1.0, 1.0, 1.0, 1.0)).a;\n"
"        }\n"
"#       endif\n"
"        return SheenRoughness * Material.Sheen.RoughnessFactor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"\n"
"// Anisotropy\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_anisotropy\n"
"\n"
"float3 GetAnisotropy(VSOutput              VSOut,\n"
"                     PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_ANISOTROPY\n"
"    {\n"
"        float3 Anisotropy = float3(0.5, 0.5, 1.0);\n"
"#       if USE_ANISOTROPY_MAP\n"
"        {\n"
"            Anisotropy = SampleTexture(g_AnisotropyMap,\n"
"                                       g_AnisotropyMap_sampler,\n"
"                                       VSOut,\n"
"                                       Material.Textures[AnisotropyTextureAttribId],\n"
"                                       float4(1.0, 1.0, 1.0, 1.0)).rgb;\n"
"        }\n"
"#       endif\n"
"        Anisotropy.xy = Anisotropy.xy * 2.0 - 1.0;\n"
"        Anisotropy.z *= Material.Anisotropy.Strength;\n"
"        return Anisotropy;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return float3(0.0, 0.0, 0.0);\n"
"    }\n"
"#endif\n"
"}\n"
"\n"
"\n"
"// Iridescence\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_iridescence\n"
"\n"
"float GetIridescence(VSOutput              VSOut,\n"
"                     PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_IRIDESCENCE\n"
"    {\n"
"        float Iridescence = 1.0;\n"
"#       if USE_IRIDESCENCE_MAP\n"
"        {\n"
"            Iridescence = SampleTexture(g_IridescenceMap,\n"
"                                        g_IridescenceMap_sampler,\n"
"                                        VSOut,\n"
"                                        Material.Textures[IridescenceTextureAttribId],\n"
"                                        float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"        }\n"
"#       endif\n"
"        return Iridescence * Material.Iridescence.Factor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"\n"
"float GetIridescenceThickness(VSOutput              VSOut,\n"
"                              PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_IRIDESCENCE\n"
"    {\n"
"        float Thickness = 0.0;\n"
"#       if USE_IRIDESCENCE_THICKNESS_MAP\n"
"        {\n"
"            Thickness = SampleTexture(g_IridescenceThicknessMap,\n"
"                                      g_IridescenceThicknessMap_sampler,\n"
"                                      VSOut,\n"
"                                      Material.Textures[IridescenceThicknessTextureAttribId],\n"
"                                      float4(1.0, 1.0, 1.0, 1.0)).g;\n"
"        }\n"
"#       endif\n"
"        return lerp(Material.Iridescence.ThicknessMinimum, Material.Iridescence.ThicknessMaximum, Thickness);\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"\n"
"// Transmission\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_transmission\n"
"\n"
"float GetTransmission(VSOutput              VSOut,\n"
"                      PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_TRANSMISSION\n"
"    {\n"
"        float Transmission = 1.0;\n"
"#       if USE_TRANSMISSION_MAP\n"
"        {\n"
"            Transmission = SampleTexture(g_TransmissionMap,\n"
"                                         g_TransmissionMap_sampler,\n"
"                                         VSOut,\n"
"                                         Material.Textures[TransmissionTextureAttribId],\n"
"                                         float4(1.0, 1.0, 1.0, 1.0)).r;\n"
"        }\n"
"#       endif\n"
"        return Transmission * Material.Transmission.Factor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"\n"
"// Volume\n"
"// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_volume\n"
"\n"
"float GetVolumeThickness(VSOutput              VSOut,\n"
"                         PBRMaterialShaderInfo Material)\n"
"{\n"
"#   if ENABLE_VOLUME\n"
"    {\n"
"        float Thickness = 1.0;\n"
"#       if USE_THICKNESS_MAP\n"
"        {\n"
"            Thickness = SampleTexture(g_ThicknessMap,\n"
"                                      g_ThicknessMap_sampler,\n"
"                                      VSOut,\n"
"                                      Material.Textures[ThicknessTextureAttribId],\n"
"                                      float4(1.0, 1.0, 1.0, 1.0)).g;\n"
"        }\n"
"#       endif\n"
"        return Thickness * Material.Volume.ThicknessFactor;\n"
"    }\n"
"#   else\n"
"    {\n"
"        return 0.0;\n"
"    }\n"
"#   endif\n"
"}\n"
"\n"
"#endif // _PBR_TEXTURES_FXH_\n"
