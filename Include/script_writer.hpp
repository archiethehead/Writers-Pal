///////////////////////
// script_writer.hpp //
///////////////////////

#pragma once

#define POSITIVE 1
#define NEGATIVE -1

#define SCRIPT_SIZE scriptWriter::lineBuffer.size()
#define FIND_SPACE(x) ((int)ceilf(maxx * x.lineType))

#ifdef _DEBUG

#include "debug_output/debug_output.h"
#define DBOUT(x) dbout(x)

#else

#define DBOUT(x)

#endif

#include "line_types.hpp"
#include <curses/PDCursesMod/curses.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <windows.h>

class scriptWriter {

	// Class Variables

	CONSOLE_FONT_INFOEX fontInfo;

	bool readOnly;
	bool bufferModified;
	bool mapModified;

	std::vector<scriptLine> lineBuffer;
	std::unordered_map<int, scriptLine*> lineMap;
		
	std::string writerName;
	std::string scriptName;
	std::string scriptType;

	float currentType;

	int pageheight;
	int maxx;
	int maxy;

	//Class Methods

	int calculateLineCount(scriptLine& line);
	int findLineNum(int index);
	std::string sliceLine(scriptLine& line, int lineNum);
	
	void recentreText(std::string& text);
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