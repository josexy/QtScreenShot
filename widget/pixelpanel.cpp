#include "pixelpanel.h"
#include "capturewindow.h"
#include "pixelwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

PixelPanel::PixelPanel(QWidget *parent) :
    QWidget(parent),
    __w((CaptureWindow*)parent)
{
    setFixedSize(250,250);

    initUI();
}
void PixelPanel::initUI()
{
    QVBoxLayout* vBoxLayout=new QVBoxLayout(this);
    vBoxLayout->setMargin(0);
    vBoxLayout->setContentsMargins(0,0,0,0);
    vBoxLayout->setSpacing(0);

    QHBoxLayout* hBoxColorLayout=new QHBoxLayout();
    QVBoxLayout* vBoxInfoLayout=new QVBoxLayout();

    vBoxInfoLayout->setMargin(0);
    vBoxInfoLayout->setContentsMargins(0,0,0,0);
    vBoxInfoLayout->setSpacing(0);

    hBoxColorLayout->setMargin(0);
    hBoxColorLayout->setContentsMargins(0,0,0,0);
    hBoxColorLayout->setSpacing(0);

    pixelWidget=new PixelWidget(this);
    lbPOS=new QLabel(this); lbPOS->setAlignment(Qt::AlignCenter);
    lbMSG=new QLabel(this); lbMSG->setAlignment(Qt::AlignCenter);
    lbMSG2=new QLabel(this); lbMSG2->setAlignment(Qt::AlignCenter);
    lbColor=new QLabel(this); lbColor->setAlignment(Qt::AlignCenter);
    lbColor->setFixedSize(25,25);
    lbRGB=new QLabel(this); lbRGB->setAlignment(Qt::AlignCenter);
    vBoxLayout->addWidget(pixelWidget);

    hBoxColorLayout->addItem(new QSpacerItem(width()/4,0));
    hBoxColorLayout->addWidget(lbColor);
    hBoxColorLayout->addWidget(lbRGB);
    vBoxInfoLayout->addLayout(hBoxColorLayout);

    vBoxInfoLayout->addWidget(lbPOS);
    vBoxInfoLayout->addWidget(lbMSG);
    vBoxInfoLayout->addWidget(lbMSG2);

    vBoxLayout->addLayout(vBoxInfoLayout);
}

PixelPanel::~PixelPanel()
{
}

void PixelPanel::setPos(const QPoint& pos)
{
    point=pos;
}
QString PixelPanel::convertHEXColor()
{
    QString hexColor;
    int r=qRed(rgb);
    int g=qGreen(rgb);
    int b=qBlue(rgb);
    QString red=QString::number(r/16,16)+QString::number(r%16,16);
    QString green=QString::number(g/16,16)+QString::number(g%16,16);
    QString blue=QString::number(b/16,16)+QString::number(b%16,16);
    hexColor="#"+red.toUpper()+green.toUpper()+blue.toUpper();
    return hexColor;
}

void PixelPanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    auto pixmap=qCore->getbgPixmap();

    rgb= pixmap.toImage().pixel(point.x(),point.y());
    pixelWidget->setColor(rgb);
    pixelWidget->setPos(point);
    pixelWidget->setPixmap(pixmap);
    pixelWidget->update();

    QString styleSheet;
    if(QColor(rgb).red()> 100 && QColor(rgb).green()>100 && QColor(rgb).blue()>100)
        styleSheet="color: gray; font-weight: bold";
    else
        styleSheet="color: white; font-weight: bold";

    lbColor->setStyleSheet(QString("border:1px solid white;background-color: %1")
                           .arg(QColor(rgb).name()));
    lbRGB->setStyleSheet(styleSheet);
    lbPOS->setText(QString("(%1 , %2)").arg(point.x()).arg(point.y())); lbPOS->setStyleSheet(styleSheet);
    lbMSG->setText(tr("[C] copy color value")); lbMSG->setStyleSheet(styleSheet);
    lbMSG2->setText(tr("[Shift+A] RGB/HEX")); lbMSG2->setStyleSheet(styleSheet);

    QString rgbText;
    if(!__convertRGBHEX){
        rgbText=QString("(%3 , %4 , %5)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb));
    }else{
        rgbText=convertHEXColor();
    }

    lbRGB->setText(rgbText);

    if(__copy){
        QClipboard* clipboard=QApplication::clipboard();
        clipboard->setText(rgbText);
        __copy=false;
    }
    p.setOpacity(0.5);
    p.fillRect(rect(),Qt::black);

    QWidget::paintEvent(event);
}
