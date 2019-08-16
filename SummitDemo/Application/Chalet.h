#pragma once

#include <Renderer/Mesh.h>
#include <Renderer/Renderer.h>
#include <Renderer/Layer.h>
#include <Renderer/Camera.h>
#include <SummitAppKit/Window.h>

template<typename T>
class Range
{
public:
    class iterator {
        friend class Range<T>;
    public:
        T operator *() const { return mI; }
        const iterator &operator ++() { ++mI; return *this; }
        iterator operator ++(int) { iterator copy(*this); ++mI; return copy; }
        
        bool operator ==(const iterator &other) const { return mI == other.mI; }
        bool operator !=(const iterator &other) const { return mI != other.mI; }
        
    protected:
        iterator(T start) : mI(start) { }
        
    private:
        T mI;
    };
    
    iterator begin() const { return mBegin; }
    iterator end() const { return mEnd; }
    Range(T  begin, T end) : mBegin(begin), mEnd(end)
    {
        static_assert(std::is_integral<T>::value, "Integral required.");
    }
private:
    iterator mBegin;
    iterator mEnd;
};

class Chalet final : public Summit::Layer
{
public:
    Chalet(Summit::IRenderer& renderer);

    void OnShadowPass();
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void RenderGui() const override;
    
public:
    std::vector<Summit::Mesh> meshes;
    
private:
    Summit::IRenderer& mRenderer;    
    Summit::TextureId mChaletTextureId;
    Summit::TextureId mChaletTexture2Id;
    Summit::SamplerId mChaletSamplerId;
    Summit::PipelineId mChaletPipelineId;
    Summit::PipelineId mChaletPipeline2Id;

    // Shadow pass
    Summit::PipelineId mShadowPassPipeline;
    Summit::TextureId mShadowPassRenderTarget;
    Summit::BufferId mShadowPassMVP;

    Summit::Camera mShadowPassCamera;
};
