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
            _data = std::make_unique<Object>();
            break;
    }
}

}  // namespace SimpleJson
