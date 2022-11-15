#include "../utils.hh"
#include "covdel/ma/multiarray.hh"

using namespace covdel::ma;

static const dimension d1 { 2, 4, 3 };
static const dimension d2 { 5, 0 };

bool test_construction()
{
  EXPECT_THROW(std::invalid_argument, dimension d {};);
  TEST_SUCCESS;
}

bool test_copy_move_semantics()
{
  // constructor
  auto d3(d1), d2_(d2);
  auto d4(std::move(d2_));
  ASSERT(d3 == d1 && d4 == d2);

  // assignment
  d3 = d4;
  d4 = std::move(d1);
  ASSERT(d3 != d4 && d3.str() == "( 5 0 )" && d4.str() == "( 2 4 3 )");
  TEST_SUCCESS;
}

bool test_operators()
{
  // subscript
  ASSERT(d1[0] == 2 && d1[1] == 4 && d1[2] == 3);
  EXPECT_THROW(std::out_of_range, d1[3];);
  // equality
  ASSERT(d1 == d1 && d1 != d2 && d2 == d2);
  TEST_SUCCESS;
}

bool test_conversion()
{
  ASSERT(d1 && !d2);
  ASSERT(d1.str() == "( 2 4 3 )" && d2.str() == "( 5 0 )");
  TEST_SUCCESS;
}

bool test_getters()
{
  auto shape { d2.shape() };
  ASSERT(shape[0] == d2[0] && shape[1] == d2[1] && shape.size() == d2.ndims());
  ASSERT(d1.ndims() == 3 && d2.ndims() == 2);
  ASSERT(d1.size() == 24 && d2.size() == 0);
  TEST_SUCCESS;
}

int main()
{
  UnitTestRunner tester { "multiarray.hh", "dimension" };

  tester.run("Construction", test_construction);
  tester.run("Copy-Move", test_copy_move_semantics);
  tester.run("Operators", test_operators);
  tester.run("Conversion", test_conversion);
  tester.run("Getters", test_getters);

  return EXIT_SUCCESS;
}
