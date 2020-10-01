#ifndef CORE_FS_CHECK_H
#define CORE_FS_CHECK_H

#include <QObject>

class core_fs_check : public QObject
{
    Q_OBJECT
public:
    explicit core_fs_check(QString& file,const QString & save_directory,QObject *parent = nullptr);
};

#endif
