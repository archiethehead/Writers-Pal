#include <line_types.h>

scriptLine::line(uint16_t start, float type, std::string content) {

	lineID = getid();
	startLineNum = start;
	lineType = type;
	text = content;

}

scriptLine::~line() {

	freeid(lineID);

}

void scriptLine::cycle_type(bool right) {

	switch (right) {
	
	case true:
		cycle_val <<= 1;

	case false:
		cycle_val >>= 1;

	}

	switch (cycle_val) {

	case 0:
		cycle_val = 4;
		lineType = CHARACTER;

	case 1:
		lineType = DESCRIPTION;
	
	case 2:
		lineType = DIALOGUE;

	case 4:
		lineType = CHARACTER;

	case 8:
		cycle_val = 1;
		lineType = DESCRIPTION;
	
	}

}