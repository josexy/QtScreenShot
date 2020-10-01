#ifndef JSONTEST_JSONNUMBER_H
#define JSONTEST_JSONNUMBER_H

#include "JsonValue.h"
#include <sstream>

namespace Json {
    class JsonNumber : public JsonValue {
    public:

        explicit JsonNumber() : __In(0), __Dn(0.0), __flag(true) {}

        explicit JsonNumber(int n) : __In(n), __flag(true) {}

        explicit JsonNumber(double n) : __Dn(n), __flag(false) {}

        std::string toString() override {
            if (__flag) return std::to_string(__In);
            std::ostringstream ost;
            ost << __Dn;
            return ost.str();
        }

        JsonValueType type() const override { return JsonValueType::Number; }

        Data data() const { return Data{"", __flag ? __In : __Dn}; }

        void setData(const Data &data) override { __flag ? __In = data.data2 : __Dn = data.data2; }

    protected:
        using JsonValue::toList;
    private:
        int __In;
        double __Dn;
        bool __flag;
    };
}
#endif
