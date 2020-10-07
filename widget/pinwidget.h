#ifndef PIN_WIDGET_H
#define PIN_WIDGET_H

#include <QWidget>

class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(const QPixmap& ,QWidget * = nullptr);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
protected slots:
    void closePinWidget();
private:
    QPixmap __pixmap;
    QColor borderColor;
    QPoint offset;
};

#endif
