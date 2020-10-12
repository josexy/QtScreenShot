#include "pinwidget.h"
#include "core/core_system.h"
#include <QKeyEvent>
#include <QPainter>
#include <QShortcut>

PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent) : QWidget(parent),__pixmap(pixmap)
{
    setWindowTitle(tr("Pin Window"));
    setWindowIcon(QIcon(":/svg/res/images/logo.svg"));
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(closePinWidget()));
    borderColor=qCore->getHoverColor();
    QSize size=qCore->getScreenSize();
    move((size.width()-width())/2,(size.height()-height())/2);
}
void PinWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(rect(),__pixmap);
    p.setPen(QPen(borderColor,2));
    p.drawRect(rect());
    QWidget::paintEvent(e);
}
void PinWidget::enterEvent(QEvent *)
{
    setCursor(Qt::SizeAllCursor);
    borderColor=qCore->getHoverColor();
    update();
}
void PinWidget::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
    borderColor=qCore->getCheckedColor();
    update();
}
void PinWidget::mousePressEvent(QMouseEvent *e)
{
    offset=e->pos();
}
void PinWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::MouseButton::LeftButton){
        QPoint pos=e->pos();
        move(mapToGlobal(QPoint(pos.x()-offset.x(),pos.y()-offset.y())));
    }
}
void PinWidget::closePinWidget()
{
    close();
}
