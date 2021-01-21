** To run this program, extract all files in the same directory, then type in "make" to create the executable. Once compiled, run via "movie <csv_file_name.csv>", where csv_file_name.csv" is the name of the csv file you want to run the program to read and run with. **

## Introduction
In this assignment, you'll write a program that will introduce you to programming in C on UNIX based systems.

## Learning Outcomes
How to write a C program to solve a problem? (Module 1 MLO 4)
How do you interact with the user in C programs? (Module 1 MLO 6)
How are C programs transformed into an executable form? (Module 1 MLO 7)
Define programming language constructs in C (Module 2 MLO 1)
Describe structure data types of C programming language (Module 2 MLO 3)
What are pointers and how is this mechanism supported in C? ((Module 2 MLO 4)
Describe memory allocation in Unix (Module 2 MLO 5)
Explain how to create and manipulate strings in C (Module 2 MLO 6)

## Instructions
Write a program that:
Reads a CSV file with movie data that is provided to the program as an argument
Processes the data in the file to create structs to hold data for each movie
Creates a linked list of all these structs
Gives user choices to ask questions about the movies in the data
Prints out the data about the movies per user choice
Format of the CSV File
Here is a sample file whose format corresponds to the format of the CSV file your program will be tested with.

The first row in the file contains the column headers, and not movie data.
All other rows have the same format and no columns are missing in any row.
Commas appear as delimiters between columns, but will not appear in the values of any columns.
This file has the following columns:

#### Title
This is a string with the movie title.
E.g., Iron Man 2
#### Year
This is a 4 digit integer value for the year the movie was released
E.g., 2010
#### Languages
The language or languages in which the movie was released.
One or more string values that are always enclosed within []
Multiple values are separated by semi-colons.
E.g,
[English;Portuguese;Spanish]
[English;French]
[English]
You can assume that the maximum number of languages any movie can be released in is 5.
You can assume that the maximum length of a language string is 20 characters.
#### Rating Value
A number between 1 and 10 (inclusive of both 1 and 10)
It can be an integer or a double with one digit after the decimal point
E.g.,
5
8.7
## Program Functionality
Process the input file
When your program starts it must read all data from the file and process it. After processing the file, the program must print the following message to stdout

"Processed file XYZ and parsed data for M movies"

where XYZ is the name of the file that has been process and M is the number of movies whose data has been processed (this will be 1 less than than the number of lines in the file because the first line has the column headers).

E.g., Processed file movie_sample_1.csv and parsed data for 24 movies

## Interactive Functionality
Next your program should display a menus of interactive choices to the user and process the selected choice as described below. For the text of messages to print, see the section "Sample Program Execution."

1. Show movies release in the specified year

If the user chooses this option, then ask them to enter a year and
Display the names of all the movies released in that year, one on each line
Your program can assume that the user will enter a 4 digit integer for the year
If the data doesn't have any movies released in that year, print a message about this. 
2. Show highest rated movie for each year

If the user chooses this option, then for each year for which at least one movie was released, display a movie that had the highest rating along with the year and the rating with one line per year.
In case of ties, display any one movie that had the highest rating that year.
Display the data in the form: YYYY RatingValue MovieTitle
The data doesn't have been sorted by year or by rating value.
E.g.,
2010 8.5 Avengers: Infinity War
2012 8.1 The Avengers
3. Show movies and their year of release for a specific language

If the user chooses this option, ask them to enter a language and
For all movies released in the specified language
Display the year of release and the movie title, one line per movie
If the data doesn't include any movie released in this language, print a message about it.
4. Exit

If the user choose this option, the program should exit.
## Notes:

You can assume that when the program asks user to enter an integer, the user will indeed enter an integer (i.e., you don't need to verify the data type of the user input).
For the interaction choice if  the user enters an incorrect integer (i.e., something other than 1 to 4), print an error message and again present the 4 choices to the user. 
Technical Requirements
Your program is required to

Read data from the file line by line
Break-up the line into tokens
Create a struct called movie with the data for that line
Create a linked list containing all the movie structures
In the Resources Section we have provided sample code that you can adapt to meet these technical requirements.

## Sample Program Execution
Here is a complete example of executing the assignment with the provided sample CSV file.

<pre><code>$ ./movies movie_sample_1.csv 
Processed file movie_sample_1.csv and parsed data for 24 movies

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 1
Enter the year for which you want to see movies: 1999
No data about movies released in the year 1999

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 1
Enter the year for which you want to see movies: 2012
The Avengers
Rise of the Guardians
Anna Karenina

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 2
2008 7.90 Iron Man
2009 7.60 Sherlock Holmes
2010 7.00 Iron Man 2
2013 7.20 Iron Man 3
2017 7.90 Thor: Ragnarok
2012 8.10 The Avengers
2016 7.80 Captain America: Civil War
2018 8.50 Avengers: Infinity War
2015 7.40 Avengers: Age of Ultron
2011 7.00 Thor
2014 7.80 Captain America: The Winter Soldier
2003 6.60 Right on Track

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 3
Enter the language for which you want to see movies: English
2008 The Incredible Hulk
2009 Sherlock Holmes
2008 Iron Man
2010 Iron Man 2
2013 Iron Man 3
2017 Thor: Ragnarok
2012 The Avengers
2016 Doctor Strange
2018 Avengers: Infinity War
2015 Avengers: Age of Ultron
2011 Thor
2013 Thor: The Dark World
2017 Spider-Man: Homecoming
2011 Captain America: The First Avenger
2016 Captain America: Civil War
2015 Ant-Man
2014 Captain America: The Winter Soldier
2018 Mary Queen of Scots
2016 Revolting Rhymes Part One
2017 The Glass Castle
2016 Free Fire
2003 Right on Track
2012 Rise of the Guardians
2012 Anna Karenina

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 3
Enter the language for which you want to see movies: Punjabi
No data about movies released in Punjabi

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 5
You entered an incorrect choice. Try again.

1. Show movies released in the specified year
2. Show highest rated movie for each year
3. Show the title and year of release of all movies in a specific language
4. Exit from the program

Enter a choice from 1 to 4: 4
$<pre></code>
