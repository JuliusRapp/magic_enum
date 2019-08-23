// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#define MAGIC_ENUM_RANGE_MIN -120
#define MAGIC_ENUM_RANGE_MAX 120
#include <magic_enum.hpp>

#include <array>
#include <string_view>
#include <sstream>

enum class Color { RED = -12, GREEN = 7, BLUE = 15 };

enum class Numbers : int { one = 10, two = 20, three = 30, many = 127 };

enum Directions { Up = 85, Down = -42, Right = 120, Left = -120 };

enum number : unsigned long { one = 100, two = 200, three = 300, four = 400 };

namespace magic_enum {
template <>
struct enum_range<number> {
  static constexpr int min = 100;
  static constexpr int max = 300;
};
}

using namespace magic_enum;

TEST_CASE("enum_cast") {
  SECTION("from_string") {
    constexpr auto cr = enum_from_string_nothrow<Color>("RED");
    REQUIRE(cr.value() == Color::RED);
    REQUIRE(enum_from_string_nothrow<Color>("GREEN").value() == Color::GREEN);
    REQUIRE(enum_from_string_nothrow<Color>("BLUE").value() == Color::BLUE);
    REQUIRE_FALSE(enum_from_string_nothrow<Color>("None").has_value());

    constexpr auto no = enum_from_string_nothrow<Numbers>("one");
    REQUIRE(no.value() == Numbers::one);
    REQUIRE(enum_from_string_nothrow<Numbers>("two").value() == Numbers::two);
    REQUIRE(enum_from_string_nothrow<Numbers>("three").value() == Numbers::three);
    REQUIRE_FALSE(enum_from_string_nothrow<Numbers>("many").has_value());
    REQUIRE_FALSE(enum_from_string_nothrow<Numbers>("None").has_value());

    constexpr auto dr = enum_from_string_nothrow<Directions>("Right");
    REQUIRE(enum_from_string_nothrow<Directions>("Up").value() == Directions::Up);
    REQUIRE(enum_from_string_nothrow<Directions>("Down").value() == Directions::Down);
    REQUIRE(dr.value() == Directions::Right);
    REQUIRE(enum_from_string_nothrow<Directions>("Left").value() == Directions::Left);
    REQUIRE_FALSE(enum_from_string_nothrow<Directions>("None").has_value());

    constexpr auto nt = enum_from_string_nothrow<number>("three");
    REQUIRE(enum_from_string_nothrow<number>("one").value() == number::one);
    REQUIRE(enum_from_string_nothrow<number>("two").value() == number::two);
    REQUIRE(nt.value() == number::three);
    REQUIRE_FALSE(enum_from_string_nothrow<number>("four").has_value());
    REQUIRE_FALSE(enum_from_string_nothrow<number>("None").has_value());
  }

  SECTION("from_string_nocase") {
    constexpr auto cr = enum_from_string_nocase_nothrow<Color>("ReD");
    REQUIRE(cr.value() == Color::RED);
    REQUIRE(enum_from_string_nocase_nothrow<Color>("GrEeN").value() == Color::GREEN);
    REQUIRE(enum_from_string_nocase_nothrow<Color>("BLUE").value() == Color::BLUE);
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<Color>("None").has_value());

    constexpr auto no = enum_from_string_nocase_nothrow<Numbers>("oNe");
    REQUIRE(no.value() == Numbers::one);
    REQUIRE(enum_from_string_nocase_nothrow<Numbers>("Two").value() == Numbers::two);
    REQUIRE(enum_from_string_nocase_nothrow<Numbers>("three").value() == Numbers::three);
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<Numbers>("many").has_value());
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<Numbers>("None").has_value());

    constexpr auto dr = enum_from_string_nocase_nothrow<Directions>("RiGHt");
    REQUIRE(enum_from_string_nocase_nothrow<Directions>("up").value() == Directions::Up);
    REQUIRE(enum_from_string_nocase_nothrow<Directions>("Down").value() == Directions::Down);
    REQUIRE(dr.value() == Directions::Right);
    REQUIRE(enum_from_string_nocase_nothrow<Directions>("Left").value() == Directions::Left);
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<Directions>("None").has_value());

    constexpr auto nt = enum_from_string_nocase_nothrow<number>("ThrEe");
    REQUIRE(enum_from_string_nocase_nothrow<number>("One").value() == number::one);
    REQUIRE(enum_from_string_nocase_nothrow<number>("two").value() == number::two);
    REQUIRE(nt.value() == number::three);
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<number>("four").has_value());
    REQUIRE_FALSE(enum_from_string_nocase_nothrow<number>("None").has_value());
  }

  SECTION("from_integral") {
    constexpr auto cr = enum_from_integral_nothrow<Color>(-12);
    REQUIRE(cr.value() == Color::RED);
    REQUIRE(enum_from_integral_nothrow<Color>(7).value() == Color::GREEN);
    REQUIRE(enum_from_integral_nothrow<Color>(15).value() == Color::BLUE);
    REQUIRE_FALSE(enum_from_integral_nothrow<Color>(0).has_value());

    constexpr auto no = enum_from_integral_nothrow<Numbers>(10);
    REQUIRE(no.value() == Numbers::one);
    REQUIRE(enum_from_integral_nothrow<Numbers>(20).value() == Numbers::two);
    REQUIRE(enum_from_integral_nothrow<Numbers>(30).value() == Numbers::three);
    REQUIRE_FALSE(enum_from_integral_nothrow<Numbers>(127).has_value());
    REQUIRE_FALSE(enum_from_integral_nothrow<Numbers>(0).has_value());

    constexpr auto dr = enum_from_integral_nothrow<Directions>(120);
    REQUIRE(enum_from_integral_nothrow<Directions>(85).value() == Directions::Up);
    REQUIRE(enum_from_integral_nothrow<Directions>(-42).value() == Directions::Down);
    REQUIRE(dr.value() == Directions::Right);
    REQUIRE(enum_from_integral_nothrow<Directions>(-120).value() == Directions::Left);
    REQUIRE_FALSE(enum_from_integral_nothrow<Directions>(0).has_value());

    constexpr auto nt = enum_from_integral_nothrow<number>(300);
    REQUIRE(enum_from_integral_nothrow<number>(100).value() == number::one);
    REQUIRE(enum_from_integral_nothrow<number>(200).value() == number::two);
    REQUIRE(nt.value() == number::three);
    REQUIRE_FALSE(enum_from_integral_nothrow<number>(400).has_value());
    REQUIRE_FALSE(enum_from_integral_nothrow<number>(0).has_value());
  }
}

