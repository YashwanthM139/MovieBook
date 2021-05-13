#include"dbpool.h"

using namespace soci;
namespace movie_book {

   bool DBPool::createDBPool()
   {
      try
      {
         _connPool = std::make_unique<connection_pool>(_poolSize);
      }
      catch (const std::exception& e) {
         std::cout << "Error: " << e.what() << std::endl; 
         return false;
      }
      return true;
   }

   bool DBPool::initializePool(const std::string& dbname, const std::string& connectString)
   {
      try
      {
         for(int i=0; i<_poolSize; ++i)
         {
            soci::session& sqlSession = _connPool->at(i);
            sqlSession.open(dbname, connectString);
            if(!sqlSession.is_connected())
            {
               std::cout<<"Pool Creation Failed"<<std::endl;
               return false;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(const std::exception& e)
      {
         std::cerr << "Error: " << e.what() << std::endl; 
         return false;
      }
      return true;
   }

   std::unique_ptr<DBPool::DBConnection> DBPool::getConnection()
   {
      std::unique_ptr<DBConnection> DBConn= nullptr;
      try
      {
         std::size_t pos = -1;
         if(_connPool->try_lease(pos, 1000))
         {
            session& s = _connPool->at(pos);
            DBConn = std::make_unique<DBPool::DBConnection>(pos, s);
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(const std::exception& e)
      {
         std::cerr << "Error: " << e.what() << std::endl; 
         return DBConn;
      }
      return DBConn;
   }

   bool DBPool::releaseConnection(int pos)
   {
      try
      {
         if((pos < 0) && (pos >= _poolSize))
         {
            return false;
         }
         else
         {
            _connPool->give_back(pos);
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(const std::exception& e)
      {
         std::cerr << "Error: " << e.what() << std::endl; 
         return false;
      }
      return true;
   }
}
