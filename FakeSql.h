#include <SqlWrapper.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

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

//    std::string table;
//     while (dbFile >> table) {
//        tables.push_back(table);
//    }
//    
//    

//

std::vector<std::string> words;
std::string currentWord;

        while(!dbFile.eof() &&(dbFile >> currentWord)) {
            words.push_back(currentWord);
        }
    for (auto i : words)  //debug
    {
        std::cout<<"tables : "<<i<<"in database: " << dataBase << std::endl;
    }


}



std::string  FakeSqlWrapper::SendQuerry(std::string querry)
{
    std::stringstream querryStream (querry);
    std::string token;
    std::vector<std::string> tokens;
    while(getline(querryStream, token, ' ')) {
        token.erase(std::remove(token.begin(), token.end(), ','), token.end());
        for (auto & c : token) {
            c = std::tolower(c);
                }
     tokens.push_back(token);
    }


            for (auto i : tokens){ //debug
                std::cout<<"querry tokens nb: "<< tokens.size()<<"content: "<<i<<std::endl;
            }
            
            
    if (tokens[0] == "select") {
        std::vector<std::string> columns;
        int i =1;
        while ((tokens[i] != "from") && (i<tokens.size())) {
            std::string column = tokens[i];
            columns.push_back(column);
            i++;
        }
        i++; //skip keyword "from"
        std::string table = tokens[i];
        
        std::cout<<"table in use: "<<table<<std::endl;
        
        auto it =   std::find_if(tables.begin(), tables.end(),[&](std::string tbl) {return (tables[i] == tbl);});
        if (it != tables.end()) {
            std::fstream tableFile;
            tableFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                tableFile.open(table,  std::fstream::in | std::fstream::out | std::fstream::app);
            } catch(const std::ifstream::failure& e) {
                std::cerr << "Error: " << e.what();
            }

            std::string line;
            std::vector<std::string>columns;
            while (std::getline(tableFile, line, ' ')) {
                columns.push_back(line);
            }

            for (auto i : columns){
                std::cout<<"selected coulmns: " << i <<std::endl;
            }


        }
    }

    if (tokens[0] == "insert") {
        std::vector<std::string> columns;
        int i =1;
        while ((tokens[i] != "into") && (i<tokens.size())) {
            std::string column = tokens[i];
            columns.push_back(column);
            i++;
        }

    }
}
