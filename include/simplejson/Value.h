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
    Value(Bool val) : _data(val) {}
    Value(Integer val) : _data(val) {}
    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    Value(T val) : Value(Integer(val)) {}
    Value(Real val) : _data(val) {}
    Value(std::string_view str) : _data(String(str)) {}
    Value(const char* str) : Value(std::string_view(str)) {}
    Value(const std::string& str) : Value(std::string_view(str)) {}

    Value(const Value& other);
    Value(Value && other) = default;

    Value& operator=(Value other);
    void swap(Value & other);

public:
    [[nodiscard]] ValueType type() const {
        return static_cast<ValueType>(_data.index());
    }
    [[nodiscard]] bool isNull() const {
        return std::holds_alternative<Null>(_data);
    }
    [[nodiscard]] bool isBool() const {
        return std::holds_alternative<Bool>(_data);
    }
    [[nodiscard]] bool isInteger() const {
        return std::holds_alternative<Integer>(_data);
    }
    [[nodiscard]] bool isReal() const {
        return std::holds_alternative<Real>(_data);
    }
    [[nodiscard]] bool isString() const {
        return std::holds_alternative<String>(_data);
    }
    [[nodiscard]] bool isArray() const {
        return std::holds_alternative<PArray>(_data);
    }
    [[nodiscard]] bool isObject() const {
        return std::holds_alternative<PObject>(_data);
    }

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
    std::variant<Null, Bool, Integer, Real, String, PArray, PObject> _data;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_VALUE_H
