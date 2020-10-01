#ifndef JSONTEST_JSONARRAY_H
#define JSONTEST_JSONARRAY_H

#include "JsonBoolean.h"
#include "JsonNull.h"
#include "JsonString.h"
#include "JsonNumber.h"
#include "JsonFormatter.h"
#include <vector>
#include <memory>

namespace Json {
    class JsonArray : public JsonValue, private JsonFormatter {
    public:
        typedef std::vector<std::shared_ptr<JsonValue>> json_array_vector_pair_t;
        typedef json_array_vector_pair_t::const_iterator json_array_citerator;
        typedef json_array_vector_pair_t::iterator json_array_iterator;
        typedef json_array_vector_pair_t::value_type json_array_element_t;
    public:
        explicit JsonArray(bool format = false, int indent = 4, bool escape = true)
                : JsonFormatter(indent), __format(format), __escape(escape) {}


        bool put(JsonValue *jsonValue);

        bool put(const std::shared_ptr<JsonValue> &jsonValue);

        bool put(std::string &&value) { return put(new JsonString(value)); }

        bool put(int value) { return put(new JsonNumber(value)); }

        bool put(double value) { return put(new JsonNumber(value)); }

        bool put(bool value) { return put(new JsonBoolean(value)); }

        bool put() { return put(new JsonNull()); }

        const json_array_element_t &get(int index);

        json_array_citerator find(JsonValue *jsonValue);

        // current JsonArray object type
        JsonValueType type() const override { return JsonValueType::Array; }

        std::string toString() override;

        Data data(int index) { return get(index)->data(); }

        /**
         * get value from JsonArray by order index
         * @param index
         * @return JsonValue*
         */
        JsonValue *value(int index) { return get(index).get(); }

        JsonValue *value(const std::string &) override { return nullptr; }

        /**
         * the size of JsonArray including value object
         * @return
         */
        int size() const { return __lstJsonArray.size(); }

        json_array_iterator begin() { return __lstJsonArray.begin(); }

        json_array_iterator end() { return __lstJsonArray.end(); }

    protected:
        using JsonValue::data;
        using JsonValue::setData;

        void __toList();

        const std::vector<std::string> &toList() override;

    private:
        bool __format, __escape;
        json_array_vector_pair_t __lstJsonArray;
        std::vector<std::string> __lstJsonText;
    };
}
#endif
