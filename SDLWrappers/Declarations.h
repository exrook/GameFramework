#ifndef _SDL_WRAPPER_DECLARATIONS_H_
#define _SDL_WRAPPER_DECLARATIONS_H_

#include "SDL.h"
#include "ControlInterfaces/Declarations.h"

namespace jer
{
	class App;   	
    /// Singleton class which encapsulates the SDL subsystem starts and quits on destruction, inherits Loadable and Runnable.
	
    template<class IMPLEMENTATION> class Logger;
    /// Templatized subclass of ControlInterface Loadable/Loopable/Displayable Implementations which logs errors from SDL.
}

#endif /*_SDL_WRAPPER_DECLARATIONS_H_*/