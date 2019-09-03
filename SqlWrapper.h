#include <mysql/mysql.h>
#include <string>

#pragma once
class SqlWrapper {
    MYSQL _mysql;
    
public:
    SqlWrapper ();
    void Connect(std::string host,  std::string user, std::string password ,std::string  dataBase);
    std::string  SendQuerry(std::string querry);
};