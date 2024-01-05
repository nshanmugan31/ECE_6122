/*
Author: Nishalini Shanmugan
Class: ECE4122 
Last Date Modified: 11/21/2021

Description:

In this lab, we used a C\C++ program using CUDA to solve the steady state temperature distribution in the thin plate. 
We are passing in the number of interior points in the array and the number of iterations, and returning the number
of miliseconds it took to calculate the solution using CUDA events. We are also writing the final temperature values
into an finalTemperature.csv file. 

*/

#include <iostream>
#include <list>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <iomanip>


using namespace std;

//Call the kernal and fill the data with Jacobi Iteration
__global__ void distributionPlate(double*h, double*g, unsigned long n) 
{
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    int i = blockIdx.y * blockDim.y + threadIdx.y;

    if((i > 0) && (i < n-1) && (j > 0) && (j < n-1))
    {
        g[i*n+j] = 0.25*(h[(i-1)*n + j] + h[(i+1)*n + j]+h[i*n + (j-1)] + h[i*n +(j+1)]);
    }

}

//Create the array
void populateArray(double *a, int dim)
{
    for(int i=0;i<dim;i++)
    {
        for(int j=0; j<dim;j++)
        {
            //If the first row is greater than 30% or less than 70%, add 100 into each element
            if ((j > 0.3*(dim-1)) && (j < 0.7*(dim-1)) && (i==0))
            {
                a[i*dim+j]=100;
            }
            //Otherwise add 20 to all the elements
            else
            {
                a[i*dim+j]=20;
            }
        }
    }
}

//Print the array to the finalTemperature.csv file
void printArray(double* a, int dim)
{
    std::ofstream myfile;
    //Open a file called server.log and append to it using ios::app
    myfile.open("finalTemperature.csv", std::ios::out | std::ios::trunc);
    string row;
    for(int i=0; i < dim; i++)
    {
        for(int j=0; j < dim; j++)
        {
            row +=to_string(a[i*dim+j]);
            //enter if the end of the row is reached
            if(j == dim-1)
            {
                row +="\n";
            }
            //Otherwise put a comma in between all the comments.
            else
            {
                row +=",";
            }
        }
    }
    myfile << row << std::endl;
    myfile.close();
}

int main(int argc, char* argv[])
{
    int n = 0;
    int limit = 0;
    
    //If there are 5 arguments continue, else return invalid command arguments
    if (argc == 5)
    {   
        if ((atoi(argv[2])) || (atoi(argv[4])))
        {
            n = std::stoi(argv[2]);
            limit = std::stoi(argv[4]);
            //If the number of N x N interior points and the number of iterations is less than 0, return invalid parameter
            if (n < 0 || limit < 0)
            {
                std::cout << "Invalid parameter, please check your values." <<std::endl;
            }
            else
            {
            //(n+2)*(n+2)
            const int dim = (n+2);
            const int length = dim*dim;
            const int size = sizeof(double)*(length);
            
            //allocate memory
            double*h = (double*) malloc(size);
            double*g = (double*) malloc(size); 

            //Declare and cuda host copies
            double *hd;
            double *gd;
            cudaMalloc((void**)&hd, size);
            cudaMalloc((void**)&gd, size);
            
            //fill host array with data
            populateArray(h,dim);
            populateArray(g,dim);
            
            //GPU timing
            float gpu_elapsed_time;
            cudaEvent_t gpu_start, gpu_stop;
            cudaEventCreate(&gpu_start);
            cudaEventCreate(&gpu_stop);

            //copy from host to device
            cudaEventRecord(gpu_start, 0);
            cudaMemcpy(hd, h, size, cudaMemcpyHostToDevice);
            cudaMemcpy(gd, g, size, cudaMemcpyHostToDevice);

            //call kernel
            dim3 blockSize(16,16);
            dim3 gridSize(n/16+1, n/16+1);
            
            for (int iteration = 0; iteration < limit; iteration++)
            {
                // Launch distributioPlate() kernel on GPU with gridSize blocks 
                distributionPlate<<< gridSize, blockSize >>>(hd, gd, dim);
                
                //Use swap function to swap the arrays
                swap(hd,gd);
            }

            //copy from device to host
            cudaMemcpy(h, hd, size, cudaMemcpyDeviceToHost);

            //Track the gpu start, stop, and elapsed time
            cudaEventRecord(gpu_stop,0);
            cudaEventSynchronize(gpu_stop);
            cudaEventElapsedTime(&gpu_elapsed_time, gpu_start, gpu_stop);
            cudaEventDestroy(gpu_start);
            cudaEventDestroy(gpu_stop);
            
            //Output to the console the number of milliseconds it took to calculate the solution using CUDA events
            std::cout << std::fixed;
            std::cout << "Number of Miliseconds using CUDA events:" << std::setprecision(2) << gpu_elapsed_time <<std::endl;
            printArray(h,dim);

            //free memory
            free(h);
            free(g);
            cudaFree(hd);
            cudaFree(gd);
            return 0;   
            }
                
        }
        //If they are unable to convert, return invalid parameter
        else
        {
            std::cout << "Invalid parameter, please check your values." <<std::endl;
        }
        
    }
    //Invalid command line arguments entered - Anything except 5 arguments. 
    else
    {
        std::cout << "Invalid command line arguments!" << std::endl;
    }
    return 0;
}
