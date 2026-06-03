/////////////////////
// script_writer.h //
/////////////////////

#include <curses.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <line_types.h>

class scriptWriter {

public:

	scriptWriter(bool readOnly = false);
	~scriptWriter();
	bool openScript();

private:

	// Class Variables

	bool readOnly;

	std::vector<scriptLine> lineArray;

	scriptLine startLine;
	scriptLine endLine;
	scriptLine currentLine;
	scriptLine nextLine;
	scriptLine lastLine;

	std::unordered_map<uint16_t, scriptLine> lineMap;

	std::string writerName;
	std::string scriptName;
	std::string scriptType;

	float currentType;

	//Class Methods
	
	void addLine();
	void mainLoop();

};