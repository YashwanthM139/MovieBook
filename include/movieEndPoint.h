#ifndef _CRUD_IMPLEMENTATION_H
#define _CRUD_IMPLEMENTATION_H

#include <algorithm>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

using namespace Pistache;

namespace movie_book
{
   class MovieEndPoint
   {
      public:
         explicit MovieEndPoint(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
         { }

         void init(size_t thr = 2)
         {
            auto opts = Http::Endpoint::options()
               .threads(static_cast<int>(thr));
            httpEndpoint->init(opts);
            setupRoutes();
         }

         void start()
         {
            httpEndpoint->setHandler(router.handler());
            httpEndpoint->serve();
         }
	 void doRegisterUser(const Rest::Request& request, Http::ResponseWriter response);
	 void doLoginUser(const Rest::Request& request, Http::ResponseWriter response);
	 void doListMovies(const Rest::Request& request, Http::ResponseWriter response);
         void doRateMovie(const Rest::Request& request, Http::ResponseWriter response);
	 void doListTheatre(const Rest::Request& request, Http::ResponseWriter response);
	 void doListTimeSlot(const Rest::Request& request, Http::ResponseWriter response);
	 void doBookMovie(const Rest::Request& request, Http::ResponseWriter response);

      private:
         void setupRoutes()
         {
            using namespace Rest;

            Routes::Post(router, "/user/register", Routes::bind(&MovieEndPoint::doRegisterUser, this));
            Routes::Post(router, "/user/login", Routes::bind(&MovieEndPoint::doLoginUser, this));
            Routes::Get(router, "/movie/listMovies", Routes::bind(&MovieEndPoint::doListMovies, this));
            Routes::Get(router, "/movie/rateMovie/:movieId/:rate", Routes::bind(&MovieEndPoint::doRateMovie, this));
            Routes::Get(router, "/movie/listTheatre/:movieId", Routes::bind(&MovieEndPoint::doListTheatre, this));
            Routes::Get(router, "/movie/listTimeSlot/:movieId/:theatreId", Routes::bind(&MovieEndPoint::doListTimeSlot, this));
            Routes::Post(router, "/movie/bookMovie/", Routes::bind(&MovieEndPoint::doBookMovie, this));
         }

         std::shared_ptr<Http::Endpoint> httpEndpoint;
         Rest::Router router;
   };
}
#endif
