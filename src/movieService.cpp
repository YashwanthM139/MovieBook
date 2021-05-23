#include "movieService.h"
#include <json/json.h>
#include <json/value.h>
#include <fstream>
#include <sstream>
#include "entity.h"

namespace movie_book {

   bool MovieService::registerUser(std::string userDetails)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         UserDetails user;
         bool isParseSuccess = reader.parse(userDetails, rootNode);
         if(isParseSuccess)
         {
            user.username.assign(rootNode["username"].asString());
            user.name.assign(rootNode["name"].asString());
            user.email.assign(rootNode["email"].asString());
            user.phone.assign(rootNode["phone"].asString());
            //should come encrypted
            user.password.assign(rootNode["password"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"INSERT INTO user(username, name, email, phone, password) VALUES(:username, :name, :email, :phone, :password)",
                  use(user.username, "username"), use(user.name, "name"), use(user.email, "email"), use(user.phone, "phone"), use(user.password, "password");
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::loginUser(std::string loginDetails)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         LoginDetails login;
         bool isParseSuccess = reader.parse(loginDetails, rootNode);
         if(isParseSuccess)
         {
            login.username.assign(rootNode["username"].asString());
            //should come encrypted
            login.password.assign(rootNode["password"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               std::string name;
               indicator ind;
               session& sess = dbConn->getSession();
               sess<<"select name from user where username=:username and password=:password", use(login.username), use(login.password), soci::into(name, ind);
               bool result = false;
               if(sess.got_data() &&  (ind == i_ok))
               {
                  result = true;
               }
               dbConn->releaseSession(dbpool);
               return result;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::resetPassword(std::string userCredentials)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         std::string username;
         std::string password;
         bool isParseSuccess = reader.parse(userCredentials, rootNode);
         if(isParseSuccess)
         {
            username.assign(rootNode["username"].asString());
            //should come encrypted
            password.assign(rootNode["password"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"UPDATE user set password=:password where username=:username", use(password), use(username);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteUser(const std::string& username)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from user where username=:username", use(username);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::listMovies(std::string& movieList, const std::string& language)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            Json::Value rootNode;
            session& sess = dbConn->getSession();
            std::string query;
            if(language.empty())
            {
               query.assign("select * from movie where id in (select distinct movie_id from movie_time)");
            }
            else
            {
               query.assign("select * from movie where id in (select distinct movie_id from movie_time) and language ='"+language+"'");
            }
            soci::rowset<row> rs = (sess.prepare << query.c_str());
            int i = 0;
            for (auto it = rs.begin(); it != rs.end(); ++it, ++i) {
               const row& r = *it;
               Json::Value movie;
               movie["movie_id"]=r.get<int>(0);
               movie["name"]=r.get<std::string>(1);
               movie["language"]=r.get<std::string>(2);
               movie["duration"]=r.get<int>(3);
               movie["rating"]=r.get<double>(4);
               movie["number_of_ratings"]=r.get<int>(5);
               rootNode[std::to_string(i)]=movie;
            }
            rootNode["size"]=i;
            Json::FastWriter fastWriter;
            movieList.assign(fastWriter.write(rootNode));
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::rateMovie(const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         std::string movieId;
         std::string username;
         std::string rate;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            username.assign(rootNode["username"].asString());
            movieId.assign(rootNode["movie_id"].asString());
            rate.assign(rootNode["rating"].asString());
         }
         else
         {
            return false;
         }
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            session& sess = dbConn->getSession();
            soci::rowset<row> rs = (sess.prepare << "select * from user_rating where username='"+username+"' and movie_id="+movieId);
            if(rs.begin() != rs.end())
            {
               return false;
            }
            sess<<"UPDATE movie set rating=((rating*numberOfRatings)+:rate)/(numberOfRatings+1), numberOfRatings=numberOfRatings+1 where id=:movieId", use(rate), use(movieId);
            sess<<"insert into user_rating (username, movie_id) values(:username, :movieId)", use(username), use(movieId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch(soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::listTheatre(const std::string& request, std::string& theatreList)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value requestRoot;
         std::string movieName;
         std::string language;
         std::string location;
         bool isParseSuccess = reader.parse(request, requestRoot);
         if(isParseSuccess)
         {
            movieName.assign(requestRoot["movie_name"].asString());
            language.assign(requestRoot["language"].asString());
            location.assign(requestRoot["location"].asString());
         }
         else
         {
            return false;
         }
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            Json::Value rootNode;
            session& sess = dbConn->getSession();
            std::string query;
            if(location.empty())
            {
               query.assign("select * from theatre where id in (select theatre_id from movie_time where movie_id in (select movie_id from movie where name='"+movieName+"' and language='"+language+"'))");
            }
            else
            {
               query.assign("select * from theatre where id in (select theatre_id from movie_time where movie_id in (select movie_id from movie where name='"+movieName+"' and language='"+language+"')) and location='"+location+"'");
            }
            soci::rowset<row> rs = (sess.prepare << query.c_str());
            int i = 0;
            for (auto it = rs.begin(); it != rs.end(); ++it, ++i) {
               const row& r = *it;
               Json::Value theatre;
               theatre["theatre_id"] = r.get<int>(0);
               theatre["name"] = r.get<std::string>(1);
               theatre["screen"] = r.get<std::string>(2);
               theatre["capacity"] = r.get<int>(3);
               theatre["location"] = r.get<std::string>(4);
               rootNode[std::to_string(i)] = theatre;
            }
            rootNode["size"]=i;
            Json::FastWriter fastWriter;
            theatreList.assign(fastWriter.write(rootNode));
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::listTimeSlot(int movieId, int theatreId, const std::string& date, std::string& timeSlotList)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            Json::Value rootNode;
            session& sess = dbConn->getSession();
            soci::rowset<row> rs = (sess.prepare << "select * from movie_time where movie_id="+std::to_string(movieId)+" and theatre_id="+std::to_string(theatreId)+" and date='"+date+"'");
            int i = 0;
            for (auto it = rs.begin(); it != rs.end(); ++it, ++i) {
               const row& r = *it;
               Json::Value timeSlot;
               timeSlot["movie_time_id"] = r.get<int>(0);
               timeSlot["movie_id"] = r.get<int>(1);
               timeSlot["theatre_id"] = r.get<int>(2);
               timeSlot["timing"] = r.get<std::string>(3);
               timeSlot["date"] = r.get<std::string>(4);
               timeSlot["availableSeat"] = r.get<int>(5);
               timeSlot["bookedSeat"] = r.get<int>(6);
               rootNode[std::to_string(i)] = timeSlot;
            }
            rootNode["size"]=i;
            Json::FastWriter fastWriter;
            timeSlotList.assign(fastWriter.write(rootNode));
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::bookMovie(const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         BookingDetailsRequest booking;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            booking.username.assign(rootNode["username"].asString());
            booking.movie_time_id = atoi(rootNode["movie_time_id"].asString().c_str());
            booking.numberOfSeats = atoi(rootNode["number_of_seats"].asString().c_str());
            booking.date.assign(rootNode["date"].asString());
         }

         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            Json::Value rootNode;
            session& sess = dbConn->getSession();
            soci::rowset<row> rs = (sess.prepare << "select available_seat from movie_time where id="+std::to_string(booking.movie_time_id));
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                  const row& r = *it;
                  if(r.get<int>(0) < booking.numberOfSeats)
                  {
                     dbConn->releaseSession(dbpool);
                     return false;
                  }
            }
            sess<<"UPDATE movie_time set available_seat=available_seat-:rs, booked_seat=booked_seat+:res where id=:id", use(booking.numberOfSeats), use(booking.numberOfSeats), use(booking.movie_time_id);
            sess<<"INSERT INTO booking (username, movie_time_id, number_of_seats_booked, date) VALUES(:un,:mtid,:nsb,:date)",
               use(booking.username), use(booking.movie_time_id), use(booking.numberOfSeats), use(booking.date);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      dbConn->releaseSession(dbpool);
      return false;
   }

   bool MovieService::addMovie(const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         MovieDetails movie;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            movie.name.assign(rootNode["movie_name"].asString());
            movie.language.assign(rootNode["language"].asString());
            movie.durationInMinutes=rootNode["durationInMinutes"].asUInt();
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"INSERT INTO movie(name, language, durationInMinutes) VALUES(:name, :language, :durationInMinutes)",
                  use(movie.name), use(movie.language), use(movie.durationInMinutes);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::getMovie(const std::string& request, std::string& movieDetails)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value requestRoot;
         std::string movieName;
         std::string language;
         bool isParseSuccess = reader.parse(request, requestRoot);
         if(isParseSuccess)
         {
            movieName.assign(requestRoot["movie_name"].asString());
            language.assign(requestRoot["language"].asString());
         }
         else
         {
            return false;
         }
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            session& sess = dbConn->getSession();
            std::string query;
            query.assign("select id, name, language, durationInMinutes, rating, numberofRatings from movie where name='"+movieName+"' and language ='"+language+"'");
            std::cout<<query<<std::endl;
            Json::Value movie;
            soci::rowset<row> rs = (sess.prepare << query.c_str());
            for (auto it = rs.begin(); it != rs.end(); ++it) {
               const row& r = *it;
               movie["movie_id"]=r.get<int>(0);
               movie["name"]=r.get<std::string>(1);
               movie["language"]=r.get<std::string>(2);
               movie["duration"]=r.get<int>(3);
               movie["rating"]=r.get<double>(4);
               movie["number_of_ratings"]=r.get<int>(5);
            }
            Json::FastWriter fastWriter;
            movieDetails.assign(fastWriter.write(movie));
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::updateMovie(int movieId, const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         MovieDetails movie;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            movie.name.assign(rootNode["movie_name"].asString());
            movie.language.assign(rootNode["language"].asString());
            movie.durationInMinutes=rootNode["durationInMinutes"].asUInt();
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"update movie set name=:name, language=:language, durationInMinutes=:durationInMinutes where id=:id",
                  use(movie.name), use(movie.language), use(movie.durationInMinutes), use(movieId);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteMovie(int movieId)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from movie where id=:id", use(movieId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::addScreen(const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         TheatreDetails theatre;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            theatre.name.assign(rootNode["theatre_name"].asString());
            theatre.screen.assign(rootNode["screen"].asString());
            theatre.capacity=rootNode["capacity"].asUInt();
            theatre.location.assign(rootNode["location"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"INSERT INTO theatre(name, screen, capacity, location) VALUES(:name, :screen, :capacity, :location)",
                  use(theatre.name), use(theatre.screen), use(theatre.capacity), use(theatre.location);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::getScreen(const std::string& request, std::string& screenDetails)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value requestRoot;
         std::string theatreName;
         std::string screen;
         bool isParseSuccess = reader.parse(request, requestRoot);
         if(isParseSuccess)
         {
            theatreName.assign(requestRoot["theatre_name"].asString());
            screen.assign(requestRoot["screen"].asString());
         }
         else
         {
            return false;
         }
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            Json::Value rootNode;
            session& sess = dbConn->getSession();
            std::string query;
            query.assign("select * from theatre where name='"+theatreName+"' and screen ='"+screen+"'");
            std::cout<<query<<std::endl;
            soci::rowset<row> rs = (sess.prepare << query.c_str());
            for (auto it = rs.begin(); it != rs.end(); ++it) {
               const row& r = *it;
               rootNode["theatre_id"]=r.get<int>(0);
               rootNode["name"]=r.get<std::string>(1);
               rootNode["screen"]=r.get<std::string>(2);
               rootNode["capacity"]=r.get<int>(3);
               rootNode["location"]=r.get<std::string>(4);
            }
            Json::FastWriter fastWriter;
            screenDetails.assign(fastWriter.write(rootNode));
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::updateScreen(int theatreId, const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         TheatreDetails theatre;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            theatre.name.assign(rootNode["theatre_name"].asString());
            theatre.screen.assign(rootNode["screen"].asString());
            theatre.capacity=rootNode["capacity"].asUInt();
            theatre.location.assign(rootNode["location"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"update theatre set name=:name, screen=:screen, capacity=:capacity, location=:location where id=:id",
                  use(theatre.name), use(theatre.screen), use(theatre.capacity), use(theatre.location), use(theatreId);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteScreen(int theatreId)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            std::cout<<"Executing delete query"<<std::endl;
            dbConn->getSession()<<"delete from theatre where id=:id", use(theatreId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::addCast(const std::string& body)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         Json::Reader reader;
         Json::Value rootNode;
         TimeSlotDetails timeSlot;
         bool isParseSuccess = reader.parse(body, rootNode);
         if(isParseSuccess)
         {
            timeSlot.movieId = rootNode["movie_id"].asUInt();
            timeSlot.theatreId = rootNode["theatre_id"].asUInt();
            timeSlot.timing.assign(rootNode["timing"].asString());
            timeSlot.availableSeat = rootNode["available_seat"].asUInt();
            timeSlot.bookedSeat = rootNode["booked_seat"].asUInt();
            timeSlot.date.assign(rootNode["date"].asString());
            dbConn = dbpool->getConnection();
            if(dbConn != nullptr)
            {
               dbConn->getSession()<<"INSERT INTO movie_time(movie_id, theatre_id, timing, available_seat, booked_seat, date) VALUES(:mid, :tid, :timing, :as, :bs, :date)",
                  use(timeSlot.movieId), use(timeSlot.theatreId), use(timeSlot.timing), use(timeSlot.availableSeat), use(timeSlot.bookedSeat), use(timeSlot.date);
               dbConn->releaseSession(dbpool);
               return true;
            }
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteCastByMovie(int movieId)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from movie_time where movie_id=:id", use(movieId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteCastByTheatre(int theatreId)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from movie_time where theatre_id=:id", use(theatreId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteCastByMovieAndTheatre(int movieId, int theatreId)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from movie_time where movie_id=:id and theatre_id=:tid", use(movieId), use(theatreId);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

   bool MovieService::deleteCastByDateInTheatre(int movieId, int theatreId, const std::string& date)
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = nullptr;
      try
      {
         dbConn = dbpool->getConnection();
         if(dbConn != nullptr)
         {
            dbConn->getSession()<<"delete from movie_time where movie_id=:id and theatre_id=:tid and date=:date", use(movieId), use(theatreId), use(date);
            dbConn->releaseSession(dbpool);
            return true;
         }
      }
      catch (soci::mysql_soci_error const & e)
      {
         std::cerr << "MySQL error: " << e.err_num_<< " " << e.what() << std::endl;
      }
      catch(std::exception &e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
      if(dbConn != nullptr)
      {
         dbConn->releaseSession(dbpool);
      }
      return false;
   }

}
