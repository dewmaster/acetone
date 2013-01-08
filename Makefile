.SUFFIXES: .c .o
CC = gcc
EXEC = truth
CCFLAGS =  -g
OBJS = sMath.o truth.o acetone.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -lm -o ${EXEC} ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c $<

run: ${EXEC}
	./${EXEC}
    
clean:
	rm -f ${EXEC} ${OBJS}

sMath.o: sMath.c sMath.h
truth.o: truth.c truth.h
acetone.o: acetone.c
