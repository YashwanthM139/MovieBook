#include <fstream>

#include "config.h"

namespace movie_book {

   bool Config::parse(const std::string& configPath)
   {
      std::ifstream jsonDoc(configPath, std::ifstream::binary);
      bool isParseSuccess = _reader.parse(jsonDoc, _rootNode);
      jsonDoc.close();
      return isParseSuccess;
   }

   std::string Config::getDBName()
   {
      return _rootNode["DBName"].asString();
   }

   std::string Config::getDBUser()
   {
      return _rootNode["DBUser"].asString();
   }

   std::string Config::getDBPassword()
   {
      return _rootNode["DBPassword"].asString();
   }

   std::string Config::getDBPort()
   {
      return _rootNode["DBPort"].asString();
   }

   std::string Config::getDBHost()
   {
      return _rootNode["DBHost"].asString();
   }

   uint32_t Config::getRestControllerPort()
   {
      return atoi(_rootNode["RestControllerPort"].asString().c_str());
   }
}
