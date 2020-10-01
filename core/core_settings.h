#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include "core.h"
#include "JSON/include/JsonDocument.h"

using namespace Json;

class core_settings
{
public:
    explicit core_settings(const std::string& filename="config.json");
    ~core_settings();
    Data read(const std::string &,const std::string&);
    void write(const std::string &,const std::string&,Data);
    void flush();
private:
    JsonDocument * pDoc=nullptr;
    std::ifstream ifs;
    std::ofstream ofs;
    std::string config_json_filename;
};

#endif
