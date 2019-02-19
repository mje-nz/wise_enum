//
// Created by Matthew Edwards on 19/02/19.
//

#include "wise_enum.h"

#include <Arduino.h>


#include <cassert>

// Equivalent to enum Color {GREEN = 2, RED};
namespace my_lib {
WISE_ENUM(Color, (GREEN, 2), RED)
}

// Equivalent to enum class MoreColor : int64_t {BLUE, BLACK = 1};
WISE_ENUM_CLASS((MoreColor, int64_t), BLUE, (BLACK, 1))

// Inside a class, must use a different macro, but still works
struct Bar {
    WISE_ENUM_MEMBER(Foo, BUZ)
};

// Adapt an existing enum you don't control so it works with generic code
namespace another_lib {
enum class SomebodyElse { FIRST, SECOND };
}
WISE_ENUM_ADAPT(another_lib::SomebodyElse, FIRST, SECOND)

void setup() {
    Serial.begin(9600);

    // Number of enumerations:
    static_assert(wise_enum::size<my_lib::Color> == 2, "");
    Serial.print("Number of enumerators: ");
    Serial.println(wise_enum::size<my_lib::Color>);

    // Iterate over enums
    Serial.println("Enum values and names:");
    for (auto e : wise_enum::range<my_lib::Color>) {
        Serial.print(static_cast<int>(e.value));
        Serial.print(" ");
        Serial.println(e.name);
    }
    Serial.println();

    // Convert any enum to a string
    Serial.println(wise_enum::to_string(my_lib::Color::RED));

    // Convert any string to an optional enum
    my_lib::Color x1, x2;
    bool success1 = wise_enum::from_string<my_lib::Color>("GREEN", x1);
    auto success2 = wise_enum::from_string<my_lib::Color>("Greeeeeeen", x2);

    assert(success1);
    assert(x1 == my_lib::Color::GREEN);
    assert(!success2);

    // Everything is constexpr, and a type trait is made available for easy use in
    // enable_if/tag dispatch
    static_assert(wise_enum::is_wise_enum_v<my_lib::Color>, "");
    static_assert(!wise_enum::is_wise_enum_v<int>, "");
    enum flub { blub, glub };
    static_assert(!wise_enum::is_wise_enum_v<flub>, "");
    // We made a regular enum wise!
    static_assert(wise_enum::is_wise_enum_v<another_lib::SomebodyElse>, "");

    // Assert underlying type
    static_assert(std::is_same<int64_t, std::underlying_type_t<MoreColor>>::value,
                  "");

    // Full API available for adapted wise enums
    for (auto e : wise_enum::range<another_lib::SomebodyElse>) {
        Serial.print(static_cast<int>(e.value));
        Serial.print(" ");
        Serial.println(wise_enum::to_string(e.value));
    }
}

void loop() {}
