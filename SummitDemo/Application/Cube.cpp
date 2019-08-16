//#include "Cube.h"
//#include "Math.h"
//
//using namespace Summit;
//using namespace Summit::Renderer;
//
//Cube::Cube(Summit::IRenderer& renderer)
//{
//    // Fill vertex streams with data
//    auto& positionStream = mVertexBuffer.GetDataStream<0>().AddData(
//        // Quad 1
//        Vec4{ -0.5f, -0.5f, -0.5f, 1.0f },
//        Vec4{  0.5f, -0.5f, -0.5f, 1.0f },
//        Vec4{  0.5f, -0.5f,  0.5f, 1.0f },
//        Vec4{ -0.5f, -0.5f,  0.5f, 1.0f },
//
//        // Quad 2
//        Vec4{ -0.5f,  0.5f, -0.5f, 1.0f },
//        Vec4{  0.5f,  0.5f, -0.5f, 1.0f },
//        Vec4{  0.5f,  0.5f,  0.5f, 1.0f },
//        Vec4{ -0.5f,  0.5f,  0.5f, 1.0f }
//    );
//    
//    auto& colorStream = mVertexBuffer.GetDataStream<1>().AddData(
//        Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
//        Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },
//        Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
//        Vec4{ 1.0f, 1.0f, 0.0f, 1.0f },
//
//        Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
//        Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },
//        Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
//        Vec4{ 1.0f, 1.0f, 0.0f, 1.0f }
//    );
//    
//    auto& texCoordsStream = mVertexBuffer.GetDataStream<2>().AddData(
//        Vec2{ 1.0f, 1.0f },
//        Vec2{ 0.0f, 1.0f },
//        Vec2{ 0.0f, 0.0f },
//        Vec2{ 1.0f, 0.0f },
//
//        Vec2{ 1.0f, 1.0f },
//        Vec2{ 0.0f, 1.0f },
//        Vec2{ 0.0f, 0.0f },
//        Vec2{ 1.0f, 0.0f }
//    );
//    
//    auto& indexStream = mVertexBuffer.GetIndexStream().AddData(
//        0, 3, 2, 2, 1, 0, // bottom
//        4, 5, 6, 6, 7, 4, // top
//        4, 7, 3, 3, 0, 4, // left
//        6, 5, 1, 1, 2, 6, // right
//        5, 4, 0, 0, 1, 5, // back
//        7, 6, 2, 2, 3, 7  // forward
//    );
//    
//    mVertexBuffer.SetRenderer(&renderer);
//    
//    // Push data to GPU
//    positionStream.Unlock();
//    colorStream.Unlock();
//    texCoordsStream.Unlock();
//    indexStream.Unlock();
//}
//
//const VertexBufferBase* Cube::GetVertexBufferPointer() const
//{
//    return &mVertexBuffer;
//}
