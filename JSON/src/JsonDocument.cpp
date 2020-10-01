#include "../include/JsonDocument.h"

namespace Json {

    JsonDocument::JsonDocument(const std::string &jsonText, bool format, int indent, bool escape) {
        JsonConvert jc(jsonText, format, indent, escape);
        jsonValuePtr = jc.convertJson();
    }

    JsonDocument::JsonDocument(FILE *fp, bool format, int indent, bool escape) {
        JsonConvert jc(fp, format, indent, escape);
        jsonValuePtr = jc.convertJson();
    }

    JsonDocument::JsonDocument(std::ifstream &ifs, bool format, int indent, bool escape) {
        JsonConvert jc(ifs, format, indent, escape);
        jsonValuePtr = jc.convertJson();
    }

    bool JsonDocument::isObject() {
        if (!jsonValuePtr)return false;
        return jsonValuePtr->type() == JsonValueType::Object;
    }

    JsonObject *JsonDocument::object() {
        if (!isObject())return nullptr;
        return dynamic_cast<JsonObject *>(jsonValuePtr.get());
    }

    bool JsonDocument::isArray() {
        if (!jsonValuePtr)return false;
        return jsonValuePtr->type() == JsonValueType::Array;
    }

    JsonArray *JsonDocument::array() {
        if (!isArray())return nullptr;
        return dynamic_cast<JsonArray *>(jsonValuePtr.get());
    }

    std::string JsonDocument::toJson() {
        return jsonValuePtr->toString();
    }

    std::shared_ptr <JsonDocument> JsonDocument::fromJson(const std::string &jsonText) {
        return std::shared_ptr<JsonDocument>(new JsonDocument(jsonText));
    }
}