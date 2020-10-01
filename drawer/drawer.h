#ifndef DRAWER_H
#define DRAWER_H

#include <QObject>
#include <QPainter>

class Drawer : public QObject
{
    Q_OBJECT
public:
    explicit Drawer(QObject *parent = nullptr);
    ~Drawer(){}
};

#endif
