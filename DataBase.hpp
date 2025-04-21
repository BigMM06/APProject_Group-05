<<<<<<< HEAD
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <regex>
#include <cstdarg>

#include "Column.hpp"
#include "Record.hpp"
#include "Schema.hpp"
#include "Metadata.hpp"

string detectType(const string &input)
{
    regex intPattern("^-?\\d+$");
    regex floatPattern("^-?\\d*\\.\\d+$");

    if (regex_match(input, intPattern))
    {
        return "int";
    }
    else if (regex_match(input, floatPattern))
    {
        return "float";
    }
    else
    {
        return "string";
    }
}

string toLowerCase(const string &str)
{
    string lowerStr = str;
    if (detectType(str) == "string")
    {
        for (char &c : lowerStr)
        {
            if (c >= 'A' && c <= 'Z')
                c = tolower(c);
        }
    }
    return lowerStr;
}

class DataBase
{
private:
    vector<Record> records;

public:
    class Dbinfo
    {
    public:
        vector<Schema> schema;
        void insertTable(string tableName, int columnCount, string columnName[], string columnDataTypes[])
        {
            if (columnCount <= 0)
            {
                cout << "Invalid column count!\n";
                return;
            }
            for (int i = 0; i < schema.size(); i++)
            {
                if (schema[i].tableName == tableName)
                {
                    cout << "Table already exists: " << tableName << endl;
                    return;
                }
            }
            schema.push_back(Schema());
            Schema &lastSchema = schema.back();
            lastSchema.tableName = tableName;
            for (int i = 0; i < columnCount; i++)
            {
                lastSchema.columns.push_back(Column());
                lastSchema.columns[i].name = columnName[i];
                lastSchema.columns[i].dataType = toLowerCase(columnDataTypes[i]);
            }
            cout << "Table created successfully: " << tableName << endl;
        }
        void deleteTable(string tableName, DataBase &db)
        {
            int tableIndex = findTable(tableName);
            if (tableIndex != -1)
            {
                for (int i = 0; i < db.records.size(); i++)
                {
                    if (db.records[i].metadata.tableName == tableName && db.records[i].metadata.isDeleted == false)
                    {
                        db.records[i].metadata.isDeleted = true;
                    }
                }
                schema.erase(schema.begin() + tableIndex);
                cout << "Table deleted successfully: " << tableName << endl;
                return;
            }
            cout << "Table not found: " << tableName << endl;
        }
        int findTable(string tableName)
        {
            for (int i = 0; i < schema.size(); i++)
            {
                if (toLowerCase(schema[i].tableName) == toLowerCase(tableName))
                {
                    return i;
                }
            }
            return -1;
        }
        void showTable(string tableName, DataBase &db)
        {
            int tableIndex = findTable(tableName);
            if (tableIndex != -1)
            {
                cout << "Table " << tableName << " :\n";
                cout << "| ";
                for (int i = 0; i < schema[tableIndex].columns.size(); i++)
                {
                    cout << schema[tableIndex].columns[i].name << " | ";
                }
                cout << endl;
                for (int i = 0; i < schema[tableIndex].columns.size(); i++)
                {
                    cout << "-------";
                }
                cout << endl;
                for (int i = 0; i < db.records.size(); i++)
                {
                    if (db.records[i].metadata.tableName == tableName && db.records[i].metadata.isDeleted == false)
                    {
                        cout << "| ";
                        for (int j = 0; j < db.records[i].recordInfo.size(); j++)
                        {
                            cout << db.records[i].recordInfo[j] << " | ";
                        }
                        cout << endl;
                    }
                }
            }
            else
            {
                cout << "Table not found: " << tableName << endl;
            }
        }
        void allTables()
        {
            if (schema.size() == 0)
            {
                cout << "No table found!" << endl;
                return;
            }
            cout << "Tables in the database:" << endl;
            for (int i = 0; i < schema.size(); i++)
            {
                cout << schema[i].tableName << " | Column count: " << schema[i].columns.size() << endl;
            }
        }
    } dbinfo;

