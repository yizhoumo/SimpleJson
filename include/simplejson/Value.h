#ifndef SIMPLEJSON_VALUE_H
#define SIMPLEJSON_VALUE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SimpleJson {

enum class ValueType { Null, Bool, Number, String, Array, Object };

class Value;
struct Null {};
using Bool = bool;
using Number = double;
using String = std::string;
using Array = std::vector<Value>;
using Object = std::unordered_map<String, Value>;

class Value {
public:
    // ctor
    explicit Value(ValueType type);
    Value(Null val = Null()) : _type(ValueType::Null), _data(val) {}
    Value(Bool val) : _type(ValueType::Bool), _data(val) {}
    Value(Number val) : _type(ValueType::Number), _data(val) {}
    template <typename Integer,
              typename = std::enable_if_t<std::is_integral_v<Integer>>>
    Value(Integer val) : _type(ValueType::Number), _data(Number(val)) {}
    Value(String val) : _type(ValueType::String), _data(std::move(val)) {}
    // Value(Array val) : _type(ValueType::Array), _data(std::move(val)) {}
    // Value(Object val) : _type(ValueType::Object), _data(std::move(val)) {}

    // TODO: copy ctor?

    ValueType type() const { return _type; }
    bool isNull() const { return _type == ValueType::Null; }
    bool isBool() const { return _type == ValueType::Bool; }
    bool isNumber() const { return _type == ValueType::Number; }
    Bool asBool() const { return std::get<Bool>(_data); }
    Number asNumber() const { return std::get<Number>(_data); }

private:
    ValueType _type;
    std::variant<Null, Bool, Number, String> _data;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_VALUE_H
