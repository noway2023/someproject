#ifndef COLORCOMBOX_H
#define COLORCOMBOX_H

#include <QToolButton>
#include <QRgb>

class ColorCombox : public QToolButton
{
    Q_OBJECT

public:
    ColorCombox(QWidget* parent = 0);
    ~ColorCombox();

    void setButtonIcon(const QString& imageFile, QColor color);

private:
    QMenu* createColorMenu(const char* slot, const char* slotColorBoard);
    QIcon createColorToolButtonIcon(const QString& imageFile, QColor color);
    QIcon createColorIcon(QColor color);

public:
    void setColor(QRgb color);
    QRgb getColor();

signals:
    void sigColorChanged(QColor color);

public slots:
    void OnColorChanged();           // 文本颜色设置
    void OnShowColorBoard();         // 颜色板
    void changeColor(QColor color);

private:
    QColor m_color;
};

#endif