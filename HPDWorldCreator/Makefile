CC=g++
INC=-I../Bullet/src \
	-I../Bullet/src/BulletDynamics \
	-I../Bullet/src/BulletCollision \
	-I../Bullet/src/LinearMath \
	-I../Bullet/Extras/Serialize/BulletWorldImporter \
	-I../Bullet/Extras/Serialize/BulletFileLoader

LIBDIR=-L../Bullet/build/Extras/Serialize/BulletWorldImporter \
	   -L../Bullet/build/Extras/Serialize/BulletFileLoader \
	   -L../Bullet/build/src/BulletDynamics \
	   -L../Bullet/build/src/BulletCollision \
	   -L../Bullet/build/src/LinearMath

LIBS=-Wl,-Bstatic -lBulletWorldImporter -lBulletFileLoader -lBulletDynamics  -lBulletCollision -lLinearMath -Wl,-Bdynamic

OPT=-O2

HPD: main.cpp $(SRC)
	$(CC) $(OPT) $(INC) $(LIBDIR) main.cpp -o WorldBuilder $(SRC) $(LIBS)
	
%.o: %.cpp %.h
	$(CC) $(OPT) $(INC) $(LIBDIR) $(LIBS) -c $<

clean:
	rm -f *.o
	rm -f WorldBuilder
