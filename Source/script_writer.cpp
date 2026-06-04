///////////////////////
// script_writer.cpp //
///////////////////////

#include <script_writer.h>

bool scriptWriter::openScript() {

	scriptWriter::mainLoop();
	return true;

}

int scriptWriter::findSpace(float type) {

	return (int)scriptWriter::maxx * type;

}

void scriptWriter::setLines(int y) {

}

void scriptWriter::centreText(std::string &text) {

	std::string buffer = text;
	text = std::string((scriptWriter::maxx - buffer.length()) / 2, ' ') + buffer;

}

void scriptWriter::sortBuffer() {

	std::sort(scriptWriter::lineBuffer.begin(), scriptWriter::lineBuffer.end());
	scriptWriter::bufferModified = false;

}

bool scriptWriter::movex(int &x, int modifier) {

	x += modifier;
	return true;

}

void scriptWriter::movey(int &y, int &relativey, int modifier) {

	if (modifier == POSITIVE  && y == scriptWriter::maxy - 1) {

		if (y == scriptWriter::scriptSize) {
		
			scriptWriter::addLine((y + relativey + 1), scriptWriter::currentType);
		
		}

		if (y + relativey == scriptWriter::lineBuffer.size() - 1) {
		
			return;
		
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

	int maxy = scriptWriter::maxy;
	int size = scriptWriter::scriptSize;
		
	for (int i = 0; i < size; i++) {
	
		scriptWriter::lineBuffer[i].startLineNum += maxy;
	
	}

	int halfPoint = maxy / 2;
	for (int i = 0; i < maxy; i++) {
	
		if (i == halfPoint) {
		
			scriptWriter::addLine(i, COVER, scriptWriter::scriptName);
		
		}

		else if (i == halfPoint + 1) {
		
			scriptWriter::addLine(i, COVER, scriptWriter::scriptType);

		}

		else if (i == halfPoint + 3) {

			std::string writtenBy = "Written by ";
			writtenBy += scriptWriter::writerName;
			scriptWriter::centreText(writtenBy);

			scriptWriter::addLine(i, COVER, writtenBy);

		}

		else {

			scriptWriter::addLine(i, COVER);

		}

		scriptWriter::scriptSize++;
	
	}

}

void scriptWriter::addLine(uint16_t startLine, float type, std::string content) {

	scriptLine newLine(startLine, type, content);
	scriptWriter::lineBuffer.push_back(newLine);
	scriptWriter::scriptSize++;
	scriptWriter::bufferModified = true;

}

void scriptWriter::mainLoop() {

	initscr();
	
	raw();
	keypad(stdscr, true);
	resize_term(30, 120);

	int x = 0, y = 0, relativey = 0, lineStart = 0, lineEnd = 0, key = 0;
	scriptWriter::maxx = getmaxx(stdscr), scriptWriter::maxy = getmaxy(stdscr);

	scriptWriter::bufferModified = true;

	scriptWriter::scriptName = "The Sandman";
	scriptWriter::writerName = "Archie Healy";
	scriptWriter::scriptType = "Draft One";

	scriptWriter::centreText((scriptWriter::scriptName));
	scriptWriter::centreText((scriptWriter::scriptType));

	scriptWriter::addLine(0, DESCRIPTION, "INT. THERAPISTS OFFICE -- DAY");
	scriptWriter::addLine(1, CHARACTER, "THERAPIST");
	scriptWriter::addLine(2, DIALOGUE, "Did you do it?");

	scriptWriter::coverPage();
	bool writing = true;

	while (writing) {

		clear();

		if (bufferModified) { scriptWriter::sortBuffer(); }

		for (int i = 0; i < scriptWriter::maxy; i++) {
		
			scriptLine currentLine = scriptWriter::lineBuffer[i + relativey];
			int currentSpace = scriptWriter::findSpace(currentLine.lineType);

			mvaddstr(currentLine.startLineNum - relativey, currentSpace, currentLine.text.c_str());
		
		}

		mvaddstr(y, x, "");
		refresh();

		key = getch();

		if (key == 27) {
			
			writing = false;
		
		}

		else if (key == KEY_RIGHT) {

			scriptWriter::movex(x, POSITIVE);

		}

		else if (key == KEY_LEFT) {
		
			x--;
		
		}

		else if (key == KEY_DOWN) {
		
			scriptWriter::movey(y, relativey, POSITIVE);
		
		}

		else if (key == KEY_UP) {
		
			scriptWriter::movey(y, relativey, NEGATIVE);
		
		}

		else if (key == KEY_RESIZE) {
		
			scriptWriter::maxx = getmaxx(stdscr), scriptWriter::maxy = getmaxy(stdscr);

		}

		else if (scriptWriter::readOnly) { continue; }

		else {

			x++;
			char key_value[] = { key, '\0' };
			mvprintw(y, x, key_value);

		}
	

	}

	endwin();

}