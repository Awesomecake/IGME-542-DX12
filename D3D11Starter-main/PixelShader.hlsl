#include "Lighting.hlsli"

Texture2D Albedo : register(t0);
Texture2D NormalMap : register(t1);
Texture2D RoughnessMap : register(t2);
Texture2D MetalnessMap : register(t3);
SamplerState BasicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    float3 surfaceColor = float3(1,1,1);
    
    float3 unpackedNormal = normalize(NormalMap.Sample(BasicSampler, input.UV).rgb * 2 - 1);
    
    float3 albedoColor = pow(Albedo.Sample(BasicSampler, input.UV).rgb, 2.2f);
    float roughness = RoughnessMap.Sample(BasicSampler, input.UV).r;
    float metalness = MetalnessMap.Sample(BasicSampler, input.UV).r;
    float3 specularColor = lerp(F0_NON_METAL, albedoColor.rgb, metalness);
    
    input.Normal = normalize(input.Normal);
    float3 T = normalize(input.Tangent); // Must be normalized here or before
    T = normalize(T - input.Normal * dot(T, input.Normal)); // Gram-Schmidt assumes T&N are normalized!
    input.Normal = mul(unpackedNormal, float3x3(T, cross(T, input.Normal), input.Normal)); // Note multiplication order!
        
    float3 totalLight = CalcLights(input, surfaceColor.xyz, specularColor, roughness, metalness, 0/*shadowAmount*/);
 
    return float4(pow(surfaceColor.xyz * albedoColor * totalLight, 1.0f / 2.2f), 1);
}