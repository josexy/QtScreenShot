#include "basecombobox.h"
#include <QValidator>
#include <QGraphicsDropShadowEffect>

BaseComboBox::BaseComboBox(const QString& tooltip,QWidget *parent) : QComboBox(parent)
{
    QString ss="QComboBox{ border:1px solid black; background-color: white; color:black; }"
               "QComboBox::drop-down{ background-color: %1; color:black; }"
               "QComboBox QAbstractItemView{ "
               "background-color: white; "
               "border:1px solid black;"
               "color:black;"
               "selection-background-color: %1;"
               "}";
    setFont(QFont(qCore->getFontFamily()));
    setStyleSheet(ss.arg(qCore->getWidgetBgColor().name()));
    setToolTip(tooltip);
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);

    enableShadowEffect();
}

void BaseComboBox::enableShadowEffect()
{
    if(qCore->getShadowEffect()){
        auto dsEffect = new QGraphicsDropShadowEffect(this);
        dsEffect->setBlurRadius(5);
        dsEffect->setOffset(0);
        dsEffect->setColor(QColor(Qt::black));
        setGraphicsEffect(dsEffect);
    }
}
