#include "imageuploader.h"
#include <QProcess>

UploaderThread::UploaderThread(const QString &fn,QObject *parent)
    : QThread(parent),
      filename(fn) { }

void UploaderThread::run()
{
    if(filename.isEmpty()) return;
    QString appPath = QCoreApplication::applicationDirPath();
    QString script=QDir::toNativeSeparators(appPath+"/smms_uploader.py");
    if(!QFile::exists(script))return;

    QProcess proc;
    proc.setEnvironment(QProcess::systemEnvironment());

    QStringList params;
    params<<script;
    params<<qCore->getSMMSAuthorization();
    params<<filename;

    if(qCore->getEnableProxy()){
        QString proxies=qCore->getProxyType()?"socks5":"http";
        proxies+="://"+qCore->getProxyIP()+":"+QString::number(qCore->getProxyPort());
        params<<proxies;
    }
    proc.start("python",params);
    proc.waitForStarted();
    proc.waitForFinished();

    __LOG__(proc.readAllStandardOutput(),1);
}
