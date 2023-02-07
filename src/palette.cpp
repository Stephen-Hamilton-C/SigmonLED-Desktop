#include "palette.h"

Palette::Palette():
    _name("Untitled"), _colors({{QColor(0, 0, 0), QColor(0, 0, 0)}})
{ }

Palette::Palette(Palette& palette):
    _name(palette.getName()), _colors(palette.getColors())
{

}

Palette::Palette(QList<QColor> colors):
    _name("Untitled"), _colors(colors)
{
    validateColors();
}

QList<QColor> Palette::getFullPalette()
{
    if(_colors.size() == 16) return _colors;

    // Repeat the list until a pattern is created
    auto fullPalette = QList<QColor>(_colors);
    while(fullPalette.size() < 16) {
        fullPalette.append(_colors);
    }

    return fullPalette;
}

QString Palette::getName()
{
    return _name;
}

QList<QColor> Palette::getColors()
{
    return _colors;
}

void Palette::setName(const QString name)
{
    _name = name;
}

bool Palette::canExpand()
{
    return _colors.size() < 16;
}

bool Palette::canShrink()
{
    return _colors.size() > 1;
}

void Palette::expand()
{
    if(!canExpand()) {
        throw std::runtime_error("Cannot expand a palette with 16 colors!");
    }

    do {
        _colors.append(QColor(0, 0, 0));
    } while (16 % _colors.size() != 0);
}

void Palette::shrink()
{
    if(!canShrink()) {
        throw std::runtime_error("Cannot shrink a palette with only 1 color!");
    }

    do {
        _colors.removeLast();
        } while(16 % _colors.size() != 0);
}

QByteArray Palette::toQByteArray()
{
    const uint8_t bytesLen = 50;
    char bytes[bytesLen] = {};
    bytes[0] = 'C';
    bytes[bytesLen - 1] = '\n';

    QList<QColor> colors = getFullPalette();
    for(int i = 0; i < colors.size(); i++) {
        uint8_t r = exclude10(colors[i].red());
        uint8_t g = exclude10(colors[i].green());
        uint8_t b = exclude10(colors[i].blue());
        bytes[i*3+1] = r;
        bytes[i*3+2] = g;
        bytes[i*3+3] = b;
    }

    return QByteArray::fromRawData(bytes, bytesLen);
}

Palette::Palette(QString name, QList<QColor> colors)
    : _name(name), _colors(colors)
{
    validateColors();
}

void Palette::validateColors()
{
    if(_colors.empty() || 16 % _colors.size()) {
        throw std::invalid_argument("A Palette must have 1, 2, 4, 8, or 16 colors!");
    }
}

uint8_t Palette::exclude10(uint8_t num)
{
    if(num == 10) return 11;
    return num;
}
