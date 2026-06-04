/////////////////////
// script_writer.h //
/////////////////////

#define POSITIVE 1
#define NEGATIVE -1

#include <curses.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <line_types.h>
#include <algorithm>

class scriptWriter {

public:

	bool openScript();

private:

	// Class Variables

	bool readOnly;

	std::vector<scriptLine> lineBuffer;
	std::unordered_map<uint16_t, scriptLine*> lineMap;
	uint16_t scriptSize;

	scriptLine* startLine;
	scriptLine* endLine;
	scriptLine* currentLine;
	scriptLine* nextLine;
	scriptLine* lastLine;
		
	std::string writerName;
	std::string scriptName;
	std::string scriptType;

	float currentType;

	int maxx;
	int maxy;

	//Class Methods
	
	void setLines(int y);
	int findSpace(float type);

	void movex(int* x, int modifier);
	void movey(int* y, int* relativey, int modifier);

	void coverPage();
	void addLine(uint16_t startLine, float type, std::string content = "");
	void mainLoop();

};