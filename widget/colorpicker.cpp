#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

ColorPicker::ColorPicker(const QString &tooltip, QWidget *parent) : QLabel(parent)
{
    setToolTip(tooltip);
    setCursor(Qt::PointingHandCursor);
    buttonType=ButtonType::Hover;
}

void ColorPicker::setColor(QColor c)
{
    color=c;
    update();
}

QColor ColorPicker::getColor() const
{
    return color;
}

void ColorPicker::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.setPen(QPen(Qt::white,2));
    p.drawRect(rect());
    switch (buttonType) {
    case ButtonType::Pressed:p.setBrush(color.darker(120));break;
    case ButtonType::Checked:p.setBrush(color.darker(140));break;
    default:p.setBrush(color);break;
    }
    p.drawRect(rect());

    QWidget::paintEvent(e);
}

void ColorPicker::mousePressEvent(QMouseEvent *ev)
{
    if(ev->type()==QMouseEvent::MouseButtonPress){
        if(ev->button()==Qt::RightButton){
            QColor c=QColorDialog::getColor(qCore->getWidgetBgColor(),this);
            if (c.spec()!=QColor::Invalid){
                color=c;
                emit sendColor(color);
                update();
            }
        }
        else if(ev->button()==Qt::LeftButton){
            buttonType=ButtonType::Pressed;
            update();
        }
    }
}

void ColorPicker::mouseReleaseEvent(QMouseEvent *)
{
    buttonType=ButtonType::Checked;
    update();
}

void ColorPicker::enterEvent(QEvent *)
{
    buttonType=ButtonType::Hover;
    update();
}

void ColorPicker::leaveEvent(QEvent *)
{
    buttonType=ButtonType::Leave;
    update();
}
