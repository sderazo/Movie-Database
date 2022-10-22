
/*=====================================================================
  Movie is the item used in our program. The unique variable in Movie
  is the imdb code, which will be used for the HashTable.
 ======================================================================*/




#ifndef MOVIE_H
#define MOVIE_H


#include <iostream>
using namespace std;
class Movie
{
private:
    string imdb; //unique
    string title; //not unique
    string director;
    string genre;
    int year;
    double rating;

public:

    //Constructors
    Movie()
    {
        imdb = "";
        title = "";
        director = "";
        genre = "";
        year = -1;
        rating = -1;
    }

    Movie(string code, string name, string direct, string type, int yr, double rate)
    {
        imdb = code;
        title = name;
        director = direct;
        genre = type;
        year = yr;
        rating = rate;
    }

    //setters
    void setImdb(string input)
    {
        imdb = input;
    }
    void setTitle(string input)
    {
        title = input;
    }
    void setDirector(string input)
    {
        director = input;
    }
    void setGenre(string input)
    {
        genre = input;
    }
    void setYear(int input)
    {
        year = input;
    }
    void setRating(double input)
    {
        rating = input;
    }

    //getters
    string getImdb() const
    {
        return imdb;
    }
    string getTitle() const
    {
        return title;
    }
    string getDirector() const
    {
        return director;
    }
    string getGenre() const
    {
        return genre;
    }
    int getYear() const
    {
        return year;
    }
    double getRating() const
    {
        return rating;
    }

    bool isEmpty() const
    {
        if(title == "") return true;
        return false;
    }

};

#endif
