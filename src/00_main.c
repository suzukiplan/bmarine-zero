#include "header.h"

void main(void) 
{
    init_palette();

    for (uint8_t i = 0; i < 8; i++) {
        GV->hi[i] = 7 - i + 1;
        GV->sc[i] = i + 1;
    }
    title();
}