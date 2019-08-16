//#include <metal_stdlib>
//using namespace metal;
//
//#include <simd/simd.h>
//
//struct FragmentUniforms
//{
//    float brightness;
//};
//
//struct VertexUniforms
//{
//    simd::float4x4 mvp;
//};
//
//struct Vertex
//{
//    float4 pos       [[attribute(0)]];
//    float4 color     [[attribute(1)]];
//    float4 normal    [[attribute(2)]];
//};
//
//struct VertexOut
//{
//    float4 color;
//    float4 pos [[position]];
//    float4 normal;
//};
//
//vertex VertexOut vertexShader(Vertex in [[stage_in]], constant VertexUniforms &uniforms [[buffer(3)]])
//{
//    VertexOut out;
//    
//    // Pass the vertex color directly to the rasterizer
//    out.color = in.color;
//    // Pass the already normalized screen-space coordinates to the rasterizer
//    out.pos = uniforms.mvp * float4(in.pos.x, in.pos.y, in.pos.z, in.pos.w);
//    //out.texCoord = in.texCoord;
//    
//    return out;
//}
//
//fragment float4 fragmentShader(VertexOut interpolated [[stage_in]],
//                               constant FragmentUniforms &uniforms [[buffer(0)]],
//                               texture2d<float, access::sample> baseColorTexture [[texture(0)]],
//                               sampler baseColorSampler [[sampler(0)]])
//{
//    //float4 baseColor = baseColorTexture.sample(baseColorSampler, interpolated.texCoord).rgba;
//    return interpolated.color;
//}
//
