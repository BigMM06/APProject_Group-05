#include <iostream>
#include <string>
#include <vector>
#include "MetaData.hpp"
using namespace std;

class Record
{
public:
    MetaData metadata;
    vector<string> recordInfo;

    void display() const
    {
        cout << "Table Name: " << metadata.tableName << endl;
        cout << "Is Deleted: " << (metadata.isDeleted ? "Yes" : "No") << endl;
        cout << "Record Data: ";
        for (const auto &item : recordInfo)
        {
            cout << item << " ";
        }
        cout << endl;
    }
    void setDeleted(bool deleted)
    {
        metadata.isDeleted = deleted;
    }
};