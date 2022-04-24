//
// Created by Kirill Danilchuk <kirill.danilchuk01@gmail.com> on 24/04/2022.
//
#include <iostream>
#include <test/test.hpp>

TEST_SUITE(MainTests) {
  TEST(JustWork) {
    ASSERT_TRUE(true);
  }

  TEST(JustWork2) {
    EXCEPT_TRUE(true);
  }

  TEST(ThrowException) {
    throw 2;
  }

  TEST(JustNotWork1) {
    EXCEPT_TRUE(false);
  }

  TEST(JustNotWork2) {
    ASSERT_TRUE(false);
  }
}

RUN_ALL_TESTS();
