import Foundation

enum GraphicsAPI : String {
    case Metal
    case Vulkan
    case OpenGL
    case DirectX
}

public protocol RendererDelegate : NSObjectProtocol {
    func getGraphicsAPI() -> String
    func getActiveDeviceDescription() -> String
}
