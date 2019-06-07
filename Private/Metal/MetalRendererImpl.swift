import Foundation

public class MetalRenderer : NSObject, RendererDelegate {

    @objc init?(str: String) {
        print("Instantiated Swift Metal Renderer")
    }
    
    @objc public func getGraphicsAPI() -> String {
        return GraphicsAPI.Metal.rawValue
    }
}
