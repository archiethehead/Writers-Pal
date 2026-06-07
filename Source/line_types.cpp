////////////////////
// line_types.cpp //
////////////////////

#include <line_types.hpp>

scriptLine::line(int start, float type, std::string content) {

	startLineNum = start;
	lineType = type;
	text = content;

}

bool scriptLine::operator < (const line& thisLine) {

	return (startLineNum < thisLine.startLineNum);

}

scriptLine::~line() {

	

}

void scriptLine::cycleType(bool right) {

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