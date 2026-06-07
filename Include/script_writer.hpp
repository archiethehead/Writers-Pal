/////////////////////
// script_writer.h //
/////////////////////

#ifndef SCRIPT_WRITER_H
#define SCRIPT_WRITER_H

#define POSITIVE 1
#define NEGATIVE -1

#define SCRIPT_SIZE scriptWriter::lineBuffer.size()
#define FIND_SPACE(x) ((int)ceilf(maxx * x.lineType))

#include <line_types.hpp>
#include <curses.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class scriptWriter {

	// Class Variables

	bool readOnly;
	bool bufferModified;

	std::vector<scriptLine> lineBuffer;
	std::unordered_map<int, scriptLine*> lineMap;
		
	std::string writerName;
	std::string scriptName;
	std::string scriptType;

	float currentType;

	int maxx;
	int maxy;

	//Class Methods

	int calculateLineCount(scriptLine& line);
	int findLineNum(int index);

	void centreText(std::string& text);
	void sortBuffer();
	void mapLines();

	bool movex(int& x, int modifier);
	void movey(int& y, int &relativey, int modifier);

	void coverPage();
	void addLine(int startLine, float type, std::string content = "");
	void mainLoop();

public:

	scriptWriter();
	~scriptWriter();
	bool openScript();

};

#endif