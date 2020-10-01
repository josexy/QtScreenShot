#include "core_system.h"

core * core_system::instance()
{
    static core __core;
    return &__core;
}

TextLogger *core_system::getTextLogger()
{
    static TextLogger __textLogger;
    return &__textLogger;
}
