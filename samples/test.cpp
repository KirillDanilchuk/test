//
// Created by Kirill Danilchuk <kirill.danilchuk01@gmail.com> on 03/05/2022.
//

#include <test/test.hpp>

TEST_SUITE(JustSuite) {
  TEST(JustTest) {
    // pass
  }
}

TEST_SUITE(Assert) {
  TEST(True) {
    ASSERT_TRUE(true);
  }
}

TEST_SUITE(Except) {
  TEST(True) {
    EXCEPT_TRUE(true);
  }
  TEST(False) {
    EXCEPT_TRUE(false);
  }
}