#ifndef JSONTEST_JSONNULL_H
#define JSONTEST_JSONNULL_H

#include "JsonValue.h"

namespace Json {
    class JsonNull : public JsonValue {
    public:
        explicit JsonNull() {}

        JsonValueType type() const override { return JsonValueType::Null; }

        std::string toString() override { return "null"; }

        bool isNull() const override { return true; }

    protected:
        using JsonValue::setData;
        using JsonValue::toList;
    };
}
#endif
