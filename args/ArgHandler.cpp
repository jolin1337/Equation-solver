/**
 * This code was made by Johannes Lindén
 *  On Mittuniversitetet Sundsvall Universitet
 *  As an projekt to the course ObjectBaserad Programmering
 *  Contact: johannes.93.1337@gmail.com if there is any questions about contribution, modifying or sharing of this code


 *
**/
#include "ArgHandler.h"
#include <iostream>
#include <iomanip>

ArgHandler::ArgHandler():argsc(0) {}

int ArgHandler::loadOptions(int argc, char *argv[]){
	this->argsc = argc-1;

	uint optLength = optionsM.size();								// get amount of options
	if(optLength == 0)												// if there is no options, return
		return EXIT;
	option *ops = new option[optLength];							// allocate the amount of options for parsing
	ExOption *tmp;													// the current option we will look at

	std::string flag = "", 											// flag is the flag passed in to getopt function
				args;												// args is the options prefix to get the option 
																	// from optionsM
	std::map<std::string, ExOption>::iterator it = optionsM.begin();// start op optionsM
	for (int i = 0; i < optLength; i++, it++){						// iterate through all the options avalable	
		ops[i] = it->second;										// sets option i 
		if(ops[i].val == 0){
			i--;
			optLength--;
			continue;
		}

		flag += ops[i].val;											// add the new option to flag
		args += ops[i].val;											// add the new option to args
		if(ops[i].val && ops[i].has_arg != no_argument)		// if this option needs an argument
			flag += ':';											// add ':' to flag
	}

	char c;															// c will become the prefix of an option passed 
																	// by user
	int optionIndex = 0;											// will be the current option index
	while ((c = getopt_long(argc, argv, flag.c_str(), ops, &optionIndex)) != -1) {
		uint ipos = args.find_first_of(c);							// if c is a valid flag
		if(c != '?' && c != ':' && ipos != std::string::npos){		// and c is not ? or :
			if(optarg && optarg[0] == '-')
				return ABORT;
			tmp = &optionsM[ops[ipos].name];						// get the flag
			tmp->isPassedArg = true;								// set the isPassedArg variable to true
			tmp->optarg = optarg;									// set the optarg variable to optarg
		}
	}
	delete[] ops;													// delete the temporarly created memory
	return CONTINUE;												// return CONTINUE
}


ArgHandler::ExOption ArgHandler::getOption(std::string name) {
	return optionsM[name];											// return the option name if it exist
}
void ArgHandler::setOption(option op){
	optionsM[op.name] = ExOption(op);								// sets the op.name option
}

void ArgHandler::setOption(std::string name, int hasArg, int *flag, int val){
	optionsM[name] = ExOption(name, hasArg, flag, val);				// sets the name option with the given 
																	// properties
}


// ExOption declarations:

ArgHandler::ExOption::ExOption():isPassedArg(false), optarg(0) {
	// set every thing in option to 0 or 0
	name = 0;
	has_arg = 0;
	flag = 0;
	val = 0;
}
ArgHandler::ExOption::ExOption(option op):isPassedArg(false), optarg(0) {
	// copy the op parameter to this object
	name = op.name;
	has_arg = op.has_arg;
	flag = op.flag;
	val = op.val;
}
ArgHandler::ExOption::ExOption(std::string name, int hasArg, int *flag, int val):isPassedArg(false), optarg(0) {
	// copy the parameters to this object
	this->name = name.c_str();
	this->has_arg = hasArg;
	this->flag = flag;
	this->val = val;
}


int ArgHandler::ExOption::getValue(){
	if(optarg != 0 && optarg[0] >= '0' && optarg[0] <= '9')		// if first pos in this argument is a number
		return atoi(optarg);										// try to convert it to an integer
	throw ABORT;													// else throw this ABORT, OPeration-code error
}
std::string ArgHandler::ExOption::getString(){
	if(!optarg) return "";
	return (std::string)optarg;										// return optarg as an string
}
bool ArgHandler::ExOption::isPassedArgument(){
	return isPassedArg;												// return if this option has been passed by the user
}