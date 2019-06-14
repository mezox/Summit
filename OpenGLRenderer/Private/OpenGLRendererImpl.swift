import Cocoa
import OpenGL.GL3

public class GLRenderer : NSObject, RendererDelegate {

    @objc init?(str: String) {
        print("Instantiated Swift GL Renderer")
    }
    
    @objc public func getGraphicsAPI() -> String {
        return GraphicsAPI.OpenGL.rawValue
    }
    
    @objc public func getActiveDeviceDescription() -> String {
        let attr = [
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAOpenGLProfile),
            NSOpenGLPixelFormatAttribute(NSOpenGLProfileVersion3_2Core),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAColorSize), 24,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAAlphaSize), 8,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADoubleBuffer),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADepthSize), 32,
            0
        ]
        
        let format = NSOpenGLPixelFormat(attributes: attr)
        var context = NSOpenGLContext(format: format!, share: nil)
        context?.makeCurrentContext()
        
        let vendor = String(cString: glGetString(GLenum(GL_VENDOR)))
        let renderer = String(cString: glGetString(GLenum(GL_RENDERER)))
        return String("\(vendor) \(renderer)")
    }
}
