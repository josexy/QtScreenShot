#include "textlogger.h"

TextLogger::TextLogger(const QString& file) : __File(file)
{
    __File.open(QIODevice::Append|QIODevice::Text);
}

TextLogger::~TextLogger()
{
    if(__File.isOpen()){
        __File.flush();
        __File.close();
    }
}

void TextLogger::writeLine(const QString &text, TextLoggerType loggerType)
{
    QString sloggerType;
    switch (loggerType) {
    case TextLoggerType::Information: sloggerType="Information"; break;
    case TextLoggerType::Warning: sloggerType="Warning"; break;
    case TextLoggerType::Error: sloggerType="Error"; break;
    default:sloggerType="Unknown";break;
    }
    QString __data=QString("*[%1] %2 <-> %3\n")
            .arg(sloggerType)
            .arg(text)
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    __File.write(__data.toLatin1());
    __File.flush();
}
