#ifndef MOVIE_BOOK_CONFIG_H
#define MOVIE_BOOK_CONFIG_H

#include <iostream>

#include <json/json.h>
#include <json/value.h>

namespace movie_book {
class Config {
   public:
      static Config& getInstance()
      {
         static Config instance;
         return instance;
      }
      bool parse(const std::string& configPath);
      std::string getDBName();
      std::string getDBUser();
      std::string getDBPassword();
      std::string getDBPort();
      std::string getDBHost();
      uint32_t getRestControllerPort();
   private:
      Config(){};
      ~Config(){};
      Json::Reader _reader;
      Json::Value _rootNode;
};
}

#endif
