// Giovany Calleja
// Han Qiang
// Sabrina Diaz-Erazo
// Zohreh Ashtarilarki


/*=====================================================================
  Main is where the bulk of the action happens. Here you can find the various
  functions that work with each other in order to ensure items(Movies)
  are stored quickly and efficiently.
 ======================================================================*/


#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "HashTable.h" // Hash Table ADT
#include "BinarySearchTree.h"  // BST ADT
#include "Movie.h"

#include "StackADT.h" // for undo delete function

using namespace std;

//Main Functions
int menu();
void readFile(string fileName, BinarySearchTree<Movie*>&, HashTable<Movie*>&);
void insertMovie (BinarySearchTree<Movie*>&, HashTable<Movie*>&);
void deleteManager(BinarySearchTree<Movie*>&,HashTable<Movie*>&, Stack<Movie>&);
void searchManagerPrimary(HashTable<Movie*>&);
void searchManagerSecondary(BinarySearchTree<Movie*>&);
void displayManager(const BinarySearchTree<Movie*>&, const HashTable<Movie*>&);
void help();
void undoDelete(BinarySearchTree<Movie*>&, HashTable<Movie*>&, Stack<Movie>&);
void writeFile(HashTable<Movie*> table);


int compare(Movie *& m1, Movie *&m2);

//Validate Variables Functions
bool imdbValidate(string &imdb);
bool titleValidate(string &title);
bool validateGenre(string &genre);
bool validateDirector(string &director);




//Calculating for other functions
bool isPrime(int n);
int getPrime(int n);
int countLine(string fileName);

//Printing and Outputting
stringstream getOutData(Movie *&movie);
void calculateLongest(Movie *& movie);
void header();
void printTail();
void hDisplay (Movie *& movie);
void vDisplay (Movie *&movie);
void tDisplay (Movie *&movie);
void iDisplay (Movie *& movie, int level);


//Variables to keep track of for outputting
int longestTitle = 0;
int longestImdb = 0;
int longestYear = 0;
int longestDirector = 0;
int longestGenre = 0;

int main()
{
    cout << "This program searches for Movies in a Database." << endl;
    cout << "Each movie utilizes variables: Title, Imdb Code, Year, Director, Genre, and Rating" << endl;
    cout << "To get started, enter an input file you would like to use." << endl;


    string decision;
    int line;

    string filename;
    cout << "\nInput file name: ";
    cin >> filename;
    line = countLine(filename);

    Stack<Movie> movieStack;
    BinarySearchTree<Movie*> movieBST;
    HashTable<Movie*> *movieHashTable = new HashTable<Movie*>(line);

    readFile(filename, movieBST, *movieHashTable);


    int choice;
    choice = menu();


    while(choice != 0)
    {
        if (choice == 1) //Inserting a New Movie
        {
            insertMovie(movieBST, *movieHashTable);

        }
        if (choice == 2) //Deleting a Movie
        {

            deleteManager(movieBST,*movieHashTable, movieStack);

        }
        if (choice == 3)//Undoing a deletion
        {
            undoDelete(movieBST, *movieHashTable, movieStack);
        }
        if (choice == 4) //Searching for Movie(IMDB)
        {
            searchManagerPrimary(*movieHashTable);
        }
        if (choice == 5) //Searching for Movie(Title)
        {
            cin.clear();
            cin.ignore();
            searchManagerSecondary(movieBST);


        }
        if (choice == 6) //Displaying Movies by Name
        {

            displayManager(movieBST,*movieHashTable);
        }
        if (choice == 7) //help
        {
            help();
        }
        if (choice == 8)
        {
            writeFile(*movieHashTable);
        }
        if (choice == 9)
        {
            movieHashTable->displayStatistics();
        }
        choice = menu();
    }
}



// Count the lines of a file
// Determine the hashsize for hashtable
int countLine(string fileName)
{
    string line;
    int size = 0;
    ifstream myFile(fileName);

    while(getline(myFile, line))
    {

        size++;
    }
    myFile.close();

    size = getPrime(size * 2);
    return size;
}




