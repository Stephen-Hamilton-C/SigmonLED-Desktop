#include "palettemode.h"

PaletteMode PaletteMode::STATIC(0);
PaletteMode PaletteMode::SCROLLING(1);
PaletteMode PaletteMode::SOLID(2);

char PaletteMode::value()
{
    return _value;
}

PaletteMode::PaletteMode(uint8_t value): _value((char)value)
{

}