    template <typename... Args>
    void insertRecord(const string &tableName, Args... args)
    {
        constexpr size_t fieldCount = sizeof...(Args);

        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        if (fieldCount == 0)
        {
            cout << "Record is empty!" << endl;
            return;
        }
        else if (dbinfo.schema[tableIndex].columns.size() == 0)
        {
            cout << "Table " << tableName << " is empty!" << endl;
            return;
        }

        string record[fieldCount] = {static_cast<string>(args)...};

        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) &&
                toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) &&
                !records[i].metadata.isDeleted)
            {
                cout << "Record with key " << record[0] << " already exists in table: " << tableName << endl;
                return;
            }
            else if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) &&
                     toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) &&
                     records[i].metadata.isDeleted)
            {
                records[i].recordInfo.clear();
                for (int j = 0; j < fieldCount; j++)
                {
                    if (dbinfo.schema[tableIndex].columns[j].dataType == detectType(record[j]))
                    {
                        records[i].recordInfo.push_back(record[j]);
                    }
                    else
                    {
                        cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[j].name << endl;
                        return;
                    }
                }
                records[i].metadata.isDeleted = false;
                cout << "Record restored in table: " << tableName << " with new values and record key: " << record[0] << endl;
                return;
            }
        }

        if (dbinfo.schema[tableIndex].columns.size() == fieldCount)
        {
            records.push_back(Record());
            for (int i = 0; i < fieldCount; ++i)
            {
                if (dbinfo.schema[tableIndex].columns[i].dataType == detectType(record[i]))
                {
                    records.back().recordInfo.push_back(record[i]);
                }
                else
                {
                    cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[i].name << endl;
                    return;
                }
            }
            records.back().metadata.tableName = tableName;
            cout << "Record inserted successfully into table: " << tableName << endl;
            cout << "key of this record is the first member of the record : " << record[0] << endl;
        }
        else
        {
            cout << "Record members count and columns not matched!" << endl;
        }
    }

    // I should write insertRecord function with infinite arguments
    // So if you want to compile and use main file, uncomment  it
    /*void insertRecord(vector<string> record, string tableName)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        if (record.size() == 0)
        {
            cout << "Record is empty!" << endl;
            return;
        }
        else if (dbinfo.schema[tableIndex].columns.size() == 0)
        {
            cout << "Table " << tableName << " is empty!" << endl;
            return;
        }

        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) && records[i].metadata.isDeleted == false)
            {
                cout << "Record with key " << record[0] << "already exists in table: " << tableName << endl;
                return;
            }
            else if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) && records[i].metadata.isDeleted == true)
            {
                records[i].recordInfo.clear();
                for (int j = 0; j < record.size(); j++)
                {
                    if (dbinfo.schema[tableIndex].columns[j].dataType == detectType(record[j]))
                    {
                        records[i].recordInfo.push_back(record[j]);
                    }
                    else
                    {
                        cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[j].name << endl;
                        return;
                    }
                }
                records[i].metadata.isDeleted = false;
                cout << "Record restored in table: " << tableName << " with new values and record key: " << record[0] << endl;
                return;
            }
        }
        if (dbinfo.schema[tableIndex].columns.size() == record.size())
        {
            records.push_back(Record());
            for (int i = 0; i < record.size(); i++)
            {
                if (dbinfo.schema[tableIndex].columns[i].dataType == detectType(record[i]))
                {
                    records.back().recordInfo.push_back(record[i]);
                }
                else
                {
                    cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[i].name << " should be: " << dbinfo.schema[tableIndex].columns[i].dataType << endl;
                    return;
                }
            }
            records.back().metadata.tableName = tableName;
            cout << "Record inserted successfully into table: " << tableName << endl;
            cout << "key of this record is the first member of the record : " << record[0] << endl;
        }
        else if (dbinfo.schema[tableIndex].columns.size() != record.size())
        {
            cout << "Record members count and columns not matched!";
        }
    }
    */
    void findRecords(string tableName, string columnName, string value)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }
        int columnIndex = -1;
        for (int i = 0; i < dbinfo.schema[tableIndex].columns.size(); i++)
        {
            if (toLowerCase(dbinfo.schema[tableIndex].columns[i].name) == toLowerCase(columnName))
            {
                columnIndex = i;
                break;
            }
        }
        if (columnIndex == -1)
        {
            cout << "Column not found: " << columnName << endl;
            return;
        }

        cout << "Record(s) found in table: " << tableName << " including value: " << value << " : \n";
        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[columnIndex]) == toLowerCase(value) && records[i].metadata.isDeleted == false)
            {
                cout << "| ";
                for (int j = 0; j < records[i].recordInfo.size(); j++)
                {
                    cout << records[i].recordInfo[j] << " | ";
                }
                cout << endl;
            }
        }
    }
    void updateRecord(string tableName, string columnName, string key, string newValue)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        int columnIndex = -1;
        for (int i = 0; i < dbinfo.schema[tableIndex].columns.size(); i++)
        {
            if (toLowerCase(dbinfo.schema[tableIndex].columns[i].name) == toLowerCase(columnName))
            {
                columnIndex = i;
                break;
            }
        }

        if (columnIndex == -1)
        {
            cout << "Column not found: " << columnName << endl;
            return;
        }

        if (columnIndex == 0)
        {
            for (int i = 0; i < records.size(); i++)
            {
                if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(newValue))
                {
                    cout << "You can't edit the key of this record because the new value causes duplicate keys!\n";
                    return;
                }
            }
        }

        bool founded = false;
        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) &&
                toLowerCase(records[i].recordInfo[0]) == toLowerCase(key) &&
                records[i].metadata.isDeleted == false)
            {
                if (dbinfo.schema[tableIndex].columns[columnIndex].dataType == detectType(newValue))
                {
                    records[i].recordInfo[columnIndex] = newValue;
                    cout << "Record updated successfully in table: " << tableName << endl;
                    cout << "Column: " << dbinfo.schema[tableIndex].columns[columnIndex].name << endl;
                    cout << "New value: " << newValue << endl;
                    founded = true;
                    break;
                }
                else
                {
                    cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[columnIndex].name << " should be: " << dbinfo.schema[tableIndex].columns[columnIndex].dataType << endl;
                    return;
                }
            }
        }
        if (!founded)
            cout << "Record not found in table: " << tableName << " with key: " << key << endl;
    }
    void deleteRecord(string tableName, string key)
    {
        if (dbinfo.findTable(tableName) == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        for (int i = 0; i < records.size(); i++)
        {

            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(key))
            {
                if (records[i].metadata.isDeleted == false)
                {
                    cout << "Record deleted successfully from table: " << tableName << endl;
                    records[i].metadata.isDeleted = true;
                    return;
                }
                else
                {
                    cout << "Record already deleted from table: " << tableName << endl;
                    return;
                }
            }
        }

        cout << "Record not found in table: " << tableName << endl;
    }
};
=======
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <regex>
using namespace std;

