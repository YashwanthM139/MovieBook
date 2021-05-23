#ifndef MOVIE_BOOK_SERVICE_H
#define MOVIE_BOOK_SERVICE_H

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
         bool resetPassword(std::string userCredentials);
         bool deleteUser(const std::string& username);

         bool addMovie(const std::string& body);
         bool getMovie(const std::string& request, std::string& movieDetails);
         bool updateMovie(int movieId, const std::string& body);
         bool deleteMovie(int movieId);
         bool addScreen(const std::string& body);
         bool getScreen(const std::string& request, std::string& screenDetails);
         bool updateScreen(int theatreId, const std::string& body);
         bool deleteScreen(int theatreId);
         bool addCast(const std::string& body);
         bool deleteCastByMovie(int movieId);
         bool deleteCastByTheatre(int theatreId);
         bool deleteCastByMovieAndTheatre(int movieId, int theatreId);
         bool deleteCastByDateInTheatre(int movieId, int theatreId, const std::string& date);

         bool listMovies(std::string& movieList, const std::string& language="");
         bool listTheatre(const std::string& request, std::string& theatreList);
         bool listTimeSlot(int movieId, int theatreId, const std::string& date, std::string& timeSlot);
         bool bookMovie(const std::string& body);
         bool rateMovie(const std::string& body);
      private:
         MovieService(){}
         ~MovieService(){}
         std::shared_ptr<DBPool> dbpool {nullptr};
   };

}

#endif
