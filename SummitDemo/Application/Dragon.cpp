#include "Dragon.h"

#include <unordered_map>

#include "tiny_obj_loader.h"

Dragon::Dragon(Summit::IRenderer& renderer)
{
    mVertexBuffer.SetRenderer(&renderer);
    
//    tinyobj::attrib_t attrib;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    std::string warn, err;
//
//    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "/Users/tkubovcik/Downloads/dragon.obj"))
//    {
//        throw std::runtime_error(warn + err);
//    }
    
    auto& positionStream = mVertexBuffer.GetDataStream<0>();
    auto& colorStream = mVertexBuffer.GetDataStream<1>();
    auto& normalStream = mVertexBuffer.GetDataStream<2>();
    auto& indexStream = mVertexBuffer.GetIndexStream();
    
    auto& positionData = positionStream.GetData();
    auto& colorData = colorStream.GetData();
    auto& normalData = normalStream.GetData();
    auto& indexData = indexStream.GetData();
    
    std::vector<Vertex> vertices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
    
//    for (const auto& shape : shapes)
//    {
//        for (const auto& index : shape.mesh.indices)
//        {
//            Vertex v;
//            v.position = Vec4(
//                attrib.vertices[3 * index.vertex_index + 0],
//                attrib.vertices[3 * index.vertex_index + 1],
//                attrib.vertices[3 * index.vertex_index + 2],
//                              1.0f);
//            
//            v.color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
//            
//            v.normal = Vec4(
//                            attrib.normals[3 * index.vertex_index + 0],
//                            attrib.normals[3 * index.vertex_index + 1],
//                            attrib.normals[3 * index.vertex_index + 2],
//                            1.0f);
//            
//            if (uniqueVertices.count(v) == 0)
//            {
//                uniqueVertices[v] = static_cast<uint32_t>(vertices.size());
//                vertices.push_back(v);
//            }
//            
//            indexData.push_back(uniqueVertices[v]);
//        }
//    }
    
    for(const auto& vertex : vertices)
    {
        positionData.push_back(vertex.position);
        colorData.push_back(vertex.color);
        normalData.push_back(vertex.normal);
    }
    
    positionStream.Unlock();
    colorStream.Unlock();
    normalStream.Unlock();
    indexStream.Unlock();
}

const Summit::Renderer::VertexBufferBase* Dragon::GetVertexBufferPointer() const
{
    return &mVertexBuffer;
}
