#include <Renderer/Renderer.h>
#include <dlfcn.h>

#include <iostream>

void* metalRendererHandle{ nullptr };

void Load()
{
    if (!metalRendererHandle)
    {
        metalRendererHandle = dlopen("libMetalRenderer.dylib", RTLD_LAZY | RTLD_LOCAL);
        if(!metalRendererHandle)
        {
            std::cout << "Failed to load module metal renderer" << std::endl;
            return;
        }
        
        std::cout << "Loaded module metal renderer" << std::endl;
    }
}

void Unload()
{
    if(metalRendererHandle)
    {
        if (dlclose(metalRendererHandle))
        {
            std::cout << "Failed to unload module metal renderer" << std::endl;
            return;
        }
        
        metalRendererHandle = nullptr;
        
        std::cout << "Unloaded module metal renderer" << std::endl;
    }
}

void* GetFunctionPointer(const std::string& name)
{
    if (!metalRendererHandle)
    {
        std::cout << "Failed to load function " << name << " from module \"" << "metal renderer" << "\". Module not loaded" << std::endl;
        return nullptr;
    }
    
    auto funcPtr = dlsym(metalRendererHandle, name.c_str());
    if (!funcPtr)
    {
        std::cout << "Failed to load function " << name << " from module \"" << "metal renderer" << "\": " << dlerror() << std::endl;
    }
    
    return funcPtr;
}

typedef uint64_t(*FuncParam)();

int main(int argc, char** argv)
{
    Load();
    const auto createMetalRendererFunc = GetFunctionPointer("createRenderer");
    FuncParam f = (FuncParam)createMetalRendererFunc;
    
    const auto metalRenderer = (Summit::IRenderer*)f();
    
    Unload();
    
    return 0;
}