TEST_CASE("enum_integer") {
  constexpr auto cr = enum_integer(Color::RED);
  REQUIRE(cr == -12);
  REQUIRE(enum_integer(Color::GREEN) == 7);
  REQUIRE(enum_integer(Color::BLUE) == 15);
  REQUIRE(enum_integer(static_cast<Color>(0)) == 0);

  constexpr auto no = enum_integer(Numbers::one);
  REQUIRE(no == 10);
  REQUIRE(enum_integer(Numbers::two) == 20);
  REQUIRE(enum_integer(Numbers::three) == 30);
  REQUIRE(enum_integer(Numbers::many) == 127);
  REQUIRE(enum_integer(static_cast<Numbers>(0)) == 0);

  constexpr auto dr = enum_integer(Directions::Right);
  REQUIRE(enum_integer(Directions::Left) == -120);
  REQUIRE(enum_integer(Directions::Down) == -42);
  REQUIRE(enum_integer(Directions::Up) == 85);
  REQUIRE(dr == 120);
  REQUIRE(enum_integer(static_cast<Directions>(0)) == 0);

  constexpr auto nt = enum_integer(number::three);
  REQUIRE(enum_integer(number::one) == 100);
  REQUIRE(enum_integer(number::two) == 200);
  REQUIRE(nt == 300);
  REQUIRE(enum_integer(number::four) == 400);
  REQUIRE(enum_integer(static_cast<number>(0)) == 0);
}

