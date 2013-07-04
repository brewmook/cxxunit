#pragma once

namespace cxxunit {

class Test
{
public:
    Test(const std::string & aName, const std::string & aDescription)
      : name(aName), description(aDescription)
    {}

    virtual ~Test() {}
    virtual void run() = 0;

    std::string name;
    std::string description;
};

template <typename Function>
class NullaryTest : public Test
{
public:
    NullaryTest(const std::string & aName, const std::string & aDescription, Function aFunction)
      : Test(aName, aDescription), function(aFunction)
    {}

    virtual ~NullaryTest() {}

    virtual void run()
    {
        function();
    }

private:
    Function function;
};

template <typename Argument, typename Function>
class UnaryTest : public Test
{
public:
    UnaryTest(const std::string & aName, const std::string & aDescription, Argument aArgument, Function aFunction)
      : Test(aName, aDescription), argument(aArgument), function(aFunction)
    {}

    virtual ~UnaryTest() {}

    virtual void run()
    {
        function(argument);
    }

private:
    Argument argument;
    Function function;
};

template <typename Argument1, typename Argument2, typename Function>
class BinaryTest : public Test
{
public:
    BinaryTest(const std::string & aName,
               const std::string & aDescription,
               Argument1 aArgument1,
               Argument2 aArgument2,
               Function aFunction)
      : Test(aName, aDescription),
        argument1(aArgument1),
        argument2(aArgument2),
        function(aFunction)
    {}

    virtual ~BinaryTest() {}

    virtual void run()
    {
        function(argument1, argument2);
    }

private:
    Argument1 argument1;
    Argument2 argument2;
    Function function;
};

} // cxxunit
