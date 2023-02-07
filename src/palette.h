#ifndef PALETTE_H
#define PALETTE_H

#include <QList>
#include <QColor>
#include <QString>
#include <QByteArray>

class Palette
{
public:
    Palette();
    Palette(Palette& palette);
    Palette(QString name, QList<QColor> colors);
    Palette(QList<QColor> colors);

    QList<QColor> getFullPalette();
    QString getName();
    QList<QColor> getColors();

    void setName(const QString name);

    bool canExpand();
    bool canShrink();
    void expand();
    void shrink();
    void setColor(int index, QColor color);
    QByteArray toQByteArray();

private:
    QString _name = "Untitled";
    QList<QColor> _colors = QList<QColor>();

    void validateColors();
    uint8_t exclude10(uint8_t num);
};

#endif // PALETTE_H
