#include "header.h"

void main(void) 
{
    init_palette();

    for (uint8_t i = 0; i < 8; i++) {
        GV->hi[i] = 0;
        GV->sc[i] = 0;
    }
    title();
}