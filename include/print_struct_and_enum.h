#pragma once
#include <type_traits>
#include <iostream>
#include <string>
#include <unordered_map>

template <typename T, typename U = void>
struct has_member_print : std::false_type {};

template <typename T>
struct has_member_print<T, decltype(void(std::declval<T>().__print(std::cout, "")))> : std::true_type {};

#define REGISTER_MEMBER(m) #m, m

#define REGISTER_STRUCT(...)                                                                          \
    void __print(std::ostream & out, const std::string & name, const std::string & indent = "") const \
    {                                                                                                 \
        out << indent << name << ":\n";                                                               \
        __print_members(out, indent + "  ", __VA_ARGS__);                                             \
    }                                                                                                 \
                                                                                                      \
    template<typename Last>                                                                           \
    typename std::enable_if<has_member_print<Last>::value, void>::type                                \
    __print_members(std::ostream & out, const std::string & indent,                                   \
                    const std::string & lname, const Last & last) const                               \
    {                                                                                                 \
        last.__print(out, lname, indent);                                                             \
    }                                                                                                 \
                                                                                                      \
    template<typename Last>                                                                           \
    typename std::enable_if<!has_member_print<Last>::value, void>::type                               \
    __print_members(std::ostream & out, const std::string & indent,                                   \
                    const std::string & lname, const Last & last) const                               \
    {                                                                                                 \
        out << indent << lname << ": " << last << std::endl;                                          \
    }                                                                                                 \
                                                                                                      \
    template<typename First, typename... Others>                                                      \
    typename std::enable_if<has_member_print<First>::value, void>::type                               \
    __print_members(std::ostream & out, const std::string & indent,                                   \
    const std::string & fname, const First & first, const Others &... others) const                   \
    {                                                                                                 \
        first.__print(out, fname, indent);                                                            \
        __print_members(out, indent, others...);                                                      \
    }                                                                                                 \
    template<typename First, typename... Others>                                                      \
    typename std::enable_if<!has_member_print<First>::value, void>::type                              \
    __print_members(std::ostream & out, const std::string & indent,                                   \
    const std::string & fname, const First & first, const Others &... others) const                   \
    {                                                                                                 \
        out << indent << fname << ": " << first << std::endl;                                         \
        __print_members(out, indent, others...);                                                      \
    }

#define PRINT_STRUCT(ostream_obj, obj)  obj.__print(ostream_obj, #obj)

#define REGISTER_ENUM(ENUM_TYPE, ...)                                                            \
namespace __Enum_##ENUM_TYPE                                                                     \
{                                                                                                \
    static std::unordered_map<ENUM_TYPE, std::string> __map;                                     \
                                                                                                 \
    template<typename Last>                                                                      \
    void __add_members(const std::string & lname, const Last & last)                             \
    {                                                                                            \
        __map.insert({ last, lname });                                                           \
    }                                                                                            \
                                                                                                 \
    template<typename First, typename... Others>                                                 \
    void __add_members(const std::string & fname, const First & first, const Others &... others) \
    {                                                                                            \
        __map.insert({ first, fname });                                                          \
        __add_members(others...);                                                                \
    }                                                                                            \
                                                                                                 \
    int __dummy_##ENUM_TYPE = []() -> int { __add_members(__VA_ARGS__); return 0; }();           \
}                                                                                                \
std::ostream & operator<<(std::ostream & out, const ENUM_TYPE & e)                               \
{                                                                                                \
    return out << __Enum_##ENUM_TYPE::__map[e];                                                  \
}
