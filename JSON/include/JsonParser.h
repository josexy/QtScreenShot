#ifndef JSONTEST_JSONPARSER_H
#define JSONTEST_JSONPARSER_H

#include "JsonObject.h"
#include "JsonReader.h"
#include <locale>
#include <codecvt>
#include <cmath>

namespace Json {

    class JsonConvert;

    class JsonParser {
    public:

        friend class JsonConvert;

        JsonParser() = delete;

        explicit JsonParser(const std::string &jsonText) : __reader(jsonText) {}

        explicit JsonParser(const JsonReader &jsonReader) : __reader(jsonReader) {}

        explicit JsonParser(std::ifstream &ifs) : __reader(ifs) {}

        explicit JsonParser(FILE *fp) : __reader(fp) {}

        void parseJsonDocument();

        static std::string __num2str(double d) {
            if (std::isinf(d))return "0";
            std::stringstream oss;
            oss << std::fixed << d;
            std::string s;
            oss >> s;
            return s;
        }

        static double __str2num(const std::string &s) {
            std::stringstream ss;
            ss << s;
            double d;
            ss >> d;
            return d;
        }

        static bool is_normal_char(char sn) { return (sn == '{' || sn == '}' || sn == '[' || sn == ']' || sn == ':'); }

        static bool is_space_char(char sn) { return (sn == ' ' || sn == '\r' || sn == '\n' || sn == '\t'); }

        static bool is_hex(char sn) { return sn >= '0' && (sn <= 'f' || sn <= 'F'); }

        static bool is_oct(char sn) { return sn >= '0' && sn <= '7'; }

        static bool is_number(char sn) { return sn >= '0' && sn <= '9'; }

        static bool is_escape(char sn) {
            return (sn == 'a' || sn == 'b' || sn == 'f' || sn == 'r' || sn == 't' ||
                    sn == 'v' || sn == '\'' || sn == '\"' || sn == '\?' ||
                    sn == '\\' || sn == '0' || sn == 'n');
        }

        static bool is_error_char(const std::string &s, char sn) {
            if (s == ":") return (sn == '}' || sn == ']' || sn == ',' || sn == ':');
            if (s == "{") return (sn == '{' || sn == '[' || sn == ',' || sn == ':');
            if (s == "[") return (sn == '}' || sn == ',' || sn == ':');
            return false;
        }

    protected:
        std::string parseNull(char prev);

        std::string parseBoolean(char prev);

        std::string parseNumber(char prev);

        std::string parseString(char prev);

        std::string resolve_escape_hex_unicode();

        std::string resolve_escape_number(char sn);

        char convert2escape(char sn);

    private:
        void buffer_reset() { __buffer_value.clear(); }

        void buffer_append(const char *value) { __buffer_value.append(value); }

        void buffer_insert(char c, int pos = 0) { __buffer_value.insert(pos, 1, c); }

        char buffer_back() const { return !__buffer_value.empty() ? __buffer_value.back() : '\0'; }

        char buffer_index(int index = 0) const { return __buffer_value.at(index); }

        void buffer_append(char value) { __buffer_value.append(1, value); }

        int buffer_size() const { return __buffer_value.size(); }

        const char *buffer_str() const { return __buffer_value.c_str(); }

    private:
        std::string __buffer_value;
        std::vector<std::string> jsonValue;
        JsonReader __reader;
        mutable bool __escape = false, __parser = false;
    };
}
#endif
