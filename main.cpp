
#include <QApplication>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QTranslator>
#include <QSystemTrayIcon>
#include "JSON/include/JsonDocument.h"
#include "core/core_system.h"
#include "tmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("QtScreenShot");
    a.setApplicationDisplayName("QtScreenShot");

    __LOG__("QtScreenShot has started",1);

    if(!QSystemTrayIcon::isSystemTrayAvailable()){
        __LOG__("System tray is not currently unavailable",3);
        return -1;
    }
    a.setQuitOnLastWindowClosed(false);

    QTranslator translator;
    std::ifstream ifs("config.json");
    __LOG__("Load config.json file",1);
    JsonDocument doc(ifs);
    if(doc.isObject()){
        ifs.close();
        try{
            if(doc.object()->value("settings")->value("language")->data().data3){
                if(translator.load(":/ts/res/translator/UI_Translation_zh_CN.qm")){
                    QCoreApplication::installTranslator(&translator);
                    __LOG__("Load translator ts file successfully!",1);
                }else{
                    __LOG__("Load translator ts file failed",2);
                }
            }
        }catch(JsonException& err){
            __LOG__(err.what(),2);
            return -1;
        }
    }

    QSystemSemaphore semaphore("QSystemSemaphore"+a.applicationName(), 1);
    semaphore.acquire();
#ifndef Q_OS_WIN32
    QSharedMemory sm(a.applicationName());
    if(sm.attach()) sm.detach();
#endif
    QSharedMemory sm2(a.applicationName());
    if(sm2.attach()){
        semaphore.release();
        __LOG__("Application has been startd!",2);
        return -1;
    }
    else{
        __LOG__("Application starts successfully!.",1);
        sm2.create(1);
    }
    semaphore.release();

    TMain w;
    w.showMinimized();
    w.setVisible(false);

    return a.exec();
}
