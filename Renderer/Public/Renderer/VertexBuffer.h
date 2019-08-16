#pragma once

#include <Renderer/Buffer.h>
#include <Renderer/Renderer.h>
#include "RendererResource.h"

#include <vector>
#include <cstdint>
#include <type_traits>
#include <tuple>
#include <functional>

namespace Summit::Renderer
{    
    enum class VertexDataInputRate
    {
        Vertex,
        Instance
    };
    
    enum class DiscardMode
    {
        None,
        Host,
        Device,
        HostAndDevice
    };
    
    struct StreamDataDescriptor
    {
        void* data{ nullptr };
        uint32_t stride{ 0 };
        uint32_t count{ 0 };
        BufferId resourceId;
    };
    
    class VertexBufferStreamBase : public IRendererResource
    {
    public:
        VertexBufferStreamBase() = default;
        virtual ~VertexBufferStreamBase() = default;
        
        VertexBufferStreamBase(const VertexBufferStreamBase& other) = delete;
        VertexBufferStreamBase(VertexBufferStreamBase&& other) = delete;
        
        VertexBufferStreamBase& operator=(const VertexBufferStreamBase& other) = delete;
        VertexBufferStreamBase& operator=(VertexBufferStreamBase&& other) = delete;
        
        /*!
         @brief Stream data descriptor getter.
         @return Vertex data stream descriptor.
         */
        [[nodiscard]] const StreamDataDescriptor& GetDataDescriptor() const noexcept { return mDataDescriptor; }
        
        /*!
         @brief Vertex data input rate getter.
         @return Rate at which are vertices treated.
         */
        [[nodiscard]] VertexDataInputRate GetVertexInputRate() const noexcept { return mInputRate; }
        
        /*!
         @brief Stream data usage getter.
         @return Usage of this vertex buffer data stream.
         */
        [[nodiscard]] BufferUsage GetUsage() const noexcept { return mUsage; }
        
        /*!
         @brief Stream data usage setter.
         @param usage Usage of this vertex buffer data stream.
         */
        void SetBufferUsage(BufferUsage usage) noexcept { mUsage = usage; }
        
        
        void SetRenderer(IRenderer* renderer) { mRenderer = renderer; }
        
        void SetMemoryType(MemoryType type) { mMemoryType = type; }

        // IRendererResource
        void InitializeDeviceResource(IRenderer& renderer) override;
        void ReleaseDeviceResource(IRenderer& renderer) override;
        
    protected:
        BufferUsage mUsage{ BufferUsage::VertexBuffer };
        VertexDataInputRate mInputRate{ VertexDataInputRate::Vertex };
        StreamDataDescriptor mDataDescriptor;
        IRenderer* mRenderer{ nullptr };
        MemoryType mMemoryType{ MemoryType::DeviceLocal };
    };

    template<typename Data>
    class VertexBufferStream final : public VertexBufferStreamBase
    {
    public:
        VertexBufferStream() = default;
        
        template<typename ...Ts>
        auto& AddData(Ts&&... ts)
        {
            (mData.push_back(std::forward<Ts>(ts)), ...);
            return *this;
        }

        [[nodiscard]] uint32_t GetElementCount() const noexcept
        {
            return static_cast<uint32_t>(mData.size());
        }

        [[nodiscard]] uint32_t GetSize() const noexcept
        {
            return static_cast<uint32_t>(mData.capacity() * sizeof(Data));
        }
        
        [[nodiscard]] uint32_t GetStride() const noexcept
        {
            return static_cast<uint32_t>(sizeof(Data));
        }

        [[nodiscard]] auto& GetData() noexcept
        {
            return mData;
        }

        void Unlock(bool discardDevice = false)
        {
            mDataDescriptor.count = static_cast<uint32_t>(mData.size());
            mDataDescriptor.stride = static_cast<uint32_t>(sizeof(Data));
            mDataDescriptor.data = mData.data();

            InitializeDeviceResource(*mRenderer);
        }
        
        void Discard(DiscardMode mode)
        {
            // TODO: Hack to prevent discard on GPU on first frame when buffers are not created
            //          Its an issue with gui.
            if(mData.empty())
                return;
            
            if(mode == DiscardMode::Device || mode == DiscardMode::HostAndDevice)
            {
                ReleaseDeviceResource(*mRenderer);
            }
            
            if(mode == DiscardMode::Host || mode == DiscardMode::HostAndDevice)
            {
                std::vector<Data> tmp{};
                std::swap(mData, tmp);
                mDataDescriptor = {};
            }
        }

    private:
        std::vector<Data> mData;
    };

    class VertexBufferBase
    {
    public:
        VertexBufferBase();
        virtual ~VertexBufferBase() = default;
        
        virtual uint32_t GetDataStreamCount() const noexcept = 0;
        virtual std::vector<StreamDataDescriptor> GetDataDescriptors() const noexcept = 0;
        
        StreamDataDescriptor GetIndicesDescriptor() const noexcept;
        VertexBufferStream<uint32_t>& GetIndexStream() noexcept;
        
    protected:
        VertexBufferStream<uint32_t> mIndexStream;
    };

    template<typename... Ts>
    class VertexBuffer : public VertexBufferBase
    {
        using VertexStreamHandler = std::function<void(VertexBufferStreamBase&)>;
        
    public:
        VertexBuffer() = default;
        
        VertexBuffer(VertexBuffer&&) = default;
        VertexBuffer(const VertexBuffer&) = delete;
        
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = default;

        template<size_t N>
        auto& GetDataStream()
        {
            static_assert(N < sizeof...(Ts), "Vertex stream index out of bounds");
            return std::get<N>(mDataStreams);
        }
        
        uint32_t GetDataStreamCount() const noexcept override
        {
            return sizeof...(Ts);
        }
        
        std::vector<StreamDataDescriptor> GetDataDescriptors() const noexcept override
        {
            std::vector<StreamDataDescriptor> descs;
            descs.reserve(sizeof...(Ts));
            
            std::apply([&descs](const auto&... stream) {
                (descs.push_back(stream.GetDataDescriptor()), ...);
            }, mDataStreams);
            
            return descs;
        }
        
        void SetRenderer(IRenderer* renderer)
        {
            ForEachStream([renderer](VertexBufferStreamBase& stream){
                stream.SetRenderer(renderer);
            });
        }
        
        void SetMemoryType(MemoryType type)
        {
            ForEachStream([type](VertexBufferStreamBase& stream){
                stream.SetMemoryType(type);
            });
        }
        
    private:
        void ForEachDataStream(VertexStreamHandler&& handler)
        {
            std::apply([&handler](auto&... stream) {
                (handler(stream), ...);
            }, mDataStreams);
        }
        
        void ForEachStream(VertexStreamHandler&& handler)
        {
            std::apply([&handler](auto&... stream) {
                (handler(stream), ...);
            }, mDataStreams);
            
            handler(mIndexStream);
        }
        
    private:
        std::tuple<VertexBufferStream<Ts>...> mDataStreams;
    };
}
