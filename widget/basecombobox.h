#ifndef BASECOMBOBOX_H
#define BASECOMBOBOX_H

#include <QComboBox>
#include "../core/core_system.h"

class BaseComboBox : public QComboBox
{
    Q_OBJECT
public:
    BaseComboBox(const QString& tooltip,QWidget *parent=nullptr);
    void enableShadowEffect();
};

#endif
