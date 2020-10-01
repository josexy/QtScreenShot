#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QLabel>
#include <QWidget>
#include "../core/core_system.h"

class ColorPicker : public QLabel
{
    Q_OBJECT
public:

    enum class ButtonType{
      Hover,
      Leave,
      Pressed,
      Checked
    };
    ColorPicker(const QString&tooltip,QWidget* parent=nullptr);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void setColor(QColor);
    QColor getColor() const;

signals:
    void sendColor(QColor);
private:
    QColor color;
    ButtonType buttonType;
};

#endif
