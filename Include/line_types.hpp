//////////////////
// line_types.h //
//////////////////

#ifndef LINE_TYPES_H
#define LINE_TYPES_H

// The float values determine
// the relative spacing apart
// on the page for the different
// types of text. The measurements
// were calculated with real
// screenplay proportions.
// 
// |
// |
// V

#define COVER 0.0f
#define DESCRIPTION 0.1765f
#define DIALOGUE 0.294118f
#define CHARACTER 0.4353f

#include <string>

typedef struct line {

	int startLineNum;
	float lineType;
	std::string text;

	line(int start = 0, float type = 0, std::string content = "");
	~line();

	bool operator < (const line& thisLine);

	void cycleType(bool right);

private:

	int cycle_val = 1;

} scriptLine;

#endif