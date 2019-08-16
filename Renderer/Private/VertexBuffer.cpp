#include <Renderer/VertexBuffer.h>

namespace Summit::Renderer
{
    void VertexBufferStreamBase::InitializeDeviceResource(IRenderer& renderer)
    {
        BufferDesc bufferDescriptor;
        bufferDescriptor.data = mDataDescriptor.data;
        bufferDescriptor.size = mDataDescriptor.stride * mDataDescriptor.count;
        bufferDescriptor.usage = mUsage;
        bufferDescriptor.memoryType = mMemoryType;
        
        mDataDescriptor.resourceId = renderer.CreateBuffer(bufferDescriptor);
    }
    
    void VertexBufferStreamBase::ReleaseDeviceResource(IRenderer& renderer)
    {
        renderer.ReleaseBuffer(mDataDescriptor.resourceId);
    }
    
    VertexBufferBase::VertexBufferBase()
    {
        mIndexStream.SetBufferUsage(Summit::BufferUsage::IndexBuffer);
    }
    
    StreamDataDescriptor VertexBufferBase::GetIndicesDescriptor() const noexcept
    {
        return mIndexStream.GetDataDescriptor();
    }
    
    VertexBufferStream<uint32_t>& VertexBufferBase::GetIndexStream() noexcept
    {
        return mIndexStream;
    }
}
