/*
Author: Nishalini Shanmugan
Class: ECE 4122
Last Date Modified: 9/21/2021

Description:

Langston's Ants: 
-if it is on a black square, it flips the color of the square to white
-rotates 90 degrees counterclockwise and moves forward one square.
-if it is on a white square, it flips the color of the square to black
-rotates 90 degrees clockwise and moves forward one square.

*/
#include <iostream>
#include <fstream>
#include <math.h> 
#include <set>
#include <string>
#include <utility>
using namespace std;

//Uses a vector to keep track of the black squares
class Vector2 {        // The class
  public:          // Access specifier    
    int x;
    int y; 

    Vector2(int n_x, int n_y) { // Constructor with parameters
      x = n_x;
      y = n_y;
    }

    bool operator <(const Vector2& vect) const
    {
        if (x == vect.x) return y < vect.y;
        else return x < vect.x;

    }
};

//Three functions 
//if it is on a black square, it flips the color of the square to white
//rotates 90 degrees counterclockwise and moves forward one square.
//if it is on a white square, it flips the color of the square to black
//rotates 90 degrees clockwise and moves forward one square.

int ant_movement(unsigned long s)
{

    std::set<Vector2> black_square; //https://www.geeksforgeeks.org/pair-in-cpp-stl/
    
    int counter = 0;
    int direction = 0;
    Vector2 mx_my(0, 0);
    //The directions are up = 0, left = 1, down = 2, right = 3
    
    for (int i=0; i < s; i++)
    {   
        //Count the number of black squares 
        if (black_square.count(mx_my) == 0)
        {
            black_square.insert(mx_my); //inserts the black squares
            direction += 1;
            direction %= 4;
        } else 
        {
            black_square.erase(mx_my);  //Erases the black squares
            direction += 3;
            direction %= 4;
        }
        
        //direction of the ant movement
        switch (direction)
        {
            case 0:
                mx_my.y = mx_my.y + 1;
                break;
            case 1:
                mx_my.x = mx_my.x - 1;
                break;
            case 2:
                mx_my.y = mx_my.y - 1;
                break;
            case 3:
                mx_my.x = mx_my.x + 1;
                break;        
        default:
            break;
        }

    }

    int output;
    output =  static_cast<int>(black_square.size());
    return output;
};



//Outputing the output into an output2.txt file
//if there are not 2 arguments, then return invalid input
int main(int argc, char* argv[])
{   
    unsigned long s;
    ofstream myfile; 
    myfile.open ("output2.txt", std::ios::out | std::ios::trunc);

    if (argc != 2)
    {
        myfile << "No invalid input" <<std::endl; 
    }
    else
    {
        s = std::stoul(argv[1]);
        myfile << ant_movement(s) <<std::endl;
        
    }

    myfile.close();
    return 0;

}


