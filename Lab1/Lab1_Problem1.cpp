/*
Author: Nishalini Shanmugan
Class: ECE 4122
Last Date Modified: 9/21/2021

Description:

Printing out the prime factors of a function.
Take in the unsigned long of an input number, and 
output the string of a output into a output1.txt file. 

*/
#include <iostream>
#include <fstream>
#include <math.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <list>
#include <set>
#include <vector>
using namespace std;

//Iterates through starting with 2 until all the prime factors are found. 
bool GetPrimeFactors (const unsigned long ulInputNumber, string &strOutput)
{
    unsigned long num = ulInputNumber;
    
    if (num <= 1) return false;
    for (int i = 2; i <= ulInputNumber; i++)
    {
        if (num % i == 0)
        {
            while (num % i == 0)
            {
                //std::cout << num;
                num = num/i; 
                //Keeps dividing out until we run out of primes. For example, 2, 3, 5, 7 and so on. 
                //Converts an into to a string
                string prime;
                stringstream ss;            
                ss << i;
                ss >> prime;
                strOutput += prime;
                
                if (num != 1){
                    strOutput += ",";  //comma after each prime
                }
            }

        }  

        if (num == 1) break;  
    }

    return true;

    
};

//Find the prime factors and returns true. 


int main(int argc, char* argv[]) 
{
    unsigned long number; //unsigned long
    string output; 
    char* p;
    bool prime_factors; 
    
    if (argc != 2)
    {
        output = "No invalid input"; //If there are not 2 arguments, then return no invalid input
    }
    else
    {
        number = std::stoul(argv[1]);
        if (!GetPrimeFactors(number, output))
        {
            output =  "No prime factors"; 
        }
    }

    //Outputs the result into a output1.txt file
    ofstream myfile;
    myfile.open ("output1.txt", std::ios::out | std::ios::trunc); //trunc deletes the output of the file
    myfile << output << std::endl;
    myfile.close();

    return 0;
   
}
