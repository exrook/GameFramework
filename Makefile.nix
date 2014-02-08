CXXFLAGS = -g -I. -std=gnu++0x -fPIC -fPIE
LDFLAGS =  -lSDL -lSDL_image -lSDL_gfx -lm -rpath /usr/local/lib -g
LIBTOOL = libtool --tag=CXX

SOURCES = Space2D/Space2D.cpp Space2D/Point.cpp Space2D/Vector.cpp Space2D/Velocity.cpp \
  GameControl/Events.cpp GameControl/GameLoop.cpp GameControl/App.cpp \
  GameControl/DataManager.cpp SurfaceX/SurfaceX.cpp \
  SurfaceX/SimpleSurf.cpp  SurfaceX/SafeSurf.cpp SurfaceX/DoubleSurf.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
LIBOBJECTS = $(SOURCES:.cpp=.lo)
LIBRARY = libGameFrame.la


all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(LIBOBJECTS)
	$(LIBTOOL) --mode=link $(CXX) $(LDFLAGS) -o $@ $^

install:
	libtool --mode=install install -c libGameFrame.la /usr/local/lib/libGameFrame.la
	mkdir -p /usr/local/include/Space2D/ /usr/local/include/GameControl/ \
	/usr/local/include/SurfaceX /usr/local/include/Entity
	install -c Space2D/*.h /usr/local/include/Space2D/
	install -c GameControl/*.h /usr/local/include/GameControl/
	install -c SurfaceX/*.h /usr/local/include/SurfaceX/
	install -c Entity/*.h /usr/local/include/Entity/

%.lo: %.cpp
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean doc

clean:
	rm -f $(OBJECTS) $(LIBOBJECTS)
	rm -rf Space2D/.libs/ GameControl/.libs/ SurfaceX/.libs/ Entity/.libs/

doc: 
	rm -r doc/
	doxygen