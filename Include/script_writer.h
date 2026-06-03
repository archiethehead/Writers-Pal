/////////////////////
// script_writer.h //
/////////////////////

#include <curses.h>
#include <string>
#include <vector>
#include <line_types.h>

class scriptWriter {

public:

	scriptWriter(bool readOnly = false);
	~scriptWriter();
	bool openScript();

private:

	bool readOnly;
	std::vector<scriptLine> lineArray;
	
	void mainLoop();


};