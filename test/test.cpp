//
// Created by Kirill Danilchuk <kirill.danilchuk01@gmail.com> on 25/04/2022.
//

#include <test/test.hpp>

ITestPtr current_test;

namespace detail {
template <typename T>
T* Singleton() {
  static T instance;
  return &instance;
}

ITestPtr CurrentTest() noexcept;
std::string GetTestFullPath(const ITestPtr& test);
void TestPassed(const ITestPtr& test);
void TestAssert(const ITestPtr& test);
void TestExcept(const ITestPtr& test);
void TestException(const ITestPtr& test);
}

TestList& GetAllTests() {
  return *detail::Singleton<TestList>();
}

void Warn(int) {
  detail::CurrentTest()->SetStatus(ITest::Status::kExceptFailed);
  throw 1;
}

void Fail(int) {
  detail::CurrentTest()->SetStatus(ITest::Status::kAssertFailed);
  throw 1;
}

ITestPtr detail::CurrentTest() noexcept {
  return current_test;
}

void AllTestPassed() {
  std::cout << "ALL TESTS PASSED" << std::endl;
}

void RegisterTest(ITestPtr test) {
  detail::Singleton<TestList>()->push_back(std::move(test));
}

void SetCurrentTest(ITestPtr test) {
  current_test = std::move(test);
}

void DispatchTest(const ITestPtr& test) {
  switch (test->GetStatus()) {
    case ITest::Status::kPassed: {
      detail::TestPassed(test);
      break;
    }
    case ITest::Status::kAssertFailed: {
      detail::TestAssert(test);
      std::abort(); // TODO: Invoke TestAborting();
    }
    case ITest::Status::kExceptFailed: {
      detail::TestExcept(test);
      break;
    }
    case ITest::Status::kException: {
      detail::TestException(test);
      break;
    }
  }
}

void detail::TestPassed(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " PASSED" << std::endl;
}

std::string detail::GetTestFullPath(const ITestPtr& test) {
  return test->SuiteName() + "::" + test->Name();
}

void detail::TestAssert(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " ASSERT" << std::endl;
}

void detail::TestExcept(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " EXCEPT" << std::endl;
}

void detail::TestException(const ITestPtr& test) {
  std::cout << GetTestFullPath(test) << " THREW EXCEPTION" << std::endl;
}