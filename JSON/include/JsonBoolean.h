#ifndef JSONTEST_JSONBOOLEAN_H
#define JSONTEST_JSONBOOLEAN_H

#include "JsonValue.h"

namespace Json {
    class JsonBoolean : public JsonValue {
    public:
        explicit JsonBoolean(bool boolean = false) : __boolean(boolean) {}

        std::string toString() override { return (__boolean ? "true" : "false"); }

        JsonValueType type() const override { return JsonValueType::Boolean; }

        Data data() const override { return Data{"", 0.0, __boolean}; }

        void setData(const Data &data) override { __boolean = data.data3; }

    protected:
        using JsonValue::toList;
    private:
        bool __boolean;
    };
}
#endif
