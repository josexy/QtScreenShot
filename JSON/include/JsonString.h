#ifndef JSONTEST_JSONSTRING_H
#define JSONTEST_JSONSTRING_H

#include "JsonValue.h"

namespace Json {
    class JsonString : public JsonValue {
    public:
        explicit JsonString(const std::string &s) : __str(s), __escape(true) {}

        void setEscapeChar(bool escape) override { __escape = escape; }

        Data data() const override { return Data{__str}; }

        JsonValueType type() const override { return JsonValueType::String; }

        void setData(const Data &data) override { __str = data.data1; }

        std::string toString() override {
            if (__escape) {
                return "\"" + __str + "\"";
            } else {
                std::string s = "\"";
                for (size_t i = 0; i < __str.size(); i++) {
                    char c = __str[i];
                    std::string tp;
                    if (is_escape(c, tp))
                        s += tp;
                    else
                        s += c;
                }
                s += "\"";
                return s;
            }
        }

        static bool is_escape(char c, std::string &s) {
            bool flag = true;
            switch (c) {
                case '\a':
                    s = "\\a";
                    break;
                case '\b':
                    s = "\\b";
                    break;
                case '\f':
                    s = "\\f";
                    break;
                case '\t':
                    s = "\\t";
                    break;
                case '\n':
                    s = "\\n";
                    break;
                case '\r':
                    s = "\\r";
                    break;
                case '\v':
                    s = "\\v";
                    break;
                case '\\':
                    s = "\\\\";
                    break;
                case '\'':
                    s = "\\\'";
                    break;
                case '\"':
                    s = "\\\"";
                    break;
                case '\0':
                    s = "\\0";
                    break;
                case '\?':
                    s = "\\?";
                    break;
                default:
                    flag = false;
                    break;
            }
            return flag;
        }

    protected:
        using JsonValue::toList;
    private:
        std::string __str;
        bool __escape;
    };
}
#endif
