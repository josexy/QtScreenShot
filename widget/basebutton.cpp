#include "basebutton.h"
#include <QGraphicsDropShadowEffect>

BaseButton::BaseButton(const QPixmap& pixmap,const QString& tooltip,QWidget *parent,bool checkable)
    : QPushButton(parent)
{
    __parent=parent;

    setCheckable(checkable);

    QString styleSheet="BaseButton{border:none;background-color:%1}"
                       "BaseButton:hover{background-color:%2}"
                       "BaseButton:pressed{background-color:%3}"
                       "BaseButton:checked{background-color:%4}";
    setStyleSheet(styleSheet
                  .arg(qCore->getNormalColor().name())
                  .arg(qCore->getHoverColor().name())
                  .arg(qCore->getPressedColor().name())
                  .arg(qCore->getCheckedColor().name()));
    setSVGIcon(pixmap);

    setToolTip(tooltip);
    enableShadowEffect();
}

BaseButton::BaseButton(const QString &pixmapUrl,const QString& tooltip, QWidget *parent, bool checkable)
    : BaseButton(QPixmap(pixmapUrl),tooltip,parent,checkable)
{}

void BaseButton::enableShadowEffect()
{
    if(qCore->getShadowEffect()){
        auto dsEffect = new QGraphicsDropShadowEffect(this);
        dsEffect->setBlurRadius(5);
        dsEffect->setOffset(0);
        dsEffect->setColor(QColor(Qt::black));
        setGraphicsEffect(dsEffect);
    }
}

void BaseButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    __prevCursor=cursor();
    setCursor(Qt::ArrowCursor);
}

void BaseButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    setCursor(__prevCursor);
}

void BaseButton::setSVGIcon(const QIcon &icon)
{
    setIcon(icon);
    setIconSize(QSize(width(),__parent->height()-qCore->borderPadding()));
}

