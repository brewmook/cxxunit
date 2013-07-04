#pragma once

#include <map>
#include <array>

#include <cxxunit/TestSuite.h>

class Blah
{
public:
    bool isOdd(int number) const
    {
        return (number % 2) == 1;
    }

    int timesTwo(int number) const
    {
        return number * 2;
    }
};

class BlahTests : public cxxunit::TestSuite
{
public:
    BlahTests() : TestSuite("BlahTests")
    {
        test("isOdd(0)", "Given 0, return false", [&]()
        {
            Blah blah;
            assert(blah.isOdd(0)).isFalse();
        });

        test("isOdd", "Given odd numbers, return true",
             testCaseData(1,3,5,7,9),
             [&](int data)
        {
            Blah blah;
            assert(blah.isOdd(data)).isTrue();
        });

        test("isOdd", "Given even numbers, return false",
             testCaseData(2,4,6,8,10),
             [&](int data)
        {
            Blah blah;
            assert(blah.isOdd(data)).isFalse();
        });

        test("timesTwo", "Return argument multiplied by two",
             testCaseData(given(2).expect(4),
                          given(3).expect(7)),
             [&](int input, int expected)
        {
            Blah blah;
            int result = blah.timesTwo(input);
            assert(result).equals(expected);
        });
    }
};
