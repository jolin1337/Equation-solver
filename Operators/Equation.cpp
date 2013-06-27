
#include "Equation.h"
#include <sstream>
#include <algorithm>
#include <limits>


const double Equation::Infinity = std::numeric_limits<double>::infinity();

Equation::Equation(std::string str):LH(0), RH(0){
	if(str.empty())return;//throw "Empty string passed";
	removeSpaces(str);
	int p = 0;
	if((p=str.find("=")) == std::string::npos)
		LH = new Node(str);
	else{
		LH = new Node(str.substr(0, p));
		RH = new Node(str.substr(p+1));
	}
}
Equation::~Equation(){
	if(LH)
		delete LH;
	if(RH)
		delete RH;
}

var Equation::compute_show(){
	var words;
	std::string d = LH->data;
	std::string a = "a";
	for (int i = 0; i < d.size(); i++) {
		if(isalpha(d[i])){
			for (int j = i; j < d.size(); j++) {
				if(!isalpha(d[j])){
					std::string t = d.substr(i,j-i);
					if(!words[t]){
						words[t] = a[0];
						a[0]++;
					}
					std::string an = "a";
					an[0] = (char)words[t];
					d.replace(i,j-i,an);
					i++;
					break;
				}
				if(j+1 >= d.size()){
					std::string t = d.substr(i,j-i);
					if(t.empty())
						continue;
					d.replace(i,std::string::npos,a);
					words[t]++;
					i = j;
				}
			}
		}
	}
	std::string op = words.begin()->first;
	if(words.begin()->first == "lim"){
		// calc limit
		std::cout << "LIMIT CALC:...\n";
	}
	for (var::iterator it = words.begin(); it != words.end(); ++it) {
		std::cout << it->first <<"=" << (char)it->second << "\n"; 
	}
	return words;
}

std::string Equation::calc() throw(const char *){
	if(LH && RH)
		return Equation(LH->vStr() + "-" + RH->vStr()).calc();
	if(LH)
		return LH->vStr();
	if(RH)
		return RH->vStr();
	throw "Not defined equation";
}
double Equation::eval() throw(const char *){
	if(LH && RH)
		return (LH->value() - RH->value());
	if(LH)
		return LH->value();
	if(RH)
		return RH->value();
	throw "Not defined equation";
}

void Equation::removeSpaces(std::string &str){
	std::string temp = "";
	for(size_t i = 0; i < str.length(); i++)
		if(str[i] != ' ')
			temp += str[i];
	
	str = temp;
}




// Equation::Node


Equation::Node::Node(std::string d, Node *a, Node *b):a(a),b(b), data(d), op(P){
	if(d.empty() || a || b) return;//throw "Empty string passed";
	int fc = d.find_first_not_of(" ");
	if(fc != std::string::npos && d[fc] == '('){// remove paranteses
		int p = 1,
			j = fc+1;
		for (; j < d.size(); j++) {
			if(d[j] == '(')
				p++;
			if(d[j] == ')')
				p--;
			if(!p)
				break;
		}
		if(d.size()-1 <= j )
			d = d.substr(fc+1, j-1);
	}
	for (int i = 0; i < d.size(); i++) {
		if(d[i] == '('){	// jump over parantes
			int p = 1;
			for (int j = i+1; j < d.size(); j++) {
				if(d[j] == '(')
					p++;
				if(d[j] == ')')
					p--;
				if(!p){
					i = j;
					break;
				}
			}
		}
		if(i && (d[i] == '+' || d[i] == '-')){	// handle +/- operations
			if(d[i-1] == '*')continue;

			this->a = new Node(d.substr(0,i));
			this->b = new Node(d.substr(i));
			break;
		}
		if(i+1 >= d.size()){				// multiplication/division
			int j = d.find("(");
			if(j != std::string::npos){
				int l = d.find_first_not_of("+-*/"),
					m = d.find_first_of("*/");
				if(m != std::string::npos && d[m] == '/'){
					op = D;
					if(j>m)
						j--;
					if(l>m)
						l--;
					d.replace(m,1,"");
				}
				else
					op = X;
				if(l < j){
					this->a = new Node(d.substr(0,j));
					this->b = new Node(d.substr(j));
				}
				else{
					int p = 1;
					for (int k = j+1; k < d.size(); k++) {
						if(d[k] == '(')
							p++;
						if(d[k] == ')')
							p--;
						if(!p){
							p = k+1;
							break;
						}
					}
					this->a = new Node(d.substr(j, p-j));
					this->b = new Node(d.substr(p));
				}
				break;
			}
			else{
				this->a = new Node();
				this->a->data = d;
			}
		}
	}
	clean();
}

Equation::Node::~Node(){
	if(a)
		delete a;
	if(b)
		delete b;
}


