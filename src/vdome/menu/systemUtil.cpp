#include "systemUtil.h"

#ifdef TARGET_LINUX
#elif defined(TARGET_OSX)
#include <Cocoa/Cocoa.h>
#elif defined(TARGET_WIN32)
#endif

namespace vd {

void systemUtil::setAppFocus() {
#ifdef TARGET_LINUX

#elif defined(TARGET_OSX)
  
    [NSApp activateIgnoringOtherApps:YES];
    NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
    if(appWindow) {
        [appWindow makeKeyAndOrderFront:appWindow];
    }
#elif defined(TARGET_WIN32)
#endif
    
}

void systemUtil::moveMouse(ofPoint pos){
    shared_ptr<ofAppGLFWWindow> glfw =
    static_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());
    glfwSetCursorPos(glfw->getGLFWWindow(), pos.x, pos.y);
}
    
}/////
