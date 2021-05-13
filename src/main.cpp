#include <algorithm>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <movieEndPoint.h>
#include <dbpool.h>
#include <movieService.h>

using namespace Pistache;
using namespace soci;

int main(int argc, char* argv[])
{

   //TODO: Move to configuration
   int poolSize = 10;
   std::shared_ptr<movie_book::DBPool> mysqlDBPool = std::make_shared<movie_book::DBPool>(poolSize);
   if(!mysqlDBPool->createDBPool())
   {
      std::cout<<"ERROR: DBPool Creation Failed"<<std::endl;
      exit(1);
   }
   //TODO: Move to configuration
   mysqlDBPool->initializePool("mysql", "db=movie_book user=mysqluser password=mysqluser139");

   movie_book::MovieService::getInstance().setDBPool(mysqlDBPool);

   //TODO: Move to configuration
   Port port(9080);
   int thr = 2;

   Address addr(Ipv4::any(), port);

   std::cout << "Cores = " << hardware_concurrency() << std::endl;
   std::cout << "Using " << thr << " threads" << std::endl;

   movie_book::MovieEndPoint movies(addr);

   movies.init(thr);
   movies.start();
   return 0;
}
