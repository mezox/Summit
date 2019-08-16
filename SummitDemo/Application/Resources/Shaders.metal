//
//  Shaders.metal
//  RendererMacOS
//
//  Created by Tomas Kubovcik on 30/05/2019.
//  Copyright © 2019 Summit. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include <simd/simd.h>

struct FragmentUniforms
{
    float brightness;
    float reflectivity;
    float shininess;
    simd::float3 lightColor;
};

struct VertexUniforms
{
    simd::float4x4 model;
    simd::float4x4 view;
    simd::float4x4 projection;
    simd::float4 cameraPosition;
};

struct PerInstanceUniforms
{
    simd::float4x4 modelMatrix;
};

struct Vertex
{
    float4 pos       [[attribute(0)]];
    float4 color     [[attribute(1)]];
    float4 normal    [[attribute(2)]];
};

struct VertexOut
{
    float4 color;
    float4 pos [[position]];
    float4 normal;
    float3 toLight;
    float3 toCamera;
};

vertex VertexOut vertexShader(Vertex in [[stage_in]],
                              constant VertexUniforms &uniforms [[buffer(3)]])
{
    VertexOut out;
    
    float4 worldPosition = uniforms.model * in.pos;
    
    // Pass the vertex color directly to the rasterizer
    out.color = in.color;
    // Pass the already normalized screen-space coordinates to the rasterizer
    out.pos = uniforms.projection * uniforms.view * worldPosition;
    out.normal = (uniforms.model * float4(in.normal.xyz, 0.0));
    
    float3 lightPosition = float3(200.0, 200.0, 100.0);
    
    out.toLight = lightPosition - worldPosition.xyz;
    out.toCamera = worldPosition.xyz - uniforms.cameraPosition.xyz;
    
    return out;
}

fragment float4 fragmentShader(VertexOut interpolated [[stage_in]],
                               constant FragmentUniforms &uniforms [[buffer(0)]],
                               texture2d<float, access::sample> baseColorTexture [[texture(0)]],
                               sampler baseColorSampler [[sampler(0)]])
{
    float3 unit_normal = normalize(interpolated.normal.xyz);
    float3 unit_to_light = normalize(interpolated.toLight.xyz);
    float3 unit_to_camera = normalize(interpolated.toCamera.xyz);
    
    float3 light_direction = -unit_to_light;
    float3 reflected_light_direction = reflect(light_direction, unit_normal);
    
    // diffuse
    float n_dot_l = dot(unit_normal, unit_to_light);
    float n_dot_l_max = max(n_dot_l, 0.0);
    
    float3 diffuse_color = n_dot_l_max * uniforms.lightColor;
    
    // specular
    float r_dot_c = dot(reflected_light_direction, unit_to_camera);
    float specular_factor = max(r_dot_c, 0.0);
    specular_factor = pow(specular_factor, uniforms.shininess);
    float3 specular_color = specular_factor * uniforms.lightColor * uniforms.reflectivity;
    
    //float4 baseColor = baseColorTexture.sample(baseColorSampler, interpolated.texCoord).rgba;
    return float4(diffuse_color, 1.0) * interpolated.color + float4(specular_color, 1.0);
}

// ------------------------ Lighting section -------------------------------------
struct vertex_lighting
{
    simd::float4 pos       [[attribute(0)]];
    simd::float4 color     [[attribute(1)]];
    simd::float2 texCoord  [[attribute(2)]];
};

struct vertex_lighting_out
{
    float4 color;
    float4 pos [[position]];
    float2 texCoord;
};

struct fragment_shader_args
{
    texture2d<float, access::sample> texture    [[ id(0) ]];
    sampler texture_sampler                     [[ id(1) ]];
};

vertex vertex_lighting_out vertex_func_lighting(vertex_lighting in [[stage_in]],
                                                constant VertexUniforms &uniforms [[buffer(3)]],
                                                constant PerInstanceUniforms *perInstanceUniforms [[buffer(4)]],
                                                ushort iid [[instance_id]])
{
    float4x4 instanceModelMatrix = perInstanceUniforms[iid].modelMatrix;
    
    vertex_lighting_out out;
    
    // Pass the vertex color directly to the rasterizer
    out.color = in.color;
    // Pass the already normalized screen-space coordinates to the rasterizer
    out.pos = uniforms.projection * uniforms.view * instanceModelMatrix * in.pos;
    out.texCoord = in.texCoord;
    
    return out;
}

fragment float4 fragment_func_lighting(         vertex_lighting_out     interpolated        [[ stage_in ]],
                                        device  fragment_shader_args&   arguments           [[ buffer(0) ]])
{
    sampler s = arguments.texture_sampler;
    float4 color = arguments.texture.sample(s, interpolated.texCoord);
    return color;
}
