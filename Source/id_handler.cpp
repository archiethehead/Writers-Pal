#include <id_handler.h>

bool idHashmap[0xffff] = { false };

uint16_t getid() {

	uint16_t newid = 0;

	do {
	
		uint16_t newid = randid();
	
	} while (!(idHashmap[newid]));

	idHashmap[newid] = true;
	return newid;

}

uint16_t randid() {

	time_t timestamp;
	time(&timestamp);

	timestamp = (uint64_t)timestamp * 48271 % 0x7fffffff;
	return (uint16_t)(timestamp % 0xffff);

}

void freeid(uint16_t id) {

	idHashmap[id] = false;

}