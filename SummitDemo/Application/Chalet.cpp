#include "Chalet.h"
#include "ObjLoader.h"
#include "tiny_obj_loader.h"

#include <Renderer/Effect.h>
#include <Renderer/Camera.h>

#include <unordered_map>

#import <Foundation/Foundation.h>
#import <CoreGraphics/CGImage.h>
#import <AppKit/NSImage.h>

#include <imgui/imgui.h>

using namespace Summit;

namespace
{
    Summit::TextureId createTextureWithPath(NSString* path, Summit::IRenderer& renderer)
    {
        NSImage *image = [NSImage imageNamed:path];
        CGImageRef imageRef = [image CGImage];
        
        // Create a suitable bitmap context for extracting the bits of the image
        NSUInteger width = CGImageGetWidth(imageRef);
        NSUInteger height = CGImageGetHeight(imageRef);
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        uint8_t *rawData = (uint8_t *)calloc(height * width * 4, sizeof(uint8_t));
        NSUInteger bytesPerPixel = 4;
        NSUInteger bytesPerRow = bytesPerPixel * width;
        NSUInteger bitsPerComponent = 8;
        CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                     bitsPerComponent, bytesPerRow, colorSpace,
                                                     kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        
        // Flip the context so the positive Y axis points down
        CGContextTranslateCTM(context, 0, height);
        CGContextScaleCTM(context, 1, -1);
        
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
        CGContextRelease(context);
        
        return renderer.CreateTexture2D(width, height, rawData);
    }
}

