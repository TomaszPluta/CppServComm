#include <SqlWrapper.h>
#include <sstream>
#include <vector>
#include <algorithm>

   SqlWrapper::SqlWrapper (){
       
   }
   
    void SqlWrapper::Connect(std::string host,  std::string user, std::string password ,std::string  dataBase){
        
    }
    
    
    
    
    
    std::string  SqlWrapper::SendQuerry(std::string querry){
        std::stringstream querryStream (querry);
        std::string token;
        std::vector<std::string> tokens;
        while(getline(querryStream, token, ' ')){
            token.erase(std::remove(token.begin(), token.end(), ','), token.end());
            for (auto & c : token){
                c = std::tolower(c);
            }
            tokens.push_back(token);
        }
        
         if (tokens[0] == "select"){
            std::vector<std::string> columns;
            int i =1;
            while ((tokens[i] != "from") && (i<tokens.size())){
                std::string column = tokens[i];
                columns.push_back(column);
                i++;
            }
            
            }
         }

