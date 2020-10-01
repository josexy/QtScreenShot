#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "core.h"
#include "../helper/textlogger.h"

class core_system
{
public:
    static core *instance();
    static TextLogger * getTextLogger();
private:
    core_system();
    core_system(const core_system&);
    core_system& operator=(const core_system&);
};

// global instance
#define qCore core_system::instance()
#define qCoreTextLogger core_system::getTextLogger()

#define __LOG__(TEXT,ID) qCoreTextLogger->writeLine((TEXT),static_cast<TextLoggerType>((ID)))

#endif
