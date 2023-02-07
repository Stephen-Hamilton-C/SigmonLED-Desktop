#ifndef PALETTEMODE_H
#define PALETTEMODE_H


class PaletteMode
{
public:
    static PaletteMode STATIC;
    static PaletteMode SCROLLING;
    static PaletteMode SOLID;

    char value();

private:
    PaletteMode(char value);

    char _value;
};

#endif // PALETTEMODE_H
