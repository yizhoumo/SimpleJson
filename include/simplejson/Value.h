#ifndef SIMPLEJSON_VALUE_H
#define SIMPLEJSON_VALUE_H

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace SimpleJson {

enum class ValueType { Null, Bool, Integer, Real, String, Array, Object };

using Bool = bool;
using Integer = long long;
using Real = double;

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
    explicit Value(std::string_view str)
        : _type(ValueType::String), _data(String(str)) {}
    explicit Value(const char* str) : Value(std::string_view(str)) {}

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
    [[nodiscard]] std::string_view asStringView() const {
        return std::get<String>(_data);
    }
    [[nodiscard]] std::string asString() const {
        auto str = asStringView();
        return std::string(str.data(), str.size());
    }
    [[nodiscard]] const char* asCString() const {
        return asStringView().data();
    }

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
    // TODO: remove `_type`, which is included in `_data`
    ValueType _type = ValueType::Null;
    std::variant<Null, Bool, Integer, Real, String, PArray, PObject> _data;
};

}  // namespace SimpleJson

#endif  // SIMPLEJSON_VALUE_H
