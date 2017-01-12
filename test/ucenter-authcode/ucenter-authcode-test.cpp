// ucenter-authcode-test.cpp main
//

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

#include "ucenter_authcode.h"


int main(int argc, char* argv[])
{
	std::string eng_key = "Adding the gist -r <id> command to read a gist";
	std::string eng = "Summary of pull requests, issues opened, and commits. Learn how we count contributions.?!{}[]\\/~!@#$#$%$^^%&*()_+";

	ucenter_authcode eng_ua(eng_key);
	std::string eng_en = eng_ua.encode(eng);
	std::string eng_de = eng_ua.decode(eng_en);

	std::cout << "eng_key:" << eng_key << std::endl;

	std::cout << "eng:" << eng << std::endl;
	std::cout << "eng_en:" << eng_en << std::endl;
	std::cout << "eng_de:" << eng_de << std::endl;
	std::cout << "eng == eng_de:" << (eng == eng_de) << std::endl;



	return 0;
}

