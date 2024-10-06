#include <sstream>
#include "print_struct_and_enum.h"

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

int main(int argc, char const * argv[])
{
    // Color c = Color::GREEN;
    // std::cout << "color c is: " << c << std::endl;

    Person p = { "Bob", 18, 1.8, {"BIT", 42}, {Color::RED, "basketball"} };
    PRINT_STRUCT(std::cout, p);

    // std::stringstream ss;
    // PRINT_STRUCT(ss, p);
    // ss << "here is stringstream!" << std::endl;
    // std::cout << ss.str();

    return 0;
}
