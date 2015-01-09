.SUFFIXES: .cc .o
CC = g++
EXEC = makeExec
CCFLAGS = -Wall
OBJS = DirectoryExplorer.o Vertex.o MazeAlgorithms.o FileMazeGenerator.o

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -o ${EXEC} ${OBJS}
	./${EXEC}

.cc.o:
	${CC} ${CCFLAGS} -c $<

run: ${EXEC}
	./${EXEC}
    
clean:
	rm -f ${EXEC} ${OBJS}

#line 9: automatic run after compile