all: Mandelbrot.cpp
	g++ -c Mandelbrot.cpp -O3 -msse4.2 -mavx2 -mfma
	g++ Mandelbrot.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
