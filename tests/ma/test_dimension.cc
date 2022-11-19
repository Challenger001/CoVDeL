#include "../utils.hh"
#include "covdel/ma/multiarray.hh"

using namespace covdel::ma;

static const dimension d1 { 2, 4, 3 };
static const dimension d2 { 5, 0 };

bool dim_operators()
{
  dimension d = std::vector<size_t> { 2, 4, 3 };
  ASSERT(d == d1);
  ASSERT(d1[0] == 2 && d1[1] == 4 && d1[2] == 3);
  EXPECT_THROW(std::out_of_range, d1[3];);
  ASSERT(d1 == d1 && d1 != d2 && d2 == d2);
  TEST_SUCCESS;
}

bool dim_construction()
{
  EXPECT_THROW(std::invalid_argument, dimension d {};);
  dimension d { std::vector<size_t> { 2, 4, 3 } };
  ASSERT(d == d1);
  TEST_SUCCESS;
}

bool dim_getters()
{
  auto shape { d2.shape() };
  ASSERT(shape[0] == d2[0] && shape[1] == d2[1] && shape.size() == d2.ndims());
  ASSERT(d1.ndims() == 3 && d2.ndims() == 2);
  ASSERT(d1.size() == 24 && d2.size() == 0);
  ASSERT(d1.str() == "( 2 4 3 )" && d2.str() == "( 5 0 )");
  TEST_SUCCESS;
}

bool dim_copy_move_semantics()
{
  auto d3 { d1 }, d2_ { d2 };
  auto d4 { std::move(d2_) };
  ASSERT(d3 == d1 && d4 == d2);
  d3 = d4;
  d4 = std::move(d1);
  ASSERT(d3 != d4 && d3.str() == "( 5 0 )" && d4.str() == "( 2 4 3 )");
  TEST_SUCCESS;
}

bool dim_general()
{
  auto d3 { d2 }, d4 { d1 };
  d3.swap(d4);
  ASSERT(d3.str() == "( 2 4 3 )" && d4.str() == "( 5 0 )");
  TEST_SUCCESS;
}

bool index_all()
{
  dimension d { 3, 10, 8 };
  index i1 { 2, 8, 1 }, i2 { 4, 1, 1 };
  ASSERT(i1 < d && i2 >= d);
  ASSERT(i1.ndims() == 3 && i1.flat(d) == 225);  // 1+8*8+80*2=225
  TEST_SUCCESS;
}

int main()
{
  UnitTestRunner tester { "multiarray.hh", "dimension" };

  tester.run("Operators", dim_operators);
  tester.run("Construction", dim_construction);
  tester.run("Getters", dim_getters);
  tester.run("Copy-Move", dim_copy_move_semantics);
  tester.run("General", dim_general);
  tester.run("Index", index_all);

  return tester.passed() == tester.total() ? EXIT_SUCCESS : EXIT_FAILURE;
}
