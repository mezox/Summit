#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct shadow_pass_output
{
    float4 position [[position]];
};

struct vertex_positions
{
    simd::float4 position;
};

struct shadow_pass_uniforms
{
    simd::float4x4 view;
    simd::float4x4 projection;
    simd::float4x4 inverseView;
};

vertex shadow_pass_output shadow_vertex_func(const device vertex_positions*     positions [[ buffer(0) ]],
                                             constant     shadow_pass_uniforms& uniforms  [[ buffer(3) ]],
                                             uint                               vid       [[ vertex_id ]])
{
    shadow_pass_output out;
    out.position = uniforms.projection * uniforms.view * float4(positions[vid].position);

    return out;
}
