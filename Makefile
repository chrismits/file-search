###########################################################################
##                  Makefile for Proj2 (GERP)                            ##
###########################################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 -O2
LDFLAGS  = -g3


gerp: main.o stringProcessing.o WordHasher.o FSTree.o DirNode.o
	${CXX} ${LDFLAGS} -o gerp main.o stringProcessing.o \
							WordHasher.o FSTree.o DirNode.o

main.o: main.cpp WordHasher.h FSTree.o DirNode.o
stringProcessing.o: stringProcessing.h stringProcessing.cpp
WordHasher.o: WordHasher.h WordHasher.cpp 

clean:
	rm -rf gerp *.o *.dSYM

provide:
	provide comp15 proj2part3 README Makefile WordHasher.cpp \
								WordHasher.h main.cpp \
								entryStructs.h stringProcessing.h \
								stringProcessing.cpp commands.txt \
								testHasher.cpp