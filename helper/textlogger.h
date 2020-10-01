#ifndef TEXTLOGGER_H
#define TEXTLOGGER_H

#include <QFile>
#include <QDateTime>

enum class TextLoggerType{
    Unknown=0,
    Information=1,
    Warning=2,
    Error=3
};

class TextLogger
{
public:
    explicit TextLogger(const QString&file="log.txt");
    ~TextLogger();

    void writeLine(const QString &text ,TextLoggerType loggerType=TextLoggerType::Information);

private:
    QFile __File;
};

#endif
