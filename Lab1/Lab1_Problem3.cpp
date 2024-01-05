/*
Author: Nishalini Shanmugan
Class: ECE 4122
Last Date Modified: 9/21/2021

Description:

*/
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <set>
#include <string>

/*
-Created an ECELineSegment Class with begining point and ending point
-ECEPoint - keeps the x and y value of each point
-An array of ECE lineSegments
-Each of these line segments - besides a begining and ending point has a surface normal
-Surface normal is a member of ECEline Segment. Only one component -j
dx = x2 - x1 and dy = y2 - y1 (Normal vector of a line segment)
There are two normals: (-dy, dx) and (dy, -dx). It is pointing in increasing y.
So y must be positive and x is negative
-Loop over each line segments to determine where the intersects occurred. 
-This should only occur on one line segment. Excluding the orignal line segment. 
-Ignore the first line segment and focus on the other two
-Line Segment AB, Line Segment BC, Line Segment CA, Normal AB (0), Normal BC (1), Normal CA (2)
-This will help check if the ray is still inside
-If the y value is less than 0.01. The ray has escaped and we are done. 
-How to get reflection vector: r=d−2(d⋅n)n. Scalar times a vector. 2vxi + 2vy. 
-You can scale it to get a vector. Know our starting point (d). Know surface normal (which is n).
-r is the reflection vector. Using the vector to get the other point. 
-Now we keep iterating it. 
*/

//To make sure it is intersected in the triangle
//Creates the ray
class Vector2 {        // The class
  public:          // Access specifier    
    double x;
    double y; 

    Vector2(double n_x, double n_y) { // Constructor with parameters
      x = n_x;
      y = n_y;
    }

    Vector2(){x = 0; y = 0;}

    Vector2 diff(Vector2 inPt)
    {
      return Vector2(x-inPt.x, y-inPt.y);
    }

    Vector2 add(Vector2 inPt)
    {
      return Vector2(x+inPt.x, y+inPt.y);
    }   
    double mag(){ //Magnitude 
        return sqrt(x*x + y*y);
    }

    Vector2 norm(){ //Normal Vector
        return Vector2(x/mag(), y/mag());
    }

    void print() {
        std::cout << x << " --- " << y << "\n";
    }
};


//How do you know if the vector is intersected in the triangle. Determine the distance between the line 
//segment. If the intersection is in between, the it shoudl be less than the total distance. 
//If it intersected outside, the distance is going to be larger. 

class LineSegment 
{
  public:
    Vector2 startPoint;
    Vector2 dir;
    Vector2 normDir;
    double m;
    double c;
    Vector2 ptA; 
    Vector2 ptB;

    LineSegment(){

    }
    LineSegment(Vector2 pt1, Vector2 pt2):normDir(-dir.y,dir.x), startPoint(pt1), ptA(pt1), ptB(pt2),dir(pt2.diff(pt1))//constructor
    {
      //Overload the operator
      //ptA = pt1;
      //ptB = pt2;
      //startPoint = pt1;      
      //dir = pt2.diff(pt1);
      //normDir = Vector2(-dir.y, dir.x);
      normDir = normDir.norm();
      m = (pt2.y - pt1.y)/(pt2.x - pt1.x);
      c = pt1.y - m * pt1.x;
    }

    //Amount of time it takes the intersect ray to reach a new wall
    double intersectTime(LineSegment lineB)
    {
        
        return (lineB.m * startPoint.x + lineB.c - startPoint.y)/(dir.y - lineB.m * dir.x);
    }

    Vector2 intersectPoint(LineSegment lineB)
    {
        double t = intersectTime(lineB);
        return Vector2(startPoint.x + t * dir.x, startPoint.y + t * dir.y);
    }

    void print() {
        std::cout << ptA.x << " --- " << ptA.y << "   ++++++++++++++  " << ptB.x << " --- " << ptB.y << "\n";
    }
};
//
//Coordinates of the the triangle
Vector2 VertexA(-10, 10*sqrt(3));
Vector2 VertexB(10, 10*sqrt(3));
Vector2 VertexC(0, 0);

LineSegment WallA(VertexA, VertexB);
LineSegment WallB(VertexB, VertexC);
LineSegment WallC(VertexA, VertexC);


//Find the next ray after intersection
LineSegment GetNextRay(LineSegment wallSegment, LineSegment currRay) 
{
    
    Vector2 intersectionPoint = wallSegment.intersectPoint(currRay); 
    Vector2 nextVector(0, 0);
    double dot = wallSegment.normDir.x * currRay.dir.x + wallSegment.normDir.y * currRay.dir.y;

    nextVector.x = currRay.dir.x - 2 * dot * wallSegment.normDir.x;
    nextVector.y = currRay.dir.y - 2 * dot * wallSegment.normDir.y;
    Vector2 nextPoint = intersectionPoint.add(nextVector);
    LineSegment res(intersectionPoint, nextPoint);
    return res;
}

//Wall segment and current direction of the ray. Reflected vector formula

int getMinPositive(double x, double y)
{
    if (x <= 0 || x > 1) return y;
    else if (y <= 0 || y > 1) return x;
    else if (x < y) return x;
    else return y;
}

//The next wall has to be one of two that does not include the current wall. 
//Return the minimim amout of time it would be reach the other two wall. 
LineSegment FindNextWall(LineSegment ray)
{
    LineSegment next;
    double tA = ray.intersectTime(WallA);
    double tB = ray.intersectTime(WallB);
    if (tA < 0.0000001) tA = -1;
    if (tB < 0.0000001) tB = -1;

    if (tA <= 0 || tA > 1) next = WallB;
    else if (tB <= 0 || tB > 1) next = WallA;
    else if (tA < tB) next = WallA;
    else return next = WallB;

    tA = ray.intersectTime(next);
    tB = ray.intersectTime(WallC);
    if (tA < 0.0000001) tA = -1;
    if (tB < 0.0000001) tB = -1;

    if (tA <= 0 || tA > 1) return WallC;
    else if (tB <= 0 || tB > 1) return next;
    else if (tA < tB) return next;
    else return WallC;  

}

//Number of bounces of the wall. If y < 0.01, then the laser will escape. 
//Keep looping through until it escapes. 
int GetNumBounces(LineSegment initRay){

    LineSegment ray = initRay;
    int count = 0;
    bool done = false;
    while (!done)
    {        
        LineSegment wall = FindNextWall(ray);

        Vector2 intPoint = wall.intersectPoint(ray);
        
        if (intPoint.y <= 0.01) {
            done = true;
            continue;
        } else {
            count += 1;
            ray = GetNextRay(wall, ray);
        }
    }

    return count;
};

//Outputing the output into an output3.txt file
//if there are not 2 arguments, then return invalid input
int main (int argc, char* argv[])
{
    //double input = atof(argv[0]);
    std::ofstream myfile; 
    myfile.open ("output3.txt", std::ios::out | std::ios::trunc);
    
    if (argc != 2)
    {
        myfile << "No invalid input" <<std::endl;
    }
    else
    {
        double input = std::stod(argv[1]);
        if (input != 0) 
        {
            Vector2 orig(0, 0);
            Vector2 firstDir(input, 10*sqrt(3));
            LineSegment firstRay(orig, firstDir);
            myfile << GetNumBounces(firstRay) << std::endl; //return the number of times it bounces off the wall
        }
        else 
        {
            myfile << "1" << std::endl; //if the input is 0, then return 1. 
        }
        
    }

    myfile.close();
    return 0;

}