string detectType(const string &input)
{
    regex intPattern("^-?\\d+$");
    regex floatPattern("^-?\\d*\\.\\d+$");

    if (regex_match(input, intPattern))
    {
        return "int";
    }
    else if (regex_match(input, floatPattern))
    {
        return "float";
    }
    else
    {
        return "string";
    }
}

string toLowerCase(const string &str)
{
    string lowerStr = str;
    if (detectType(str) == "string")
    {
        for (char &c : lowerStr)
        {
            if (c >= 'A' && c <= 'Z')
                c = tolower(c);
        }
    }
    return lowerStr;
}

class DataBase
{
private:
    vector<Record> records;

public:
    class Dbinfo
    {
    public:
        vector<Schema> schema;
        void insertTable(string tableName, int columnCount, string columnName[], string columnDataTypes[])
        {
            if (columnCount <= 0)
            {
                cout << "Invalid column count!\n";
                return;
            }
            for (int i = 0; i < schema.size(); i++)
            {
                if (schema[i].tableName == tableName)
                {
                    cout << "Table already exists: " << tableName << endl;
                    return;
                }
            }
            schema.push_back(Schema());
            Schema &lastSchema = schema.back();
            lastSchema.tableName = tableName;
            for (int i = 0; i < columnCount; i++)
            {
                lastSchema.columns.push_back(Column());
                lastSchema.columns[i].name = columnName[i];
                lastSchema.columns[i].dataType = toLowerCase(columnDataTypes[i]);
            }
            cout << "Table created successfully: " << tableName << endl;
        }
        void deleteTable(string tableName, DataBase &db)
        {
            int tableIndex = findTable(tableName);
            if (tableIndex != -1)
            {
                for (int i = 0; i < db.records.size(); i++)
                {
                    if (db.records[i].metadata.tableName == tableName && db.records[i].metadata.isDeleted == false)
                    {
                        db.records[i].metadata.isDeleted = true;
                    }
                }
                schema.erase(schema.begin() + tableIndex);
                cout << "Table deleted successfully: " << tableName << endl;
                return;
            }
            cout << "Table not found: " << tableName << endl;
        }
        int findTable(string tableName)
        {
            for (int i = 0; i < schema.size(); i++)
            {
                if (toLowerCase(schema[i].tableName) == toLowerCase(tableName))
                {
                    return i;
                }
            }
            return -1;
        }
        void showTable(string tableName, DataBase &db)
        {
            int tableIndex = findTable(tableName);
            if (tableIndex != -1)
            {
                cout << "Table " << tableName << " :\n";
                cout << "| ";
                for (int i = 0; i < schema[tableIndex].columns.size(); i++)
                {
                    cout << schema[tableIndex].columns[i].name << " | ";
                }
                cout << endl;
                for (int i = 0; i < schema[tableIndex].columns.size(); i++)
                {
                    cout << "-------";
                }
                cout << endl;
                for (int i = 0; i < db.records.size(); i++)
                {
                    if (db.records[i].metadata.tableName == tableName && db.records[i].metadata.isDeleted == false)
                    {
                        cout << "| ";
                        for (int j = 0; j < db.records[i].recordInfo.size(); j++)
                        {
                            cout << db.records[i].recordInfo[j] << " | ";
                        }
                        cout << endl;
                    }
                }
            }
            else
            {
                cout << "Table not found: " << tableName << endl;
            }
        }
        void allTables()
        {
            if (schema.size() == 0)
            {
                cout << "No table found!" << endl;
                return;
            }
            cout << "Tables in the database:" << endl;
            for (int i = 0; i < schema.size(); i++)
            {
                cout << schema[i].tableName << " | Column count: " << schema[i].columns.size() << endl;
            }
        }
    } dbinfo;

