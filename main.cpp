#include <iostream>
#include "DataBase.hpp"

using namespace std;

int main()
{
    string input;
    DataBase database;
    cout << "Welcome to the Database Management System!\n";
    cout << "In this program record keys are the first member of each record!\n";
    cout << "Enter a command ('help' to help 'exit' to quit)";
    while (1)
    {
        cout << " > ";
        cin >> input;
        if (toLowerCase(input) == "exit")
        {
            break;
        }
        else if (toLowerCase(input) == "help")
        {
            cout << "Available commands:\n";
            cout << " exit - Exit the program\n";
            cout << " Tables: \n";
            cout << "   newtable - Create a new table\n";
            cout << "   deletetable - Delete a table\n";
            cout << "   showtable - Find and Show a table\n";
            cout << "   alltables - Show all tables(just by name and column count)\n";
            cout << " Records: \n";
            cout << "   newrecord - Insert a new record\n";
            cout << "   deleterecord - Delete a record\n";
            cout << "   findrecords - Find record(s) by value\n";
            cout << "   sfindrecords - Find record(s) by special column\n";
            cout << "   updaterecord - Update a record\n";
        }
        else if (toLowerCase(input) == "newtable")
        {
            string tableName;
            cout << "Enter table name: ";
            cin >> tableName;
            int columnCount;
            cout << "Enter column count: ";
            cin >> columnCount;
            string columnName[columnCount];
            DataType *columnDataTypes[columnCount];
            cout << "Enter column names one by one and click 'Enter' button after each one\n";
            for (int i = 0; i < columnCount; i++)
            {
                cout << "Enter column name: \n";
                cin.ignore();
                getline(cin, columnName[i]);
                cout << "Enter column data type ('int' for integer / 'string' for words / 'float' for floating point numbers / 'date' for date): ";
                string tempDataType;
                cin >> tempDataType;
                if (toLowerCase(tempDataType) != "int" && toLowerCase(tempDataType) != "string" && toLowerCase(tempDataType) != "float" && toLowerCase(tempDataType) != "date")
                {
                    cout << "Invalid data type! Enter this member again" << endl;
                    i--;
                }
                else if (toLowerCase(tempDataType) == "int")
                {
                    columnDataTypes[i] = new IntegerType();
                }
                else if (toLowerCase(tempDataType) == "string")
                {
                    columnDataTypes[i] = new StringType();
                }
                else if (toLowerCase(tempDataType) == "float")
                {
                    columnDataTypes[i] = new FloatType();
                }
                else if (toLowerCase(tempDataType) == "date")
                {
                    columnDataTypes[i] = new DateType();
                }
            }
            bool validColumnName = false;
            string specialColumn;
            while (!validColumnName)
            {
                cout << "Which column is special for searching?\n";
                cin >> specialColumn;
                for (string name : columnName)
                {
                    if (specialColumn == name)
                        validColumnName = true;
                }
            }
            database.dbinfo.insertTable(tableName, columnCount, columnName, columnDataTypes, specialColumn);
        }
        else if (toLowerCase(input) == "deletetable")
        {
            string tableName;
            cout << "Enter the table name: ";
            cin >> tableName;
            database.dbinfo.deleteTable(tableName, database);
        }
        else if (toLowerCase(input) == "showtable")
        {
            string tableName;
            cout << "Enter the table name: ";
            cin >> tableName;
            database.dbinfo.showTable(tableName, database);
        }
        else if (toLowerCase(input) == "alltables")
        {
            database.dbinfo.allTables();
        }
        else if (toLowerCase(input) == "newrecord")
        {
            string tableName;
            cout << "Enter the table name: ";
            cin >> tableName;
            vector<string> record;
            cout << "Enter members one by one and click 'Enter' button after each one (Enter 'done' after last one)\n";
            cin.ignore();
            while (1)
            {
                string member;
                getline(cin, member);
                if (toLowerCase(member) == "done")
                    break;
                record.push_back(member);
            }
            database.insertRecord(record, tableName);
        }
        else if (toLowerCase(input) == "deleterecord")
        {
            string tableName, key;
            cout << "Enter the table name: ";
            cin >> tableName;
            cout << "Enter the key of the record(First Member!): ";
            cin.ignore();
            getline(cin, key);
            database.deleteRecord(tableName, key);
        }
        else if (toLowerCase(input) == "findrecords")
        {
            string tableName, columnName, value;
            cout << "Enter the table name: ";
            cin >> tableName;
            cout << "Enter the column name: ";
            cin >> columnName;
            cout << "Enter the value: ";
            cin >> value;
            database.findRecords(tableName, columnName, value);
        }
        else if (toLowerCase(input) == "updaterecord")
        {
            string tableName, columnName, key, newValue;
            cout << "Enter the table name: ";
            cin >> tableName;
            cout << "Enter the column name: ";
            cin >> columnName;
            cout << "Enter the key of the record(First Member!): ";
            cin.ignore();
            getline(cin, key);
            cout << "Enter the new value: ";
            getline(cin, newValue);
            database.updateRecord(tableName, columnName, key, newValue);
        }
        else if (toLowerCase(input) == "sfindrecords")
        {
            string tableName, value;
            cout << "Enter the table name: ";
            cin >> tableName;
            cout << "Enter the value: ";
            cin >> value;
            database.specialRecordFinder(tableName, value);
        }
        else
        {
            cout << "Invalid command! Type 'help' for available commands.\n";
        }
    }

    return 0;
}
