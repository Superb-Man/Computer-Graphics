#pragma once
#include <GL/glut.h> 
#include<cmath>
class point {
    public:
    GLfloat x, y, z ;
    point() {

    }
    point(GLfloat x, GLfloat y , GLfloat z) {
        this->x = x ;
        this->y = y ;
        this->z = z ;
    }
    point operator*(float s) const{
        return point(x*s,y*s,z*s) ;
    }
    point operator/(float s) const{
        return point(x/s,y/s,z/s) ;
    }
    point operator+(point p) const{
        return point(x+p.x , y+p.y , z+p.z) ;
    }

    point operator-(point p) const{
        return point(x - p.x , y - p.y , z - p.z) ;
    }

};

//From Slide 
// at first finding a vector which is perpenticular to both of
// axis and vector
// Then v = u sin(alpha) + v cos(alpha)
point& rotation(point& axis, point& vector , GLfloat angle){
    point u = point(axis.y * vector.z - vector.y * axis.z ,
                    -axis.x * vector.z + vector.x * axis.z, axis.x * vector.y - axis.y * vector.x) ;
    vector = u * sin(angle) + vector * cos(angle) ;

    return vector ;
}