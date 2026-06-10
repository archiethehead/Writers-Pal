///////////////////////
// script_writer.cpp //
///////////////////////

#include "script_writer.hpp"

scriptWriter::scriptWriter() {

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(consoleHandle, FALSE, &fontInfo);

	CONSOLE_FONT_INFOEX newFontInfo = fontInfo;
	newFontInfo.FontFamily = 0x0;
	wcscpy_s(newFontInfo.FaceName, L"Courier");

	SetCurrentConsoleFontEx(consoleHandle, TRUE, &newFontInfo);

	initscr();
	nocbreak();
	noecho();
	cbreak();
	keypad(stdscr, true);
	resize_term(0, 0);

	currentType = COVER;
	pageheight = 55;
	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);

	bufferModified = true;
	mapModified = true;

	scriptName = "The Sandman";
	writerName = "Written by Archie Healy";
	scriptType = "Draft One";

	centreText(writerName);
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
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(consoleHandle, TRUE, &fontInfo);

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

}

std::string scriptWriter::sliceLine(scriptLine& line, int lineNum) {

	int size = line.text.size();
	if (size == 1 || line.text == "") {
	
		return "";
	
	}

	int numOfLines = calculateLineCount(line);
	int len = maxx - (FIND_SPACE(line) * 2);
	int pos = len * (lineNum - 1);

	if (numOfLines < lineNum) {
	
		return "";

	}
	
	if (lineNum == numOfLines) {
	
		len = line.text.length();
		if (numOfLines > 1) {
		
			len = len - (len * (numOfLines));
		
		}
	
	}

	std::string lineText = line.text.substr(pos, len);
	return lineText;

}

void scriptWriter::recentreText(std::string& text) {

	int len = text.length();
	int startIndex = -1;

	const char* cstring = text.c_str();
	char currentChar = ' ';
	while (currentChar == ' ') {
	
		startIndex++;
		currentChar = cstring[startIndex];
	
	}

	text = text.substr(startIndex, len - ((startIndex)));
	centreText(text);
	mapModified = true;

}

void scriptWriter::centreText(std::string& text) {

	int len = text.length();
	if (maxx < len) {
	
		return;
		 
	}

	text = std::string((maxx - len) / 2, ' ') + text;

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
		
		scriptLine* currentLine = &lineBuffer[i];
		int lineCount = calculateLineCount(*currentLine);

		for (int j = 0; j < lineCount; j++) {

			lineMap.insert_or_assign(i + j + offset, currentLine);
		
		}

		offset += lineCount - 1;

	}

}

void scriptWriter::moveDownLines(int y, float type) {

	//int size = SCRIPT_SIZE;
	//if (y == size - 1) {

	//	addLine((y + 1), type);
	//	lineMap.insert_or_assign(size, &(lineBuffer[SCRIPT_SIZE - 1]));

	//}

	//y--;
	//for (int i = 1; i < SCRIPT_SIZE - y; i++) {

	//	scriptLine* temp = lineMap[y + i];
	//	lineMap.insert_or_assign(y + i, lineMap[y + i]);
	//	lineMap.insert_or_assign(y + i + 1, temp);
	//
	//}

}

int scriptWriter::addChar(scriptLine& line, int x, int lineNum, char character) {

	int numOfLines = calculateLineCount(line);
	int len = maxx - (FIND_SPACE(line) * 2);
	int pos = len * (lineNum - 1);

	int finalLineLen = (line.text.length() - 1) - (len * (numOfLines - 1));

	if (lineNum == numOfLines) {

		len = line.text.length();
		if (numOfLines > 1) {

			len = len - (len * (numOfLines));

		}

	}

	pos += x;
	
	if (x > finalLineLen + 1) {

		line.text.insert(line.text.size(), 1, character);
		return finalLineLen + 2;
	
	}

	line.text.insert(pos, 1, character);
	return 0;

}



int scriptWriter::backspace(scriptLine& line, int x, int lineNum) {

	int numOfLines = calculateLineCount(line);
	int len = maxx - (FIND_SPACE(line) * 2);
	int pos = len * (lineNum - 1);
	
	pos += x;

	int finalLineLen = (line.text.length() - 1) - (len * (numOfLines - 1));

	if (x > finalLineLen || x == 0 || (pos == finalLineLen && lineNum == numOfLines)) {
	
		line.text.erase(line.text.size() - 1, 1);
		return finalLineLen;
	
	}

	line.text.erase(pos - 1, 1);
	return 0;

}

void scriptWriter::deleteLine(scriptLine& line) {



}

