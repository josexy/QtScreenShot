#ifndef JSONTEST_JSONDOCUMENT_H
#define JSONTEST_JSONDOCUMENT_H

#include "JsonConvert.h"
#include "JsonException.h"
#include "JsonWriter.h"

namespace Json {
    class JsonDocument {
    public:
        explicit JsonDocument(const std::string &, bool format = false, int indent = 4, bool escape = false);

        explicit JsonDocument(FILE *, bool format = false, int indent = 4, bool escape = false);

        explicit JsonDocument(std::ifstream &, bool format = false, int indent = 4, bool escape = false);

        ~JsonDocument() {}

        bool isObject();

        JsonObject *object();

        bool isArray();

        JsonArray *array();

        std::string toJson();

        static std::shared_ptr<JsonDocument> fromJson(const std::string &);

    private:
        std::shared_ptr<JsonValue> jsonValuePtr = nullptr;
    };
}

#endif
