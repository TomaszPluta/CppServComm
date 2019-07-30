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
            std::string res;
    if (tokens[0] == "SELECT") {
        std::vector<std::string> columnsQuerry;
        int i =1;
        while ((tokens[i] != "FROM") && (i<tokens.size())) {
            std::string column = tokens[i];
            columnsQuerry.push_back(column);
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
            std::vector<std::string>columnsFile;
            std::getline(tableFile, header) ;
            std::cout<<"header: "<<header<<std::endl;

            std::string column;
            std::stringstream headerStream(header);
            while(getline(headerStream, column, ',')) {
                columnsFile.push_back((column));
            }
            
           std::cout<<"columns: "<<std::endl;
            for (auto i : columnsFile) {
                std::cout<< "- "<<i <<std::endl;
            }
             std::cout<<std::endl;
            
            
            std::vector<int> clmnsNb;
            for (int i =0 ; i < columnsQuerry.size(); i++)
            {
                  for (int j =0 ;j < columnsFile.size(); j++){
                    if (columnsFile[j] == columnsQuerry[i]) {
                        clmnsNb.push_back(j);
                    }
                  }
            }
    
            
            std::string line;
            while((!tableFile.eof()  && getline(tableFile, line)))
            {
                int i = 0;
                std::stringstream ss(line);
                std::string field;
                while(getline(ss, field, ',')) {
                    if ( (std::find(clmnsNb.begin(), clmnsNb.end(), i) !=clmnsNb.end())) {
                        res += field + " ";
                    }
                     i++;
                }
                res +=  "\n";
            }
          
          

            std::cout<<"result:"<<std::endl<<res<<std::endl;
            
            
                     std::cout<<std::endl;
            
            }
                     return res;
            }
}