Chalet::Chalet(Summit::IRenderer& renderer)
    : Layer("Chalet")
    , mRenderer(renderer)
{
    fastObjMesh* mesh = fast_obj_read("/Users/kubovcikt/Dev/Summit/SummitDemo/Application/Resources/Temple.obj");
    
    std::vector<std::vector<Vertex>> verticesPerMaterial;
    verticesPerMaterial.resize(mesh->material_count);
    
    std::vector<std::unordered_map<Vertex, uint32_t>> uniqueVerticesPerMaterial;
    uniqueVerticesPerMaterial.resize(mesh->material_count);
    
    std::vector<std::vector<uint32_t>> indicesPerMaterial;
    indicesPerMaterial.resize(mesh->material_count);
    
    for (size_t meshIdx{ 0 }; meshIdx < mesh->group_count; ++meshIdx)
    {
        const fastObjGroup& grp = mesh->groups[meshIdx];
        
        int idx = 0;
        for (uint32_t faceId = 0; faceId < grp.face_count; ++faceId)
        {
            uint32_t faceVerticesCount = mesh->face_vertices[grp.face_offset + faceId];
            uint32_t faceMaterialId = mesh->face_materials[grp.face_offset + faceId];
            
            for (auto faceVertexId : Range<uint32_t>(0, faceVerticesCount))
            {
                std::ignore = faceVertexId;
                
                fastObjIndex indexGroup = mesh->indices[grp.index_offset + idx];
                
                Vertex v;

                if (indexGroup.p)
                {
                    v.position = Vec4(mesh->positions[3 * indexGroup.p + 0],
                                      mesh->positions[3 * indexGroup.p + 1],
                                      mesh->positions[3 * indexGroup.p + 2],
                                      1.0f);
                }
                
                if (indexGroup.t)
                {
                    v.texCoord = Math::Float2(mesh->texcoords[2 * indexGroup.t + 0],
                                      mesh->texcoords[2 * indexGroup.t + 1]);
                }
                
                if (indexGroup.n)
                {
                    v.normal = Vec4(mesh->normals[3 * indexGroup.n + 0],
                                    mesh->normals[3 * indexGroup.n + 1],
                                    mesh->normals[3 * indexGroup.n + 2],
                                    0.0f);
                }
                

                if (uniqueVerticesPerMaterial[faceMaterialId].count(v) == 0)
                {
                    uniqueVerticesPerMaterial[faceMaterialId][v] = static_cast<uint32_t>(verticesPerMaterial[faceMaterialId].size());
                    verticesPerMaterial[faceMaterialId].push_back(v);
                }
    
                indicesPerMaterial[faceMaterialId].push_back(uniqueVerticesPerMaterial[faceMaterialId][v]);
                
                idx++;
            }
        }
    }
    
    for (size_t materialId{ 0 }; materialId < mesh->material_count; ++materialId)
    {
        Summit::Mesh mesh;
        
        auto vb = std::make_unique<Summit::Renderer::VertexBuffer<Vec4, Vec4, Math::Float2>>();
    
        vb->SetRenderer(&renderer);
        
        auto& positionStream = vb->GetDataStream<0>();
        auto& normalsStream = vb->GetDataStream<1>();
        auto& texCoordStream = vb->GetDataStream<2>();
        auto& indexStream = vb->GetIndexStream();
    
        auto& positionData = positionStream.GetData();
        auto& normalsData = normalsStream.GetData();
        auto& texCoordData = texCoordStream.GetData();
        auto& indexData = indexStream.GetData();
        
        const auto& vertices = verticesPerMaterial[materialId];
        
        for(const auto& vertex : vertices)
        {
            positionData.push_back(vertex.position);
            normalsData.push_back(vertex.normal);
            texCoordData.push_back(vertex.texCoord);
        }
        
        const auto& indices = indicesPerMaterial[materialId];
        
        for(const auto& index : indices)
        {
            indexData.push_back(index);
        }
        
        positionStream.Unlock();
        normalsStream.Unlock();
        texCoordStream.Unlock();
        indexStream.Unlock();
        
        mesh.vertexBuffer.reset(vb.release());
        
        meshes.push_back(std::move(mesh));
    }
    
    // Prepare texture
    mChaletTextureId = createTextureWithPath(@"StructureBaseColor.png", renderer);
    mChaletTexture2Id = createTextureWithPath(@"FoliageBaseColor.png", renderer);
    mChaletSamplerId = renderer.CreateSampler({});
    
    PipelineDescriptor pipelineDesc;
    pipelineDesc.AddModule(ModuleStage::Vertex, "vertex_func_chalet");
    pipelineDesc.AddModule(ModuleStage::Fragment, "fragment_func_chalet");
    pipelineDesc.AddAttribute(Format::Float4, 0);     // positions
    pipelineDesc.AddAttribute(Format::Float4, 1);     // normals
    pipelineDesc.AddAttribute(Format::Float2, 2);     // texcoords
    //    chaletPipelineDesc.AddResource(UniformType::Texture, ModuleStage::Fragment, 0, chaletTextureId.id);
    pipelineDesc.AddResource(UniformType::Sampler, ModuleStage::Fragment, 0, mChaletSamplerId.id);
    pipelineDesc.SetPipelineName("StructurePipeline");
    
    mChaletPipelineId = renderer.CreatePipeline(pipelineDesc);

    meshes[0].textures.push_back({ mChaletTextureId, 0, 1 });
    meshes[1].textures.push_back({ mChaletTexture2Id, 0, 1 });

    // Create shadow pass pipeline
    PipelineDescriptor shadowPipelineDesc;
    shadowPipelineDesc.AddModule(ModuleStage::Vertex, "shadow_vertex_func");
    shadowPipelineDesc.SetPipelineName("Shadow Pass");
    shadowPipelineDesc.enableBlending = false;

    mShadowPassPipeline = renderer.CreatePipeline(shadowPipelineDesc);

    // Create depth state or use predefined

    // Create depth texture
    TextureDescriptor desc;
    desc.width = 2048;
    desc.height = 2048;

    mShadowPassRenderTarget = renderer.CreateTexture2D(desc);

    mShadowPassCamera.SetPosition(Math::Float3(500.0f, 200.0f, 0.0f));
    mShadowPassCamera.SetRotation(Math::Float3(Math::Float3(DegreesToRadians(-15.0f), DegreesToRadians(-90.0f), 0.0f)));
    mShadowPassCamera.SetPerspectiveProjection(60.0f, 1280.0f, 720.0f, 0.1f, 100000.0f);

    
}
    
void Chalet::OnAttach()
{
    
}

void Chalet::OnDetach()
{
    
}

void Chalet::RenderGui() const
{
    ImGui::ShowDemoWindow();
}

void Chalet::OnShadowPass()
{
//    mShadowPassCamera.Update();
//
//    Summit::RenderPassDescriptor rdesc;
//    rdesc.depthAttachment = mShadowPassRenderTarget;
//
//    mRenderer.BeginScene(mShadowPassCamera);
//    mRenderer.BeginRenderPass(rdesc, "ShadowPrePass");
//    mRenderer.RenderObject3d(meshes[0], mShadowPassPipeline);
//    mRenderer.RenderObject3d(meshes[1], mShadowPassPipeline);
//    mRenderer.EndRenderPass();
//    mRenderer.EndScene();
}

void Chalet::OnUpdate()
{
    mRenderer.RenderObject3d(meshes[0], mChaletPipelineId);
    mRenderer.RenderObject3d(meshes[1], mChaletPipelineId);
}