TEST_CASE("enum_value") {
  constexpr auto cr = enum_value<Color>(0);
  REQUIRE(cr == Color::RED);
  REQUIRE(enum_value<Color>(1) == Color::GREEN);
  REQUIRE(enum_value<Color>(2) == Color::BLUE);

  constexpr auto no = enum_value<Numbers>(0);
  REQUIRE(no == Numbers::one);
  REQUIRE(enum_value<Numbers>(1) == Numbers::two);
  REQUIRE(enum_value<Numbers>(2) == Numbers::three);

  constexpr auto dr = enum_value<Directions>(3);
  REQUIRE(enum_value<Directions>(0) == Directions::Left);
  REQUIRE(enum_value<Directions>(1) == Directions::Down);
  REQUIRE(enum_value<Directions>(2) == Directions::Up);
  REQUIRE(dr == Directions::Right);

  constexpr auto nt = enum_value<number>(2);
  REQUIRE(enum_value<number>(0) == number::one);
  REQUIRE(enum_value<number>(1) == number::two);
  REQUIRE(nt == number::three);
}

TEST_CASE("enum_values") {
  constexpr auto s1 = enum_values<Color>();
  REQUIRE(s1 == std::array<Color, 3>{{Color::RED, Color::GREEN, Color::BLUE}});

  constexpr auto s2 = enum_values<Numbers>();
  REQUIRE(s2 == std::array<Numbers, 3>{{Numbers::one, Numbers::two, Numbers::three}});

  constexpr auto s3 = enum_values<Directions>();
  REQUIRE(s3 == std::array<Directions, 4>{{Directions::Left, Directions::Down, Directions::Up, Directions::Right}});

  constexpr auto s4 = enum_values<number>();
  REQUIRE(s4 == std::array<number, 3>{{number::one, number::two, number::three}});
}

TEST_CASE("enum_count") {
  constexpr auto s1 = enum_count<Color>();
  REQUIRE(s1 == 3);

  constexpr auto s2 = enum_count<Numbers>();
  REQUIRE(s2 == 3);

  constexpr auto s3 = enum_count<Directions>();
  REQUIRE(s3 == 4);

  constexpr auto s4 = enum_count<number>();
  REQUIRE(s4 == 3);
}

TEST_CASE("enum_name") {
  SECTION("automatic storage") {
    constexpr Color cr = Color::RED;
    constexpr auto cr_name = enum_name(cr);
    Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
    REQUIRE(cr_name == "RED");
    REQUIRE(enum_name(Color::BLUE) == "BLUE");
    REQUIRE(enum_name(cm[1]) == "GREEN");
    REQUIRE(enum_name(static_cast<Color>(0)).empty());

    constexpr Numbers no = Numbers::one;
    constexpr auto no_name = enum_name(no);
    REQUIRE(no_name == "one");
    REQUIRE(enum_name(Numbers::two) == "two");
    REQUIRE(enum_name(Numbers::three) == "three");
    REQUIRE(enum_name(Numbers::many).empty());
    REQUIRE(enum_name(static_cast<Numbers>(0)).empty());

    constexpr Directions dr = Directions::Right;
    constexpr auto dr_name = enum_name(dr);
    REQUIRE(enum_name(Directions::Up) == "Up");
    REQUIRE(enum_name(Directions::Down) == "Down");
    REQUIRE(dr_name == "Right");
    REQUIRE(enum_name(Directions::Left) == "Left");
    REQUIRE(enum_name(static_cast<Directions>(0)).empty());

    constexpr number nt = number::three;
    constexpr auto nt_name = enum_name(nt);
    REQUIRE(enum_name(number::one) == "one");
    REQUIRE(enum_name(number::two) == "two");
    REQUIRE(nt_name == "three");
    REQUIRE(enum_name(number::four).empty());
    REQUIRE(enum_name(static_cast<number>(0)).empty());
  }

  SECTION("static storage") {
    constexpr Color cr = Color::RED;
    constexpr auto cr_name = enum_name<cr>();
    constexpr Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
    REQUIRE(cr_name == "RED");
    REQUIRE(enum_name<Color::BLUE>() == "BLUE");
    REQUIRE(enum_name<cm[1]>() == "GREEN");
    REQUIRE(enum_name<static_cast<Color>(0)>().empty());

    constexpr Numbers no = Numbers::one;
    constexpr auto no_name = enum_name<no>();
    REQUIRE(no_name == "one");
    REQUIRE(enum_name<Numbers::two>() == "two");
    REQUIRE(enum_name<Numbers::three>() == "three");
    REQUIRE(enum_name<Numbers::many>() == "many");
    REQUIRE(enum_name<static_cast<Numbers>(0)>().empty());

    constexpr Directions dr = Directions::Right;
    constexpr auto dr_name = enum_name<dr>();
    REQUIRE(enum_name<Directions::Up>() == "Up");
    REQUIRE(enum_name<Directions::Down>() == "Down");
    REQUIRE(dr_name == "Right");
    REQUIRE(enum_name<Directions::Left>() == "Left");
    REQUIRE(enum_name<static_cast<Directions>(0)>().empty());

    constexpr number nt = number::three;
    constexpr auto nt_name = enum_name<nt>();
    REQUIRE(enum_name<number::one>() == "one");
    REQUIRE(enum_name<number::two>() == "two");
    REQUIRE(nt_name == "three");
    REQUIRE(enum_name<number::four>() == "four");
    REQUIRE(enum_name<static_cast<number>(0)>().empty());
  }
}

