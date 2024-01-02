#include "header.h"

void main(void) 
{
    init_palette();

    for (uint8_t i = 0; i < 8; i++) {
        GV->sc[i] = 7 - i + 1;
        GV->hi[i] = i + 1;
    }
    title();
}