void Equation::Node::clean(){
	if(data[0] == P || data[0] == X)
		data.replace(0,1,"");
	int s = data.size();
	if(data[s-1] == P || data[s-1] == X)
		data.replace(s-1,1,"");
	if(a)
		a->clean();
	if(b)
		b->clean();
}
void Equation::Node::print() const{
	std::cout << getStr();
	std::cout << "\n";
}
std::string Equation::Node::vStr() const  throw(const char *){
	var vals;
	double v = 0;
	std::string vv = "";
	if(!a && !b){
		std::string d = Data();
		int i = d.find_first_of("*/-+",1);
		while(d[i-1] == '*' && (d[i] == '-' || d[i] == '+'))
			i = d.find_first_of("*-+",i+1);
		char c = d[i];
		std::replace(d.begin(), d.end(), c, ' ');
		if(c == '*')
			v++;
		bool df = 0;
		std::stringstream ss(d);
		while(!ss.eof()){
			double tmp;
			char variable = '\0';
			if(!(ss >> tmp)){// TODO: Store variable from ss.str()
				std::string rem;
				ss.str(ss.str());
				ss.clear();
				double remRes = 1.0;
				std::string cc;
				while(!ss.eof()){
					if(!(ss >> tmp)){
						if(ss.eof())
							break;
						ss.clear();
						cc += ss.get();
						continue;
					}
					remRes *= tmp;
				}
				v -= remRes;
				std::stringstream ss2;
				ss2 << v;
				vv = ss2.str();
				ss2.str("");
				ss2.clear();
				ss2 << remRes;
				vv = ss2.str() + '*' + cc;
				return vv;
			}
			switch(c){
				case '*':
					v*=tmp;
					break;
				case '/':
					if(!df){
						v = tmp;
						df = true;
					}
					else{
						// if(tmp == 0)
						// 	throw "Division by zero!";
						v /= tmp;
					}
					break;
				default:
					v += tmp;
			}
			std::stringstream ss2;
			ss2 << v;				// TODO: add all variables as well!
			vv = ss2.str();
		}
	}
	else{
		std::string va = "", vb = "";
		if(a)
			va = a->vStr();
		if(b)
			vb = b->vStr();
		if(op == X && a && b)
			vv += va + "*" + vb;
		else if(op == D && a && b){
			std::string val = vb;	// TODO: separate under paranteses if posible!??
			// if(val == 0)
			// 	throw "Division by zero!";
			vv += va + "/" + val;
		}
		else{
			if(a)
				vv += "+" + va;
			if(b)
				vv += "+" + vb;
		}
		Node *tmp = new Node(vv);

		std::string vars = "";
		for (size_t i = 0; i < vv.length(); ++i)
			if(isalpha(vv[i])){
				int begin = vv.find_last_of("+-",i),
					end = vv.find_first_of("+-",i);
				while(begin > 0 && vv[begin] == '-' && vv[begin-1] == '*')
					vv.find_last_of("+-",begin-1);
				while(end > 0 && vv[end] == '-' && vv[end-1] == '*')
					vv.find_first_of("+-",end+1);
				if(begin == std::string::npos)
					begin = 0;
				vars += vv.substr(begin, end - begin);
				if(end != std::string::npos)
					i = end+1;
				else
					i = vv.length();
			}
		std::stringstream ss;
		ss << tmp->value();	// real value
		vv = ss.str() + vars;
		delete tmp;
		tmp = 0;
		// for (int i = 0; i < va.size(); i++) {
		// 	if(isalpha(va[i])){
		// 		vv = va;
		// 		break;
		// 	}
		// }
		// for (int i = 0; i < vb.size(); i++) {
		// 	if(isalpha(vb[i])){
		// 		if(vv == va)
		// 			vv += "+"+vb;
		// 		else
		// 			vv = vb;
		// 		break;
		// 	}
		// }
	}
	return vv;
	/*std::stringstream ss;
	double d = value();
	ss << d;
	std::string v = ss.str();
	return v;*/
}
double Equation::Node::value() const  throw(const char *){
	double v = 0;
	if(!a && !b){
		std::string d = Data();
		if(d.empty())
			return 0;
		int i = d.find_first_of("*/-+",1);
		while(d[i-1] == '*' && (d[i] == '-' || d[i] == '+'))
			i = d.find_first_of("*-+",i+1);
		char c = d[i];
		std::replace(d.begin(), d.end(), d[i], ' ');

		if(c == '*')
			v++;
		bool df = 0;
		std::stringstream ss(d);
		while(!ss.eof()){
			double tmp;
			if(!(ss >> tmp)){
				std::string rem;
				ss.str(ss.str());
				ss.clear();
				double remRes = 1.0;
				while(!ss.eof()){
					if(!(ss >> tmp))
						break;
					remRes *= tmp;
				}
				v -= remRes;
				return v;
			}
			switch(c){
				case '*':
					v*=tmp;
					break;
				case '/':
					if(!df){
						v = tmp;
						df = true;
					}
					else{
						// if(tmp == 0)
						// 	throw "Division by zero!";
						v /= tmp;
					}
					break;
				default:
					v += tmp;
			}
		}
	}
	else{
		if(op == X && a && b)
			v += a->value() * b->value();
		else if(op == D && a && b){
			int val = b->value();
			// if(val == 0)
			// 	throw "Division by zero!";
			v += a->value() / val;
		}
		else{
			if(a)
				v += a->value();
			if(b)
				v += b->value();
		}
	}
	return v;
}
std::string Equation::Node::Data() const{
	return data;
}
std::string Equation::Node::getStr() const{
	if(!a && !b)
		return Data();
	std::string s = "";
	bool parantes = (op == X || op == D);
	if(a){
		int ai = a->data.find_first_of("(-");
		if(a->op == P && parantes && ai != std::string::npos)
			s += "(";
		s += a->getStr();
		if(a->op == P && parantes && ai != std::string::npos)
			s += ")";
		if(b)
			s += (char)op;
	}
	if(b){
		int bi = b->data.find_first_of("(-");
		if(b->op == P && parantes && bi != std::string::npos)
			s += "(";
		s += b->getStr();
		if(b->op == P && parantes && bi != std::string::npos)
			s += ")";
	}
	return s;
}

// @unsupported!
std::string Equation::Node::get(int i){
	if(i && a)
		return a->get(i-1);
	if(i && b)
		return b->get(i-1);
	return data;
}