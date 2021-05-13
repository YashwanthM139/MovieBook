#include "movieService.h"
#include <json/json.h>
#include <json/value.h>
#include <fstream>
#include <sstream>
#include "entity.h"

namespace movie_book {

   /*
content_type: application/json
expected_json:
{
username:'',
name:'',
email:'',
phone:'',
password:''
}
    */

bool MovieService::registerUser(std::string userDetails)
{
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
         std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
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
   return false;
}

/*
content_type: application/json
expected_json:
{
username:'',
password:''
}
 */
bool MovieService::loginUser(std::string loginDetails)
{
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
         std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
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
   return false;
}


bool MovieService::listMovies(std::string& movieList)
{
   try
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
      if(dbConn != nullptr)
      {
         Json::Value rootNode;
         session& sess = dbConn->getSession();
         soci::rowset<row> rs = (sess.prepare << "select * from movie");
         for (auto it = rs.begin(); it != rs.end(); ++it) {
            const row& r = *it;
            Json::Value movie;
            movie["name"]=r.get<std::string>(1);
            movie["language"]=r.get<std::string>(2);
            movie["duration"]=r.get<int>(3);
            movie["rating"]=r.get<float>(4);
            movie["numberofRatings"]=r.get<int>(5);
            rootNode[std::to_string(r.get<int>(0))]=movie;
         }
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
   return false;
}

bool MovieService::rateMovie(int movieId, int rate)
{
   try
   {
      Json::Reader reader;
      Json::Value rootNode;
      std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
      if(dbConn != nullptr)
      {
         Json::Value rootNode;
         session& sess = dbConn->getSession();
         sess<<"UPDATE movie set rating=((rating*numberOfRatings)+:rate)/(numberOfRatings+1) where id=:movieId", use(rate), use(movieId);
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
   return false;
}

bool MovieService::listTheatre(int movieId, std::string& theatreList)
{
   try
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
      if(dbConn != nullptr)
      {
         Json::Value rootNode;
         session& sess = dbConn->getSession();
         soci::rowset<row> rs = (sess.prepare << "select * from theatre where id in (select theatre_id from movie_time where movie_id="+std::to_string(movieId)+")");
         for (auto it = rs.begin(); it != rs.end(); ++it) {
            const row& r = *it;
            Json::Value theatre;
            theatre["name"] = r.get<std::string>(1);
            theatre["screen"] = r.get<std::string>(2);
            theatre["capacity"] = r.get<int>(3);
            theatre["location"] = r.get<std::string>(4);
            rootNode[std::to_string(r.get<int>(0))] = theatre;
         }
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
   return false;
}

bool MovieService::listTimeSlot(int movieId, int theatreId, std::string& timeSlotList)
{
   try
   {
      std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
      if(dbConn != nullptr)
      {
         Json::Value rootNode;
         session& sess = dbConn->getSession();
         soci::rowset<row> rs = (sess.prepare << "select * from movie_time where movie_id="+std::to_string(movieId)+" and theatre_id="+std::to_string(theatreId));
         for (auto it = rs.begin(); it != rs.end(); ++it) {
            const row& r = *it;
            Json::Value timeSlot;
            timeSlot["movie_id"] = r.get<int>(1);
            timeSlot["theatre_id"] = r.get<int>(2);
            timeSlot["timing"] = r.get<std::string>(3);
            timeSlot["date"] = r.get<std::string>(4);
            timeSlot["available_seat"] = r.get<int>(5);
            timeSlot["booked_seat"] = r.get<int>(6);
            rootNode[std::to_string(r.get<int>(0))] = timeSlot;
         }
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
   return false;
}

/*
content_type: application/json
expected_json:
{
username:'',
number_of_seats:'',
date:'',
movie_time_id:''
}
 */
bool MovieService::bookMovie(std::string body)
{
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

      std::unique_ptr<DBPool::DBConnection> dbConn = dbpool->getConnection();
      if(dbConn != nullptr)
      {
         Json::Value rootNode;
         session& sess = dbConn->getSession();
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
   return false;
}
}
