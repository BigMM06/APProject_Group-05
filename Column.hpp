#include <iostream>
#include <string>
#include <vector>

#include "DataType.hpp"

using namespace std;
#ifndef COLUMN_HPP
#define COLUMN_HPP
class Column
{
public:
    string name;
    DataType *dataType;

    Column()
    {
        return;
    }
    Column(const string columnName, DataType *columnDataType)
        : name(columnName), dataType(columnDataType) {}

    string getName() const
    {
        return name;
    }
    DataType *getDataType() const
    {
        return dataType;
    }
    void setName(const string &columnName)
    {
        name = columnName;
    }
    void setDataType(DataType *columnDataType)
    {
        dataType = columnDataType;
    }
};
#endif
