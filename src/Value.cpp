#include "simplejson/Value.h"

#include <cassert>

namespace SimpleJson {

Value::Value(ValueType type) : _type(type) {
    switch (type) {
        case ValueType::Null:
            _data = Null();
            break;
        case ValueType::Bool:
            _data = Bool();
            break;
        case ValueType::Integer:
            _data = Integer();
            break;
        case ValueType::Real:
            _data = Real();
            break;
        case ValueType::String:
            _data = String();
            break;
        case ValueType::Array:
            _data = std::make_unique<Array>();
            break;
        case ValueType::Object:
            _data = PObject();
            break;
    }
}

std::string_view Value::asStringView() const {
    return static_cast<std::string_view>(std::get<String>(_data));
}

std::string Value::asString() const {
    auto sv = this->asStringView();
    return std::string(sv.data(), sv.size());
}

const char* Value::asCString() const {
    return this->asStringView().data();
}

size_t Value::size() const {
    return this->asArray().size();
}

bool Value::empty() const {
    return this->asArray().empty();
}

Value& Value::operator[](const size_t index) {
    return this->asArray()[index];
}

const Value& Value::operator[](const size_t index) const {
    return this->asArray()[index];
}

void Value::clear() {
    this->asArray().clear();
}

void Value::resize(const size_t size) {
    this->asArray().resize(size);
}

void Value::append(Value value) {
    this->asArray().push_back(std::move(value));
}

Value::String::String(std::string_view str) {
    if (str.empty()) {
        return;
    }

    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    _ptr = std::make_unique<char[]>(str.size() + 1);
    _size = str.size();
    str.copy(_ptr.get(), _size);
    _ptr[_size] = 0;
}

}  // namespace SimpleJson
