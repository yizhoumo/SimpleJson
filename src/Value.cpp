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
        case ValueType::Number:
            _data = Number();
            break;
        case ValueType::String:
            _data = String();
            break;
        case ValueType::Array:
            //_data = Array();
            break;
        case ValueType::Object:
            //_data = Object();
            break;
        default:
            assert(false);
    }
}

}  // namespace SimpleJson
