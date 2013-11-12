CPP = g++
C_FLAGS = -fPIC -O -c -DGL_GLEXT_PROTOTYPES

LINK = g++ -fPIC

OBJS = Controller.o ModelView.o

libmvc.so: $(OBJS)
	$(LINK) -shared -o libmvc.so $(OBJS)
	cp libmvc.so ../lib/

Controller.o: Controller.h Controller.c++
	$(CPP) $(C_FLAGS) Controller.c++

ModelView.o: ModelView.h ModelView.c++
	$(CPP) $(C_FLAGS) ModelView.c++
