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

int main(int argc, char** argv)
{
    Load();
    Unload();
    
    return 0;
}
