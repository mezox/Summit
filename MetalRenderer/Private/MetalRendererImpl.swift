import Foundation

import Metal

public class MetalRenderer : NSObject, RendererDelegate {

    private let device : MTLDevice!
    private let commandQueue: MTLCommandQueue!
    
    @objc init?(str: String) {
        print("Instantiated Swift Metal Renderer")
        
        guard let graphicsDevice = MTLCreateSystemDefaultDevice() else {
            device = nil
            commandQueue = nil
            
            print("MetalRenderer: Metal is not supported on this device")
            return
        }
        
        device = graphicsDevice
        
        guard let queue = device.makeCommandQueue() else {
            commandQueue = nil
            print("MetalRenderer: Failed to create command queue")
            return
        }
        
        commandQueue = queue
    }
    
    @objc public func getGraphicsAPI() -> String {
        return GraphicsAPI.Metal.rawValue
    }
    
    @objc public func getActiveDeviceDescription() -> String {
        return device.name;
    }
}
