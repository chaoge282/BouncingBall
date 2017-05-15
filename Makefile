CC        = g++
C	  = cpp
H	  = h

CFLAGS    = -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm
  endif
endif

HFILES =  Camera.${H} Vector.${H} Utility.${H}
OFILES =  Camera.o  Vector.o Utility.o

PROJECT   = bounceInBox

${PROJECT}: ${PROJECT}.o ${OFILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS} -lm

${PROJECT}.o: ${PROJECT}.${C} ${HFILES} ${INCFLAGS}
	${CC} ${CFLAGS} -c ${INCFLAGS} ${PROJECT}.${C}



Camera.o: Camera.${C} Camera.${H}  Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Camera.${C}
Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm -f core.* *.o *~ ${PROJECT}
