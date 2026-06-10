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

	if (!(line.text.size())) {
	
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
	if (size == 0 || line.text == "") {
	
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

int scriptWriter::lastLineLen(scriptLine& line) {

	int numOfLines = calculateLineCount(line);
	int len = maxx - (FIND_SPACE(line) * 2);
	int finalLineLen = (line.text.length() - 1) - (len * (numOfLines - 1));
	return finalLineLen;

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

	lineMap.clear();

	for (int i = 0; i < bufferSize; i++) {
		
		scriptLine* currentLine = &lineBuffer[i];
		int lineCount = calculateLineCount(*currentLine);

		for (int j = 0; j < lineCount; j++) {

			lineMap.insert_or_assign(i + j + offset, currentLine);
		
		}

		offset += lineCount - 1;

	}

	mapModified = false;


}

void scriptWriter::moveDownLines(int y, float type) {

	int size = SCRIPT_SIZE;
	int numOfLines = size - y - 1;

	for (int i = 1; i < numOfLines; i++) {
	
		lineBuffer[y + i].startLineNum++;
	
	}

	addLine(y + 1, type);

	bufferModified = true;
	mapModified = true;

}

// The return variable for thr addChar and backspace functions below can either 
// be 0 (signifying that x isn't past the end of the text), or non-zero (x is past
// the end of the text). If non-zero, the value must be equal to the length of the
// final line of the block.
//
// |
// |
// |
// V

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

	// Clearing the map removes garbage data left over from
	// past writes, and prevents crashes with backspacing.

	lineBuffer.erase(lineBuffer.begin() + line.startLineNum);
	mapModified = true;

}

bool scriptWriter::movex(int& x, scriptLine& line, int relativeLineNum, int modifier) {

	int minSpace = FIND_SPACE(line);
	float maxSpace = maxx - minSpace;
	
	if (x <= minSpace && modifier == NEGATIVE) {
	
		// Base case, at the top of the script.

		if (!line.startLineNum) {
		
			return false;

		}

		if (relativeLineNum == 1) {
		
			scriptLine* previousLine = lineMap[line.startLineNum - 1];

			x = previousLine->text.length() + FIND_SPACE_POINTER(previousLine);
			currentType = previousLine->lineType;
			return true;
		
		}
		
		x = maxSpace;
		return true;
		
	
	}

	else if (x > maxSpace - 1 && modifier == POSITIVE) {

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

		// Base case, at
		// the top of the
		// script.

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

	// Reserving space for the vector before it is
	// forced to on a 0-capacity write prevents it
	// from corrupting neighbouring variables,
	// primarily the line map. Under NO CIRCUMSTANCES
	// is this conditional to be removed.
	//
	// |
	// V

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

		if (bufferModified) {

			sortBuffer();

		}

		if (mapModified) { 
			
			mapLines();
		
		}

		int absolutey = y + relativey;
		


		for (int i = 0; i < maxy; i++) {

			scriptLine* line = lineMap[i + relativey];

			if (!line) {
			
				for (int n = 0; n < maxy - i; n++) {
				
					addLine(SCRIPT_SIZE, DESCRIPTION);
				
				}

				mapLines();
				line = lineMap[i + relativey];
			
			}

			int lineNum = findLineNum(i + relativey);
			std::string text = sliceLine(*(line), lineNum);
			int currentSpace = ((int)ceilf(maxx * line->lineType));


			mvaddstr(i, 0 + currentSpace, text.c_str());
		
		}

		scriptLine* currentLine = lineMap[absolutey];
		scriptLine* previousLine = currentLine;

		if (absolutey != 0) {
		
			previousLine = lineMap[absolutey - 1];
		
		}

		// Sets x to the minimum boundary if we've
		// wrapped to a different block.

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

			bool isMoveY = movex(x, *(currentLine), findLineNum(absolutey), POSITIVE);

			if (isMoveY) {
			
				movey(y, relativey, POSITIVE);
			
			}

		}

		else if (key == KEY_LEFT) {
		
			bool isMoveY = movex(x, *(currentLine), findLineNum(absolutey), NEGATIVE);

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

		else if (readOnly || currentType == COVER) { continue; }

		else if (key == KEY_ENTER || key == 10 || key == 13) {
		
			moveDownLines(y + relativey , currentType);
			movey(y, relativey, POSITIVE);

		}
		
		else if (key == 8) {

			int lastLineLength = lastLineLen(*(previousLine));

			// Checks to see if a line needs to, and can be, deleted from the line
			// buffer.

			if (currentLine->text.size() == 0 && SCRIPT_SIZE != pageheight + 1) {
			
				deleteLine(*(currentLine));
				int prevMinSpace = FIND_SPACE_POINTER(previousLine);
				currentType = previousLine->lineType;
				x = prevMinSpace + lastLineLength + 1;
				movey(y, relativey, NEGATIVE);

			}


			else {

				// isFinalLine stores either 0, or the length of the final line of the current line,
				// with the return variable being decided from backspace as to whether or not x is
				// logically past the end of the script line (0 = no, anything else = yes).

				int isFinalLine = backspace(*(currentLine), x - minSpace, findLineNum(absolutey));

				if (isFinalLine) {

					x = minSpace + isFinalLine;

				}

				else {

					bool isMoveY = movex(x, *(currentLine), findLineNum(absolutey), NEGATIVE);

					// if this is the last line of the current block and wrapping is about to occur,
					// the cursor must be moved twice in order to move it completely out-of-bounds,
					// and prevent accidental writing on a different block by not wrapping back up.

					if (!lastLineLength) {
					
						isMoveY = movex(x, *(currentLine), findLineNum(absolutey), NEGATIVE);
					
					}

					if (isMoveY) {

						movey(y, relativey, NEGATIVE);

					}

				}

			}
		
		}

		else {

			// isFinalLine stores either 0, or the length of the final line of the current line,
			// with the return variable being decided from addChar as to whether or not x is
			// logically past the end of the script line (0 = no, anything else = yes).

			int isFinalLine = addChar(*(currentLine), x - FIND_SPACE_POINTER(currentLine), findLineNum(absolutey), (char)key);
			
			if (isFinalLine) {
			
				x = minSpace + isFinalLine;
			
			}

			else {

				bool isMoveY = movex(x, *(currentLine), findLineNum(absolutey), POSITIVE);

				if (isMoveY) {

					mapLines();
					movey(y, relativey, POSITIVE);
					x++;

				}

			}

		}
	

	}

}