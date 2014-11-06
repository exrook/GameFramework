#include "Space2D.h"
#include "GameUtilities.h"
#include "ControlInterfaces.h"
#include "SDLWrappers.h"

using namespace std;
using namespace jer;

class FPSDisplayer: public Displayable
{
private:
	const FPSManager *fps;
	
public:
	FPSDisplayer(const FPSManager * const fpsMan): fps(fpsMan) {};
	
public:
	const SUCCESS display() const override
	{
		cout<<"Current FPS: "<<fps->getFPS()<<endl;
		return SUCCEEDED;
	}
	
};

class EventHandler: public Events
{
private:
	PhysicsEngine * world;
	EasyDisplay * displayer;
	const Texture& image;
	const Texture& image2;
	const FPSManager * const fps;
	shared_ptr<PhysicalEntity> current;
	
	
public:
	virtual ~EventHandler() {};
	EventHandler(PhysicsEngine * const w, EasyDisplay * const d, const Texture& img, const Texture& img2, const FPSManager * const f): world(w), displayer(d), image(img), image2(img2), fps(f), current(nullptr) {};
	
public:
	const SUCCESS mouseButtonPressed(const Uint32 window, const Uint32 mouse, const Uint8 button, const Uint8 clicks, const Point<int> &mPos)
    {
        if(button == SDL_BUTTON_LEFT && clicks == 1)
		{
			current.reset(new PhysicalEntity(new ScaledTexture(image, Dimensions<int>(100, 100)), new PhysicalObject(100.0, mPos, new CircleCollidable(Point<double>(0, 0), 50), fps)));
		}
		else if(button == SDL_BUTTON_RIGHT && clicks == 1)
		{
			current.reset(new PhysicalEntity(new ScaledTexture(image2, Dimensions<int>(100, 100)), new PhysicalObject(100.0, mPos, new RectangleCollidable(Rectangle<double>(Point<double>(-50, -50), Dimensions<double>(100, 100))), fps)));
		}
		
		displayer->push_back(current);
		return SUCCEEDED;
    }
    
    const SUCCESS mouseButtonReleased(const Uint32 window, const Uint32 mouse, const Uint8 button, const Uint8 clicks, const Point<int> &mPos)
    {
        if(clicks == 1 && current != nullptr)
        {
			Point<double> d(Point<double>(mPos)-(current->getPhysicalObject()->getPosition()));
			if(button == SDL_BUTTON_RIGHT)
			{
				current->setImage(new ScaledTexture(image2, d*2.0));
				current->getPhysicalObject()->getCollider()->setOffset(d*-1.0);
				current->getPhysicalObject()->getCollider()->setDimensions(d*2.0);
				current->getPhysicalObject()->setMass(4.0*double(d.x()*d.y()));
			}
			else if(button == SDL_BUTTON_LEFT)
			{
				Scalar<double> rad(pythagoras(d));
				current->setImage(new ScaledTexture(image, Dimensions<double>(rad, rad)*2.0));
				current->getPhysicalObject()->getCollider()->setDimensions(Dimensions<double>(rad, rad));
				current->getPhysicalObject()->setMass(rad*rad*M_PI);
			}
			
			world->push_back(current->getPhysicalObject());
            current.reset();
		}
        return SUCCEEDED;
    }
    
    const SUCCESS mouseMove(const Uint32 window, const Uint32 mouse, const Delta<Point<int> > &movement,
                            const bitset<N_MOUSE_BUTTONS> &buttons)
    {
        if(current == nullptr)
            return SUCCEEDED;
        
		if(buttons[SDL_BUTTON_LEFT-1])
		{
			Scalar<double> rad(pythagoras(Point<double>(movement.get())-(current->getPhysicalObject()->getPosition())));
			current->setImage(new ScaledTexture(image, Dimensions<double>(rad, rad)*2.0));
		}
		else if(buttons[SDL_BUTTON_RIGHT-1])
		{
			Dimensions<double> d(Point<double>(movement.get())-current->getPhysicalObject()->getPosition());
			current->setImage(new ScaledTexture(image2, d*2.0));
		}
		
		return SUCCEEDED;
    }
	
	const SUCCESS keyPressed(const Uint32 window, const SDL_Keysym &key)
	{
		if(key.scancode == SDL_SCANCODE_Z && current == nullptr)
		{
			cout<<"Undoing"<<endl;
			if(displayer->size() > 1)
			{
				displayer->pop_back();
				world->pop_back();
			}
		}
		if(key.scancode == SDL_SCANCODE_ESCAPE)
		{
			return appExited();
		}
		return SUCCEEDED;
	}
};

int main(int argc, char **argv)
{
	string image(argc > 1? argv[1]:"circle.png");
	string image2(argc > 2? argv[2]:"test.png");
	
	CompositeProcess compProc;
    EasyData *data = new EasyData;
    
    FileManager *files = new FileManager;
    data->push_back(shared_ptr<FileManager>(files));
    
    shared_ptr<Window> win(new Window("Physics Woah", Point<int>(50, 50), Dimensions<int>(640, 480), Window::SHOWN | Window::RESIZABLE));
    shared_ptr<RedrawRenderer<HardRenderer> > ren(new RedrawRenderer<HardRenderer>(win, true, true));
    data->push_back(win);
    data->push_back(ren);
    
    Texture *tex = new Texture(files->access<Surface>(image), ren.get());
    data->push_back(shared_ptr<Texture>(tex));
	
	Texture *tex2 = new Texture(files->access<Surface>(image2), ren.get());
	data->push_back(shared_ptr<Texture>(tex2));
    
    EasyDisplay *displayer = new EasyDisplay;
    displayer->push_back(ren);
	
	PhysicsEngine *engine = new PhysicsEngine();
	
	EasyLoop *looper = new EasyLoop();
	looper->push_back(EventLoop::GetReference());
    looper->push_back(shared_ptr<PhysicsEngine>(engine));
	
	FPSManager *fps;
	looper->push_back(shared_ptr<FPSManager>(fps = new FPSManager(60, 200)));
	
	EventHandler handler(engine, displayer, *tex, *tex2, fps);
    Events::SetListener(&handler);
	
	displayer->push_back(shared_ptr<FPSDisplayer>(new FPSDisplayer(fps)));
	
    compProc.setLoader(data);
    compProc.setLooper(looper);
    compProc.setDisplayer(displayer);
    
    App::GetApp().setApp(&compProc);
    return App::GetApp().run();	
	
}