//Displays the Menu and obtains User Input, will determine next step
int menu()
{
    int choice;
    cout << "\nChoose an option below" << endl;
    cout << "======================" << endl;
    cout << "1. Add Movie" << endl;
    cout << "2. Delete Movie" << endl;
    cout << "3. Undo Delete" << endl;
    cout << "4. Search for Movie(by imdb code)" << endl;
    cout << "5. Search for Movie (by Title)" << endl;
    cout << "6. Display All Movies (In Order by Title)" << endl;
    cout << "7. Help" << endl;
    cout << "8. Write to File" << endl;
    cout << "9. Display Statistics" << endl;
    cout << "0. Quit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    while((cin.fail()) || (choice < 0) || (choice > 9))
    {
        cin.clear();
        cin.ignore();
        cout << "Please input a valid choice." << endl;
        cin >> choice;
    }
    return choice;
}









// read data from an input file
// insert data to a BST and hashtable.
// call the rehash function if the loadfactor of the hashtable is larger than 75

void readFile(string fileName, BinarySearchTree<Movie*> &tree, HashTable<Movie*> &table)
{
    string line;
    ifstream myFile(fileName);


    if (!myFile)
    {
        cout << "failed to open" << endl;

        exit(0);
    }
    while(getline(myFile, line))
    {
        int year;
        double rate;
        string IMDb;
        string title;
        string director;
        string genre;

        stringstream temp(line);
        temp >> IMDb;
        temp.ignore();
        getline(temp, title, ';');
        temp >> year;
        temp >> genre;
        temp.ignore();
        getline(temp, director, ';');
        temp >> rate;


        Movie *aMovie = new Movie(IMDb, title, director, genre, year, rate);
        calculateLongest(aMovie);

        tree.insert(compare, aMovie);
        table.insertHash(aMovie);
    }


    myFile.close();

    if (table.getLoadFactor() >= 75)
    {
        int newSize = table.getSize();
        newSize = getPrime(newSize * 2);
        table.rehash(newSize);

    }
}






/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Movie Insert: Add a new movie with all it's components
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

void insertMovie(BinarySearchTree<Movie*>& tree, HashTable<Movie*>& table)
{
    cout << "\nInsert\n";
    cout << "======\n";
    char option = 'y';
    while (option != 'n' && option != 'N')
    {
        if (option == 'y' || option == 'Y')
        {
            int year;
            double rate;
            string IMDb;
            string title;
            string director;
            string genre;

            cout << "Enter IMDb code: ";
            cin >> IMDb;
            Movie *returnedMovie = new Movie;
            while (!imdbValidate(IMDb) || table.searchHash(returnedMovie, IMDb) != 0)
            {
                cout << "No Copies or improper IMDb codes." << endl;
                cout << "The proper imdb format is shown through the url.(tt0000000)" << endl;
                cout << "Enter IMDb code: ";
                cin >> IMDb;
            }
            cin.clear();
            cin.ignore();

            cout << "Enter movie name: ";
            getline(cin, title);
            while (!titleValidate(title))
            {
                cout << "Please enter a proper title." << endl;
                cout << "Enter movie name: ";
                cin.clear();
                cin.ignore();
                getline(cin, title);
            }


            cout << "Enter Year Release(1888 above): ";
            cin >> year;
            if (cin.fail())
            {
                cout << "Input invalid, restart and try again." << endl;
                exit(1);
            }
            while (year < 1888)
            {
                cin.clear();
                cin.ignore();
                cout << "Year is unrealistic (<1888), or you didn't put in a year, try again." << endl;
                cout << "Enter Year Release: ";
                cin >> year;
                if (cin.fail())
                {
                    cout << "Input invalid, restart and try again." << endl;
                    exit(1);
                }
            }


            cout << "Enter Genre of Movie: ";
            cin >> genre;
            while (!validateGenre(genre))
            {
                cout << "Please enter proper genre." << endl;
                cout << "Enter Genre of Movie: ";
                cin >> genre;
            }

            cin.ignore();
            cout << "Enter Movie Director: ";
            getline(cin, director);
            while (!validateDirector(director))
            {
                cout << "Please enter proper director." << endl;
                cout << "Enter Movie Director: ";
                getline(cin, director);
            }


            cout << "Enter Movie Rating: ";
            cin >> rate;
            while (cin.fail() || rate > 10 || rate < 1)
            {
                cout << "Please enter a realistic rating (1.0-10.0)" << endl;
                cout << "Enter Movie Rating: ";
                cin >> rate;
            }

            Movie *aMovie = new Movie(IMDb, title, director, genre, year, rate);
            calculateLongest(aMovie);

            tree.insert(compare, aMovie);
            table.insertHash(aMovie);


            if (table.getLoadFactor() >= 75)
            {
                int newSize = table.getSize();
                newSize = getPrime(newSize * 2);
                table.rehash(newSize);

            }

            cout << "\nWould you like to insert another movie?(Y/N): ";
            cin >> option;
        }
        else
        {
            cin.clear();
            cin.ignore();
            cout << "Please enter a valid input.(Y/N): ";
            cin >> option;
        }
    }
}


