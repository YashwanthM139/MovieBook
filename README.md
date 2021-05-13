**Introduction:**
A small demonstration to build restful application in c++ to perform CRUD operations on movie booking database.
A sample database is created. And restful api's are provided to perform ticket booking.
Built application is present in app directory.
Application is built with meson build system on a linux x86_64 [ubuntu 18.04] system.
Required libraries are placed in libs folder.
C++17 is required for compilation as a dependency for some of the features and libraries.

**Following libraries are used:** 
SOCI- for db connection management
mysqlclient - for interaction with mysql database
Pistache - for rest end points creation 
Jsoncpp - for Json based operations


**Rest endpoints:**
user/register
user/login
movie/listMovies
movie/rateMovie
movie/listTheatre
movie/listTimeSlot
movie/bookMovie

**Flows Considered:**
Movie -> Theatre -> time slot -> booking
Movie -> rate movie
