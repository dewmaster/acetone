.SUFFIXES: .c .o
CC = gcc
EXEC = truth
CCFLAGS =  -g
OBJS = truth.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -lm -o ${EXEC} ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c $<

run: ${EXEC}
	./${EXEC}
    
clean:
	rm -f ${EXEC} ${OBJS}

truth.o: truth.c