
#include "../include/JsonConvert.h"
#include "../include/JsonException.h"

void Json::JsonConvert::convertObject(JsonObject *jsonObject, const std::string &value) {
    if (value[0] == JSON_OBJECT_BEGIN) {
        while (__ptr < __len) {
            std::string v = __lstJsonValue[__ptr];
            if (v[0] == JSON_OBJECT_END) {
                __ptr++;
                break;
            }
            if (v[0] != JSON_OBJ_SEPARATE) {
                __ptr++;
                continue;
            }
            std::string json_key = __lstJsonValue[__ptr - 1];
            std::string json_value = __lstJsonValue[__ptr + 1];
            __ptr++;

            __remove_quotation(json_key);
            if (json_value[0] == '\"') {
                __remove_quotation(json_value);
                jsonObject->put(json_key, std::string(json_value));
            } else if (json_value == "true" || json_value == "false") {
                jsonObject->put(json_key, json_value == "true");
            } else if (json_value == "null") {
                jsonObject->put(json_key);
            } else if (json_value[0] == '+' || json_value[0] == '-' || __isNumber(json_value)) {
                jsonObject->put(json_key, JsonParser::__str2num(json_value));
            } else if (json_value[0] == JSON_ARRAY_BEGIN) {
                __ptr++;
                JsonArray *x = new JsonArray();
                convertArray(x, json_value);
                jsonObject->put(json_key, x);
            } else if (json_value == _JSCS(JSON_OBJECT_BEGIN)) {
                JsonObject *x = new JsonObject();
                __ptr++;        // skip current character '{'
                convertObject(x, json_value);
                jsonObject->put(json_key, x);
            }
        }
    }
}

void Json::JsonConvert::convertArray(JsonArray *jsonArray, const std::string &value) {
    if (value[0] == JSON_ARRAY_BEGIN) {
        while (__ptr < __len) {
            std::string v = __lstJsonValue[__ptr++];
            if (v[0] == JSON_ARRAY_END) { break; }
            if (v[0] == '\"') {
                __remove_quotation(v);
                jsonArray->put(std::string(v));
            } else if (v == "true" || v == "false") {
                jsonArray->put(v == "true");
            } else if (v == "null") {
                jsonArray->put();
            } else if (v[0] == '+' || v[0] == '-' || __isNumber(v)) {
                jsonArray->put(JsonParser::__str2num(v));
            } else if (v[0] == JSON_ARRAY_BEGIN) {
                JsonArray *x = new JsonArray();
                convertArray(x, v);
                jsonArray->put(x);
            } else if (v[0] == JSON_OBJECT_BEGIN) {
                JsonObject *x = new JsonObject();
                convertObject(x, v);
                jsonArray->put(x);
            }
        }
    }
}

std::shared_ptr <Json::JsonValue> Json::JsonConvert::convertJson() {
    if (jsonValuePtr) return jsonValuePtr;
    if (__lstJsonValue.empty()) return nullptr;

    std::string v = __lstJsonValue[0];
    if (v == _JSCS(JSON_OBJECT_BEGIN)) {
        jsonValuePtr = std::shared_ptr<JsonValue>(new JsonObject(__format, __indent, true, true));
        convertObject(dynamic_cast<Json::JsonObject *>(jsonValuePtr.get()), v);
    } else if (v == _JSCS(JSON_ARRAY_BEGIN)) {
        jsonValuePtr = std::shared_ptr<JsonValue>(new JsonArray(__format, __indent));
        __ptr++;
        convertArray(dynamic_cast<Json::JsonArray *>(jsonValuePtr.get()), v);
    } else throw JsonException("Error: JSON document parse failed");
    return jsonValuePtr;
}
