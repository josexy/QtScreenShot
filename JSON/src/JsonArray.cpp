
#include "../include/JsonArray.h"
#include "../include/JsonException.h"

bool Json::JsonArray::put(JsonValue *jsonValue) {
    if (!jsonValue) return false;
    if (find(jsonValue) != __lstJsonArray.end())return false;
    jsonValue->setEscapeChar(__escape);
    __lstJsonArray.push_back(std::shared_ptr<JsonValue>(jsonValue));
    return true;
}

bool Json::JsonArray::put(const std::shared_ptr<JsonValue> &jsonValue) {
    if (jsonValue == nullptr)return false;
    if (find(jsonValue.get()) != __lstJsonArray.end())return false;
    jsonValue->setEscapeChar(__escape);
    __lstJsonArray.push_back(jsonValue);
    return true;
}

const Json::JsonArray::json_array_element_t &Json::JsonArray::get(int index) {
    if (index < 0 || index >= size())throw JsonException("Error: Invalid Index");
    return __lstJsonArray.at(index);
}

Json::JsonArray::json_array_citerator Json::JsonArray::find(JsonValue *jsonValue) {
    for (size_t i = 0; i < __lstJsonArray.size(); i++)
        if (__lstJsonArray[i].get() == jsonValue)
            return __lstJsonArray.begin() + i;
    return __lstJsonArray.end();
}

void Json::JsonArray::__toList() {
    __lstJsonText.clear();
    __lstJsonText.push_back(_JSCS(JSON_ARRAY_BEGIN));
    int __index = 0, __size = __lstJsonArray.size();
    for (std::shared_ptr<JsonValue> jsonValue : __lstJsonArray) {
        for (auto &&p : jsonValue->toList()) __lstJsonText.push_back(p);
        __index++;
        if (__index < __size) __lstJsonText.push_back(_JSCS(JSON_SEPARATE));
    }
    __lstJsonText.push_back(_JSCS(JSON_ARRAY_END));
}

const std::vector<std::string> &Json::JsonArray::toList() {
    __toList();
    return __lstJsonText;
}

std::string Json::JsonArray::toString() {
    __toList();
    std::string str;
    if (!__format)
        for (auto _S : __lstJsonText) str += _S;
    else
        str = getJson(__lstJsonText);
    return str;
}






