Please find the code part of the deliverables in rest_api.zip.

Required libraries are placed in libs folder 
and also mentioned in meson.build.
Application(crud) is present in builddir directory.
Routes of rest api to perform db operations is present movieEndPoint.h (Controller).
I’ve tested it currently using postman.

Have used following libraries:
SOCI- for db connection management 
Pistache - for rest end points
Jsoncpp - for Json based operations

Rest endpoints:
user/register
user/login
movie/listMovies
movie/rateMovie
movie/listTheatre
movie/listTimeSlot
movie/bookMovie

Flows Considered:
Movie -> Theatre -> time slot -> booking
Movie -> rateMovie
