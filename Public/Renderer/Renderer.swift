import Foundation

enum GraphicsAPI : String {
    case Metal
    case Vulkan
    case OpenGL
    case DirectX
}

enum BufferUsage {
    case VertexBuffer
}

struct BufferDescriptor {
    let usage : BufferUsage
    let size : UInt32
}

public protocol RendererDelegate : NSObjectProtocol {
    func getGraphicsAPI() -> String
    func getActiveDeviceDescription() -> String
    func createBuffer(size: UInt32, data: UnsafeRawPointer)
}
