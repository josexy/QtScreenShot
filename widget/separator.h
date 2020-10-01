#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <QLabel>
#include "../core/core_system.h"

class Separator : public QLabel
{
    Q_OBJECT
public:
    Separator(QWidget *parent=nullptr);
    void paintEvent(QPaintEvent *);
};

#endif
