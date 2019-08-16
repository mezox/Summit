#include <metal_stdlib>

using namespace metal;

struct VertexIn
{
    float2 position  [[attribute(0)]];
    float2 texCoords [[attribute(1)]];
    uchar4 color     [[attribute(2)]];
};

struct VertexOut
{
    float4 position [[position]];
    float2 texCoords;
    float4 color;
};

struct vertex_uniform_viewport
{
    float4x4 projectionMatrix;
};

vertex VertexOut vertex_main(VertexIn in [[stage_in]],
                             constant vertex_uniform_viewport &viewport_uniforms [[buffer(3)]])
{
    VertexOut out;
    
    out.position = viewport_uniforms.projectionMatrix * float4(in.position, 0, 1);
    out.texCoords = in.texCoords;
    out.color = float4(in.color) / float4(255.0);
    return out;
}

struct fragment_shader_args
{
    texture2d<float, access::sample> texture    [[ id(0) ]];
    sampler texture_sampler                     [[ id(1) ]];
};

fragment float4 fragment_main(VertexOut in [[stage_in]],
                              device fragment_shader_args&      arguments           [[ buffer(0) ]])
{
    float4 tex = arguments.texture.sample(arguments.texture_sampler, in.texCoords);
    return in.color * tex;
}
