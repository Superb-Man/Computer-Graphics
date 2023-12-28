#pragma once
#include <bits/stdc++.h> 

class point {
    public:
    double x, y, z ;
    point() {

    }
    point(double x, double y , double z) {
        this->x = x ;
        this->y = y ;
        this->z = z ;
    }
    point operator*(double s) const{
        return point(x*s,y*s,z*s) ;
    }
    point operator/(double s) const{
        return point(x/s,y/s,z/s) ;
    }
    point operator+(const point& p) const{
        return point(x+p.x , y+p.y , z+p.z) ;
    }

    point operator-(point& p) const{
        return point(x - p.x , y - p.y , z - p.z) ;
    }

};

point crossProduct(point u,point v) {
    return point(u.y * v.z - v.y * u.z ,
                    -u.x * v.z + v.x * u.z, u.x * v.y - u.y * v.x) ;
}

double dotProduct(point u, point v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

point normalize(point& p) {
    double length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z) ;
    return point(p.x / length , p.y / length , p.z / length) ;
}
struct Triangle{
    point a, b, c;
};