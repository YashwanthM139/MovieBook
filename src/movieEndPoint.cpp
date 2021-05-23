#include <algorithm>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "movieEndPoint.h"
#include "movieService.h"

using namespace Pistache;

namespace movie_book {

   bool MovieEndPoint::checkContentTypeJson(const Rest::Request& request, Http::ResponseWriter& response)
   {
         auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
         if (content_type != nullptr)
         {
            if (content_type->mime() == MIME(Application, Json))
            {
               std::cout << " content_type is application/json" << std::endl;
               return true;
            }
            else
            {
               std::cout << " Invalid content_type: "<<content_type->mime().toString()<< std::endl;
               response.send(Http::Code::Method_Not_Allowed);
            }
         }
         else
         {
            response.send(Http::Code::Method_Not_Allowed);
         }
         return false;
   }

   void MovieEndPoint::doWelcome(const Rest::Request& request, Http::ResponseWriter response)
   {
      response.send(Http::Code::Ok, "Welcome to Movie Book");
   }

   void MovieEndPoint::doRegisterUser(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doRegisterUser:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().registerUser(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doLoginUser(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doLoginUser:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().loginUser(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doResetPassword(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doResetPassword"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().resetPassword(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteUser(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteUser"<<std::endl;
         auto username = request.param(":username").as<std::string>();
         if(MovieService::getInstance().deleteUser(username))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doListMovies(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doListMovies"<<std::endl;
         std::string movieList;
         if(MovieService::getInstance().listMovies(movieList))
         {
            response.send(Http::Code::Ok, movieList.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doListMoviesByLanguage(const Rest::Request& request, Http::ResponseWriter response)
   {
      try
      {
         std::cout<<"doListMovies"<<std::endl;
         auto language = request.param(":language").as<std::string>();
         std::string movieList;
         if(MovieService::getInstance().listMovies(movieList, language))
         {
            response.send(Http::Code::Ok, movieList.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doRateMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doRateMovie:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().rateMovie(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doListTheatre(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doListTheatre:"<<std::endl;
         std::string theatreList; 
         if(MovieService::getInstance().listTheatre(request.body(), theatreList))
         {
            response.send(Http::Code::Ok, theatreList.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doListTimeSlot(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doListTimeSlot"<<std::endl;
         auto movieId = request.param(":movie_id").as<int>();
         std::cout<<"MovieId:"<<movieId<<std::endl;
         auto theatreId = request.param(":theatre_id").as<int>();
         std::cout<<"theatreId:"<<theatreId<<std::endl;
         auto date = request.param(":date").as<std::string>();
         std::cout<<"date:"<<date<<std::endl;
         std::string timeSlotList;
         if(MovieService::getInstance().listTimeSlot(movieId, theatreId, date, timeSlotList))
         {
            response.send(Http::Code::Ok, timeSlotList.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doBookMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doBookMovie:\nRequest Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().bookMovie(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doAddMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doAddMovie:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().addMovie(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doGetMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doGetMovie:"<<std::endl;
         std::string movieDetails;
         if(MovieService::getInstance().getMovie(request.body(), movieDetails))
         {
            response.send(Http::Code::Ok, movieDetails.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doUpdateMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         auto movieId = request.param(":movie_id").as<int>();
         std::cout<<"doUpdateMovie:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().updateMovie(movieId, request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteMovie:"<<std::endl;
         auto movieId = request.param(":movie_id").as<int>();
         if(MovieService::getInstance().deleteMovie(movieId))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doAddScreen(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doAddScreen:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().addScreen(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doGetScreen(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doGetScreen:"<<std::endl;
         std::string theatreDetails;
         if(MovieService::getInstance().getScreen(request.body(), theatreDetails))
         {
            response.send(Http::Code::Ok, theatreDetails.c_str());
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doUpdateScreen(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         auto theatreId = request.param(":theatre_id").as<int>();
         std::cout<<"doUpdateScreen:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().updateScreen(theatreId, request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteScreen(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteScreen:"<<std::endl;
         auto theatreId = request.param(":theatre_id").as<int>();
         if(MovieService::getInstance().deleteScreen(theatreId))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doAddCast(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         if(!checkContentTypeJson(request, response))
         {
            return;
         }
         std::cout<<"doAddCast:"<<std::endl;
         std::cout<<"Request Body: "<<request.body()<<std::endl;
         if(MovieService::getInstance().addCast(request.body()))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteCastByMovie(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteCastByMovie:"<<std::endl;
         auto movieId = request.param(":movie_id").as<int>();
         if(MovieService::getInstance().deleteCastByMovie(movieId))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteCastByTheatre(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteCastByTheatre:"<<std::endl;
         auto theatreId = request.param(":theatre_id").as<int>();
         if(MovieService::getInstance().deleteCastByTheatre(theatreId))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteCastByMovieAndTheatre(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteCastByMovie:"<<std::endl;
         auto movieId = request.param(":movie_id").as<int>();
         auto theatreId = request.param(":theatre_id").as<int>();
         if(MovieService::getInstance().deleteCastByMovieAndTheatre(movieId, theatreId))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

   void MovieEndPoint::doDeleteCastByDateInTheatre(const Rest::Request& request, Http::ResponseWriter response)
   {
      try {
         std::cout<<"doDeleteCastByMovie:"<<std::endl;
         auto movieId = request.param(":movie_id").as<int>();
         auto theatreId = request.param(":theatre_id").as<int>();
         auto date = request.param(":date").as<std::string>();
         if(MovieService::getInstance().deleteCastByDateInTheatre(movieId, theatreId, date))
         {
            response.send(Http::Code::Ok);
         }
         else
         {
            response.send(Http::Code::Bad_Request);
         }
      }
      catch(std::exception& e)
      {
         std::cout<<"Exception:"<<e.what()<<std::endl;
      }
      catch(...)
      {
         std::cout<<"Exception Occurred"<<std::endl;
      }
   }

}
