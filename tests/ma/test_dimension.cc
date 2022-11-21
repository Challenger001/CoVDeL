#include "../utils.hh"
#include "covdel/ma/dimension.hh"

using namespace covdel::ma;

static const dimension d1 { 3, 10, 1 }, d2 { 5, 0 };
static const index i1 { 2, 8, 0 }, i2 { 4, 1, 2 };

bool dsi_operators()
{
  ASSERT(d1[0] == 3 && d1[1] == 10 && d1[2] == 1);
  ASSERT(i1[0] == 2 && i1[1] == 8 && i1[2] == 0);
  EXPECT_THROW(std::out_of_range, d1[3];);
  EXPECT_THROW(std::out_of_range, i1[3];);
  ASSERT(d1 == d1 && d1 != d2 && d2 == d2);
  ASSERT(i1 == i1 && i1 != i2 && i2 == i2);
  TEST_SUCCESS;
}

bool dsi_construction()
{
  EXPECT_THROW(std::invalid_argument, dimension d {};);
  EXPECT_THROW(std::invalid_argument, index i {};);
  EXPECT_THROW(std::invalid_argument, CODE(dimension d { 1, 2, 3, 4, 5, 6, 7 };));
  EXPECT_THROW(std::invalid_argument, CODE(index i { 1, 2, 3, 4, 5, 6, 7 };));
  TEST_SUCCESS;
}

bool dsi_copy_move()
{
  auto d3 { d1 }, d2_ { d2 };
  auto d4 { std::move(d2_) };
  ASSERT(d3 == d1 && d4 == d2);
  d3 = d4, d2_ = d1;
  d4 = std::move(d2_);
  ASSERT(d3 != d4 && d3 == d2 && d4 == d1);
  TEST_SUCCESS;
}

bool dsi_general()
{
  ASSERT(d1.ndims() == 3 && d2.ndims() == 2);
  ASSERT(i1.ndims() == 3 && i2.ndims() == 3);
  ASSERT(d1.str() == "( 3 10 1 )" && d2.str() == "( 5 0 )");
  ASSERT(i1.str() == "( 2 8 0 )" && i2.str() == "( 4 1 2 )");
  auto d3 { d2 }, d4 { d1 };
  d3.swap(d4);
  ASSERT(d3 == d1 && d4 == d2);
  auto i3 { i2 }, i4 { i1 };
  i3.swap(i4);
  ASSERT(i3 == i1 && i4 == i2);
  TEST_SUCCESS;
}

bool dim_specific()
{
  ASSERT(d1.size() == 30 && d2.size() == 0);
  auto d3 { d1 };
  d3.squeeze();
  ASSERT(d3.str() == "( 3 10 )");
  TEST_SUCCESS;
}

bool idx_specific()
{
  ASSERT(i1 < d1 && i2 >= d1);
  EXPECT_THROW(std::invalid_argument, (void)(i2 < d2););
  ASSERT(i1.flat(d1) == 28);
  EXPECT_THROW(std::out_of_range, i2.flat(d1););
  TEST_SUCCESS;
}

int main()
{
  UnitTestRunner tester { "dimension.hh", "_dsi | dimension | index" };

  tester.run("Operators", dsi_operators);
  tester.run("Construction", dsi_construction);
  tester.run("Copy-Move", dsi_copy_move);
  tester.run("General", dsi_general);
  tester.run("Dimension", dim_specific);
  tester.run("Index", idx_specific);

  return tester.passed() == tester.total() ? EXIT_SUCCESS : EXIT_FAILURE;
}
