#include <SqlWrapper.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>






class FakeSqlWrapper : public SqlWrapper
{
    std::vector<std::string> tables;
public:
    FakeSqlWrapper ();
    void Connect(std::string host,  std::string user, std::string password, std::string  dataBase);
    std::string  SendQuerry(std::string querry);
};




FakeSqlWrapper::FakeSqlWrapper ()
{


}

void FakeSqlWrapper::Connect(std::string host,  std::string user, std::string password, std::string  dataBase)
{
    std::ifstream dbFile;
    dbFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        dbFile.open(dataBase.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    } catch(const std::ifstream::failure& e) {
        std::cerr << "Error: " << e.what();
    }



    std::vector<std::string> tables;
    std::string table;

    while(!dbFile.eof() &&(dbFile >> table)) {

        tables.push_back(table);
    }
    std::cout<<"database in use: " << dataBase<<std::endl;
    std::cout<<"tables: " << std::endl;
    for (auto i : tables) { //debug
        std::cout<< "- "<<i <<std::endl;
    }
    std::cout<<std::endl;

}



std::string  FakeSqlWrapper::SendQuerry(std::string querry)
{
    std::stringstream querryStream (querry);
    std::string token;
    std::vector<std::string> tokens;
    while(getline(querryStream, token, ' ')) {
        token.erase(std::remove(token.begin(), token.end(), ','), token.end());
        tokens.push_back(token);
    }

    std::cout<<"querry tokens nb: "<< tokens.size()<<"\ncontent: "<<std::endl;
    for (auto i : tokens) { //debug
        std::cout<<"- "<<i<<std::endl;
    }
    std::cout<<std::endl;

    if (tokens[0] == "SELECT") {
        std::vector<std::string> columns;
        int i =1;
        while ((tokens[i] != "FROM") && (i<tokens.size())) {
            std::string column = tokens[i];
            columns.push_back(column);
            i++;
        }
        i++; //skip keyword "from"
        std::string table = tokens[i];

        std::cout<<"table in use: "<<table<<std::endl;

//        auto it =   std::find_if(tables.begin(), tables.end(),[&](std::string tbl) {
//            std::cout<<i<<std::endl;
//            return (tables[i] == table);
//        });


        //  if (it != tables.end()) {
        if (1) {
            std::fstream tableFile;
            tableFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                tableFile.open(table,  std::fstream::in | std::fstream::out | std::fstream::app);
            } catch(const std::ifstream::failure& e) {
                std::cerr << "Error: " << e.what();
            }

            std::string header;
            std::vector<std::string>columns;
            std::getline(tableFile, header) ;
            std::cout<<"header: "<<header<<std::endl;

            std::string column;
            std::stringstream headerStream(header);
            while(getline(headerStream, column, ' ')) {
                columns.push_back((column));
            }
            
           std::cout<<"columns: "<<std::endl;
            for (auto i : columns) {
                std::cout<< "- "<<i <<std::endl;
            }



//            std::map< int, std::vector<std::string>>columnsContent;
//            columnsContent[1].push_back("ala");
            std::vector<std::string> v1;
            std::vector<std::string> v2;

            std::string s;
            while(!tableFile.eof() ){
                for (int i=0; i <columns.size(); i++){
                    tableFile>>s;
                       std::cout<<i<<" : "<<std::endl;
                      std::cout<<s<<std::endl;
                    if ( i == 1 ){
                        v1.push_back(s);
                            std::cout<<"-^^^s^^^^-"<<std::endl;
                    }
                    }
        }
        
            std::cout<<"column 1:"<<std::endl;
            for (auto i : v1){
                std::cout<<i<<std::endl;
            }
}




//                while(!tableFile.eof() &&(dbFile >> currentWord)) {
//            words.push_back(currentWord);
            //  }

            std::cout<<std::endl;

        }
    }

 

//
//
//            for (auto & c : table) {
//            c = std::tolower(c); //change to separate func, and use only fo keyword. Create separete func for keywords
//              }
