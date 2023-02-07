#ifndef PALETTEMODE_H
#define PALETTEMODE_H

#include <stdint.h>

class PaletteMode
{
public:
    static PaletteMode STATIC;
    static PaletteMode SCROLLING;
    static PaletteMode SOLID;

    char value();

private:
    PaletteMode(uint8_t value);

    char _value;
};

#endif // PALETTEMODE_H
