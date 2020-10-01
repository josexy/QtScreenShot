
#include "../include/JsonParser.h"
#include "../include/JsonException.h"

// parser null object value
std::string Json::JsonParser::parseNull(char prev) {
    buffer_reset();
    for (;;) {
        if (prev + __reader.substr(3) == "null") {
            buffer_append("null");
            __reader.moveTo(3);
        }
        char c = __reader.peek();
        if (c == JSON_SEPARATE || c == JSON_OBJECT_END || c == JSON_ARRAY_END) {
            break;
        } else if (is_space_char(__reader.get()))continue;
        else throw JsonException(__reader);
    }
    return buffer_str();
}

// parser boolean[true/false] value
std::string Json::JsonParser::parseBoolean(char prev) {
    buffer_reset();
    for (;;) {
        if (prev + __reader.substr(3) == "true") {
            buffer_append("true");
            __reader.moveTo(3);
        } else if (prev + __reader.substr(4) == "false") {
            buffer_append("false");
            __reader.moveTo(4);
        }
        char c = __reader.peek();
        if (c == JSON_SEPARATE || c == JSON_OBJECT_END || c == JSON_ARRAY_END) { // true,  true}  true]
            break;
        } else if (is_space_char(__reader.get())) continue;    // true   , true  }  true  ]
        else throw JsonException(__reader);
    }
    return buffer_str();
}

// parser number value
std::string Json::JsonParser::parseNumber(char prev) {
    buffer_reset();
    bool __unique = false, __negative = false;
    // -.1234 => -0.1234, .1234 => 0.1234
    if (prev == '+');
    else if (prev == '-')__negative = true;
    else if (prev == '.') {
        buffer_append("0.");
        __unique = true;
    } else buffer_append(prev);

    char c = __reader.get();
    if (!__unique) {  // +., -., 1.
        if (c == '.') {
            if (!is_number(prev)) buffer_append('0');
            buffer_append(c);
            __unique = true;
        } else if (buffer_back() == '0' && is_number(c)) {
            __reader.back();
            throw JsonException(__reader);
        } else if ((prev == '+' && c == '-') || (prev == '+' && c == '+')
                   || (prev == '-' && c == '-') || (prev == '-' && c == '+'))
            throw JsonException(__reader);
        else if (is_number(c))buffer_append(c);
        else __reader.back();
    } else __reader.back();

    for (;;) {
        char c = __reader.get();

        if (is_space_char(c)) continue;

        if (is_number(c) || c == '.' || c == 'e' || c == 'E') {
            // 1e+2, 1e-2, -1e2, .1e+2, 0.1e+2, -.1e+2, -e+2, -0e+2, .0e-3
            if (c == 'e' || c == 'E') {
                if (buffer_back() == '.') { // .E-3
                    __reader.back();
                    throw JsonException(__reader);
                }
                buffer_append(c);
                c = __reader.get();
                if (!(is_number(c) || c == '+' || c == '-')) throw JsonException(__reader);
                buffer_append(c);
            } else if (c == '.') {
                if (__unique) throw JsonException(__reader);
                buffer_append(c);
                __unique = true;
            } else {
                if (buffer_size() >= 2) {
                    if (buffer_index() == '0' && is_number(buffer_index(1))) {
                        __reader.moveTo(-(buffer_size() - 1));
                        throw JsonException(__reader);
                    }
                }
                buffer_append(c);
            }
        } else {
            if (buffer_back() == '.') throw JsonException(__reader);
            if (c == JSON_SEPARATE || c == JSON_OBJECT_END || c == JSON_ARRAY_END)break;
            if (is_space_char(c))continue;
            else throw JsonException(__reader);
        }
    }
    __reader.back();
    if (__negative)
        buffer_insert('-');
    return __num2str(strtod((buffer_str()), nullptr));
}

