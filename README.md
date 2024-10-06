# Easy to Print Struct and Enum in C++!

## 1. 通过宏注册的形式，实现枚举支持以流的形式打印，且输出为字符串，而非整数形式。 
以如下形式注册枚举，其中REGISTER_ENUM需要编译期可见，用于创建operator<<重载的声明；REGISTER_ENUM_STREAM中实现operator<<重载（即，分文件编写时，把REGISTER_ENUM放到头文件中，把REGISTER_ENUM_STREAM放在cpp中）：
``` c++
enum class Color
{
    RED,
    GREEN,
    BLUE
};
REGISTER_ENUM(Color,
              REGISTER_MEMBER(Color::RED),
              REGISTER_MEMBER(Color::GREEN),
              REGISTER_MEMBER(Color::BLUE));
REGISTER_ENUM_STREAM(Color);
```
然后使用流正常输出：
``` c++
Color c = Color::GREEN;
std::cout << "color c is: " << c << std::endl;
```
输出结果如下：  
``` c++
> color c is: Color::GREEN
```
  
## 2. 通过宏注册的形式，实现结构体以流的形式打印，且支持结构体嵌套！  
以如下形式注册结构体，包括匿名结构体和上述注册过的枚举。
``` c++
struct School
{
    std::string name;
    int position;

    REGISTER_STRUCT(REGISTER_MEMBER(name),
                    REGISTER_MEMBER(position));
};


struct Person
{
    std::string name;
    int age;
    double height;
    School school;
    struct 
    {
        Color color;
        std::string hobby;

        REGISTER_STRUCT(REGISTER_MEMBER(color),
                        REGISTER_MEMBER(hobby));
    } favorite;

    REGISTER_STRUCT(REGISTER_MEMBER(name),
                    REGISTER_MEMBER(age),
                    REGISTER_MEMBER(height),
                    REGISTER_MEMBER(school),
                    REGISTER_MEMBER(favorite));
};
```
然后可以以动态方式赋值，也可以静态：
``` c++
Person p = { "Bob", 18, 1.8, {"BIT", 42}, {Color::RED, "basketball"} };
std::cin >> p.favorite.hobby;
```
使用如下的宏实现向任意流对象中输出：
``` c++
PRINT_STRUCT(std::cout, p);
```
输出结果：
``` c++
>rap  
>p:  
>  name: p  
>  age: 18  
>  height: 1.8  
>  school:  
>    name: school  
>    position: 42  
>  favorite:  
>    color: Color::RED  
>    hobby: rap  
```
使用stringstream流也很简单：
``` c++
    std::stringstream ss;
    PRINT_STRUCT(ss, p);
    ss << "here is stringstream!" << std::endl;
    std::cout << ss.str();
```
使用stringstream流效果如下：
``` c++
>p:
>  name: p
>  age: 18
>  height: 1.8
>  school:
>    name: school
>    position: 42
>  favorite:
>    color: Color::RED
>    hobby: rap
>here is stringstream!
```
