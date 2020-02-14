#include "simplejson/Value.h"

#include <cassert>

namespace SimpleJson {

Value::Value(ValueType type) {
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
            _data = std::make_unique<Object>();
            break;
    }
}

Value::Value(const Value& other) {
    switch (other.type()) {
        case ValueType::Null:
            _data = Null();
            break;
        case ValueType::Bool:
            _data = other.asBool();
            break;
        case ValueType::Integer:
            _data = other.asInteger();
            break;
        case ValueType::Real:
            _data = other.asReal();
            break;
        case ValueType::String:
            _data = String(other.asStringView());
            break;
        case ValueType::Array:
            _data = std::make_unique<Array>(other.asArray());
            break;
        case ValueType::Object:
            _data = std::make_unique<Object>(other.asObject());
            break;
    }
}

Value& Value::operator=(Value other) {
    this->swap(other);
    return *this;
}

void Value::swap(Value& other) {
    _data.swap(other._data);
}

bool Value::operator==(const Value& other) const {
    if (this->type() != other.type()) {
        return false;
    }

    switch (this->type()) {
        case ValueType::Null:
            return true;
        case ValueType::Bool:
            return this->asBool() == other.asBool();
        case ValueType::Integer:
            return this->asInteger() == other.asInteger();
        case ValueType::Real:
            return this->asReal() == other.asReal();
        case ValueType::String:
            return this->asStringView() == other.asStringView();
        case ValueType::Array:
            return this->asArray() == other.asArray();
        case ValueType::Object:
            return this->asObject() == other.asObject();
    }
    // never goto here
    return false;
}

bool Value::operator!=(const Value& other) const {
    return !(*this == other);
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
    switch (this->type()) {
        case ValueType::Array:
            return this->asArray().size();
        case ValueType::Object:
            return this->asObject().size();
        default:
            return 0;
    }
}

bool Value::empty() const {
    switch (this->type()) {
        case ValueType::Null:
            return true;
        case ValueType::Array:
            return this->asArray().empty();
        case ValueType::Object:
            return this->asObject().empty();
        default:
            return false;
    }
}

void Value::clear() {
    if (this->isArray()) {
        this->asArray().clear();
    } else if (this->isObject()) {
        this->asObject().clear();
    }
}

Value& Value::operator[](const size_t index) {
    return this->asArray()[index];
}

const Value& Value::operator[](const size_t index) const {
    return this->asArray()[index];
}

void Value::resize(const size_t size) {
    this->asArray().resize(size);
}

void Value::append(Value value) {
    this->asArray().push_back(std::move(value));
}

Value& Value::operator[](const std::string& key) {
    return this->asObject()[key];
}

const Value& Value::operator[](const std::string& key) const {
    return this->asObject().at(key);
}

bool Value::isMember(const std::string& key) const {
    return this->asObject().count(key) > 0;
}

Value Value::removeMember(const std::string& key) {
    if (!this->isMember(key)) {
        return Value();
    }

    auto& object = this->asObject();
    auto res = Value(std::move(object.at(key)));
    object.erase(key);

    return res;
}

std::vector<std::string> Value::getMemberNames() const {
    const auto& object = this->asObject();
    std::vector<std::string> res;
    res.reserve(object.size());
    for (const auto& pair : object) {
        res.push_back(pair.first);
    }
    return res;
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
