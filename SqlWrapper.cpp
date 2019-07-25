#include "SqlWrapper.h"

SqlWrapper::SqlWrapper(){
     mysql_init(&_mysql);
}
    
    
 void SqlWrapper::Connect(std::string host,  std::string user, std::string password ,std::string  dataBase){
     mysql_real_connect(&_mysql,  host.c_str(), user.c_str(), password.c_str(),  dataBase.c_str(), 0, NULL, 0);
     mysql_select_db(&_mysql, dataBase.c_str());
 }
 
 std::string  SqlWrapper::SendQuerry(std::string querry){
     mysql_query(&_mysql, querry.c_str());
     MYSQL_RES * mRes = mysql_store_result(&_mysql);
     MYSQL_ROW  mRow;
     
     
     std::string result = " ";
     int cnt = mysql_field_count(&_mysql) ;
     if (cnt > 0){
    while ((mRow = mysql_fetch_row(mRes)) != NULL)
    {
        for (unsigned int i =0; i < mysql_num_fields(mRes); i++){
            std::string data = (mRow[i]  == NULL ? "0" : mRow[i] );
            result += data +  std::string(" | ");
        }
        result +=  "\n";
    }
     }
    return result;
 }
 