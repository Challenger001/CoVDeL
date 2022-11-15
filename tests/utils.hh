#ifndef __COVDEL_TESTS_UTILS_HH_1668199044__
#define __COVDEL_TESTS_UTILS_HH_1668199044__

// assert macro, return if condition is false
#define ASSERT(condition) \
 if (!(condition)) return false;

// check if the input code snippet throws the expected exception
#define EXPECT_THROW(exception, code) \
 bool thrown { false };               \
 try {                                \
  code                                \
 }                                    \
 catch (const exception &) {          \
  thrown = true;                      \
 }                                    \
 if (!thrown) return false;

// placeholder for successful return of test function
#define TEST_SUCCESS return true;

#include <iostream>

class UnitTestRunner {
public:
  UnitTestRunner(const std::string &class_file, const std::string &class_name)
  {
    std::cout << "Testing '" << class_name << "' [" << class_file << "]\n";
  }

  ~UnitTestRunner()
  {
    std::cout << "Done.\nPassed : " << m_passed << '/' << m_total << '\n';
  }

  template<typename... _TestFuncArgs>
  void run(
    const std::string &test_name, bool test_func(_TestFuncArgs...), _TestFuncArgs... args)
  {
    ++m_total;
    std::cout << "  " << test_name << " - ";
    if (test_func(args...)) {
      std::cout << "passed\n";
      ++m_passed;
    } else
      std::cout << "failed\n";
  }

private:
  unsigned m_passed;
  unsigned m_total;
};

#endif
