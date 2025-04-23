#include <iostream>
#include <string>
#include <vector>

using namespace std;
#ifndef COLUMN_HPP
#define COLUMN_HPP
class Column
{
public:
    string name;
    string dataType;

    Column(const string columnName = "", const string columnDataType = "")
        : name(columnName), dataType(columnDataType) {}

    string getName() const
    {
        return name;
    }
    string getDataType() const
    {
        return dataType;
    }
    void setName(const string &columnName)
    {
        name = columnName;
    }
    void setDataType(const string &columnDataType)
    {
        dataType = columnDataType;
    }
};
#endif
