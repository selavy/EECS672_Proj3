CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -lmvc
OGL_LIBRARIES = -lglut -lGLU -lGL

OBJS = project3.o MatrixIF.o GeneralMV.o Square.o Board.o Pawn.o King.o Cylinder.o Sphere.o Bishop.o

all: project3
	./project3

project3: $(OBJS) ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
	$(LINK) -o project3 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project3.o: project3.c++
	$(CPP) $(C_FLAGS) project3.c++
GeneralMV.o: GeneralMV.h GeneralMV.c++
	$(CPP) $(C_FLAGS) GeneralMV.c++
MatrixIF.o: MatrixIF.h MatrixIF.c++
	$(CPP) $(C_FLAGS) MatrixIF.c++
Square.o: Square.h Square.c++
	$(CPP) $(C_FLAGS) Square.c++
King.o: King.h King.c++
	$(CPP) $(C_FLAGS) King.c++
Pawn.o: Pawn.h Pawn.c++
	$(CPP) $(C_FLAGS) Pawn.c++
Board.o: Board.h Board.c++
	$(CPP) $(C_FLAGS) Board.c++
Cylinder.o: Cylinder.h Cylinder.c++
	$(CPP) $(C_FLAGS) Cylinder.c++
Sphere.o: Sphere.h Sphere.c++
	$(CPP) $(C_FLAGS) Sphere.c++
Bishop.o: Bishop.h Bishop.c++
	$(CPP) $(C_FLAGS) Bishop.c++

clean:
	rm -f *~ *.o project3


