/*
Author: Nishalini Shanmugan
Class: ECE 4122
Last Date Modified: 10/15/2021

Description:

*/
#include <iostream>
#include <fstream>
#include <math.h> 
#include <stdlib.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
#include <random>
#include <atomic>
using namespace std;

/*
The purpose of this document is to show a multithreaded approach of traversing 
an array. The bottom row of the 5 by 5 array has seeds. We want to randomly move
using uniform_int_distribution to move all the seeds to the top row. After we 
push back all the threads the VecThread, then we join the threads together.
We return the number to threads used, average number of steps, and the total number 
runs needed for convergence. 
*/

/*
Orientation of the ant_movement
Before Running
        
      
    0 0 0 0 0 0
y   1 0 0 0 0 0
    2 0 0 A 0 0
    3 0 0 0 0 0 
    4 1 1 1 1 1
      0 1 2 3 4
          x

Orientation of the ant_movement
After Running - Exits the while loop
        
      
    0 1 1 1 1 1
y   1 0 0 0 0 0
    2 0 0 A 0 0
    3 0 0 0 0 0 
    4 0 0 0 0 0 
      0 1 2 3 4
          x


*/

std::atomic<std::uint64_t> atomic_steps{0};  //global variable for the atomic steps
std::atomic<std::uint64_t> total_runs{0};    //total runs
void steps_function(int n) //multithreading function
{
    //n simulations 
    //one rand number generator per thread
    //seed it once using time
    
    std::default_random_engine generator(clock() + random_device{}()); //randomize the steps

    for (int i = 0; i<n; i++)
    {
        int ant_movement[5][5];  //create 5 by 5 grid
        for (int i=0; i<5; i++)
        {
            ant_movement[i][4]=1; //ant_movement[x][y]
            ant_movement[i][3]=0; //set the x=4 bottom row of the array to 1
            ant_movement[i][2]=0;
            ant_movement[i][1]=0;
            ant_movement[i][0]=0;
        }

        int x = 2; //set the position of the ant at (2,2)
        int y = 2;
        int s = 0; //s is seed
        bool isDone = false;
        uint64_t steps = 0; 
        while(!isDone) //while the top row of the x is not 1, continue running through the loop
        {
            //if () //if x = 0 and y = 0, 0,4; 4,4; 4,0;
            //int a = rand()%4; //rand number generator 
            //cout << x << y <<endl;
            //cout << ant_movement[0][0]<<endl;
            //cout << ant_movement[0][1]<<endl;
            //cout << s <<endl;
            std::uniform_int_distribution<int> distribution(0,3); //randomize the directions
            int a = distribution(generator); 
            switch(a){
                case 0:
                    if (x>0)
                    { //left
                        x = x-1; 
                        //cout <<"moved left" <<endl; 
                        steps++;
                    }
                    else
                    {
                        continue;
                    }
                    break;
                case 1:
                    if (x<4)
                    { //right 
                        x = x+1;
                        //cout <<"moved right" <<endl; 
                        steps++;
                    }
                    else{
                        continue;
                    }
                    break;
                case 2:
                    if (y<4)
                    { //down
                        y = y+1;
                        //cout <<"moved down" <<endl;
                        steps++; 
                    }
                    else
                    {
                        continue;
                    }
                    break; 
                case 3:
                    if (y>0)
                    { //up
                        y = y-1;
                        //cout <<"moved up" <<endl; 
                        steps++;
                    }
                    else
                    {
                        continue;
                    }
                    break; 
            }

            if ((ant_movement[x][y]==1) && (y==4) && (s==0)) //if you don't have a seed and you are on the bottom row, pick up a seed
            {
                ant_movement[x][4]=0;
                //cout << ant_movement[0][4] << ant_movement[1][4] << ant_movement[2][4] << ant_movement[3][4] << ant_movement[4][4] << endl; 
                s=1;//u have a seed
                
            }

            if ((ant_movement[x][y]==0) && (y==0) && (s==1)) //if you have a seed and you are on the upper row, put the seed down
            {
                ant_movement[x][0]=1;
                //cout << ant_movement[0][0] << ant_movement[1][0] << ant_movement[2][0] << ant_movement[3][0] << ant_movement[4][0] << endl; 
                s=0; //no seed
            }

            
            if((ant_movement[0][0] == 1) && (ant_movement[1][0] == 1) && (ant_movement[2][0] == 1) && (ant_movement[3][0] == 1) && (ant_movement[4][0] == 1)){
                isDone = true; //only exit the while loop when the upper row all has a seeds in it
            }


        }
        atomic_steps += steps; //increse the number of steps 
        total_runs += 1; //count the number of runs it is taking
    }
    

}

int main()
{
    int num_threads = std::thread::hardware_concurrency(); //calcuate the number of supported threads. Should be 24 on pace-ice
    unsigned long long runs = 10000000 / num_threads; //calculate the number of runs per thread
    double average = 0;

    std:vector<std::thread>threads; 

    for (int i = 0; i<=num_threads+1; ++i) //plus 1 for the num_threads to improve the accuracy
    {
        threads.push_back(std::thread(steps_function, runs));
    }

    

    for (auto& th:threads) //join the threads at the end
        th.join();

    average = (double)atomic_steps / (double)total_runs; //take the steps and divide it by the number of runs
    //we are counting the number of runs as we loop

    ofstream myfile; 
    myfile.open ("ProblemOne.txt", std::ios::out | std::ios::trunc);
    myfile << "Number of Threads created:" << num_threads <<std::endl;
    myfile << "Expected number of steps:" << std::setprecision(9) <<average <<std::endl;
    myfile << "Total number of runs needed for solution convergence:" << total_runs <<std::endl;
    myfile.close();
    return 0;

}