
#ifndef Equation_H
#define Equation_H
#include <string>
#include <vector>
#include <map>

#include <iostream>


typedef std::map<std::string, double> var;
class Equation {
public:
	static const double Infinity;
	enum Operator {
		P='+',M='-',X='*',D='/'
	};
	Equation(std::string str = "");
	~Equation();
	var compute_show();
	std::string calc() throw(const char *);
	double eval() throw(const char *);

	static void removeSpaces(std::string &str);
	operator const char *(){
		if(LH && RH)
			return (LH->getStr() +"="+ RH->getStr()).c_str();
		if(LH)
			return (LH->getStr() + "=0").c_str();
		if(RH)
			return (RH->getStr() + "=0").c_str();
		return "";
	}
	void print() const{
		LH->print();
	}
	
private:
	struct Node {
	public:
		Node(std::string d="", Node *a=0, Node *b=0);
		~Node();
		/**
		 * cleans data in begining and end of data variable
		 */
		void clean();
		/**
		 * prints the equation on screen by calling getStr();
		 */
		void print() const;

		std::string vStr() const throw(const char *);
		/** 
		 * calculates the value of the equation term by term
		 * @return the calculated value
		 */
		double value() const throw(const char *);
		/**
		 * uses data variable for the return statement.
		 * @return the string that this object has been created with(ie a term)
		 */
		std::string Data() const;
		/**
		 * this will collect all the terms in the equation one by one and 
		 * concat them to a std::string
		 * @return the concated string
		 */
		std::string getStr() const;
		operator const char*(){
			return getStr().c_str();
		}

		/**
		 * 	this will return the term at the position i when i separated by + or -.
		 * 	
		 * @unsupported
		 * @param  i position i will be the term returned
		 * @return   the string representation of this term
		 */
		std::string get(int i=0);
		Node *a,*b;
		std::string data;
		Operator op;
	};
	Node *LH, *RH;
};

#endif
