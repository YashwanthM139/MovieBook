#ifndef MOVIE_BOOK_DATABASE_POOL_H
#define MOVIE_BOOK_DATABASE_POOL_H

#include <string>
#include <iostream>
#include <memory>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

using namespace soci;

namespace movie_book {
   class DBPool
   {
      public:
         DBPool(int size):_poolSize(size){}
         ~DBPool(){}
         bool createDBPool();
         bool initializePool(const std::string& dbname, const std::string& connectString);
         bool releaseConnection(int pos);
         class DBConnection
         {
            public:
               DBConnection(int position, session& s): pos(position),conn(s){}
               ~DBConnection() {}
               session& getSession(){ return conn; }
               void releaseSession(std::shared_ptr<DBPool> connPool){ connPool->releaseConnection(pos); }
            private:
               int pos {-1};
               session& conn;
         };
         std::unique_ptr<DBPool::DBConnection> getConnection();

      private:
         std::unique_ptr<connection_pool> _connPool {nullptr};
         int _poolSize {1};
   };

}
#endif
