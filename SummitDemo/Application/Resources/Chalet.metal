#include <metal_stdlib>
using namespace metal;

#include <simd/simd.h>

struct vertex_uniforms
{
    simd::float4x4 view;
    simd::float4x4 projection;
    simd::float4x4 inverseView;
};

struct vertex_attributes
{
    float4 position     [[attribute(0)]];
    float4 normal       [[attribute(1)]];
    float2 tex_coord    [[attribute(2)]];
};

struct transformed_vertex
{
    float4 position [[position]];
    float4 normal;
    float2 tex_coord;
    float3 to_light;
    float3 to_camera;
};

// Vertex shader
vertex transformed_vertex vertex_func_chalet(vertex_attributes in [[stage_in]],
                                             constant vertex_uniforms &uniforms [[buffer(3)]])
{
    float4 world_position = in.position;
    
    transformed_vertex transformed;
    transformed.position = uniforms.projection * uniforms.view * world_position;
    transformed.normal = float4(in.normal.xyz, 0.0);
    transformed.tex_coord = in.tex_coord;
    
    float3 light_position = float3(0.0, 2000.0, 500.0);
    
    transformed.to_light = light_position - world_position.xyz;
    transformed.to_camera = world_position.xyz - (uniforms.inverseView * float4(0.0, 0.0, 0.0, 1.0f)).xyz;
    
    return transformed;
}

struct fragment_shader_args
{
//    texture2d<float, access::sample> texture    [[ id(0) ]];
    sampler texture_sampler                     [[ id(0) ]];
};

struct fragment_uniforms
{
    float brightness;
    float reflectivity;
    float shininess;
    simd::float3 light_color;
};

// Fragment shader
fragment float4 fragment_func_chalet(           transformed_vertex      interpolated        [[ stage_in ]],
                                       constant fragment_uniforms&      uniforms            [[ buffer(1) ]],
                                       device   fragment_shader_args&   arguments           [[ buffer(0) ]],
                                       texture2d<float, access::sample> baseColorTexture    [[ texture(0) ]])
{
    sampler s = arguments.texture_sampler;
    float4 albedo_color = baseColorTexture.sample(s, interpolated.tex_coord);
    
    float3 unit_normal = normalize(interpolated.normal.xyz);
    float3 unit_to_light = normalize(interpolated.to_light.xyz);
    float3 unit_to_camera = normalize(interpolated.to_camera.xyz);
    
    float3 light_direction = -unit_to_light;
    float3 reflected_light_direction = reflect(light_direction, unit_normal);
    
    // diffuse
    float n_dot_l = dot(unit_normal, unit_to_light);
    float n_dot_l_max = max(n_dot_l, 0.0);
    
    float3 diffuse_color = n_dot_l_max * uniforms.light_color;
    
    // specular
    float r_dot_c = dot(reflected_light_direction, unit_to_camera);
    float specular_factor = max(r_dot_c, 0.0);
    specular_factor = pow(specular_factor, uniforms.shininess);
    float3 specular_color = specular_factor * uniforms.light_color * uniforms.reflectivity;
    
    return albedo_color * float4(diffuse_color, 1.0);// + float4(specular_color, 1.0);
}