/*~*~*~*~*~*~*~*~*~*~*~*~
searchManagerPrimary: The purpose of this function is to search for and display
a movie in the database using the primary key, which is the IMDb of a movie.
~*~*~*~*~*~*~*~*~*~*~*~*/
void searchManagerPrimary(HashTable <Movie*> &movieHashTable)
{
    string targetCode;

    cout << "\n Search\n";
    cout <<   "=======\n";

    cout << "\nEnter an IMDb code (or Q to stop searching) : \n";
    cin >> targetCode;

    while (targetCode != "Q")
    {

        Movie *found = new Movie;
        Movie *key = new Movie;
        key->setImdb(targetCode);


        if ((movieHashTable.searchHash(found, targetCode)) != 0)
        {
            cout << "Imdb found:" << endl;
            vDisplay(found);
            cout << "Enter an IMDb code or Q to quit: ";
            cin >> targetCode;
        }
        else
        {
            cout << "Imdb Code:" << targetCode << " not found!" << endl;
            cout << "Enter an IMDb code or Q to quit: ";
            cin >> targetCode;
        }
    }
}




/*~*~*~*~*~*~*~*~*~*~*~*~
searchManagerSecondary: The purpose of this function is to search for and display all
movie matches in the database using the secondary key, which is the title of the movie.
~*~*~*~*~*~*~*~*~*~*~*~*/
void searchManagerSecondary( BinarySearchTree<Movie*>&tree)
{
    string targetName;
    Movie *aMovie = new Movie;

    cout << "\n Secondary Search\n";
    cout <<   "=======\n";

    cout << "\nEnter a movie title (or Q to stop searching) : \n";
    Movie *target = new Movie;
    getline(cin, targetName);
    string Q = "Q";

    while (targetName.compare(Q) != 0)
    {

        target->setTitle(targetName);

        if (tree.search(compare, target, aMovie))
        {
            cout << "Movie Found:" << endl;
            vDisplay(aMovie);
        }
        else
        {
            cout << "Movie \"" << targetName << "\" not found!" << endl;
        }
        cout << "Enter a movie title or Q to quit: ";
        getline(cin, targetName);
    }
}


/*~*~*~*~*~*~*~*~*~*~*~*~
displayManager: The purpose of this function is to display all movies from the
database sorted by the secondary key, which is the title of the movie.
~*~*~*~*~*~*~*~*~*~*~*~*/
void displayManager(const BinarySearchTree <Movie*>&tree, const HashTable<Movie*> &hash)
{
    int option;
    cout << "\nDisplay Options" << endl;
    cout << "===============" << endl;
    cout << "1.Display in Title Order." << endl;
    cin >> option;
    if (option == 1)
    {
        header();
        tree.inOrder(tDisplay);
        printTail();
    }
    else if (option == 2) //Secret option, indented display
    {
        cout << "Indented Display" << endl;
        cout << "================" << endl;
        tree.printTree(iDisplay);
        printTail();
    }
    else
    {
        cout << "Not an appropriate option." << endl;
    }
}


