#include "core_settings.h"

core_settings::core_settings(const std::string &filename): config_json_filename(filename)
{
    QString default_config_json=
R"({
    "settings": {
        "save_directory": "%1",
        "font_family": "%2",
        "language": false,
        "start_on_boot": false,
        "shadow_effect": false,
        "image_type": "PNG",
        "image_quality": 50,
        "dot_size": 12,
        "format_string": "yyyy_MM_dd_hh_mm_ss",
        "dot_color": "#3300FF",
        "border_color": "#33FFFF",
        "ui_color": "#33FFFF",
        "enable_upload": false,
        "enable_proxy": false,
        "smms_authorization": ""
    },
    "keymap": {
        "cap_desktop": "Ctrl+Shift+A",
        "cap_desktop_copy": "Ctrl+Shift+C",
        "cap_usercontrol": "F1",
        "w_settings": "Alt+Shift+S",
        "w_quit": "Alt+Shift+Q"
    },
    "proxies": {
        "type": "http",
        "ip": "127.0.0.1",
        "port": 8888
    }
})";
    std::string json = QString(default_config_json)
            .arg(core::getCurrentUserPaths().at(1))
            .arg(core::getAppFont().family())
            .toStdString().c_str();

    ifs.open(config_json_filename);
    pDoc=new JsonDocument(ifs,true);
    ifs.close();

    if(!pDoc->object()){
        delete pDoc;
        ofs.open(config_json_filename,std::ios::out);
        ofs.write(json.c_str(),json.size());
        ofs.flush();
        pDoc=new JsonDocument(json,true);
    }
}

core_settings::~core_settings()
{
    if(pDoc) delete pDoc;
}

Data core_settings::read(const std::string & primaryKey,const std::string& attr)
{
    if(!pDoc)return Data();
    if(!pDoc->object())return Data();
    return pDoc->object()->value(primaryKey)->value(attr)->data();
}

void core_settings::write(const std::string & primaryKey,const std::string& attr,Data data)
{
    if(!pDoc)return;
    pDoc->object()->value(primaryKey)->value(attr)->setData(data);
}

void core_settings::flush()
{
    if(!pDoc)return;
    std::string s=pDoc->toJson();

    ofs.open(config_json_filename,std::ios::out|std::ios::trunc);
    ofs.write(s.c_str(),s.size());
    ofs.flush();
    ofs.close();
}
