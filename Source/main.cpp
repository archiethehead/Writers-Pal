//////////////
// main.cpp //
//////////////

#include "script_writer.hpp"

void newScript(std::string scriptName = "", std::string scriptType = "") {


	if (!scriptName.size()) {

		std::cout << "\n    Enter the script name here > ";
		std::getline(std::cin, scriptName);

	}

	if (!scriptType.size()) {

		std::cout << "\n    Enter the script type here > ";
		std::getline(std::cin, scriptType);

	}

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

	std::fstream file(".config", std::fstream::out);

	if (!file.is_open()) {
	
		return false;
	
	}

	file << name;
	file.close();
	return true;
	
}

int main(int argc, char *argv[]) {

	std::string writerName;

	bool isConfig = loadConfig(writerName);

	if (!isConfig) {
	
		writerName = "Example Name";
	
	}
	
	if (argc > 1) {

		if (!(strcmp(argv[1], "new"))) { 
			
			switch (argc) {
			
			case 2:
				newScript();
				break;

			case 3:
				std::cout << argv[2];
				newScript(argv[2]);
				break;

			case 4:
				newScript(argv[2], argv[3]);
				break;

			}

			return 0;

		}

		else if (!(strcmp(argv[1], "setname"))) {
		
			if (argc == 3) {
			
				bool isNameSet = setName(argv[2]);
				
				if (isNameSet) {
				
					std::cout << "\n    Name set -> " << argv[2] << "\n";
					return 0;

				}

				std::cout << "\n    ERROR: Unable to set name -> " << argv[2] << " :(\n";
				return 0;
			
			}

			std::cout << "\n    ERROR: Invalid number of arguments :( - try 'writerspal help'\n";
			return -1;
		
		}

		else {

			scriptWriter script;
			bool isFile;

			switch (argc) {

			case 2:
				isFile = script.openScript(argv[1]);
				break;

			case 3:
				isFile = script.openScript(argv[1], true);
				break;

			}
			
			if (!isFile) {

				std::cout << "\n    ERROR: " << argv[1] << " is not a recognised command or file :( - try 'writerspal help'\n";

			}

		}
	
	}

	else {
	
		newScript();
	
	}

	return 0;

}