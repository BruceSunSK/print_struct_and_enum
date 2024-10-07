#pragma once
#include <type_traits>
#include <iostream>
#include <string>
#include <unordered_map>


/// @brief 检验目标类型是否包含成员print时的类型，此时为false
/// @tparam T 待检验的类型，需手动指定
/// @tparam U 为void，不需要指定
template <typename T, typename U = void>
struct has_member_print : std::false_type {};

/// @brief 检验目标类型是否包含成员print时的类型，由于含有成员print，满足下列推导，此时为true
/// @tparam T 待检验的类型，需手动指定
template <typename T>
struct has_member_print<T, decltype(void(std::declval<T>().__print(std::cout, "")))> : std::true_type {};

/// @brief 注册结构体或者枚举对象时需要调用
/// @param m 对象
#define REGISTER_MEMBER(m) #m, m

/// @brief 注册结构体时需要调用，依次传入注册后的对象名。需要编译期可见，一般放在头文件中
/// @param ... 结构体中所有的对象
#define REGISTER_STRUCT(...)                                                                                \
    void __print(std::ostream & __out, const std::string & __name, const std::string & __indent = "") const \
    {                                                                                                       \
        __out << __indent << __name << ":\n";                                                               \
        __print_members(__out, __indent + "  ", __VA_ARGS__);                                               \
    }                                                                                                       \
                                                                                                            \
    template<typename Last>                                                                                 \
    typename std::enable_if<has_member_print<Last>::value, void>::type                                      \
    __print_members(std::ostream & __out, const std::string & __indent,                                     \
                    const std::string & __lname, const Last & __last) const                                 \
    {                                                                                                       \
        __last.__print(__out, __lname, __indent);                                                           \
    }                                                                                                       \
                                                                                                            \
    template<typename Last>                                                                                 \
    typename std::enable_if<!has_member_print<Last>::value, void>::type                                     \
    __print_members(std::ostream & __out, const std::string & __indent,                                     \
                    const std::string & __lname, const Last & __last) const                                 \
    {                                                                                                       \
        __out << __indent << __lname << ": " << __last << std::endl;                                        \
    }                                                                                                       \
                                                                                                            \
    template<typename First, typename... Others>                                                            \
    typename std::enable_if<has_member_print<First>::value, void>::type                                     \
    __print_members(std::ostream & __out, const std::string & __indent,                                     \
    const std::string & __fname, const First & __first, const Others &... others) const                     \
    {                                                                                                       \
        __first.__print(__out, __fname, __indent);                                                          \
        __print_members(__out, __indent, others...);                                                        \
    }                                                                                                       \
                                                                                                            \
    template<typename First, typename... Others>                                                            \
    typename std::enable_if<!has_member_print<First>::value, void>::type                                    \
    __print_members(std::ostream & __out, const std::string & __indent,                                     \
    const std::string & __fname, const First & __first, const Others &... others) const                     \
    {                                                                                                       \
        __out << __indent << __fname << ": " << __first << std::endl;                                       \
        __print_members(__out, __indent, others...);                                                        \
    }

/// @brief 打印注册后的结构体对象数据，支持任意数据流对象
/// @param osream_obj 要打印到的数据流对象
/// @param obj 待打印的注册后的结构体对象
#define PRINT_STRUCT(ostream_obj, obj)  obj.__print(ostream_obj, #obj)

/// @brief 注册枚举时需要调用，传入枚举类型。需要编译期可见，一般放在头文件中
/// @param ENUM_TYPE 枚举类型
#define REGISTER_ENUM(ENUM_TYPE)                                                                     \
namespace __Enum_##ENUM_TYPE                                                                         \
{                                                                                                    \
    extern std::unordered_map<ENUM_TYPE, std::string> __map;                                         \
    extern int __dummy_##ENUM_TYPE;                                                                  \
}                                                                                                    \
std::ostream & operator<<(std::ostream & __out, const ENUM_TYPE & e);

/// @brief 注册枚举后需要调用，传入枚举类型后依次传入注册后的对象名，以便实现 operator<< 重载打印字符串的效果。
/// @param ENUM_TYPE 枚举类型
/// @param ... 枚举中所有的对象
#define REGISTER_ENUM_BODY(ENUM_TYPE, ...)                                                           \
namespace __Enum_##ENUM_TYPE                                                                         \
{                                                                                                    \
    std::unordered_map<ENUM_TYPE, std::string> __map;                                                \
    template<typename Last>                                                                          \
    void __add_members(const std::string & __lname, const Last & __last)                             \
    {                                                                                                \
        __map.insert({ __last, __lname });                                                           \
    }                                                                                                \
                                                                                                     \
    template<typename First, typename... Others>                                                     \
    void __add_members(const std::string & __fname, const First & __first, const Others &... others) \
    {                                                                                                \
        __map.insert({ __first, __fname });                                                          \
        __add_members(others...);                                                                    \
    }                                                                                                \
                                                                                                     \
    int __dummy_##ENUM_TYPE = []() -> int { __add_members(__VA_ARGS__); return 0; }();               \
}                                                                                                    \
std::ostream & operator<<(std::ostream & __out, const ENUM_TYPE & e)                                 \
{                                                                                                    \
   return __out << __Enum_##ENUM_TYPE::__map[e];                                                     \
}