TEST_CASE("enum_names") {
  constexpr auto s1 = enum_names<Color>();
  REQUIRE(s1 == std::array<std::string_view, 3>{{"RED", "GREEN", "BLUE"}});

  constexpr auto s2 = enum_names<Numbers>();
  REQUIRE(s2 == std::array<std::string_view, 3>{{"one", "two", "three"}});

  constexpr auto s3 = enum_names<Directions>();
  REQUIRE(s3 == std::array<std::string_view, 4>{{"Left", "Down", "Up", "Right"}});

  constexpr auto s4 = enum_names<number>();
  REQUIRE(s4 == std::array<std::string_view, 3>{{"one", "two", "three"}});
}

TEST_CASE("enum_entries") {
  constexpr auto s1 = enum_entries<Color>();
  REQUIRE(s1 == std::array<std::pair<Color, std::string_view>, 3>{{{Color::RED, "RED"}, {Color::GREEN, "GREEN"}, {Color::BLUE, "BLUE"}}});

  constexpr auto s2 = enum_entries<Numbers>();
  REQUIRE(s2 == std::array<std::pair<Numbers, std::string_view>, 3>{{{Numbers::one, "one"}, {Numbers::two, "two"}, {Numbers::three, "three"}}});

  constexpr auto s3 = enum_entries<Directions>();
  REQUIRE(s3 == std::array<std::pair<Directions, std::string_view>, 4>{{{Directions::Left, "Left"}, {Directions::Down, "Down"}, {Directions::Up, "Up"}, {Directions::Right, "Right"}}});

  constexpr auto s4 = enum_entries<number>();
  REQUIRE(s4 == std::array<std::pair<number, std::string_view>, 3>{{{number::one, "one"}, {number::two, "two"}, {number::three, "three"}}});
}

TEST_CASE("ostream_operators") {
  auto test_ostream = [](auto e, std::string_view name) {
    using namespace ostream_operators;
    std::stringstream ss;
    ss << e;
    REQUIRE(ss.str() == name);
  };

  test_ostream(std::make_optional(Color::RED), "RED");
  test_ostream(Color::GREEN, "GREEN");
  test_ostream(Color::BLUE, "BLUE");
  test_ostream(static_cast<Color>(0), "0");
  test_ostream(std::make_optional(static_cast<Color>(0)), "0");

  test_ostream(std::make_optional(Numbers::one), "one");
  test_ostream(Numbers::two, "two");
  test_ostream(Numbers::three, "three");
  test_ostream(Numbers::many, "127");
  test_ostream(static_cast<Numbers>(0), "0");
  test_ostream(std::make_optional(static_cast<Numbers>(0)), "0");

  test_ostream(std::make_optional(Directions::Up), "Up");
  test_ostream(Directions::Down, "Down");
  test_ostream(Directions::Right, "Right");
  test_ostream(Directions::Left, "Left");
  test_ostream(static_cast<Directions>(0), "0");
  test_ostream(std::make_optional(static_cast<Directions>(0)), "0");

  test_ostream(std::make_optional(number::one), "one");
  test_ostream(number::two, "two");
  test_ostream(number::three, "three");
  test_ostream(number::four, "400");
  test_ostream(static_cast<number>(0), "0");
  test_ostream(std::make_optional(static_cast<number>(0)), "0");
}

