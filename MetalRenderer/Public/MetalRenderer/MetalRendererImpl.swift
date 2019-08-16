import Foundation

import Metal
import MetalKit

public class MetalRenderer : NSObject, RendererDelegate, MTKViewDelegate {

    private let device : MTLDevice!
    private let commandQueue: MTLCommandQueue!
    
    private var buffers: [MTLBuffer] = []
    
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
    
    @objc public func createBuffer(size: UInt32, data: UnsafeRawPointer) {
                
        let vbo = device.makeBuffer(bytes: data, length: Int(size), options: [])!
        buffers.append(vbo)
    }
    
    @objc func setup(view: NSView) {
        guard let mtkView = view as? MTKView else {
            print("View attached to ViewController is not an MTKView!")
            return
        }
        
        mtkView.device = device
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.delegate = self
        mtkView.clearColor = MTLClearColorMake(0.0, 180.0 / 255.0, 0.0, 1.0)
    }
    
    
    // MTKViewDelegate protocol
    public func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
    
    public func draw(in view: MTKView) {
        
        guard let commandBuffer = commandQueue.makeCommandBuffer() else {
            return
        }
        
        // Get the default MTLRenderPassDescriptor from the MTKView argument
        guard let renderPassDescriptor = view.currentRenderPassDescriptor else {
            return
        }
        
        // We compile renderPassDescriptor to a MTLRenderCommandEncoder.
        guard let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) else {
            return
        }

        renderEncoder.endEncoding()
//
//        // Change default settings. For example, we change the clear color from black to red.
//        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 180.0 / 255.0, 0.0, 1.0)
//
        // Tell Metal to send the rendering result to the MTKView when rendering completes
        commandBuffer.present(view.currentDrawable!)

        // Finally, send the encoded command buffer to the GPU.
        commandBuffer.commit()
    }
}
