#include <iostream>
#include <regex>
#include <string>
using namespace std;
#ifndef DATATYPE
#define DATATYPE

class DataType
{

public:
    virtual bool detectType(string input) = 0;
    virtual string getType() = 0;
};

class IntegerType : public DataType
{
    int intInput;

public:
    bool detectType(string input) override
    {
        regex intPattern("^-?\\d+$");
        if (regex_match(input, intPattern))
        {
            intInput = stoi(input);
            return true;
        }
        else
            return false;
    }
    string getType() override
    {
        return "int";
    }
};

class StringType : public DataType
{
public:
    bool detectType(string input) override
    {
        return true;
    }
    string getType() override
    {
        return "string";
    }
};

class DateType : public DataType
{
public:
    bool detectType(string input) override
    {
        regex datePattern(R"(^\d{4}/\d{2}/\d{2}$)");
        if (regex_match(input, datePattern))
        {
            int year = stoi(input.substr(0, 4));
            int month = stoi(input.substr(5, 2));
            int day = stoi(input.substr(8, 2));
            if (month >= 1 && month <= 12 && day >= 1 && day <= 31)
                return true;
        }
        return false;
    }
    string getType() override
    {
        return "date";
    }
};

class FloatType : public DataType
{
    double floatInput;

public:
    bool detectType(string input) override
    {
        regex floatPattern("^-?\\d*\\.\\d+$");
        if (regex_match(input, floatPattern))
        {
            floatInput = stod(input);
            return true;
        }
        return false;
    }
    string getType() override
    {
        return "float";
    }
};
#endif