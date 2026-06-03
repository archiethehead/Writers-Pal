//////////////////
// id_handler.h //
//////////////////

#include <stdint.h>
#include <ctime>

extern bool idHashmap[];

uint16_t getid();
uint16_t randid();
void freeid(uint16_t id);