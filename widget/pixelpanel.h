#ifndef PIXELPANEL_H
#define PIXELPANEL_H

#include <QWidget>

class CaptureWindow;
class PixelWidget;
class QLabel;

class PixelPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PixelPanel(QWidget *parent = nullptr);
    ~PixelPanel();
    void paintEvent(QPaintEvent *);

    void setPos(const QPoint &);
    QString convertHEXColor();

    bool __copy=false;
    bool __convertRGBHEX=false;

    void initUI();

private:
    CaptureWindow *__w;
    PixelWidget * pixelWidget;

    QLabel *lbPOS;
    QLabel *lbMSG;
    QLabel *lbMSG2;
    QLabel *lbColor;
    QLabel *lbRGB;

    QRgb rgb;
    QPoint point;
};

#endif
