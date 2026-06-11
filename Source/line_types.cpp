////////////////////
// line_types.cpp //
////////////////////

#include "line_types.hpp"

scriptLine::line(int start, float type, std::string content) {

	startLineNum = start;
	lineType = type;
	text = content;

	if (lineType == DESCRIPTION) {
	
		cycle_val = 1;
	
	}

	else if (lineType == DIALOGUE) {

		cycle_val = 2;

	}

	else if (lineType == CHARACTER) {

		cycle_val = 4;

	}

}

bool scriptLine::operator < (const line& thisLine) {

	return (startLineNum < thisLine.startLineNum);

}

scriptLine::~line() {

	

}

void scriptLine::cycleType(bool right) {

	switch ((int)right) {
	
	case 1:
		cycle_val <<= 1;
		break;

	case 0:
		cycle_val >>= 1;
		break;

	default:
		break;

	}

	switch (cycle_val) {

	case 0:
		cycle_val = 4;
		lineType = CHARACTER;
		break;

	case 1:
		lineType = DESCRIPTION;
		break;
	
	case 2:
		lineType = DIALOGUE;
		break;

	case 4:
		lineType = CHARACTER;
		break;

	case 8:
		cycle_val = 1;
		lineType = DESCRIPTION;
		break;
	
	}

}