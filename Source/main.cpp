//////////////
// main.cpp //
//////////////

#include "script_writer.hpp"
#include <iostream>
#include <fstream>

void newScript() {

	std::string scriptName;
	std::string scriptType;

	std::cout << "\n    Enter the script name here > ";
	std::getline(std::cin, scriptName);

	std::cout << "\n    Enter the script type here > ";
	std::getline(std::cin, scriptType);

	scriptWriter script;
	script.newScript(scriptName, scriptType);

}

bool loadConfig(std::string& writerName) {

	std::fstream file(".config", std::fstream::in);

	if (!file.is_open()) {
	
		file.open(".config", std::fstream::out);

		file << "Example Name";

		file.open(".config", std::fstream::in);
	
	}

	if (!getline(file, writerName)) {

		return false;

	}

	file.close();
	return true;

};

bool setName(char name[]) {

	

	return true;

}

int main(int argc, char *argv[]) {

	std::string writerName;

	bool isConfig = loadConfig(writerName);

	if (!isConfig) {
	
		writerName = "Example Name";
	
	}
	
	if (argc > 1) {

		if (!(strcmp(argv[1], "new"))) { newScript(); }

		else if (!(strcmp(argv[1], "setname"))) {
		
			if (argc == 3) {
			
				setName(argv[2]);
				return 0;
			
			}

			std::cout << "\n    ERROR: Invalid number of arguments :( - try 'writerspal help'\n";
			return -1;
		
		}

		else {
		
			std::cout << "\nERROR:     " << argv[1] << " is not a recognised command :( - try 'writerspal help'\n";

		}
	
	}

	else {
	
		newScript();
	
	}

	return 0;

}