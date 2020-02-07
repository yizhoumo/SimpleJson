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
            _data = PArray();
            break;
        case ValueType::Object:
            _data = PObject();
            break;
    }
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
