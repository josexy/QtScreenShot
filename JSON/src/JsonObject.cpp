
#include "../include/JsonObject.h"
#include "../include/JsonException.h"

bool Json::JsonObject::put(const std::shared_ptr<JsonString> &jsonKey,
                     const std::shared_ptr<JsonValue> &jsonValue) {
    if (jsonKey == nullptr || jsonValue == nullptr) return false;
    if (find_and_throw_exception(jsonKey->data().data1))return false;
    jsonKey->setEscapeChar(__escape);
    jsonValue->setEscapeChar(__escape);
    if (find(jsonKey.get(), jsonValue.get()) != __mpJsonObject.end()) return false;
    __mpJsonObject.push_back(json_object_element_t(jsonKey, jsonValue));
    return true;
}

bool Json::JsonObject::put(JsonString *jsonKey, JsonValue *jsonValue) {
    return put(std::shared_ptr<JsonString>(jsonKey), std::shared_ptr<JsonValue>(jsonValue));
}

Json::JsonObject::json_object_citerator Json::JsonObject::find(JsonString *jsonKey, JsonValue *jsonValue) {
    for (size_t i = 0; i < __mpJsonObject.size(); i++)
        if (__mpJsonObject[i].key.get() == jsonKey || __mpJsonObject[i].value.get() == jsonValue)
            return __mpJsonObject.begin() + i;
    return __mpJsonObject.end();
}

bool Json::JsonObject::find_and_throw_exception(const std::string &jsonKey) {
    bool _F = false;
    for (auto &&p:__mpJsonObject)
        if (p.key->data().data1 == jsonKey) {
            _F = true;
            break;
        }
    if (!_F)return _F;
    if (__throw_exception) throw JsonException("Error: Duplicate key: \"" + jsonKey + "\"!");
    else return _F;
}

const Json::JsonObject::json_object_element_t &Json::JsonObject::get(int index) {
    if (index < 0 || index >= size()) throw JsonException("Error: Invalid Index!");
    return __mpJsonObject.at(index);
}

Json::JsonValue *Json::JsonObject::value(const std::string &jsonKey) {
    for (auto &&p:__mpJsonObject) if (p.key->data().data1 == jsonKey)return p.value.get();
    throw JsonException("Error: Not found JSON Object Key: \"" + jsonKey + "\"!");
}

const std::vector<std::string> &Json::JsonObject::toList() {
    __lstJsonText.clear();
    __lstJsonText.push_back(_JSCS(JSON_OBJECT_BEGIN));
    int __index = 0, __size = __mpJsonObject.size();
    for (auto iter = __mpJsonObject.begin(); iter != __mpJsonObject.end(); ++iter) {
        __lstJsonText.push_back(iter->key->toString());
        __lstJsonText.push_back(_JSCS(JSON_OBJ_SEPARATE));
        auto x = iter->value->toList();
        for (size_t i = 0; i < x.size(); ++i) __lstJsonText.push_back(x.at(i));
        __index++;
        if (__index < __size) __lstJsonText.push_back(_JSCS(JSON_SEPARATE));
    }
    __lstJsonText.push_back(_JSCS(JSON_OBJECT_END));
    return __lstJsonText;
}

std::string Json::JsonObject::toString() {
    toList();
    std::string str;
    if (!__format) for (auto _S : __lstJsonText) str += _S;
    else str = getJson(__lstJsonText);
    return str;
}

void Json::JsonObject::__set(const std::string &jsonKey, const Data &data) {
    if (!find_and_throw_exception(jsonKey))return;
    if (value(jsonKey)->isNull())return;
    if (value(jsonKey)->type() == JsonValueType::Array || value(jsonKey)->type() == JsonValueType::Object)return;
    value(jsonKey)->setData(data);
}
