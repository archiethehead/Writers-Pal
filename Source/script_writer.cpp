///////////////////////
// script_writer.cpp //
///////////////////////

#include "script_writer.hpp"

scriptWriter::scriptWriter() {

	initscr();
	raw();
	keypad(stdscr, true);
	resize_term(30, 30);

	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);

	bufferModified = true;

	scriptName = "The Sandman";
	writerName = "Archie Healy";
	scriptType = "Draft One";

	centreText(scriptName);
	centreText(scriptType);

	addLine(0, DESCRIPTION, "INT. THERAPISTS OFFICE -- DAY");
	addLine(1, CHARACTER, "THERAPIST");
	addLine(2, DIALOGUE, "Did you do it?sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss");
	addLine(3, DIALOGUE, " ");

	coverPage();
	sortBuffer();
	mapLines();

}

scriptWriter::~scriptWriter() {

	endwin();

}

bool scriptWriter::openScript() {

	mainLoop();
	return true;

}

int scriptWriter::calculateLineCount(scriptLine& line) {

	// Base case for empty cover page lines, which falsely
	// return 0 for the number of lines it occupies (logical
	// error).
	//
	// |
	// |
	// V

	if (!(line.text.size()) && !(line.lineType)) {
	
		return 1;
	
	}

	int padding = FIND_SPACE(line);
	int currentSpace = line.text.length();
	float maxSpace = maxx - (padding * 2);

	float lineCount = currentSpace / maxSpace;
	int finalCount = (int)ceilf(lineCount);

	return finalCount;

}

int scriptWriter::findLineNum(int index) {

	if (index == 0) {
	
		return 1;
	
	}

	scriptLine* line = (lineMap[index]);
	scriptLine* temp = line;
	int counter = 0;

	while (temp == line) {
		
		counter++;
		temp = lineMap[index - counter];
	
	}

	return counter;

	return 1;

}

std::string scriptWriter::sliceLine(scriptLine& line, int lineNum) {

	int numOfLines = calculateLineCount(line);
	int len = maxx - (FIND_SPACE(line) * 2);
	int pos = len * (lineNum - 1);
	
	if (lineNum == numOfLines) {
	
		len = line.text.length();
		if (numOfLines > 1) {
		
			len = len - (len * (numOfLines));
		
		}
	
	}

	std::string lineText = line.text.substr(pos, len);
	return lineText;

}


void scriptWriter::centreText(std::string& text) {

	std::string buffer = text;
	text = std::string((maxx - buffer.length()) / 2, ' ') + buffer;

}

void scriptWriter::sortBuffer() {

	std::sort(lineBuffer.begin(), lineBuffer.end());
	bufferModified = false;

}

void scriptWriter::mapLines() {

	int bufferSize = SCRIPT_SIZE;
	int lineCount = 0;
	int offset = 0;

	for (int i = 0; i < bufferSize; i++) {
		
		scriptLine& currentLine = lineBuffer[i];
		int lineCount = calculateLineCount(currentLine);

		for (int j = 0; j < lineCount; j++) {

			lineMap.insert_or_assign(i + j + offset, &(currentLine));
		
		}

		offset += lineCount - 1;

	}

}

bool scriptWriter::movex(int& x, int modifier) {

	x += modifier;
	return true;

}

void scriptWriter::movey(int& y, int& relativey, int modifier) {

	if (modifier == POSITIVE  && y == maxy - 1) {

		if (y + relativey == SCRIPT_SIZE - 1) {
		
			addLine((y + relativey + 1), currentType);
		
		}

		relativey += 1;
		return;
	
	}

	else if (modifier == NEGATIVE && y == 0) {

		if (!relativey) {
		
			return;
		
		}

		relativey -= 1;
		return;
	
	}

	y += modifier;

}

void scriptWriter::coverPage() {

	int size = SCRIPT_SIZE;
		
	for (int i = 0; i < size; i++) {
	
		lineBuffer[i].startLineNum += maxy;
	
	}

	int halfPoint = maxy / 2;
	for (int i = 0; i < maxy; i++) {
	
		if (i == halfPoint) {
		
			addLine(i, COVER, scriptName);
		
		}

		else if (i == halfPoint + 1) {
		
			addLine(i, COVER, scriptType);

		}

		else if (i == halfPoint + 3) {

			std::string writtenBy = "Written by ";
			writtenBy += writerName;
			centreText(writtenBy);

			addLine(i, COVER, writtenBy);

		}

		else {

			addLine(i, COVER);

		}
	
	}

}

void scriptWriter::addLine(int startLine, float type, std::string content) {

	scriptLine newLine(startLine, type, content);
	lineBuffer.push_back(newLine);
	bufferModified = true;

}

void scriptWriter::mainLoop() {

	int x = 0, y = 0, relativey = 0, lineStart = 0, lineEnd = 0;

	int key = 0;

	bool writing = true;
	bool resize = false;

	while (writing) {
 
		clear();

		if (bufferModified) { 
			
			mapLines();
		
		}

		if (resize) {

			maxx = getmaxx(stdscr), maxy = getmaxy(stdscr);

		}
		
		for (int i = 0; i < maxy; i++) {

			scriptLine* line = lineMap[i + relativey];
			scriptLine currentLine = *line;

			int lineNum = findLineNum(i + relativey);
			std::string text = sliceLine(currentLine, lineNum);
			int currentSpace = FIND_SPACE(currentLine);


			mvaddstr(i, 0 + currentSpace, text.c_str());
		
		}

		mvaddstr(y, x, "");
		refresh();

		key = getch();

		if (key == 27) {
			
			writing = false;
		
		}

		else if (key == KEY_RIGHT) {

			movex(x, POSITIVE);

		}

		else if (key == KEY_LEFT) {
		
			movex(x, NEGATIVE);
		
		}

		else if (key == KEY_DOWN) {

			movey(y, relativey, POSITIVE);
		
		}

		else if (key == KEY_UP) {
		
			movey(y, relativey, NEGATIVE);
		
		}

		else if (key == KEY_RESIZE) {
			
			resize = true;

		}

		else if (readOnly) { continue; }

		else {

			x++;
			char key_value[] = { key, '\0' };
			mvprintw(y, x, key_value);

		}
	

	}

}