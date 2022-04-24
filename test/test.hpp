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

ITestPtr current_test;

template <typename T>
T* Singleton() {
  static T instance;
  return &instance;
}

using TestList = std::vector<ITestPtr>;

void RegisterTest(ITestPtr test) {
  Singleton<TestList>()->push_back(std::move(test));
}

TestList& GetAllTests() {
  return *Singleton<TestList>();
}

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

std::string GetTestFullPath(const ITestPtr& test) {
  return test->SuiteName() + "::" + test->Name();
}

void TestPassed(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " PASSED" << std::endl;
}

void TestAssert(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " ASSERT" << std::endl;
}

void TestExcept(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " EXCEPT" << std::endl;
}

void TestException(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " THREW EXCEPTION" << std::endl;
}

void AllTestPassed() {
  std::cout << "ALL TESTS PASSED" << std::endl;
}

void Fail(int) {
  current_test->SetStatus(ITest::Status::kAssertFailed);
  throw 1;
}

void Warn(int) {
  current_test->SetStatus(ITest::Status::kExceptFailed);
  throw 1;
}

void DispatchTest(const ITestPtr& test) {
  switch (test->GetStatus()) {
    case ITest::Status::kPassed: {
      TestPassed(test);
      break;
    }
    case ITest::Status::kAssertFailed: {
      TestAssert(test);
      std::abort();
    }
    case ITest::Status::kExceptFailed: {
      TestExcept(test);
      break;
    }
    case ITest::Status::kException: {
      TestException(test);
      break;
    }
  }
}

#define REQUEST_SEMICOLON(expression) do { expression } while(false)

#define RUN_ALL_TESTS()       \
int main() {                  \
  auto& tests = GetAllTests();\
  for (auto&& test : tests) { \
    current_test = test;      \
    test->Run();              \
    DispatchTest(test);       \
  }                           \
  AllTestPassed();            \
}                             \
int request_semicolon_global_value_dont_use_it

#define ASSERT_TRUE(condition) \
REQUEST_SEMICOLON(if (!(condition)) { Fail(__LINE__); })

#define EXCEPT_TRUE(condition) \
REQUEST_SEMICOLON(if (!(condition)) { Warn(__LINE__); })