#include <mysql.h>
#include <string>


class SqlWrapper {
    MYSQL _mysql;
    
public:
    SqlWrapper ();
    void Connect(std::string host,  std::string user, std::string password ,std::string  dataBase);
    std::string  SendQuerry(std::string querry);
};