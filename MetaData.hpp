#include <string>

#ifndef METADATA_HPP
#define METADATA_HPP
struct MetaData
{
    string tableName;
    bool isDeleted;
    MetaData() : tableName(""), isDeleted(false) {}
    MetaData(const string &name, bool deleted) : tableName(name), isDeleted(deleted) {}
};
#endif