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
				return -1;
			
			}

			std::cout << "\n    ERROR: Invalid number of arguments :( - try 'writerspal help'\n";
			return -1;
		
		}

		else if (!(strcmp(argv[1], "help"))) {
		
			std::cout << "\n    Writers-Pal - The Terminal User Interface for Screenwriting!\n"
				<< "\n    new [script name] [type] ---> This command will create  a script in the current working directory,"
				<< "\n    with the (optional) name and type arguments. You will be prompted to enter these if they are"
				<< "\n    not given as arguments. Surround either in quotation marks to allow for space. As for type,"
				<< "\n    recommended templates are as follows:\n"
				<< "\n        Draft -> Ideal for scripts that are proof of concept, tests, or experiments.\n"
				<< "\n        Shooting Script -> Intended for final, production ready use, with director annotations\n        embedded.\n"
				<< "\n        Speculative Script -> When story elements are the main focus of the piece, great for\n        aspiring writers.\n"
				<< "\n        Continuity Script -> An amended version of a script after production has wrapped, for\n        an accurate transcription of the final piece.\n\n"
				<< "\n    setname [writer name] ---> This command will change the environment variable Writers-Pal uses to"
				<< "\n    determine what to write in scripts under your name. It is once again recommended to encase"
				<< "\n    the argument in quotation marks for proper string parsing.\n\n"
				<< "\n    help ---> This command will print the command menu you are currently reading!\n\n"
				<< "\n    [script name] ---> This command will check the working directory for the script name specified"
				<< "\n    in the command argument. If it doesn't exist, an error will be thrown. The same will happne if"
				<< "\n    it's an invalid or misspelled command.\n\n";
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
				return -1;

			}

		}
	
	}

	else {
	
		newScript();
	
	}

	return 0;

}