    void insertRecord(vector<string> record, string tableName)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        if (record.size() == 0)
        {
            cout << "Record is empty!" << endl;
            return;
        }
        else if (dbinfo.schema[tableIndex].columns.size() == 0)
        {
            cout << "Table " << tableName << " is empty!" << endl;
            return;
        }

        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) && records[i].metadata.isDeleted == false)
            {
                cout << "Record with key " << record[0] << "already exists in table: " << tableName << endl;
                return;
            }
            else if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(record[0]) && records[i].metadata.isDeleted == true)
            {
                records[i].recordInfo.clear();
                for (int j = 0; j < record.size(); j++)
                {
                    if (dbinfo.schema[tableIndex].columns[j].dataType == detectType(record[j]))
                    {
                        records[i].recordInfo.push_back(record[j]);
                    }
                    else
                    {
                        cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[j].name << endl;
                        return;
                    }
                }
                records[i].metadata.isDeleted = false;
                cout << "Record restored in table: " << tableName << " with new values and record key: " << record[0] << endl;
                return;
            }
        }
        if (dbinfo.schema[tableIndex].columns.size() == record.size())
        {
            records.push_back(Record());
            for (int i = 0; i < record.size(); i++)
            {
                if (dbinfo.schema[tableIndex].columns[i].dataType == detectType(record[i]))
                {
                    records.back().recordInfo.push_back(record[i]);
                }
                else
                {
                    cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[i].name << " should be: " << dbinfo.schema[tableIndex].columns[i].dataType << endl;
                    return;
                }
            }
            records.back().metadata.tableName = tableName;
            cout << "Record inserted successfully into table: " << tableName << endl;
            cout << "key of this record is the first member of the record : " << record[0] << endl;
        }
        else if (dbinfo.schema[tableIndex].columns.size() != record.size())
        {
            cout << "Record members count and columns not matched!";
        }
    }
    void findRecords(string tableName, string columnName, string value)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }
        int columnIndex = -1;
        for (int i = 0; i < dbinfo.schema[tableIndex].columns.size(); i++)
        {
            if (toLowerCase(dbinfo.schema[tableIndex].columns[i].name) == toLowerCase(columnName))
            {
                columnIndex = i;
                break;
            }
        }
        if (columnIndex == -1)
        {
            cout << "Column not found: " << columnName << endl;
            return;
        }

        cout << "Record(s) found in table: " << tableName << " including value: " << value << " : \n";
        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[columnIndex]) == toLowerCase(value) && records[i].metadata.isDeleted == false)
            {
                cout << "| ";
                for (int j = 0; j < records[i].recordInfo.size(); j++)
                {
                    cout << records[i].recordInfo[j] << " | ";
                }
                cout << endl;
            }
        }
    }
    void updateRecord(string tableName, string columnName, string key, string newValue)
    {
        int tableIndex = dbinfo.findTable(tableName);
        if (tableIndex == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        int columnIndex = -1;
        for (int i = 0; i < dbinfo.schema[tableIndex].columns.size(); i++)
        {
            if (toLowerCase(dbinfo.schema[tableIndex].columns[i].name) == toLowerCase(columnName))
            {
                columnIndex = i;
                break;
            }
        }

        if (columnIndex == -1)
        {
            cout << "Column not found: " << columnName << endl;
            return;
        }

        if (columnIndex == 0)
        {
            for (int i = 0; i < records.size(); i++)
            {
                if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(newValue))
                {
                    cout << "You can't edit the key of this record because the new value causes duplicate keys!\n";
                    return;
                }
            }
        }

        bool founded = false;
        for (int i = 0; i < records.size(); i++)
        {
            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) &&
                toLowerCase(records[i].recordInfo[0]) == toLowerCase(key) &&
                records[i].metadata.isDeleted == false)
            {
                if (dbinfo.schema[tableIndex].columns[columnIndex].dataType == detectType(newValue))
                {
                    records[i].recordInfo[columnIndex] = newValue;
                    cout << "Record updated successfully in table: " << tableName << endl;
                    cout << "Column: " << dbinfo.schema[tableIndex].columns[columnIndex].name << endl;
                    cout << "New value: " << newValue << endl;
                    founded = true;
                    break;
                }
                else
                {
                    cout << "Data type mismatch for column: " << dbinfo.schema[tableIndex].columns[columnIndex].name << " should be: " << dbinfo.schema[tableIndex].columns[columnIndex].dataType << endl;
                    return;
                }
            }
        }
        if (!founded)
            cout << "Record not found in table: " << tableName << " with key: " << key << endl;
    }
    void deleteRecord(string tableName, string key)
    {
        if (dbinfo.findTable(tableName) == -1)
        {
            cout << "Table not found: " << tableName << endl;
            return;
        }

        for (int i = 0; i < records.size(); i++)
        {

            if (toLowerCase(records[i].metadata.tableName) == toLowerCase(tableName) && toLowerCase(records[i].recordInfo[0]) == toLowerCase(key))
            {
                if (records[i].metadata.isDeleted == false)
                {
                    cout << "Record deleted successfully from table: " << tableName << endl;
                    records[i].metadata.isDeleted = true;
                    return;
                }
                else
                {
                    cout << "Record already deleted from table: " << tableName << endl;
                    return;
                }
            }
        }

        cout << "Record not found in table: " << tableName << endl;
    }
};
>>>>>>> 6a1f9b259deeebbb77b188177d0a80768f616538
