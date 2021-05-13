#ifndef MOVIE_SERVICE_H
#define MOVIE_SERVICE_H

#include <iostream>
#include <string>
#include <memory>
#include <dbpool.h>

namespace movie_book {

   class MovieService
   {
      public:
         static MovieService& getInstance()
         {
            static MovieService instance;
            return instance;
         }
         void setDBPool(std::shared_ptr<DBPool>& pool)
         {
            dbpool = pool;
         }
         bool registerUser(std::string userDetails);
         bool loginUser(std::string loginDetails);
         bool listMovies(std::string& movieList);
         bool rateMovie(int movieId, int rate);
         bool listTheatre(int movieId, std::string& theatreList);
         bool listTimeSlot(int movieId, int theatreId, std::string& timeSlot);
         bool bookMovie(std::string body);
      private:
         MovieService(){}
         ~MovieService(){}
         std::shared_ptr<DBPool> dbpool {nullptr};
   };

}

#endif
