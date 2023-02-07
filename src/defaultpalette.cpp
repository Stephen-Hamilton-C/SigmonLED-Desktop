#include "defaultpalette.h"

DefaultPalette DefaultPalette::RAINBOW = DefaultPalette('r');
DefaultPalette DefaultPalette::RAINBOW_STRIPE = DefaultPalette('R');
DefaultPalette DefaultPalette::CLOUD = DefaultPalette('c');
DefaultPalette DefaultPalette::PARTY = DefaultPalette('p');
DefaultPalette DefaultPalette::OCEAN = DefaultPalette('o');
DefaultPalette DefaultPalette::LAVA = DefaultPalette('l');
DefaultPalette DefaultPalette::FOREST = DefaultPalette('f');
DefaultPalette DefaultPalette::CUSTOM = DefaultPalette('C');

char DefaultPalette::value()
{
    return _value;
}

DefaultPalette::DefaultPalette(char value): _value(value)
{

}
