#ifndef ARDUINOCOMMANDER_H
#define ARDUINOCOMMANDER_H

#include <QObject>
#include "devicemanager.h"
#include "palette.h"
#include "defaultpalette.h"
#include "palettemode.h"

class ArduinoCommander : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoCommander(QObject *parent = nullptr);

    void setBlending(bool blending);
    void setBrightness(uint8_t brightness);
    void setColor(QColor color);
    void setPalette(Palette& palette);
    void setPalette(DefaultPalette palette);
    void setDelay(uint16_t delay);
    void sleep();
    void wake();
    void setPaletteMode(PaletteMode paletteMode);
    void setStoredColor(QColor color);
    void setStretch(uint8_t stretch);

    DeviceManager deviceManager = DeviceManager(this);

signals:

private:
    uint8_t exclude10(uint8_t num);
};

#endif // ARDUINOCOMMANDER_H
