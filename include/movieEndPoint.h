#ifndef MOVIE_BOOK_ENDPOINT_H
#define MOVIE_BOOK_ENDPOINT_H

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

         void init(size_t thr)
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

         bool checkContentTypeJson(const Rest::Request& request,  Http::ResponseWriter& response);
         void doWelcome(const Rest::Request& request, Http::ResponseWriter response);
	 void doRegisterUser(const Rest::Request& request, Http::ResponseWriter response);
	 void doLoginUser(const Rest::Request& request, Http::ResponseWriter response);
         void doResetPassword(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteUser(const Rest::Request& request, Http::ResponseWriter response);

	 void doListMovies(const Rest::Request& request, Http::ResponseWriter response);
	 void doListMoviesByLanguage(const Rest::Request& request, Http::ResponseWriter response);
	 void doListTheatre(const Rest::Request& request, Http::ResponseWriter response);
	 void doListTheatreByLocation(const Rest::Request& request, Http::ResponseWriter response);
	 void doListTimeSlot(const Rest::Request& request, Http::ResponseWriter response);
	 void doBookMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doRateMovie(const Rest::Request& request, Http::ResponseWriter response);

         void doAddMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doGetMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doUpdateMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doAddScreen(const Rest::Request& request, Http::ResponseWriter response);
         void doGetScreen(const Rest::Request& request, Http::ResponseWriter response);
         void doUpdateScreen(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteScreen(const Rest::Request& request, Http::ResponseWriter response);
         void doAddCast(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteCastByMovie(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteCastByTheatre(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteCastByMovieAndTheatre(const Rest::Request& request, Http::ResponseWriter response);
         void doDeleteCastByDateInTheatre(const Rest::Request& request, Http::ResponseWriter response);

      private:
         void setupRoutes()
         {
            using namespace Rest;

            Routes::Get(router, "/", Routes::bind(&MovieEndPoint::doWelcome, this));
            Routes::Post(router, "/user/register", Routes::bind(&MovieEndPoint::doRegisterUser, this));
            Routes::Post(router, "/user/login", Routes::bind(&MovieEndPoint::doLoginUser, this));
            Routes::Post(router, "/user/resetPassword", Routes::bind(&MovieEndPoint::doResetPassword, this));
            Routes::Delete(router, "/user/deleteUser/:username", Routes::bind(&MovieEndPoint::doDeleteUser, this));

            Routes::Post(router, "/movie/addMovie", Routes::bind(&MovieEndPoint::doAddMovie, this));
            Routes::Post(router, "/movie/getMovie", Routes::bind(&MovieEndPoint::doGetMovie, this));
            Routes::Put(router, "/movie/updateMovie/:movie_id", Routes::bind(&MovieEndPoint::doUpdateMovie, this));
            Routes::Delete(router, "/movie/deleteMovie/:movie_id", Routes::bind(&MovieEndPoint::doDeleteMovie, this));

            Routes::Post(router, "/screen/addScreen", Routes::bind(&MovieEndPoint::doAddScreen, this));
            Routes::Post(router, "/screen/getScreen/", Routes::bind(&MovieEndPoint::doGetScreen, this));
            Routes::Put(router, "/screen/updateScreen/:theatre_id", Routes::bind(&MovieEndPoint::doUpdateScreen, this));
            Routes::Delete(router, "/screen/deleteScreen/:theatre_id", Routes::bind(&MovieEndPoint::doDeleteScreen, this));

            Routes::Post(router, "/moviecast/add", Routes::bind(&MovieEndPoint::doAddCast, this));
            Routes::Delete(router, "/moviecast/deleteByMovieID/:movie_id/", Routes::bind(&MovieEndPoint::doDeleteCastByMovie, this));
            Routes::Delete(router, "/moviecast/deleteByTheatreID/:theatre_id/", Routes::bind(&MovieEndPoint::doDeleteCastByTheatre, this));
            Routes::Delete(router, "/moviecast/delete/:movie_id/:theatre_id/", Routes::bind(&MovieEndPoint::doDeleteCastByMovieAndTheatre, this));
            Routes::Delete(router, "/moviecast/delete/:movie_id/:theatre_id/:date", Routes::bind(&MovieEndPoint::doDeleteCastByDateInTheatre, this));

            Routes::Get(router, "/movie/listMovies", Routes::bind(&MovieEndPoint::doListMovies, this));
            Routes::Get(router, "/movie/listMovies/:language", Routes::bind(&MovieEndPoint::doListMoviesByLanguage, this));
            Routes::Post(router, "/movie/listTheatre", Routes::bind(&MovieEndPoint::doListTheatre, this));
            Routes::Get(router, "/movie/listTimeSlot/:movie_id/:theatre_id/:date", Routes::bind(&MovieEndPoint::doListTimeSlot, this));
            Routes::Post(router, "/movie/rateMovie/", Routes::bind(&MovieEndPoint::doRateMovie, this));
            Routes::Post(router, "/movie/bookMovie/", Routes::bind(&MovieEndPoint::doBookMovie, this));
         }

         std::shared_ptr<Http::Endpoint> httpEndpoint;
         Rest::Router router;
   };
}
#endif
