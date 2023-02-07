#include "arduinocommander.h"
#include "qcolor.h"

ArduinoCommander::ArduinoCommander(QObject *parent)
    : QObject{parent}
{

}

void ArduinoCommander::setBlending(bool blending)
{
    const char command[] = {'l', blending, '\n'};
    auto bytes = QByteArray::fromRawData(command, 3);
    deviceManager.write(bytes);
}

void ArduinoCommander::setBrightness(uint8_t brightness)
{
    const char command[] = {'b', (char)brightness, '\n'};
    auto bytes = QByteArray::fromRawData(command, 3);
    deviceManager.write(bytes);
}

void ArduinoCommander::setColor(QColor color)
{
    uint8_t r = exclude10(color.red());
    uint8_t g = exclude10(color.green());
    uint8_t b = exclude10(color.blue());

    const char command[] = {'c', (char)r, (char)g, (char)b, '\n'};
    auto bytes = QByteArray::fromRawData(command, 5);
    deviceManager.write(bytes);
}

void ArduinoCommander::setPalette(Palette& palette)
{
    deviceManager.write(palette.toQByteArray());
}

void ArduinoCommander::setPalette(DefaultPalette palette)
{
    const char command[] = {'p', palette.value(), '\n'};
    auto bytes = QByteArray::fromRawData(command, 3);
    deviceManager.write(bytes);
}

void ArduinoCommander::setDelay(uint16_t delay)
{
    uint8_t superior = (delay >> 8) & 0xff;
    uint8_t inferior = delay & 0xff;

    const char command[] = {'d', (char)superior, (char)inferior, '\n'};
    auto bytes = QByteArray::fromRawData(command, 4);
    deviceManager.write(bytes);
}

void ArduinoCommander::sleep()
{
    const char command[] = {'0', '\n'};
    auto bytes = QByteArray::fromRawData(command, 2);
    deviceManager.write(bytes);
}

void ArduinoCommander::wake()
{
    const char command[] = {'1', '\n'};
    auto bytes = QByteArray::fromRawData(command, 2);
    deviceManager.write(bytes);
}

void ArduinoCommander::setPaletteMode(PaletteMode paletteMode)
{
    const char command[] = {'P', paletteMode.value(), '\n'};
    auto bytes = QByteArray::fromRawData(command, 3);
    deviceManager.write(bytes);
}

void ArduinoCommander::setStoredColor(QColor color)
{
    uint8_t r = exclude10(color.red());
    uint8_t g = exclude10(color.green());
    uint8_t b = exclude10(color.blue());

    const char command[] = {'S', (char)r, (char)g, (char)b, '\n'};
    auto bytes = QByteArray::fromRawData(command, 5);
    deviceManager.write(bytes);
}

void ArduinoCommander::setStretch(uint8_t stretch)
{
    const char command[] = {'s', (char)stretch, '\n'};
    auto bytes = QByteArray::fromRawData(command, 3);
    deviceManager.write(bytes);
}

uint8_t ArduinoCommander::exclude10(uint8_t num)
{
    if(num == 10) return 11;
    return num;
}
