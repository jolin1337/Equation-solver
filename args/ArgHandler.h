/**
 * This code was made by Johannes Lindén
 *  On Mittuniversitetet Sundsvall Universitet
 *  As an projekt to the course ObjectBaserad Programmering
 *  Contact: johannes.93.1337@gmail.com if there is any questions about contribution, modifying or sharing of this code


 *
**/
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include <string>
#include <map>
#ifndef ARGHANDLER_H
#define ARGHANDLER_H

typedef unsigned int uint;

class ArgHandler {
public:
	// OPeration-Codes
	enum OP{
		CONTINUE, 	// continue the loadOptions function like nothing has happend
		EXIT, 		// terminate the loadOptions function with normal exit
		ABORT		// terminate the loadOptions function with failure exit
	};
	struct ExOption;// Extended option class

	/**
	 * ArgHandler();
	 * constructs argsc to zero
	**/
	ArgHandler();

	/**
	 * int loadOptions(int argc, char *argv[]);
	 *     loads the options passed by user into the different possible options
	 *     in the program
	 * @return 0 if something went wrong 1 otherwise
	**/
	int loadOptions(int argc, char *argv[]);

	/**
	 * uint getArgCount() const
	 * @return the arguments count passed to loadOptions if that
	 *         function has been called, else 0;
	 */
	uint getArgCount() const{
		return argsc;
	}
	/**
	 * ExOption getOption(std::string name);
	 * @return a Extended option for the specific option name parameter
	**/
	ExOption getOption(std::string name);
	/**
	 * void setOption(option op);
	 *     copies the op option into an extended option object and stores it in 
	 *     optionsM
	**/
	void setOption(option op);
	/**
	 * void setOption(std::string name, int hasArg, int *flag, int val);
	 *     adds/sets the option with name name and adds the properties hasArg, 
	 *     *flag, val to it
	**/
	void setOption(std::string name, int hasArg, int *flag, int val);

	/**
	 * ExOption extends option
	 *     contains the results of the input args as well as the default data 
	 *     in the original struct
	**/
	struct ExOption : public option{
		/**
		 * ExOption();
		 *     sets all member variables to 0 or 0 or false
		**/
		ExOption();
		/**
		 * ExOption(option op);
		 *     copies all data from op to this extended option and sets the 
		 *     optarg and isPassedArg to their default values as in ExOption();
		**/
		ExOption(option op);
		/**
		 * ExOption(std::string name, int hasArg, int *flag, int val);
		 *     copies all parameters to option variables and then sets the
		 *     optarg and isPassedArg to their default values as in ExOption();
		**/
		ExOption(std::string name, int hasArg, int *flag, int val);

		/**
		 * int getValue();
		 *     returns an integer from the char *optarg variable. throws an 
		 *     error if it is not a number(NaN)
		**/
		int getValue();
		/**
		 * std::string getString();
		 * @return optarg converted to a string
		**/
		std::string getString();
		/**
		 * bool isPassedArgument();
		 * @return true if the user has passed this argument in the commandline, 
		 *         else false
		**/
		bool isPassedArgument();

		friend class ArgHandler;		// ArgHandler needs to set values of optarg and 
								// isPassedArg, so we make him a friend
	private:
		char *optarg;			// the argument passed by user
		bool isPassedArg;		// indicates if the user has specified this parameter
								// or not
	};
private:
	std::map<std::string, ExOption> optionsM;	// this map stores all the extended 
												// options with the key value as 
												// their names
	uint argsc;
};

#endif