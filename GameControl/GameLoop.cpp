#include "DataManager.h"
#include "GameLoop.h"

namespace jer {

GameLoop::GameLoop(DataManager * const dataM, GameLoop * const parent) {
  DataM = dataM;
  Parent = parent;
}

GameLoop::GameLoop(DataManager &dataM, GameLoop &parent) {
  DataM = &dataM;
  Parent = &parent;
}

GameLoop::GameLoop(const GameLoop& other): DataM(other.DataM), Parent(other.Parent) {
}
/**
  Method called by App, passed with a pointer to App, which CurrentApp is
  set to. Then runs init(), then runs loop() and render() until Running
  is false, then runs cleanup(). Any events are polled after render(), and
  passed to their respective methods by handleEvents
*/
int const GameLoop::run() {
  SDL_Event event;
  if(init() < 0)
    return Status;
  while (Running && (DataM==NULL? true: DataM->running())) {
    while(SDL_PollEvent(&event)) {
      if(handleEvent(&event) != EVENT_SUCCESS)
	return Status;
    }
    if(loop() < 0)
      break;
    if(render() < 0);
      break;
  }
  cleanup();
  return Status;
}

}
