#pragma once
#include "point.h"
#include <GL/glut.h> 
#include<cmath>

const GLfloat rotate_rate = 0.01 ;
const GLfloat move_rate   = 0.03 ;



point eyepos(2.3 ,2.3, 2.3) ;
point look(-2.3 ,-2.3 ,-2.3) ;
point right(1 ,0, 0) ;
point up (0 ,1 ,0) ;
point center(0.33,.33,.33) ;

GLfloat angle = 0 ;
GLfloat scaling = 0 ;


GLfloat radius = 1.0/sqrt(3);
GLfloat theta = (M_PI-acos(-1.0/3.0)) ;

//8 palnes for OctaHedron
GLfloat octahedralPlaneColors [][3] = {
    {1,0,0} ,
    {0,1,0} ,
    {0,0,1} ,
    {1,1,0} ,
    {0,1,1} ,
    {1,0,1} ,
    {1,1,1} ,
    {0.3,0.5,0}, 
} ;

GLfloat ballColor[][3] ={
    {1,0,0},
    {0,1,0},
} ;
