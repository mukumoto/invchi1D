#include "utils.hpp"

std::string readPar(std::string target, std::string filename)
{
	std::ifstream fp(filename);
	std::string s1,s2;
	int flag=0;
	while(fp >> s1 >> s2){
		if(s1==target) {
			flag=1;
			break;
		}
	}

	if(flag){
		return s2;
	}
	else{
		std::cerr << "cannot find " << target <<"\n";
		std::exit(0);
	}
}
