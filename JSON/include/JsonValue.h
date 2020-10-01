#ifndef HELLOWORLD_JSONVALUE_H
#define HELLOWORLD_JSONVALUE_H

#include <string>
#include <vector>

#define _JS(X) std::string((X))
#define _JSCS(C) std::string(1,(C))

namespace Json {
    enum class JsonValueType {
        Object,
        Array,
        String,
        Number,
        Boolean,
        Null
    };

    enum {
        JSON_OBJECT_BEGIN = '{',
        JSON_OBJECT_END = '}',
        JSON_ARRAY_BEGIN = '[',
        JSON_ARRAY_END = ']',
        JSON_OBJ_SEPARATE = ':',
        JSON_SEPARATE = ','
    };

    struct Data {
        std::string data1;
        double data2;
        bool data3;

        Data() {}

        Data(const std::string &d1, double d2 = 0.0, bool d3 = false) : data1(d1), data2(d2), data3(d3) {}
    };

    class JsonValue {
    public:
        virtual JsonValueType type() const = 0;

        virtual std::string toString() = 0;

        // array
        virtual JsonValue *value(int) { return this; }

        // object
        virtual JsonValue *value(const std::string &) { return this; }

        virtual Data data() const { return Data(); }

        virtual void setData(const Data &) {}

        virtual void setEscapeChar(bool) {}

        virtual bool isNull() const { return false; }

        virtual ~JsonValue() {}

        virtual const std::vector<std::string> &toList() {
            __vcsp.clear();
            __vcsp.push_back(toString());
            return __vcsp;
        }

        friend std::ostream &operator<<(std::ostream &_out, JsonValue *jsonValue) {
            if (jsonValue) _out << jsonValue->toString();
            return _out;
        }

        friend std::ostream &operator<<(std::ostream &_out, JsonValue &jsonValue) {
            _out << jsonValue.toString();
            return _out;
        }

    private:
        std::vector<std::string> __vcsp;
    };
}
#endif
