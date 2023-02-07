#ifndef DEFAULTPALETTE_H
#define DEFAULTPALETTE_H

class DefaultPalette
{
public:
    static DefaultPalette RAINBOW;
    static DefaultPalette RAINBOW_STRIPE;
    static DefaultPalette CLOUD;
    static DefaultPalette PARTY;
    static DefaultPalette OCEAN;
    static DefaultPalette LAVA;
    static DefaultPalette FOREST;
    static DefaultPalette CUSTOM;

    char value();

private:
    DefaultPalette(char value);

    char _value;
};

#endif // DEFAULTPALETTE_H
