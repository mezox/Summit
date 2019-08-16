import Cocoa
import OpenGL.GL3

public class GLRenderer : NSObject, RendererDelegate {

    private let context : NSOpenGLContext!
    
    @objc init?(str: String) {
        print("Instantiated Swift GL Renderer")
        
        let attr = [
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAOpenGLProfile),
            NSOpenGLPixelFormatAttribute(NSOpenGLProfileVersion4_1Core),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAColorSize), 24,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAAlphaSize), 8,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADoubleBuffer),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADepthSize), 32,
            0
        ]
        
        let format = NSOpenGLPixelFormat(attributes: attr)
        context = NSOpenGLContext(format: format!, share: nil)
        context?.makeCurrentContext()
    }
    
    @objc public func getGraphicsAPI() -> String {
        return GraphicsAPI.OpenGL.rawValue
    }
    
    @objc public func getActiveDeviceDescription() -> String {
        let vendor = String(cString: glGetString(GLenum(GL_VENDOR)))
        let renderer = String(cString: glGetString(GLenum(GL_RENDERER)))
        return String("\(vendor) \(renderer)")
    }
    
    @objc public func createBuffer(size: UInt32, data: UnsafeRawPointer) {
        
    }
    
    @objc func setup(view: NSView) {
        guard let glView = view as? NSOpenGLView else {
            print("View attached to ViewController is not an NSOpenGLView!")
            return
        }
        
        glView.needsDisplay = true
        
        glView.openGLContext = context
    }
}
