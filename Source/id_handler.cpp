#include <id_handler.h>

bool idHashmap[0xffff] = { false };

uint16_t getid() {

	uint16_t newid = 0;

	do {
	
		time_t timestamp;
		time(&timestamp);

		timestamp = (uint64_t)timestamp * 48271 % 0x7fffffff;
		newid = (uint16_t)(timestamp % 0xffff);
	
	} while (idHashmap[newid]);

	idHashmap[newid] = true;
	return newid;

}

void freeid(uint16_t id) {

	idHashmap[id] = false;

}