//////////////////
// line_types.h //
//////////////////

// The float values determine
// the relative spacing apart
// on the page for the different
// types of text. The measurements
// calculated with real screenplay
// proportions.
// 
// |
// |
// V

#define COVER 0.0f
#define DESCRIPTION 0.1765f
#define DIALOGUE 0.294118f
#define CHARACTER 0.4353f

#include <id_handler.h>
#include <stdint.h>
#include <string>

typedef struct line {

public:

	uint16_t lineID;
	uint16_t startLineNum;
	float lineType;
	std::string text;

	line(uint16_t start = 0, float type = 0, std::string content = "");
	~line();

	void cycle_type(bool right);

	bool operator < (const line& thisLine) {
	
		return (startLineNum < thisLine.startLineNum);
	
	}

private:

	uint8_t cycle_val = 1;

} scriptLine;