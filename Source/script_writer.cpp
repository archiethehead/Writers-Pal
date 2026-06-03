///////////////////////
// script_writer.cpp //
///////////////////////

#include <script_writer.h>

scriptWriter::scriptWriter(bool readOnly) {

	scriptWriter::readOnly = readOnly;
	scriptWriter::lineArray;


}

scriptWriter::~scriptWriter() {


}

bool scriptWriter::openScript() {

	scriptWriter::mainLoop();
	return true;

}

void scriptWriter::mainLoop() {

	initscr();
	
	raw();
	keypad(stdscr, true);
	resize_term(30, 120);

	int x = 0, y = 0, max_x = getmaxx(stdscr), max_y = getmaxy(stdscr), key = 0;
	bool writing = true;

	while (writing) {

		mvaddstr(y, x, "");
		refresh();

		key = getch();

		if (key == 27) {
			
			writing = false;
		
		}

		else if (key == KEY_RIGHT) {

			x++;

		}

		else if (key == KEY_LEFT) {
		
			x--;
		
		}

		else if (key == KEY_DOWN) {
		
			y++;
		
		}

		else if (key == KEY_UP) {
		
			y--;
		
		}

		else if (scriptWriter::readOnly) { continue; }

		else {

			char key_value[] = { key, '\0' };
			mvprintw(y, x, key_value);

		}
	

	}

	endwin();

}