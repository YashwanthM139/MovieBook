#include <algorithm>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include "movieEndPoint.h"
#include "movieService.h"

using namespace Pistache;

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
void MovieEndPoint::doRegisterUser(const Rest::Request& request, Http::ResponseWriter response)
{
   auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
   if (content_type != nullptr)
   {
      if (content_type->mime() == MIME(Application, Json))
      {
         std::cout << " content_type is application/json" << std::endl;
      }
      else
      {
         std::cout << " Invalid content_type: "<<content_type->mime().toString()<< std::endl;
         response.send(Http::Code::Method_Not_Allowed);
         return;
      }
   }
   else
   {
      response.send(Http::Code::Method_Not_Allowed);
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

/*
content_type: application/json
expected_json:
{
username:'',
password:''
}
 */
void MovieEndPoint::doLoginUser(const Rest::Request& request, Http::ResponseWriter response)
{
   auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
   if (content_type != nullptr)
   {
      if (content_type->mime() == MIME(Application, Json))
      {
         std::cout << " content_type is application/json" << std::endl;
      }
      else
      {
         std::cout << " Invalid content_type: "<<content_type->mime().toString()<< std::endl;
         response.send(Http::Code::Method_Not_Allowed);
         return;
      }
   }
   else
   {
      response.send(Http::Code::Method_Not_Allowed);
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


void MovieEndPoint::doListMovies(const Rest::Request& request, Http::ResponseWriter response)
{
   std::cout<<"doListMovies"<<std::endl;
   //TODO: Add Pagination
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

void MovieEndPoint::doRateMovie(const Rest::Request& request, Http::ResponseWriter response)
{
   std::cout<<"doRateMovie:"<<std::endl;
   auto movieId = request.param(":movieId").as<int>();
   auto rate = request.param(":rate").as<int>();
   std::cout<<"MovieId:"<<movieId<<std::endl;
   if(MovieService::getInstance().rateMovie(movieId, rate))
   {
      response.send(Http::Code::Ok);
   }
   else
   {
      response.send(Http::Code::Bad_Request);
   }
}

void MovieEndPoint::doListTheatre(const Rest::Request& request, Http::ResponseWriter response)
{
   std::cout<<"doListTheatre:"<<std::endl;
   auto movieId = request.param(":movieId").as<int>();
   std::cout<<"MovieId:"<<movieId<<std::endl;
   std::string theatreList; 
   if(MovieService::getInstance().listTheatre(movieId, theatreList))
   {
      response.send(Http::Code::Ok, theatreList.c_str());
   }
   else
   {
      response.send(Http::Code::Bad_Request);
   }
}

void MovieEndPoint::doListTimeSlot(const Rest::Request& request, Http::ResponseWriter response)
{
   std::cout<<"doListTimeSlot"<<std::endl;
   auto movieId = request.param(":movieId").as<int>();
   std::cout<<"MovieId:"<<movieId<<std::endl;
   auto theatreId = request.param(":theatreId").as<int>();
   std::cout<<"theatreId:"<<movieId<<std::endl;
   std::string timeSlotList;
      if(MovieService::getInstance().listTimeSlot(movieId, theatreId, timeSlotList))
      {
         response.send(Http::Code::Ok, timeSlotList.c_str());
      }
      else
      {
         response.send(Http::Code::Bad_Request);
      }
}

/*
content_type: application/json
expected_json:
{
username:'',
movieId:'',
theatreId:'',
movieTimeId:''
}
 */
void MovieEndPoint::doBookMovie(const Rest::Request& request, Http::ResponseWriter response)
{
   auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
   if (content_type != nullptr)
   {
      if (content_type->mime() == MIME(Application, Json))
      {
         std::cout << " content_type is application/json" << std::endl;
      }
      else
      {
         std::cout << " Invalid content_type: "<<content_type->mime().toString()<< std::endl;
         response.send(Http::Code::Method_Not_Allowed);
         return;
      }
   }
   else
   {
      response.send(Http::Code::Method_Not_Allowed);
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
}
