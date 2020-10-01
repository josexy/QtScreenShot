#ifndef JSONTEST_JSONWRITER_H
#define JSONTEST_JSONWRITER_H

#include <fstream>
#include <cstring>

namespace Json {
    class JsonWriter {
    public:
        JsonWriter() = delete;

        JsonWriter(const JsonWriter &) = delete;

        JsonWriter(JsonWriter &&) = delete;

        JsonWriter &operator=(const JsonWriter &) = delete;

        JsonWriter &operator=(JsonWriter &&) = delete;

        JsonWriter(const char *filename) { open(filename); }

        JsonWriter(char *buffer, size_t size) : __buffer(buffer), __size(size) {}

        ~JsonWriter() { close(); }

        bool open(const char *filename) {
            if (!__ofs.is_open())
                __ofs.open(filename);
            return __ofs.is_open();
        }

        void write(const std::string &text) {
            if (__ofs.is_open())
                __ofs.write(text.c_str(), text.size());
            else if (__buffer) strncpy(__buffer, text.c_str(), __size);
        }

        void close() { if (__ofs.is_open())__ofs.close(); }

    private:
        std::ofstream __ofs;
        char *__buffer;
        int __size;
    };
}
#endif