TEST_CASE("bitwise_operators") {
  using namespace bitwise_operators;

  SECTION("operator^") {
    REQUIRE(enum_integer(~Color::RED) == ~enum_integer(Color::RED));
    REQUIRE(enum_integer(~Numbers::one) == ~enum_integer(Numbers::one));
    REQUIRE(enum_integer(~Directions::Up) == ~enum_integer(Directions::Up));
    REQUIRE(enum_integer(~number::one) == ~enum_integer(number::one));
  }

  SECTION("operator|") {
    REQUIRE(enum_integer(Color::RED | Color::BLUE) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));
    REQUIRE(enum_integer(Numbers::one | Numbers::two) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));
    REQUIRE(enum_integer(Directions::Up | Directions::Down) == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));
    REQUIRE(enum_integer(number::one | number::two) == (enum_integer(number::one) | enum_integer(number::two)));
  }

  SECTION("operator&") {
    REQUIRE(enum_integer(Color::RED & Color::BLUE) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));
    REQUIRE(enum_integer(Numbers::one & Numbers::two) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));
    REQUIRE(enum_integer(Directions::Up & Directions::Down) == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));
    REQUIRE(enum_integer(number::one & number::two) == (enum_integer(number::one) & enum_integer(number::two)));
  }

  SECTION("operator^") {
    REQUIRE(enum_integer(Color::RED ^ Color::BLUE) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));
    REQUIRE(enum_integer(Numbers::one ^ Numbers::two) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));
    REQUIRE(enum_integer(Directions::Up ^ Directions::Down) == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));
    REQUIRE(enum_integer(number::one ^ number::two) == (enum_integer(number::one) ^ enum_integer(number::two)));
  }

  SECTION("operator|=") {
    Color x1 = Color::RED;
    x1 |= Color::BLUE;
    REQUIRE(enum_integer(x1) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));

    Numbers x2 = Numbers::one;
    x2 |= Numbers::two;
    REQUIRE(enum_integer(x2) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));

    Directions x3 = Directions::Up;
    x3 |= Directions::Down;
    REQUIRE(enum_integer(x3) == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));

    number x4 = number::one;
    x4 |= number::two;
    REQUIRE(enum_integer(x4) == (enum_integer(number::one) | enum_integer(number::two)));
  }

  SECTION("operator&=") {
    Color x1 = Color::RED;
    x1 &= Color::BLUE;
    REQUIRE(enum_integer(x1) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));

    Numbers x2 = Numbers::one;
    x2 &= Numbers::two;
    REQUIRE(enum_integer(x2) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));

    Directions x3 = Directions::Up;
    x3 &= Directions::Down;
    REQUIRE(enum_integer(x3) == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));

    number x4 = number::one;
    x4 &= number::two;
    REQUIRE(enum_integer(x4) == (enum_integer(number::one) & enum_integer(number::two)));
  }

  SECTION("operator^=") {
    Color x1 = Color::RED;
    x1 ^= Color::BLUE;
    REQUIRE(enum_integer(x1) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));

    Numbers x2 = Numbers::one;
    x2 ^= Numbers::two;
    REQUIRE(enum_integer(x2) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));

    Directions x3 = Directions::Up;
    x3 ^= Directions::Down;
    REQUIRE(enum_integer(x3) == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));

    number x4 = number::one;
    x4 ^= number::two;
    REQUIRE(enum_integer(x4) == (enum_integer(number::one) ^ enum_integer(number::two)));
  }
}

TEST_CASE("type_traits") {
  REQUIRE_FALSE(is_unscoped_enum_v<Color>);
  REQUIRE_FALSE(is_unscoped_enum_v<Numbers>);
  REQUIRE(is_unscoped_enum_v<Directions>);
  REQUIRE(is_unscoped_enum_v<number>);

  REQUIRE(is_scoped_enum_v<Color>);
  REQUIRE(is_scoped_enum_v<Numbers>);
  REQUIRE_FALSE(is_scoped_enum_v<Directions>);
  REQUIRE_FALSE(is_scoped_enum_v<number>);

  REQUIRE(is_fixed_enum_v<Color>);
  REQUIRE(is_fixed_enum_v<Numbers>);
  REQUIRE_FALSE(is_fixed_enum_v<Directions>);
  REQUIRE(is_fixed_enum_v<number>);
}
