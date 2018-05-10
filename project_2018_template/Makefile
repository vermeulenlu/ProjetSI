COMPILATEUR=gcc
SOURCE=main.c

Compile : main.c libfractal/fractal.c libfractal/tools.c
	@$(COMPILATEUR) -o fractal $(SOURCE) -pthread -lSDL libfractal/fractal.c libfractal/tools.c

Clean :
	rm *.o
