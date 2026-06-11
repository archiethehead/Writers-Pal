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
	raw();
	nonl();
	keypad(stdscr, true);
	resize_term(0, 0);

	currentType = COVER;
	pageheight = 55;
	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);

	bufferModified = true;
	mapModified = true;
	readOnly = true;

}

scriptWriter::~scriptWriter() {

	endwin();
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(consoleHandle, TRUE, &fontInfo);

}

bool scriptWriter::openScript(char filepath[], bool isReadOnly) {

	readOnly = isReadOnly;
	std::fstream file;
	char* path = (char*)malloc(sizeof(filepath) + 5);

	if (path == 0) {
	
		return false;
	
	}

	strcpy(path, filepath);
	strcat(path, ".txt");

	file.open(path, std::fstream::in);

	if (!file.is_open()) {
	
		return false;
	
	}

	std::vector<std::string> row;
	std::string temp;
	int index = 0;

	while (getline(file,  temp, '\n')) {
	
		row.push_back(temp);
		index++;

		if (index == 3) {
			
			scriptName = row[0];
			scriptType = row[1];
			writerName = row[2];
			row.clear();
		
		}

		if (index % 3 == 0 && index != 3) {

			addLine(std::stoi(row[0]), std::stof(row[1]), row[2]);
			row.clear();

		}
	
	}

	centreText(scriptName);
	recentreText(scriptType);
	recentreText(writerName);

	if (lineBuffer.back().startLineNum < 0xffff) {
	
		int* tempBuffer = (int*)malloc(sizeof(int) * lineBuffer.size());

		if (tempBuffer == 0) { return false; }

		for (int i = 0; i < lineBuffer.size(); i++) {
		
			tempBuffer[i] = 0;
		
		}

		for (int i = 0; i < lineBuffer.size(); i++) {
		
			tempBuffer[lineBuffer[i].startLineNum] = 1;
		
		}

		for (int i = 0; i < lineBuffer.size(); i++) {
		
			if (!tempBuffer[i]) {
			
				addLine(i, DESCRIPTION);
			
			}
		
		}
	
	}

	else {
	
		return false;
	
	}

	coverPage();
	sortBuffer();
	mapLines();

	mainLoop();
	return true;

}

void scriptWriter::newScript(std::string name, std::string type) {

	readOnly = false;

	scriptName = name;
	writerName = "Written by Archie Healy";
	scriptType = type;

	centreText(writerName);
	centreText(scriptName);
	centreText(scriptType);

	addLine(0, DESCRIPTION, "I am the first line, please edit me :)");

	coverPage();
	sortBuffer();
	mapLines();

	mainLoop();

}

void scriptWriter::saveScript() {

	int len = scriptName.length();
	int startIndex = -1;

	const char* cstring = scriptName.c_str();
	char currentChar = ' ';
	while (currentChar == ' ') {

		startIndex++;
		currentChar = cstring[startIndex];

	}

	scriptName = scriptName.substr(startIndex, len - ((startIndex)));

	std::fstream file;
	file.open((scriptName + ".txt").c_str(), std::fstream::out);

	file	<< scriptName << "\n"
			<< scriptType << "\n"
			<< writerName << "\n";
	
	for (int i = 0; i < (SCRIPT_SIZE - 1) - pageheight; i++) {
	
		scriptLine* currentLine = &(lineBuffer[i + pageheight]);

		if (!currentLine->text.size()) { continue; }

		file	<< currentLine->startLineNum - pageheight << "\n"
				<< currentLine->lineType << "\n"
				<< currentLine->text << "\n";
	
	}

	file.close();

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

	for (int i = 0; i < lineBuffer.size(); i++) {
	
		lineBuffer[i].startLineNum = i;
	
	}

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

void scriptWriter::splitLine(scriptLine& line, int x, int lineNum) {

	int numOfLines = calculateLineCount(line);

	if (lineNum > numOfLines) {

		return;

	}

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

	if (pos > line.text.size()) {
	
		moveDownLines(line.startLineNum, line.lineType);
		return;
	
	}

	std::string newText = line.text.substr(pos, line.text.size() - pos);
	line.text.erase(pos);
	moveDownLines(line.startLineNum, line.lineType);
	lineBuffer.back().text = newText;

}

void scriptWriter::mergeLines(int bufferIndex) {

	if (bufferIndex == (pageheight)) {
	
		return;
	
	}

	lineBuffer[bufferIndex - 1].text += lineBuffer[bufferIndex].text;
	deleteLine(lineBuffer[bufferIndex]);

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

	if (lineNum > numOfLines) {
	
		return 0;
	
	}

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
	
	if (x > len + 1) {

		line.text.insert(line.text.size(), 1, character);
		return finalLineLen + 2;
	
	}

	line.text.insert(pos, 1, character);
	return 0;

}

int scriptWriter::backspace(scriptLine& line, int x, int lineNum) {

	int numOfLines = calculateLineCount(line);

	if (lineNum > numOfLines) {
	
		return 0;
	
	}

	int len = maxx - (FIND_SPACE(line) * 2);
	int pos = len * (lineNum - 1);
	
	pos += x;

	int finalLineLen = (line.text.length() - 1) - (len * (numOfLines - 1));

	if ((x > finalLineLen && lineNum == numOfLines) || (pos == finalLineLen && lineNum == numOfLines)) {
	
		line.text.erase(line.text.size() - 1, 1);
		return finalLineLen;
	
	}

	if ((x > len)) {
	
		line.text.erase(pos - 2, 1);
		return 0;
	
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


		else if (key == CTRL('x')) {
		
			currentLine->cycleType(true);
			mapModified = true;
		
		}

		else if (key == CTRL('z')) {
		
			currentLine->cycleType(false);
			mapModified = true;
		
		}

		else if (key == CTRL('s')) {
		
			mvaddstr(maxy, 0, "SAVING . . .");
			refresh();
			saveScript();
			mvaddstr(maxy, 0, "SAVED!");
			refresh();
			getch();
		
		}

		else if (key == KEY_ENTER || key == 10 || key == 13) {
		
			splitLine(*(currentLine), x - minSpace, findLineNum(absolutey));
			movey(y, relativey, POSITIVE);
			x = minSpace;

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

			else if (!(x - minSpace) && (absolutey != pageheight) && (findLineNum(absolutey)) == 1) {
				
				int minSpace = FIND_SPACE_POINTER(previousLine);
				currentType = previousLine->lineType;
				int previousLastLineLen = lastLineLen(*(lineMap[absolutey - 1]));
				mergeLines(currentLine->startLineNum);
				y--;
				x = previousLastLineLen + minSpace + 1;

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
					// In addiion to this, moving it once more places the cursor in the correct position
					// for erasing the character behind the one it just erased. This can be accomplished
					// with x--, as no boundary checks are necessary.

					if (!lastLineLength) {
					
						isMoveY = movex(x, *(currentLine), findLineNum(absolutey), NEGATIVE);
					
					}

					if (isMoveY) {

						movey(y, relativey, NEGATIVE);

						x--;

					}

				}

			}
		
		}

		else if (key > 31 && key < 127) {

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

			mapModified = true;

		}
	

	}

}