// parser string["..."] value
std::string Json::JsonParser::parseString(char prev) {
    buffer_reset();
    buffer_append(prev);
    for (;;) {
        char s = __reader.get();
        if (s == '\\') {
            if (!__escape) {
                buffer_append('\\');
                buffer_append(__reader.get());
                continue;
            }
            char sn = __reader.get();
            if (is_escape(sn)) {
                buffer_append(convert2escape(sn));
                // parse hex string
            } else if (sn == 'x' || is_number(sn)) {
                std::string hex_oct = resolve_escape_number(sn);
                if (hex_oct.empty()) throw JsonException(__reader);
                buffer_append(hex_oct.c_str());
                // parse unicode string
            } else if (sn == 'u') {
                std::string unicodeS = resolve_escape_hex_unicode();
                if (unicodeS.empty()) throw JsonException(__reader);
                buffer_append(unicodeS.c_str());
            } else throw JsonException(__reader);
        } else if (s == '\"') {
            buffer_append('\"');
            for (;;) {
                char c = __reader.peek();
                if (c == JSON_SEPARATE || JsonParser::is_normal_char(c)) {
                    break;
                } else if (is_space_char(__reader.get()))continue;
                else throw JsonException(__reader);
            }
            break;
        } else buffer_append(s);
    }
    return buffer_str();
}

void Json::JsonParser::parseJsonDocument() {
    while (!__reader.eof()) {
        char s = __reader.get();
        if (is_space_char(s)) continue;
        if (s == JSON_SEPARATE) {
            if (__reader.peek() == JSON_SEPARATE) __reader.moveTo(1), throw JsonException(__reader);
            else if (is_error_char(!jsonValue.empty() ? jsonValue.back() : "\0", s)) throw JsonException(__reader);
            else continue;
        }
        if (s == '\"') {
            jsonValue.push_back(parseString(s));
        } else if (is_number(s) || s == '.' || s == '-' || s == '+') {
            jsonValue.push_back(parseNumber(s));
        } else if (s == 't' || s == 'f') {
            jsonValue.push_back(parseBoolean(s));
        } else if (s == 'n') {
            jsonValue.push_back(parseNull(s));
        } else {
            if (!jsonValue.empty() && is_error_char(jsonValue.back(), s)) {
                throw JsonException(__reader);
            } else if (is_normal_char(s))
                jsonValue.push_back(_JSCS(s));
            else throw JsonException(__reader);
        }
    }
    __parser = true;
}

char Json::JsonParser::convert2escape(char sn) {
    char c;
    switch (sn) {
        case 'a':
            c = '\a';
            break;
        case 'b':
            c = '\b';
            break;
        case 'f':
            c = '\f';
            break;
        case 'r':
            c = '\r';
            break;
        case 't':
            c = '\t';
            break;
        case 'v':
            c = '\v';
            break;
        case '\'':
            c = '\'';
            break;
        case '\"':
            c = '\"';
            break;
        case '?':
            c = '\?';
            break;
        case 'n':
            c = '\n';
            break;
        case '0':
            c = '\0';
            break;
        case '\\':
            c = '\\';
            break;
        default:
            c = sn;
            break;
    }
    return c;
}

std::string Json::JsonParser::resolve_escape_hex_unicode() {
    buffer_reset();
    for (int i = 0; i < 4; i++) {
        char c = __reader.get();
        if (!is_hex(c)) return "";
        buffer_append(c);
    }
    std::stringstream ss;
    int unicodeC;
    ss << std::hex << buffer_str();
    ss >> unicodeC;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(unicodeC);
}

std::string Json::JsonParser::resolve_escape_number(char sn) {
    bool flag_hex_oct = false;
    buffer_reset();
    if (sn == 'x')
        flag_hex_oct = true;
    else
        buffer_append(sn);
    int base = flag_hex_oct ? 16 : 8;
    if (base == 8 && !is_oct(sn))return "";
    for (int i = 0; i < 2; ++i) {
        sn = __reader.get();
        if (base == 16 && is_hex(sn))buffer_append(sn);
        else if (base == 8 && is_oct(sn)) buffer_append(sn);
        else return "";
    }
    return std::to_string(strtol(buffer_str(), nullptr, base));
}