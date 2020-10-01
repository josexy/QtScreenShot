#ifndef JSONTEST_JSONREADER_H
#define JSONTEST_JSONREADER_H

#include "JsonValue.h"
#include <fstream>

namespace Json {
    class JsonReader {
    public:
        JsonReader() : __ptr(0), __len(0) {}

        explicit JsonReader(const std::string &jsonText) : __JSON(jsonText), __ptr(0) { __len = __JSON.length(); }

        explicit JsonReader(std::ifstream &ifs) : JsonReader("") {
            if (ifs.is_open()) {
                char c;
                while ((c = ifs.get()) != EOF) __JSON += c;
                __len = __JSON.size();
            }
        }

        explicit JsonReader(FILE *fp) : JsonReader("") {
            if (fp) {
                char c;
                while ((c = fgetc(fp)) != EOF) __JSON += c;
                __len = __JSON.size();
            }
        }

        char peek() {
            if (__ptr >= __len) return '\0';
            return __JSON.at(__ptr);
        }

        char get() {
            if (__ptr >= __len) return '\0';
            return __JSON.at(__ptr++);
        }

        void moveTo(int offset) {
            if (__ptr + offset <= 0)
                __ptr = 0;
            else if (__ptr + offset >= __len)
                __ptr = __len;
            else
                __ptr += offset;
        }

        std::string substr(int size, bool backward = false) {
            if (backward) {
                if (__ptr - size <= 0)
                    return __JSON.substr(0, __ptr);
                return __JSON.substr(__ptr - size, size);
            }
            if (__ptr + size >= __len)
                return __JSON.substr(__ptr);
            return __JSON.substr(__ptr, size);
        }

        bool eof() { return __ptr >= __len; }

        int pos() const { return __ptr; }

        int length() const { return __len; }

        void back() { if (__ptr > 0) __ptr--; }

    private:
        std::string __JSON;
        int __ptr;
        int __len;
    };
}
#endif