bool scriptWriter::movex(int& x, scriptLine& line, int relativeLineNum, int modifier) {

	int minSpace = FIND_SPACE(line);
	float maxSpace = maxx - minSpace;
	
	if (x == minSpace && modifier == NEGATIVE) {
	
		if (!line.startLineNum) {
		
			return false;

		}

		if (relativeLineNum == 1) {
		
			scriptLine* previousLine = lineMap[line.startLineNum - 1];

			x = previousLine->text.length() + FIND_SPACE_POINTER(previousLine);
			currentType = previousLine->lineType;
			return true;
		
		}
		
		x = maxSpace - 1;
		return true;
		
	
	}

	else if (x == maxSpace - 1 && modifier == POSITIVE) {
	
		if (line.startLineNum == (SCRIPT_SIZE)-1) {
		
			return false;
		
		}

		x = minSpace;
		return true;
	
	}

	x += modifier;
	return false;
	
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
		
		lineBuffer[i].startLineNum += pageheight;
	
	}

	int halfPoint = pageheight / 2;
	for (int i = 0; i < pageheight; i++) {
	
		if (i == halfPoint) {
		
			addLine(i, COVER, scriptName);
		
		}

		else if (i == halfPoint + 1) {
		
			addLine(i, COVER, scriptType);

		}

		else if (i == halfPoint + 3) {

			addLine(i, COVER, writerName);

		}

		else {

			addLine(i, COVER);

		}
	
	}

}

void scriptWriter::addLine(int startLine, float type, std::string content) {

	scriptLine newLine(startLine, type, content);
	
	int size = SCRIPT_SIZE;
	int maxSize = lineBuffer.capacity();
	if (size + 1 == maxSize) {
	
		lineBuffer.reserve(size + 100);
	
	}

	lineBuffer.push_back(newLine);
	bufferModified = true;
	mapModified = true;

}

void scriptWriter::mainLoop() {

	int x = 0, y = 0, relativey = 0, lineStart = 0, lineEnd = 0;

	int key = 0;

	bool writing = true;

	while (writing) {
	
		clear();

		if (mapModified) { 
			
			mapLines();
		
		}

		if (bufferModified) {
		
			sortBuffer();
		
		}
		
		for (int i = 0; i < maxy; i++) {

			scriptLine* line = lineMap[i + relativey];

			if (!line) {
			
				for (int n = 0; n < maxy - i; n++) {
				
					addLine(SCRIPT_SIZE, DESCRIPTION);
					scriptLine& newLine = lineBuffer[SCRIPT_SIZE - 1];
					lineMap.insert_or_assign(i + relativey + n, &(newLine));
				
				}

				mapLines();
				line = lineMap[i + relativey];
			
			}

			int lineNum = findLineNum(i + relativey);
			std::string text = sliceLine(*(line), lineNum);
			int currentSpace = ((int)ceilf(maxx * line->lineType));


			mvaddstr(i, 0 + currentSpace, text.c_str());
		
		}

		scriptLine* currentLine = lineMap[y + relativey];

		if (currentLine->lineType != currentType) {
		
			x = FIND_SPACE_POINTER(currentLine);

		}

		currentType = currentLine->lineType;
		int minSpace = FIND_SPACE_POINTER(currentLine);

		mvaddstr(y, x, "");
		refresh();

		key = getch();

		if (key == 27) {
			
			writing = false;
		
		}

		else if (key == KEY_RIGHT) {

			bool isMoveY = movex(x, *(currentLine), findLineNum(y + relativey), POSITIVE);

			if (isMoveY) {
			
				movey(y, relativey, POSITIVE);
			
			}

		}

		else if (key == KEY_LEFT) {
		
			bool isMoveY = movex(x, *(currentLine), findLineNum(y + relativey), NEGATIVE);

			if (isMoveY) {
			
				movey(y, relativey, NEGATIVE);

			}
		
		}

		else if (key == KEY_DOWN) {

			movey(y, relativey, POSITIVE);
		
		}

		else if (key == KEY_UP) {
		
			movey(y, relativey, NEGATIVE);
		
		}

		else if (key == KEY_RESIZE) {

			resize_term(0,0);
			maxx = getmaxx(stdscr);
			maxy = getmaxy(stdscr);

			recentreText(scriptName);
			recentreText(scriptType);
			recentreText(writerName);

			lineMap[(pageheight) / 2]->text = scriptName;
			lineMap[((pageheight) / 2) + 1]->text = scriptType;
			lineMap[((pageheight) / 2) + 3]->text = writerName;

		}

		else if (readOnly) { continue; }

		else if (key == 8) {
		
			int isFinalLine = backspace(*(currentLine), x - minSpace, findLineNum(y + relativey));

			if (isFinalLine) {

				x = minSpace + isFinalLine;

			}

			else {

				bool isMoveY = movex(x, *(currentLine), findLineNum(y + relativey), NEGATIVE);

				if (isMoveY) {

					movey(y, relativey, NEGATIVE);

				}

			}
		
		}

		else {

			int isFinalLine = addChar(*(currentLine), x - FIND_SPACE_POINTER(currentLine), findLineNum(y + relativey), (char)key);
			
			if (isFinalLine) {
			
				x = minSpace + isFinalLine;
			
			}

			else {

				bool isMoveY = movex(x, *(currentLine), findLineNum(y + relativey), POSITIVE);

				if (isMoveY) {

					mapLines();
					movey(y, relativey, POSITIVE);

				}

			}

		}
	

	}

}