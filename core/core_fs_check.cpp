#include "core_fs_check.h"
#include <QDir>

core_fs_check::core_fs_check(QString &file, const QString &save_directory, QObject *parent)
    : QObject(parent)
{
    QDir dir(save_directory);
    QString absolutePath=dir.absolutePath();
    QString baseFileName = QFileInfo(file).baseName();
    QString suffix=QFileInfo(file).completeSuffix();

    auto files=dir.entryList();
    int index=1;
    QString tf=file;
    for(;;){
        if(files.indexOf(QFileInfo(tf).fileName())!=-1){
            tf=QDir::toNativeSeparators(absolutePath+"/"+baseFileName+"_"+QString::number(index)+"."+suffix);
            index++;
        }else break;
    }
    file=tf;
}

