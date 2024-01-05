I have included code samples from 3 labs from my advanced programming course (C++). I have included the lab description to help show what the intention of each lab was. 

Lab 1:

Problem 1: 
Printing out the prime factors of a function.
Take in the unsigned long of an input number, and 
output the string of prime factors into an output1.txt file. 

Problem 2:
Langston's Ants: 
-if it is on a black square, it flips the color of the square to white
-rotates 90 degrees counterclockwise and moves forward one square.
-if it is on a white square, it flips the color of the square to black
-rotates 90 degrees clockwise and moves forward one square.
Takes in unsigned long that indicates the number of moves the ant makes. Returns the number of black squares to a text file. 

Problem 3: 
Multiple reflections of a laser beam:
Equilateral triangle with a side length of 20 cm suspended symmetrically on the origin (the lower vertex C is at the origin) with the +x axis pointing to the right and the +y axis pointing up.
Takes in a single command line of the initial reflection's x location along the AB segment and calculates the number of times the beam is reflected off an internal surface of the white cell before exiting.

Lab 2:

Problem 1:
The purpose of this code is to show a multithreaded approach of traversing 
an array. The bottom row of the 5 by 5 array has seeds. We want to randomly move
using uniform_int_distribution to move all the seeds to the top row. After we 
push back all the threads the VecThread, then we join the threads together.
We return the number to threads used, average number of steps, and the total number 
runs needed for convergence. 

Problem 2:
The purpose of this code is to take an integration problem and 
use openMP to compute the integral. The benefits of openMP is that
we do not have spend so much time creating a vector of threads,
pushing them back, and joining them together. This code takes a single command line argument that is the number of subdivisions (N) and returns the estimated integral. 

Lab 3:

Problem 1:
In this lab, we used a C\C++ program using CUDA to solve the steady state temperature distribution in the thin plate. We are passing in the number of interior points in the array and the number of iterations, and returning the number of miliseconds it took to calculate the solution using CUDA events. We are also writing the final temperature values into an finalTemperature.csv file. 
The program needs to takes the following command line arguments:-N 256 - the number of N x N interior points, -I 10000 – the number of iterations, -q – quits the application
The code outputs to the console the number of milliseconds it took to calculate the solution using CUDA events.
