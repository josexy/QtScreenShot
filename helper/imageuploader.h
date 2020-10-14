#ifndef IMAGEUPLOADER_H
#define IMAGEUPLOADER_H

#include <QThread>
#include "core/core_system.h"

class UploaderThread: public QThread
{
    Q_OBJECT
public:
    UploaderThread(const QString &filename,QObject *parent=nullptr);
    void run();
private:
    QString filename;
};

#endif
