import Foundation

public class GLRenderer : NSObject, RendererDelegate {

    @objc init?(str: String) {
        print("Instantiated Swift GL Renderer")
    }
    
    @objc public func getGraphicsAPI() -> String {
        return GraphicsAPI.OpenGL.rawValue
    }
}
