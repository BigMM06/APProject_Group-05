#include <iostream>
#include <string>
#include <vector>
#include "Column.hpp"
using namespace std;

class Schema
{
public:
    string tableName;
    vector<Column> columns;

    Schema(const string name = "") : tableName(name) {}

    void addColumn(const Column &column)
    {
        columns.push_back(column);
    }
    void displayColumns() const
    {
        cout << "Table: " << tableName << endl;
        for (const auto &column : columns)
        {
            cout << "Column Name: " << column.getName()
                 << ", Data Type: " << column.getDataType() << endl;
        }
    }

    bool findColumn(const string &columnName) const
    {
        return any_of(columns.begin(), columns.end(),
                      [&columnName](const Column &column)
                      {
                          return column.getName() == columnName;
                      });
    }
};