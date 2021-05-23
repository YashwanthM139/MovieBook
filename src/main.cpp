#include <algorithm>
#include <thread>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "movieEndPoint.h"
#include "dbpool.h"
#include "movieService.h"
#include "config.h"

#ifndef CONFIGURATION_PATH
#define CONFIGURATION_PATH "../config/config.json"
#endif

using namespace Pistache;
using namespace soci;

int main(int argc, char* argv[])
{

   int thr = std::thread::hardware_concurrency();
   if(thr == 0)
   {
      thr = 1;
   }
   int poolSize = (thr * 2);
   if((poolSize % 10) > 0)
   {
      poolSize = ((poolSize + 10)/10 * 10);
   }
   std::shared_ptr<movie_book::DBPool> mysqlDBPool = std::make_shared<movie_book::DBPool>(poolSize);
   if(!mysqlDBPool->createDBPool())
   {
      std::cout<<"ERROR: DBPool Creation Failed"<<std::endl;
      exit(1);
   }

   if(!movie_book::Config::getInstance().parse(CONFIGURATION_PATH))
   {
      std::cout<<"ERROR: Configuration parsing failed"<<std::endl;
      exit(1);
   }
   std::string connectionString( "db="+movie_book::Config::getInstance().getDBName()+" user="+movie_book::Config::getInstance().getDBUser()+" password="+movie_book::Config::getInstance().getDBPassword()+" host="+movie_book::Config::getInstance().getDBHost()+" port="+movie_book::Config::getInstance().getDBPort() );
   mysqlDBPool->initializePool("mysql", connectionString.c_str());

   movie_book::MovieService::getInstance().setDBPool(mysqlDBPool);

   Port port(movie_book::Config::getInstance().getRestControllerPort());
   Address addr(Ipv4::any(), port);
   movie_book::MovieEndPoint movies(addr);

   movies.init(thr);
   movies.start();
   return 0;
}
