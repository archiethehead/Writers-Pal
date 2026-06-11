//////////////
// main.cpp //
//////////////

#include "script_writer.hpp"

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

int main(int argc, char *argv[]) {
	
	if (argc > 1) {

		if (!(strcmp(argv[1], "new"))) { newScript(); }

		else {
		
			std::cout << "\n    " << argv[1] << " is not a recognised command - try 'writerspal help'\n";

		}
	
	}

	else {
	
		newScript();
	
	}

	return 0;

}