/*~*~*~*~*~*~*~*~*~*~*~*~
deleteManager: The purpose of this function is to delete a movie that the user
enters. After the movie is deleted, it is pushed into a stack in case the user
wants to undo the delete later on by choosing the undo delete option.
~*~*~*~*~*~*~*~*~*~*~*~*/
void deleteManager(BinarySearchTree<Movie*> &movieBST,HashTable<Movie*> &movieHash, Stack<Movie>&movieStack)
{
    cout << "Delete: " << endl;
    cout << "Enter IMDb or Q to quit :";
    string IMDB;
    cin >> IMDB;

    Movie *search = new Movie;
    search->setImdb(IMDB);
    while (IMDB != "Q" && IMDB != "q")
    {
        Movie *returnedMovie = new Movie;
        if (movieHash.deleteHash(returnedMovie, IMDB))
        {
            search = returnedMovie;
            movieBST.remove(compare, search);
            movieStack.push(*returnedMovie);
            cout << returnedMovie->getTitle() <<" was deleted from database." << endl;

        }
        else
        {
            cout << "Error in deleting movie from database" << endl;

        }

        cout << "Enter IMDb or Q to quit :";
        cin.clear();
        cin.ignore();
        cin >> IMDB;
        search->setImdb(IMDB);
    }
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Help: describes what everything should do.
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void help()
{
    cout << "\nHelp\n";
    cout << "===========\n";
    cout << "The most important variable is the IMDb code." << endl;
    cout << "The IMDb code is unique for each film, no two films will have the same code." << endl;
    cout << "Generally the code will also signify the release order of different movies." << endl;
    cout << "Movies from the 1950's will have a smaller IMDb code than movies from the 2000's." << endl;
    cout << "When adding a new movie, make sure to put in the correct information. If you make a mistake, then simply delete the movie and enter it again." << endl;
    cout << "========================" << endl;

}







/*~*~*~*~*~*~*~*~*~*~*~*~
undoDelete: The purpose of this function is to undo the delete that the user did by popping the
movie out of the stack and inserting it back into the database.
~*~*~*~*~*~*~*~*~*~*~*~*/
void undoDelete(BinarySearchTree<Movie*>&movieBST, HashTable<Movie*>&movieHashTable, Stack<Movie>&movieStack)
{

    char option;
    cout << "\nUndo delete:" << endl;
    cout << "Are you sure you would like to undo deletions?(Y/N)" << endl;
    cin >> option;
    if (option == 'Y' || option == 'y')
    {
        while (!movieStack.isEmpty()) // undo stack is not empty
        {
            // pop each item out of the stack
            Movie *randomMovie = new Movie;
            *randomMovie = movieStack.pop();
            movieBST.insert(compare,randomMovie);
            movieHashTable.insertHash(randomMovie);
        }
        cout << "-----------------------" << endl;
        cout << "Deleted Movies Returned" << endl;
        cout << "-----------------------" << endl;
    }
}






/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Write File: write to a new file
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

void writeFile(HashTable<Movie*> table)
{
    string name = "NewMovies.txt";
    table.writeToFile(getOutData, name);
    cout << "\n============================" << endl;
    cout << "Successfully written to file." << endl;
    cout << "==============================" << endl;

}



// Everything below this Line is used inside functions above
//==========================================================



/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Get Prime: get the next prime for the parameter
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
int getPrime(int n)
{
    if (n <= 1)
        return 2;

    bool prime = false;

    while(!prime)
    {
        n++;
        prime = isPrime(n);
    }

    return n;

}



/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 IsPrime: determine if the parameter is a prime numbers
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
bool isPrime(int n)
{

    if (n == 0 || n ==1 )
    {
        return false;
    }
    else
    {
        for (int i = 2; i <= n / 2; i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
    }
    return true;
}



/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Compare: calculate the two Parameters
 - return 1 if m1 is smaller
 - return -1 if m1 is larger
 - return 0 if m1 and m2 are equal
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
int compare(Movie *& m1, Movie *&m2)
{
    int num;
    string t1 = m1->getTitle();
    string t2 = m2->getTitle();
    num = t1.compare(t2);

    if (num > 0)
    {
        return -1;
    }
    else if (num == 0)
    {
        return 0;
    }
    return 1;
}




/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 imdbValidate: make sure the imdb code is in it's proper format
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
bool imdbValidate(string &imdb)
{
    if (imdb.size() < 9)
    {
        return false;
    }
    else if (imdb.substr(0,2) != "tt")
    {
        return false;
    }
    for (char const &i : imdb.substr(3,imdb.size()))
    {
        if (isdigit(i) == 0)
            return false;
    }
    return true;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
titleValidate: makes sure title is in proper format
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
bool titleValidate(string &title)
{
    if (title.size() < 1)
    {
        return false;
    }
    else if (title[0] == ' ')
    {
        return false;
    }
    title[0] = toupper(title[0]);
    return true;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 validateGenre: make sure genre is in proper format
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
bool validateGenre(string &genre)
{
    if (genre.size() < 1)
    {
        return false;
    }
    for (char const &i : genre)
    {
        if (isalpha(i) == 0)
            return false;
    }
    genre[0] = toupper(genre[0]);
    return true;
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 validateDirector: make sure director is in proper format
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
bool validateDirector(string &director)
{
    if (director.size() < 1)
    {
        return false;
    }
    for (char const &i : director.substr(0,director.size()))
    {
        if (isdigit(i) != 0)
            return false;
    }
    director[0] = toupper(director[0]);
    return true;
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Get Out Data: get the data for output file
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
stringstream getOutData(Movie *&movie)
        {
            stringstream strm;

            strm << movie->getImdb() << " ";
            strm << movie->getTitle() << "; ";
            strm << movie->getYear() << " ";
            strm << movie->getGenre() << " ";
            strm << movie->getDirector() << "; ";
            strm << movie->getRating()<< " ";
            strm << endl;

            return strm;
        }


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Calculate Longest: To keep table aligned
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void calculateLongest(Movie *& movie)
{
    if(longestTitle < (int)movie->getTitle().size())
        longestTitle = (int)movie->getTitle().size();

    if(longestDirector < (int)movie->getDirector().size())
        longestDirector = (int)movie->getDirector().size();

    if(longestGenre < (int)movie->getGenre().size())
        longestGenre = (int)movie->getGenre().size();

    if(longestYear < (log((int)movie->getYear())+1))
        longestYear = log((int)movie->getYear())+1;

    if(longestImdb < (int)movie->getImdb().size())
        longestImdb = (int)movie->getImdb().size();
}



/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Header by Title: Header for Displaying by Title order
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void header()
{
    if (longestTitle < 5)
        longestTitle = 5;
    cout << setw(longestTitle) << "Title";
    cout << setw(longestImdb+2) <<"Imdb";
    cout << setw(longestYear+2) << "Year";
    cout << setw(longestGenre+2) <<"Genre";
    cout << setw(longestDirector+2) << "Director";
    cout << setw(10) <<"Rating" << endl;

    cout << setfill('=') << setw(longestTitle) << "" << setfill(' ');
    cout << setw(longestImdb+2) << "=========";
    cout << setw(longestYear+2) << "====";
    cout << setw(longestGenre+2) << "======";
    cout << setw(longestDirector+2) << "================";
    cout << setw(10) << "======" << endl;
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Print Tail: After a table is displayed
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void printTail()
{
    if (longestTitle < 5)
        longestTitle = 5;
    cout << setfill('=') <<
         setw(longestTitle+longestDirector+longestGenre+35)
         << "" << setfill(' ') <<endl;
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 horizontal display: all items on one line
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void hDisplay (Movie *&item)
{
    cout << left;
    cout << " " << item->getTitle() << "  "; // bst
    cout << " " << item->getImdb() << "  ";  // hash table
    cout << " " << item->getYear() << "  ";
    cout << " " << item->getGenre() << "  ";
    cout << " " << item->getDirector() << "  ";
    cout << " " << item->getRating() << "  ";
    cout << endl;
}



/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 table display: one movie per line, output on one line
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void tDisplay (Movie *&movie)
{
    if (longestTitle < 5)
        longestTitle = 5;
    cout << setw(longestTitle) << movie->getTitle() << setw(longestImdb+2);
    cout << movie->getImdb() << setw(longestYear+2);
    cout << movie->getYear() << setw(longestGenre+2);
    cout << movie->getGenre() << setw(longestDirector+2);
    cout << movie->getDirector() << setw(10);
    cout << movie->getRating();
    cout << endl;
}


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 vertical display: one item per line
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void vDisplay (Movie *&item)
{
    cout << "     Imdb: " << item->getImdb() << endl;
    cout << "    Title: " << item->getTitle() << endl;
    cout << "     Year: " << item->getYear() << endl;
    cout << "    Genre: " << item->getGenre() << endl;
    cout << " Director: " << item->getDirector() << endl;
    cout << "   Rating: " << item->getRating() << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 Indented Display: prints in indented list format, for testing purposes
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
void iDisplay(Movie *&movie, int level)
{
    for (int i = 0; i < level; i++)
        cout << "..";
    cout << level;
    cout << " " << movie->getImdb() << " ";
    cout << " " << movie->getTitle() << " ";
    cout << " " << movie->getYear() << " ";
    cout << " " << movie->getGenre() << " ";
    cout << " " << movie->getDirector() << " ";
    cout << " " << movie->getRating()<< " ";
    cout << endl;

}

