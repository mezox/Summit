#include <Renderer/Renderer.h>
#include <dlfcn.h>

#include <iostream>
#include <string>

class Module
{
public:
    Module(const std::string& name)
        : mName(name)
    {}
    
    void Load()
    {
        if (!handle)
        {
            handle = dlopen(mName.c_str(), RTLD_LAZY | RTLD_LOCAL);
            if(!handle)
            {
                std::cout << "Failed to load module metal renderer" << std::endl;
                return;
            }
            
            std::cout << "Loaded module " << mName << std::endl;
        }
    }
    
    void Unload()
    {
        if(handle)
        {
            if (dlclose(handle))
            {
                std::cout << "Failed to unload module " << mName << std::endl;
                return;
            }
            
            handle = nullptr;
            
            std::cout << "Unloaded module " << mName << std::endl;
        }
    }
    
    void* GetFunctionPointer(const std::string& name)
    {
        if (!handle)
        {
            std::cout << "Failed to load function " << name << " from module \"" << mName << "\". Module not loaded" << std::endl;
            return nullptr;
        }
        
        auto funcPtr = dlsym(handle, name.c_str());
        if (!funcPtr)
        {
            std::cout << "Failed to load function " << name << " from module \"" << mName << "\": " << dlerror() << std::endl;
        }
        
        return funcPtr;
    }
    
public:
    std::string mName;
    void* handle{ nullptr };
};

typedef uint64_t(*FuncParam)();

int main(int argc, char** argv)
{
    Module metal("libMetalRenderer.dylib"), gl("libOpenGLRenderer.dylib");
    metal.Load();
    
    const auto createMetalRendererFunc = metal.GetFunctionPointer("createRenderer");
    FuncParam mf = (FuncParam)createMetalRendererFunc;
    
    const auto metalRenderer = (Summit::IRenderer*)mf();
    
    gl.Load();
    
    const auto createGLRendererFunc = gl.GetFunctionPointer("createRenderer");
    FuncParam gf = (FuncParam)createGLRendererFunc;
    
    const auto glRenderer = (Summit::IRenderer*)gf();
    
    gl.Unload();
    
    metal.Unload();
    
    return 0;
}
