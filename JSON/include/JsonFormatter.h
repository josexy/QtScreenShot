#ifndef JSONTEST_JSONFORMATTER_H
#define JSONTEST_JSONFORMATTER_H

#include "JsonValue.h"

namespace Json {
    class JsonFormatter {
    public:
        JsonFormatter(int indent = 4) : __indent(indent) {}

    protected:

        const std::string &getJson(const std::vector<std::string> &lstJsonText) {
            __formatIndentObject(lstJsonText, __json);
            return __json;
        }

        void __formatIndentObject(const std::vector<std::string> &lstJsonText, std::string &json) {
            __json="";
            bool flag;
            int depth = 0, size = lstJsonText.size();
            for (int i = 0; i < size; ++i) {
                std::string v = lstJsonText[i];
                if (v[0] == JSON_OBJECT_BEGIN || v[0] == JSON_ARRAY_BEGIN) { // [ , {
                    flag = true;
                    depth++;
                    if (v[0] == JSON_ARRAY_BEGIN && lstJsonText[i + 1][0] == JSON_OBJECT_BEGIN) flag = false; // [{
                    if (v[0] == JSON_ARRAY_BEGIN && lstJsonText[i + 1][0] == JSON_ARRAY_BEGIN) flag = false; // [[
                    if (lstJsonText[i + 1][0] == JSON_ARRAY_END || lstJsonText[i + 1][0] == JSON_OBJECT_END)
                        flag = false; // [], {}
                    __appendJson(json, v);
                    if (flag) __appendJson(json, __LF);
                } else if (v[0] == JSON_OBJECT_END || v[0] == JSON_ARRAY_END) { // } , ]
                    flag = true;
                    if (json.back() == JSON_ARRAY_BEGIN || json.back() == JSON_OBJECT_BEGIN) flag = false; // [] {}
                    if (flag) {
                        __appendJson(json, __LF);
                        for (int i = 0; i < depth - 1; i++) __putIndentJson(json);
                    }
                    __appendJson(json, v);
                    depth -= 1;
                } else {
                    flag = true;
                    if (v[0] == JSON_SEPARATE && lstJsonText[i + 1][0] == JSON_OBJECT_BEGIN) flag = false; // ,{
                    if (v[0] == JSON_SEPARATE && lstJsonText[i + 1][0] == JSON_ARRAY_BEGIN) flag = false; // ,[
                    else if (lstJsonText[i - 1][0] == JSON_SEPARATE || lstJsonText[i - 1][0] == JSON_OBJECT_BEGIN
                             || lstJsonText[i - 1][0] == JSON_ARRAY_BEGIN)
                        for (int i = 0; i < depth; i++) __putIndentJson(json);
                    __appendJson(json, v);
                    if (v[0] == JSON_OBJ_SEPARATE) __appendJson(json, " "); // :
                    if (flag && v[0] == JSON_SEPARATE) __appendJson(json, __LF);
                }
            }
        }

        void __appendJson(std::string &json, const std::string &v) { json += v; }

        void __appendJson(std::string &json, char v) { json += v; }

        void __putIndentJson(std::string &json) { for (int i = 0; i < __indent; ++i) __appendJson(json, " "); }

    private:
        int __indent;
        std::string __json;
        char __LF = '\n';
    };
}
#endif
