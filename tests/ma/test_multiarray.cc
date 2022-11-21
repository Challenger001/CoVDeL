#include "../utils.hh"
#include "covdel/ma/factory.hh"

using namespace covdel::ma;

static const auto d1 { array<bool8>(D(1)) };
static const auto d2 { array<int8>(D(1, 2)) };
static const auto d3 { array<int16>(D(2, 2), 6) };
static const auto d4 { array<int32>(D(3, 2)) };
static const auto d5 { array<int64>(D(4, 2)) };
static const auto d6 { array<uint8>(D(1, 2, 3)) };
static const auto d7 { array<uint16>(D(2, 2, 3)) };
static const auto d8 { array<uint32>(D(3, 2, 3)) };
static const auto d9 { array<uint64>(D(4, 2, 3)) };
static const auto d10 { array<float32>(D(1, 2, 3, 4)) };
static const auto d11 { array<float64>(D(2, 2, 3, 4)) };

bool operators()
{
  auto d { array<int16>(D(2, 2), 4) };
  ASSERT(d1 == d1 && d != d3 && d2 == d2);
  ASSERT(CODE(d3[{ 0, 0 }] == 6));
  ASSERT(CODE(d3[{ 0, 1 }] == 6));
  ASSERT(CODE(d3[{ 1, 0 }] == 6));
  ASSERT(CODE(d3[{ 1, 1 }] == 6));
  EXPECT_THROW(std::out_of_range, CODE(d3[{ 2, 2 }]););
  ASSERT(bool(d1));
  ASSERT(!bool(int16 { D(1, 0) }));
  TEST_SUCCESS;
}

bool construction()
{
  auto t1 { array<int32>(D(2, 2), 6) };           // factory
  int32 t2 { D(2, 2), 6 };                        // alias
  multiarray<dtype::int32> t3 { D { 2, 2 }, 6 };  // constructor
  ASSERT(t1 == t2 && t2 == t3 && t3 == t1);
  TEST_SUCCESS;
}

bool getters()
{
  ASSERT(d5.dim().str() == "( 4 2 )");
  ASSERT(d8.type() == datatype::uint32);
  ASSERT(d11.ndims() == 4 && d11.size() == 48 && d11.is_base());
  // TODO add str() method after implementing
  TEST_SUCCESS;
}

bool copy_move_semantics()
{
  auto t1 { d2 };
  auto t2_ { d3 }, t2 { std::move(t2_) };
  ASSERT(!t1.is_base() && !t2.is_base());
  ASSERT(t1 == d2 && t2 == d3);
  t1 = d2, t2 = int16 { D(2, 2), 6 };
  ASSERT(!t1.is_base() && t2.is_base());
  ASSERT(t1 == d2 && t2 == d3);
  TEST_SUCCESS;
}

bool general()
{
  auto t1 { d9.astype<float64>() };
  ASSERT(t1.is_base() && t1 == float64 { D(4, 2, 3) });
  auto t2 { d11.copy() };
  ASSERT(t2.is_base() && t2 == d11);
  t1.swap(t2);
  ASSERT(t1 == d11 && t1.dim().str() == "( 2 2 3 4 )");
  ASSERT(t2.dim().str() == "( 4 2 3 )");
  TEST_SUCCESS;
}

bool shape_manipulation()
{
  auto d { array<int32>(D(3, 2, 5), 4) };
  ASSERT(d.dim().str() == "( 3 2 5 )");
  d.flatten();
  ASSERT(d.dim().str() == "( 30 )");
  EXPECT_THROW(std::invalid_argument, d.reshape(D(10)););
  d.reshape(D(6, 1, 1, 5));
  ASSERT(d.dim().str() == "( 6 1 1 5 )");
  d.squeeze();
  ASSERT(d.dim().str() == "( 6 5 )");
  TEST_SUCCESS;
}

int main()
{
  UnitTestRunner tester { "multiarray.hh", "multiarray" };

  tester.run("Operators", operators);
  tester.run("Construction", construction);
  tester.run("Getters", getters);
  tester.run("Copy-Move", copy_move_semantics);
  tester.run("General", general);
  tester.run("Shape Manipulation", shape_manipulation);

  return tester.passed() == tester.total() ? EXIT_SUCCESS : EXIT_FAILURE;
}
