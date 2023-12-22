#pragma once
#include <GL/glut.h>  
#include <cmath>
#include <iostream>
#include <vector>
#include "point.h"
#include "defs.h"
#include "geometry.h"

void init()
{
    glMatrixMode(GL_PROJECTION) ;
    gluPerspective(60, 1, 1, 100) ;

}

void display() {      

    glEnable(GL_DEPTH_TEST) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glMatrixMode(GL_MODELVIEW) ;             // To operate on Model-View matrix

    glLoadIdentity() ;                       
    // camera

    GLfloat d = sqrt(eyepos.x * eyepos.x +eyepos.y
                * eyepos.y + eyepos.z*eyepos.z) ;
    gluLookAt(eyepos.x, eyepos.y, eyepos.z,eyepos.x + d*look.x,
                eyepos.y+d*look.y , eyepos.z + d*look.z,
                up.x,up.y,up.z) ;

    glPushMatrix() ;
    glRotatef(angle,0,1,0) ;
    drawAxes() ;
    drawOctaHedral() ;
    drawAllCylinders() ;
    drawAllSpheres() ;
    glPopMatrix() ;

    glFlush();
}
            //             up
            //              |    / (look at the point)
            //              |   /
            //              |  /
            //              | /
            //              0----------(right)
            //             /
            //            /
            //           /
            //          /     
            //
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '.':
        scaling = std::min(scaling + 0.01, 1.0);
        break;
    case ',':
        scaling = std::max(0.0, scaling - 0.01);
        break;
    case 'd' :
        angle+= 5 ;
        break;
    case 'a' :
        angle-= 5 ;
        break ;
    case '1' :
        //rotate left
        rotation(up, look, -rotate_rate) ;
        rotation(up, right, -rotate_rate) ;
        break ;
    case '2' :
        //rotate right
        rotation(up, look, rotate_rate) ;
        rotation(up, right, rotate_rate) ;
        break ;
    case '3' :
        //rotate up
        rotation(right, look, rotate_rate) ;
        rotation(right, up, rotate_rate) ;
        break ;
    case '4' :
        //rotate down
        rotation(right, look, -rotate_rate) ; 
        rotation(right, up, -rotate_rate) ;
        break ;
    case '5':
        //clockwise
        rotation(look, right, rotate_rate) ;
        rotation(look, up, rotate_rate) ;
        break ;
    case '6' :
        //anticlock
        rotation(look, right, -rotate_rate) ;
        rotation(look, up, -rotate_rate) ;
        break ;
    }
    glutPostRedisplay() ;
}

void special(int key, int x, int y) {
    switch(key) {
    
    case GLUT_KEY_LEFT :
        eyepos = eyepos - right * move_rate ;
        break ;
    case GLUT_KEY_RIGHT :
        eyepos = eyepos + right * move_rate ;
        break ;
    case GLUT_KEY_UP :
        eyepos = eyepos + look * move_rate ;
        break ;
    case GLUT_KEY_DOWN :
        eyepos = eyepos - look * move_rate;
        break ;
    case GLUT_KEY_PAGE_UP :
        eyepos = eyepos - up * move_rate;
        break ;
    case GLUT_KEY_PAGE_DOWN :
        eyepos = eyepos + up * move_rate;
        break ;
    }
    glutPostRedisplay() ;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv) ;                      // Initialize GLUT
    glutInitWindowSize(700, 700) ;               // Set the window's initial width & height
    glutInitWindowPosition(50, 50) ;             // Position the window's initial top-left corner
    glutCreateWindow("OpenGL 3D Drawing-Offline IDC") ;      // Create a window with the given title
    
    init() ;

    glutDisplayFunc(display) ; 
    glutKeyboardFunc(keyboard) ;                 
    glutSpecialFunc(special) ;                         
    
    
    glutMainLoop() ; 
                            
}