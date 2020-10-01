#ifndef JSONTEST_JSONEXCEPTION_H
#define JSONTEST_JSONEXCEPTION_H

#include "JsonReader.h"
#include "JsonParser.h"
#include <exception>

namespace Json {
    // This JsonException is not that perfect, and it has some bugs... :(
    class JsonException : public std::exception {
    public:
        JsonException(const std::string &err) : __err(err) {}

        JsonException(JsonReader &jsonReader) : __reader(jsonReader) {}

        void escape_rs(std::string &s) const {
            std::string v, w;
            for (size_t i = 0; i < s.size(); ++i)
                if (s[i] == '\\') v += "\\";
                else if (JsonString::is_escape(s[i], w)) v += w;
                else v += s[i];
            s = v;
        }

        void error_info(int size = 10) const {
            int pos = __reader.pos() - 1;
            int len = __reader.length();
            std::string subErrorTextBackward = __reader.substr(size, true);
            std::string subErrorTextForward = __reader.substr(size);
            escape_rs(subErrorTextForward);
            escape_rs(subErrorTextBackward);
            std::stringstream ss;
            ss << "Error: JSON document parsing failed: [" << pos << "," << len << "] " << "\n";
            __err += ss.str();
            ss.clear(), ss.str("");
            ss << "==> " << "Error Text: [..."
               << subErrorTextBackward
               << subErrorTextForward << "...]";
            __err += ss.str() + "\n";
            int __ptr = subErrorTextBackward.size() + 19;
            int __len = ss.str().size();
            ss.clear(), ss.str("");
            for (int i = 0; i < __len; ++i) {
                if (i == __ptr)
                    ss << "^";
                else
                    ss << "~";
            }
            __err += ss.str();
        }

        const char *what() const noexcept override {
            if (__reader.length() > 0)
                error_info();
            return __err.c_str();
        }

    private:
        mutable std::string __err;
        mutable JsonReader __reader;
    };
}
#endif
