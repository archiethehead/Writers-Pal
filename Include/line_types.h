//////////////////
// line_types.h //
//////////////////

#define COVER 0.1765
#define DESCRIPTION 0.1765
#define DIALOGUE 0.294118
#define CHARACTER 0.4353

#include <stdint.h>
#include <string>

typedef struct line {

	uint16_t lineID;
	uint16_t startLineNum;
	float lineType;
	std::string text;

} scriptLine;