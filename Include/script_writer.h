/////////////////////
// script_writer.h //
/////////////////////

#define POSITIVE 1
#define NEGATIVE -1

#include <line_types.h>
#include <curses.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class scriptWriter {

public:

	bool openScript();

private:

	// Class Variables

	bool readOnly;
	bool bufferModified;

	std::vector<scriptLine> lineBuffer;
	std::unordered_map<uint16_t, scriptLine*> lineMap;
	uint16_t scriptSize;
		
	std::string writerName;
	std::string scriptName;
	std::string scriptType;

	float currentType;

	int maxx;
	int maxy;

	//Class Methods

	int findSpace(float type);
	void setLines(int y);

	void centreText(std::string& text);
	void sortBuffer();

	bool movex(int &x, int modifier);
	void movey(int &y, int &relativey, int modifier);

	void coverPage();
	void addLine(uint16_t startLine, float type, std::string content = "");
	void mainLoop();

};