#ifndef SIMPLEJSON_VALUE_H
#define SIMPLEJSON_VALUE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SimpleJson {

enum class ValueType { Null, Bool, Integer, Real, String, Array, Object };

using Bool = bool;
using Integer = long long;
using Real = double;
using String = std::string;

class Value {
public:
    // ctor
    explicit Value() = default;
    explicit Value(ValueType type);
    explicit Value(Bool val) : _type(ValueType::Bool), _data(val) {}

    explicit Value(Integer val) : _type(ValueType::Integer), _data(val) {}
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    explicit Value(T val) : Value(Integer(val)) {}

    explicit Value(Real val) : _type(ValueType::Real), _data(val) {}
    explicit Value(String val)
        : _type(ValueType::String), _data(std::move(val)) {}
    explicit Value(const char* str) : Value(String(str)) {}

    // TODO: copy ctor?

public:
    [[nodiscard]] ValueType type() const { return _type; }
    [[nodiscard]] bool isNull() const { return _type == ValueType::Null; }
    [[nodiscard]] bool isBool() const { return _type == ValueType::Bool; }
    [[nodiscard]] bool isInteger() const { return _type == ValueType::Integer; }
    [[nodiscard]] bool isReal() const { return _type == ValueType::Real; }
    [[nodiscard]] bool isString() const { return _type == ValueType::String; }
    [[nodiscard]] Bool asBool() const { return std::get<Bool>(_data); }
    [[nodiscard]] Integer asInteger() const { return std::get<Integer>(_data); }
    [[nodiscard]] Real asReal() const { return std::get<Real>(_data); }
    [[nodiscard]] const String& asString() const {
        return std::get<String>(_data);
    }

private:
    struct Null {};
    using Array = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;
    using PArray = std::unique_ptr<Array>;
    using PObject = std::unique_ptr<Object>;

private:
    // TODO: remove `_type`, which is included in _data
    ValueType _type = ValueType::Null;
    // TODO: lightweight version of String
    std::variant<Null, Bool, Integer, Real, String, PArray, PObject> _data;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_VALUE_H
