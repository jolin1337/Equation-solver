cpps = main.cpp args/ArgHandler.cpp Operators/Equation.cpp
header =        args/ArgHandler.h   Operators/Equation.h   Matrix/Matrix.h Matrix/Matrix.cpp
CPP = g++

all: equ_solver


equ_solver: $(cpps) $(header)
	$(CPP) -o equ_solver $(cpps)


run: equ_solver
	./equ_solver