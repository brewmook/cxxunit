#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Test.h"

namespace cxxunit {

template <typename T>
class Expectation
{
public:
    Expectation(T g, T e) : given(g), expect(e) {}
    T given;
    T expect;
};

template <typename T>
std::ostream & operator<<(std::ostream & o, const Expectation<T> & e)
{
    return o << "Given " << e.given << " expect " << e.expect;
}

template <typename T>
class Given
{
public:
    Given(T t) : value(t) {}

    Expectation<T> expect(T t)
    {
        return Expectation<T>(value, t);
    }

private:
    T value;
};

template <typename T>
class Assert
{
public:
    Assert(T t) : value(t) {}

    void isTrue()
    {
        if (true != value)
            throw std::string("Expected true, but was false");
    }

    void isFalse()
    {
        if (false != value)
            throw std::string("Expected false, but was true");
    }

    template<typename U>
    void equals(U u)
    {
        if (value != u)
        {
            std::stringstream e;
            e << "Expected '" << u << "' but was '" << value << "'";
            throw e.str();
        }
    }

private:
    T value;

};

class TestSuite
{
public:

    struct Result
    {
        Result() : passes(0), failures(0) {}
        size_t passes;
        size_t failures;
    };

    TestSuite(const std::string & aName)
        : name(aName)
    {
        Registry.push_back(this);
    }

    static int runAllTestSuites()
    {
        int failures = 0;
        for (auto i = Registry.begin(); i != Registry.end(); ++i)
        {
            failures += (*i)->runSingleSuite();
        }
        return failures;
    }

    int runSingleSuite()
    {
        std::cout
            << "Running suite " << name
            << " (" << tests.size() << " tests)"
            << std::endl;

        Result result;
        std::for_each(tests.begin(), tests.end(), [&](Test* test)
        {
            try {
                test->run();
                ++result.passes;
            }
            catch (const std::string & e)
            {
                std::cout << "FAIL: " << test->name << ": " << e << std::endl;
                std::cout << "  " << test->description << std::endl;
                ++result.failures;
            }
            catch (...)
            {
                std::cout << "FAIL: " << test->name << std::endl;
                std::cout << "  \"" << test->description << "\"" << std::endl;
                ++result.failures;
            }
        });

        std::cout
        << "  Passes: " << result.passes
        << " / Failures: " << result.failures
        << std::endl;

        return result.failures;
    }

protected:

    template <typename T>
    std::vector<T> testCaseData(T t)
    {
        std::array<T, 1> array = { t };
        return std::vector<T> (array.begin(), array.end());
    }

    template <typename T>
    std::vector<T> testCaseData(T t1, T t2)
    {
        std::array<T, 2> array = { t1, t2 };
        return std::vector<T> (array.begin(), array.end());
    }

    template <typename T>
    std::vector<T> testCaseData(T t1, T t2, T t3)
    {
        std::array<T, 3> array = { t1, t2, t3 };
        return std::vector<T> (array.begin(), array.end());
    }

    template <typename T>
    std::vector<T> testCaseData(T t1, T t2, T t3, T t4)
    {
        std::array<T, 4> array = { t1, t2, t3, t4 };
        return std::vector<T> (array.begin(), array.end());
    }

    template <typename T>
    std::vector<T> testCaseData(T t1, T t2, T t3, T t4, T t5)
    {
        std::array<T, 5> array = { t1, t2, t3, t4, t5 };
        return std::vector<T> (array.begin(), array.end());
    }

    template <typename T>
    Assert<T> assert(T t) const
    {
        return Assert<T>(t);
    }

    template <typename T>
    Given<T> given(T t) const
    {
        return Given<T>(t);
    }

    template <typename Function>
    void test(const std::string & aName,
              const std::string & aDescription,
              Function nullaryFunction)
    {
        tests.push_back(new NullaryTest<Function>(aName, aDescription, nullaryFunction));
    }

    template <typename Container, typename Function>
    void test(const std::string & name,
              const std::string & aDescription,
              Container arguments,
              Function function)
    {
        size_t n = 1;
        for (auto i = arguments.begin(); i != arguments.end(); ++i, ++n)
        {
            std::string desc = description(aDescription, n, arguments.size(), *i);
            tests.push_back(new UnaryTest<typename Container::value_type, Function>(name, desc, *i, function));
        }
    }

    template <typename Argument, typename Function>
    void test(const std::string & name,
              const std::string & aDescription,
              std::vector<Expectation<Argument>> expects,
              Function function)
    {
        size_t n = 1;
        for (auto i = expects.begin(); i != expects.end(); ++i, ++n)
        {
            std::string desc = description(aDescription, n, expects.size(), *i);
            tests.push_back(new BinaryTest<Argument, Argument, Function>(name, desc, i->given, i->expect, function));
        }
    }

private:

    template <typename T>
    std::string description(const std::string & d, size_t i, size_t n, T t)
    {
        std::stringstream ss;
        ss << d << " ["
           << i << " of " << n
           << ", data '" << t << "'"
           << "]";
        return ss.str();
    }

    std::string name;
    std::vector<Test*> tests;
    static std::vector<TestSuite*> Registry;
};

} // anonymous
