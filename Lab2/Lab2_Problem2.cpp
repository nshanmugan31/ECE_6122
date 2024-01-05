/*
Author: Nishalini Shanmugan
Class: ECE 4122
Last Date Modified: 10/15/2021

Description:

*/
#include <iostream>
#include <fstream>
#include <math.h> 
#include <stdio.h> 
#include <omp.h>
#include <set>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include<string>
using namespace std;

//use iomanip and iostream libararies
/*
The purpose of this problem is to take an integration problem and 
use openMP to compute the integral. The benefits of openMP is that
we do not have spend so much time creating a vector of threads,
pushing them back, and joining them together. 

*/

double integral(unsigned long N) //needs to be a double or the rounding will be off

{
    //double lowBound = 0;
    //double upBound = log(2)/7; 
    //double step = log(2)/(7*N);
    double sum = 0;
    #pragma omp parallel for reduction(+:sum) //open mp
    for (int i = 0; i < N; i++) 
    {
        //int midpoint = (log(2)/(14*N) + (log(2)*i)/(7*N)); //step = delta/2 + delta*i
        double midpoint = ((log(2)/(7*N))/2);
        double step = (log(2)/(7*N));
        sum += step * (14 * exp(7 * (midpoint + step*i))); //integration equation
        //int length = (log(2)/(7*N));
        //int areaOfRect = 14*exp(7*midpoint)*length;
        //sum +=areaOfRect;
    } 
    return sum; //calculates the total subdivision area
    
}

int main(int argc, char* argv[])
{   
    unsigned long N; //Number of subdivisions
    ofstream myfile; 
    myfile.open ("Lab2Prob2.txt", std::ios::out | std::ios::trunc);

    if (argc != 2)
    {
        myfile << "No valid input" << std::endl;
    }
    else
    {
        N = std::stoul(argv[1]);
        //You can try this 

        //fileHandle << fixed;
        //fileHandle << setprecision(6) << sum ;
        myfile << setprecision(6) << integral(N) <<std::endl; 
        
    }

    myfile.close();
    return 0;

} 



