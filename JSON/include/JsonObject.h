#ifndef JSONTEST_JSONOBJECT_H
#define JSONTEST_JSONOBJECT_H

#include "JsonBoolean.h"
#include "JsonNull.h"
#include "JsonString.h"
#include "JsonNumber.h"
#include "JsonFormatter.h"
#include <vector>
#include <memory>

namespace Json {
    class JsonObject : public JsonValue, private JsonFormatter {
    public:
        struct json_object_element_t {
            std::shared_ptr<JsonString> key;
            std::shared_ptr<JsonValue> value;

            json_object_element_t() {}

            json_object_element_t(const std::shared_ptr<JsonString> &__key, const std::shared_ptr<JsonValue> &__value)
                    : key(__key), value(__value) {}
        };

        typedef std::vector<json_object_element_t> json_object_vector_pair_t;
        typedef json_object_vector_pair_t::iterator json_object_iterator;
        typedef json_object_vector_pair_t::const_iterator json_object_citerator;
    public:
        explicit JsonObject(bool format = false, int indent = 4, bool escape = true, bool throw_exception = false)
                : JsonFormatter(indent), __format(format), __escape(escape),
                  __throw_exception(throw_exception) {}

        explicit JsonObject(JsonString *jsonKey, JsonValue *jsonValue) { put(jsonKey, jsonValue); }

        explicit JsonObject(const std::string &jsonKey, const std::string &jsonValue) {
            put(jsonKey, std::string(jsonValue));
        }

        bool put(const std::shared_ptr<JsonString> &jsonKey, const std::shared_ptr<JsonValue> &jsonValue);

        bool put(JsonString *jsonKey, JsonValue *jsonValue);

        bool put(const std::string &jsonKey, int jsonValue) {
            if (find_and_throw_exception(jsonKey))return false;
            return put(new JsonString(jsonKey), new JsonNumber(jsonValue));
        }

        bool put(const std::string &jsonKey, double jsonValue) {
            if (find_and_throw_exception(jsonKey))return false;
            return put(new JsonString(jsonKey), new JsonNumber(jsonValue));
        }

        bool put(const std::string &jsonKey, bool jsonValue) {
            if (find_and_throw_exception(jsonKey))return false;
            return put(new JsonString(jsonKey), new JsonBoolean(jsonValue));
        }

        bool put(const std::string &jsonKey, std::string &&jsonValue) {
            if (find_and_throw_exception(jsonKey))return false;
            return put(new JsonString(jsonKey), new JsonString(jsonValue));
        }

        bool put(const std::string &jsonKey) {
            if (find_and_throw_exception(jsonKey))return false;
            return put(new JsonString(jsonKey), new JsonNull());
        }

        bool put(const std::string &jsonKey, JsonValue *jsonValue) {
            if (!jsonValue) return false;
            if (find_and_throw_exception(jsonKey))return false;
            if (find(nullptr, jsonValue) != __mpJsonObject.end())return false;
            return put(new JsonString(jsonKey), jsonValue);
        }

        // set a single exist JsonObject key-value => {key:value}
        void set(const std::string &jsonKey, const std::string &data) { __set(jsonKey, Data{data}); }

        void set(const std::string &jsonKey, bool data) { __set(jsonKey, Data{"", 0.0, data}); }

        void set(const std::string &jsonKey, int data) { __set(jsonKey, Data("", (int) data)); }

        void set(const std::string &jsonKey, double data) { __set(jsonKey, Data{"", data}); }

        JsonValueType type() const override { return JsonValueType::Object; }

        std::string toString() override;

        json_object_citerator find(JsonString *jsonKey, JsonValue *jsonValue);

        // get current JsonValue from JsonObject by order index
        const json_object_element_t &get(int index);

        JsonValue *value(const std::string &jsonKey) override ;

        JsonValue *value(int) override { return nullptr; }

        int size() const { return __mpJsonObject.size(); }

        json_object_iterator begin() { return __mpJsonObject.begin(); }

        json_object_iterator end() { return __mpJsonObject.end(); }

    protected:
        using JsonValue::data;
        using JsonValue::setData;

        bool find_and_throw_exception(const std::string &jsonKey);

        const std::vector<std::string> &toList() override;

        void __set(const std::string &jsonKey, const Data &data);

    private:
        bool __format, __escape, __throw_exception;
        json_object_vector_pair_t __mpJsonObject;
        std::vector<std::string> __lstJsonText;
    };
}
#endif
