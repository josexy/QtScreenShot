#ifndef BASETOOL_H
#define BASETOOL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include "core/core_system.h"

class BaseTool : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTool(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
};

#endif
