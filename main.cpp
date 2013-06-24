#include <iostream>
#include <string>
#include "args/ArgHandler.h"
#include <fstream>

#include "Operators/Equation.h"

typedef unsigned int uint;

int help(){
	std::cout << "Usage: equ_solver \e[1mequation\e[m [OPTION]...\n" 
			  << "Solves a equation. It can only solve linear equation systems!\n"
			  << " Made by: Johannes Lindén \n\n"
			  << "This is the options that can be passed to the program:\n"
			  << "      --version|-v           output version information and exits\n" 
			  << "      --help|-h              shows this help and exits\n\n"
			  << "      (--equation |-e)\e[1mE\e[m      Solves the equation given by \e[1mE\e[m\n"
			  << "      (--input |-i)\e[1mfilein\e[m    reads the equation(s) from given \e[1mfilein\e[m and prints\n"
			  << "                             the solution to standard out\n"
			  << "      \n"
			  << " Exit status: \n"
			  << "      0  if OK\n"
			  << "      1  if invalid input.\n";
	return 0;
}



int main(int argc, char *argv[]) {

	try{
		ArgHandler arg;
		arg.setOption("version", no_argument, 0, 'v');
		arg.setOption("help", no_argument, 0, 'h');
		arg.setOption("equation", required_argument, 0, 'e');
		arg.setOption("input", required_argument, 0, 'i');
		if(argc<2 || arg.loadOptions(argc, argv) != 0 || arg.getOption("help").isPassedArgument()){
			return help();
		}
		if(arg.getOption("version").isPassedArgument()){
			std::cout << "Version 1.0\n";
			return 0;
		}
		ArgHandler::ExOption op;
		std::string strEq = argv[1];
		op = arg.getOption("equation");
		if(op.isPassedArgument())
			strEq = op.getString();
		op = arg.getOption("input");
		if(op.isPassedArgument()){
			std::ifstream in(op.getString().c_str());
			if(!in.good()){
				std::cout << "could not find the file: " << op.getString() << "\n";
				return 0;
			}
			// while(!in.eof()){
			// 	std::getline(in, strEq);
			// 	eqs.push_back(strEq);
			// }
			return 0;
		}
		Equation o(strEq);//("2c(x+2)+(x+2)c3+x^3-2(3-cx)c + 2*c*(x+2)+(x+2)*2*c + 2*(x+2)*c");
		std::cout << o << "=" << (o.calc()) << "\n";
	}
	catch(const char *e){
		std::cout << e << std::endl;
	}
	catch(int i){
		std::cout << i << "\n";
	}
	return 0;
}