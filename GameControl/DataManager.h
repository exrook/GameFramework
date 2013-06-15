#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "GameControl.h"
#include "GameLoop.h"

namespace GameControl {

class DataManager {
  protected:
    GameLoop* currentGL;
  public:
    /// Run by App before the first GameLoop is run
    virtual const int init()=0;
    
    virtual const bool running();
    
    virtual GameLoop * const firstGameLoop(App* parent)=0;
};

}
#endif /*_DATA_MANAGER_H_*/
