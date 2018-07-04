make all:
	g++ -o t3 t3.cpp -lglut -lGLU -lGL -lm `pkg-config opencv --cflags --libs`
make run:
	./t3