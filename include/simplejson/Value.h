#ifndef SIMPLEJSON_VALUE_H
#define SIMPLEJSON_VALUE_H

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SimpleJson {

enum class [[nodiscard]] ValueType{Null,   Bool,  Integer, Real,
                                   String, Array, Object};

using Bool = bool;
using Integer = long long;
using Real = double;

class [[nodiscard]] Value {
public:
    // ctor
    Value() = default;
    explicit Value(ValueType type);
    Value(Bool val) : _type(ValueType::Bool), _data(val) {}
    Value(Integer val) : _type(ValueType::Integer), _data(val) {}
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    Value(T val) : Value(Integer(val)) {}
    Value(Real val) : _type(ValueType::Real), _data(val) {}
    Value(std::string_view str)
        : _type(ValueType::String), _data(String(str)) {}
    Value(const char* str) : Value(std::string_view(str)) {}
    Value(const std::string& str) : Value(std::string_view(str)) {}

    Value(const Value& other);
    Value(Value && other) = default;

    Value& operator=(Value other);
    void swap(Value & other);

public:
    [[nodiscard]] ValueType type() const { return _type; }
    [[nodiscard]] bool isNull() const { return _type == ValueType::Null; }
    [[nodiscard]] bool isBool() const { return _type == ValueType::Bool; }
    [[nodiscard]] bool isInteger() const { return _type == ValueType::Integer; }
    [[nodiscard]] bool isReal() const { return _type == ValueType::Real; }
    [[nodiscard]] bool isString() const { return _type == ValueType::String; }
    [[nodiscard]] bool isArray() const { return _type == ValueType::Array; }
    [[nodiscard]] bool isObject() const { return _type == ValueType::Object; }

    [[nodiscard]] Bool asBool() const { return std::get<Bool>(_data); }
    [[nodiscard]] Integer asInteger() const { return std::get<Integer>(_data); }
    [[nodiscard]] Real asReal() const { return std::get<Real>(_data); }

    // string
    [[nodiscard]] std::string_view asStringView() const;
    [[nodiscard]] std::string asString() const;
    [[nodiscard]] const char* asCString() const;

    // array & object
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;
    void clear();

    // array
    [[nodiscard]] Value& operator[](size_t index);
    [[nodiscard]] const Value& operator[](size_t index) const;
    void resize(size_t size);
    void append(Value value);

    // object
    [[nodiscard]] Value& operator[](const std::string& key);
    [[nodiscard]] const Value& operator[](const std::string& key) const;

private:
    struct Null {};
    using Array = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;
    using PArray = std::unique_ptr<Array>;
    using PObject = std::unique_ptr<Object>;
    class String {
    public:
        String() = default;
        explicit String(std::string_view str);
        [[nodiscard]] operator std::string_view() const {
            return std::string_view(_ptr.get(), _size);
        }

    private:
        std::unique_ptr<char[]> _ptr;  // NOLINT(modernize-avoid-c-arrays)
        size_t _size = 0;
    };

private:
    [[nodiscard]] Array& asArray() { return *std::get<PArray>(_data); }
    [[nodiscard]] const Array& asArray() const {
        return *std::get<PArray>(_data);
    }
    [[nodiscard]] Object& asObject() { return *std::get<PObject>(_data); }
    [[nodiscard]] const Object& asObject() const {
        return *std::get<PObject>(_data);
    }

private:
    // TODO: remove `_type`, which is included in `_data`
    ValueType _type = ValueType::Null;
    std::variant<Null, Bool, Integer, Real, String, PArray, PObject> _data;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_VALUE_H
