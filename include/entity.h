#ifndef MOVIE_BOOK_ENTITY_H
#define MOVIE_BOOK_ENTITY_H

#include <iostream>
#include <string>

namespace movie_book {

   typedef struct UserDetails{
      std::string username;
      std::string name;
      std::string email;
      std::string phone;
      std::string password;
   } UserDetails;

   typedef struct LoginDetails{
      std::string username;
      std::string password;
   } LoginDetails;

   typedef struct MovieDetails{
      int id;
      std::string name;
      std::string language;
      int durationInMinutes;
      int rating;
      int numberofRatings;
   }MovieDetails;

   typedef struct TheatreDetails{
      int theatreId;
      std::string name;
      std::string screen;
      int capacity;
      std::string location;
   }TheatreDetails;

   typedef struct TimeSlotDetails{
      int movieTimeId;
      int movieId;
      int theatreId;
      std::string timing;
      int availableSeat;
      int bookedSeat;
      std::string date;
   }TimeSlotDetails;

   typedef struct BookingDetailsRequest{
      int movie_time_id;
      std::string username;     
      int numberOfSeats;
      std::string date;
   }BookingDetailsRequest;

}

#endif
