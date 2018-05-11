COMPILATEUR=gcc
SOURCE=main.c

Compile : main.c libfractal/fractal.c libfractal/tools.c
	@$(COMPILATEUR) -o main $(SOURCE) -pthread -lSDL libfractal/fractal.c libfractal/tools.c

lib :
	cd libfractal && $(MAKE)

clean :
	rm ./main
	cd libfractal	&& rm fractal.o
	cd libfractal	&& rm tools.o
	cd libfractal && rm libfractal.a
