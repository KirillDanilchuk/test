//
// Created by Kirill Danilchuk <kirill.danilchuk01@gmail.com> on 25/04/2022.
//

#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>

class ITest {
 public:
  enum class Status {
    kPassed,
    kException,
    kAssertFailed,
    kExceptFailed
  };

  virtual void Run() = 0;
  virtual std::string Name() = 0;
  virtual std::string SuiteName() = 0;
  virtual ~ITest() = default;

  Status GetStatus() {
    return status_;
  }

  void SetStatus(Status status) {
    status_ = status;
  }

 private:
  Status status_;
};

using ITestPtr = std::shared_ptr<ITest>;
using TestList = std::vector<ITestPtr>;

void AllTestPassed();
void DispatchTest(const ITestPtr& test);
TestList& GetAllTests();
void SetCurrentTest(ITestPtr test);
void RegisterTest(ITestPtr test);
void Fail(int);
void Warn(int);

#define TEST_SUITE(suite)                 \
namespace test_suite_##suite {            \
  std::string GetCurrentSuiteName() {     \
    return #suite;                        \
  }                                       \
}                                         \
namespace test_suite_##suite

#define TEST(name)                                \
void TestRoutine##name();                         \
struct Test##name : public ITest {                \
  void Run() override {                           \
    try {                                         \
      TestRoutine##name();                        \
      SetStatus(Status::kPassed);                 \
    } catch(...) {                                \
      if (GetStatus() == Status::kAssertFailed || \
          GetStatus() == Status::kExceptFailed) { \
        /*pass*/                                  \
      } else {                                    \
        SetStatus(Status::kException);            \
      }                                           \
    }                                             \
  }                                               \
                                                  \
  std::string Name() override {                   \
    return #name;                                 \
  }                                               \
                                                  \
  std::string SuiteName() override {              \
      return GetCurrentSuiteName();               \
  }                                               \
};                                                \
                                                  \
struct TestRegistrar##name {                      \
  TestRegistrar##name() noexcept {                \
    RegisterTest(std::make_shared<Test##name>()); \
  }                                               \
};                                                \
static TestRegistrar##name test_registrar_##name; \
                                                  \
void TestRoutine##name()

#define REQUEST_SEMICOLON(expression) do { expression } while(false)

#define RUN_ALL_TESTS()       \
int main() {                  \
  auto& tests = GetAllTests();\
  for (auto&& test : tests) { \
    SetCurrentTest(test);     \
    test->Run();              \
    DispatchTest(test);       \
  }                           \
  AllTestPassed(); /*TODO: Invoke TestFinised()*/            \
}                             \
static int request_semicolon_global_value_dont_use_it

#define ASSERT_TRUE(condition) \
REQUEST_SEMICOLON(if (!(condition)) { Fail(__LINE__); })

#define EXCEPT_TRUE(condition) \
REQUEST_SEMICOLON(if (!(condition)) { Warn(__LINE__); })