
#pragma once
#include<bits/stdc++.h>
#include<cmath>
#include"ball.h"
#include"geometry.h"
#include"defs.h"
#include"point.h"
int MODE =  0 ;
point prevPos(0,0,0) ;
point currPos(0,0,0) ;

double boundary = 10 ;
int rotate = 0 ;
double ball_theta = 45 ;
double initangle = 45 ;
int prevColision = -1 ;
int prevForward = 0 ;

Rolling_ball ball ;

int check_boundary(double x , double y) {
    double a = x + y - boundary ;
	double b = -x + y -boundary ;
	double c = x - y - boundary ;
	double d = -x - y - boundary ;

    // printf("%lf, %lf,%lf, %lf, %lf, %lf, %lf\n",x,y, a, b, c,d, boundary);
    if(a*boundary > abs(1))	{
        std::cout << "Collison here1\n" ;
        return 1 ;
    }
    if(d*boundary > abs(1))	{
        std::cout << "Collison here3\n" ;
        return 3 ;
    }
    if(b*boundary > abs(1))	{
        std::cout <<"Collison is wall 2\n" ;
        return 2 ;
    }
    if(c*boundary > abs(1))	{
        std::cout <<"Collison 4\n" ;
        return 4 ;
    }

	return 0 ;
} 
int check(point& currPos){
    int a = check_boundary(currPos.x+ball.radius,currPos.y) ;
	if(a) return a ; 
	int b = check_boundary(currPos.x-ball.radius,currPos.y) ;
	if(b)	return b ;
	int c = check_boundary(currPos.x,currPos.y-ball.radius) ;
	if(c)	return c ;
	int d = check_boundary(currPos.x,currPos.y+ball.radius) ;
	if(d)	return d ;

	return 0 ;
}

double updateAngle(int flag) {
    double temp_theta = ball_theta ;
    if(prevForward == -1) temp_theta = temp_theta + 180 ;
    if(temp_theta > 360) temp_theta = temp_theta - 360 ;


    point temp = prevPos ;
    prevPos = currPos ;
    point vec_b = temp - currPos ;
    point lol(0,0,0) ;
    if(flag == 1 || flag ==3) {
        lol.x = -currPos.y ;
        lol.y = -currPos.x ;
        // std::cout <<"Coming here yaya\n" ;
    } 

    else if(flag == 2 || flag == 4) {
        lol.x = currPos.y ;
        lol.y = currPos.x ;
        // std::cout <<"Coming here?\n" ;
    } 

    point reflection_axes = lol - currPos ;

    double val1 = sqrt(vec_b.x*vec_b.x + vec_b.y*vec_b.y) ;
    double val2 = sqrt(reflection_axes.x*reflection_axes.x + reflection_axes.y*reflection_axes.y) ;
    double dot = vec_b.x*reflection_axes.x + vec_b.y*reflection_axes.y ;
    // std::cout <<"DOt -----> " << dot <<'\n' ;
    // std::cout <<"Val1*val2----->> " << val1*val2 <<'\n' ;
    float eng = dot*1.0/(val1*val2);
    eng = acos(eng) ;
    eng = (180*eng)/M_PI ;
    if(eng > 90) eng = 180-eng ;
    // if(eng == 90) eng = eng-1 ;
    // std::cout << eng <<"angle LK\n" ;
    if(flag == 1) {
        if(temp_theta >= initangle && temp_theta <= 180)
            temp_theta = temp_theta + (180 - 2*eng) ;
        else if(temp_theta < initangle || temp_theta >180) {
            temp_theta = temp_theta + (180 + 2 * eng) ;
        }
    }
    if(flag == 2) {
        if(temp_theta >= initangle && temp_theta <= 90+initangle)
            temp_theta = temp_theta + (180 + 2*eng) ;
        else if(temp_theta > 90+initangle  || temp_theta > 180) {
            temp_theta = temp_theta + (180 - 2 * eng) ;
        }
    }
    if(flag == 3) {
        if(temp_theta <= 180+initangle)
            temp_theta = temp_theta + (180 + 2*eng) ;
        else if(temp_theta >180+initangle) {
            temp_theta = temp_theta + (180 - 2 * eng) ;
        }
    }
    if(flag == 4) {
        if(temp_theta > initangle && temp_theta <= 270+initangle)
            temp_theta = temp_theta + (180 + 2*eng) ;
        else if(temp_theta > 270+initangle || temp_theta <=initangle) {
            temp_theta = temp_theta + (180 - 2 * eng) ;
        }
    }
    std::cout << "Current angle after collsion: " << temp_theta << '\n' ;
    return temp_theta ;
}

void move(int forward) {
    prevForward = forward ;
    point temp = prevPos ;
    int flag = check(currPos) ;
    double temp_theta = ball_theta ;

    if(prevForward == -1) temp_theta = temp_theta + 180 ;
    if(temp_theta > 360) temp_theta = temp_theta - 360 ;

    if(flag)
        temp_theta = updateAngle(flag) ;

    
    double x = ball.velocity * cos(temp_theta * M_PI / 180.0) ;
    double y = ball.velocity * sin(temp_theta * M_PI / 180.0) ;
    currPos = currPos + point(x,y,0) ;
    rotate = rotate + forward * 30 ;
    ball_theta = temp_theta ;
    if(prevForward == -1) ball_theta = ball_theta + 180 ;
    if(ball_theta > 360) ball_theta = ball_theta - 360 ;
}

void init()
{
    glMatrixMode(GL_PROJECTION) ;
    gluPerspective(80, 1, 1, 1000) ;
    eyepos.x = 0 ; eyepos.y = -80 ; eyepos.z = 60 ;
    look.x = -3 ; look.y = -3 ; look.z = -3 ;
    up.x = 0 ; up.y = 1 ; up.z =   1 ;
    right.x = 10 ; right.y = 0 ; right.z = 0 ;

}

void display() {      

    glEnable(GL_DEPTH_TEST) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glMatrixMode(GL_MODELVIEW) ;             // To operate on Model-View matrix

    glLoadIdentity() ;                       
    // camera

    GLfloat d = sqrt(eyepos.x * eyepos.x +eyepos.y
                * eyepos.y + eyepos.z*eyepos.z) ;
    gluLookAt(eyepos.x, eyepos.y, eyepos.z,look.x,
                look.y , look.z,
                up.x,up.y,up.z) ;
    double squareLength = 5 ;

    glRotatef(angle,0,0,1) ;
    // glPushMatrix() ;

	// grid
	// glPushMatrix();
	// glTranslatef(0,0,-3);
	// drawBlackAndWhiteGrid(squareLength);
	// glPopMatrix();

	// // square on grid
    drawSquares(6*squareLength) ;

    //Ball drawing
    glPushMatrix() ;
    glTranslatef(currPos.x, currPos.y,currPos.z);
	glRotatef(-rotate%360, sin(ball_theta*M_PI /180),-cos(ball_theta*M_PI/180),0);
	drawRollingSphere(20,24,3) ;
    glPopMatrix();
    boundary = 6*squareLength*sqrt(2);


    drawArrow(currPos, ball_theta) ;

    glutSwapBuffers() ;
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
    // case '.':
    //     scaling = std::min(scaling + 0.01, 1.0);
    //     break;
    // case ',':
    //     scaling = std::max(0.0, scaling - 0.01);
        // break;
        case 'j': case 'J':
            ball_theta = int(ball_theta + 20) % 360;
            prevPos = currPos ;
            std::cout << prevPos.x << " " << prevPos.y << " " << prevPos.z << '\n' ;
            break;

        case 'l': case 'L':
            ball_theta = int((ball_theta - 20+360))%360;
            prevPos = currPos ;
            std::cout << ball_theta <<"angel of ball\n" ;
            break;

        case 'i': case 'I':	

            if(!MODE) { 
                if(prevForward != 1) prevPos = currPos ;
                move(1) ;
            }
            break;

        case 'k': case 'K':
            if(!MODE)  {
                if(prevForward != -1) prevPos = currPos ;
                move(-1) ;
            }
            break;

        case ' ':
            MODE^= 1 ;
            prevPos = currPos ;
            break ;
        default:
            break ;
    }
    glutPostRedisplay() ;
    
}

void special(int key, int x, int y) {
    switch(key) {
    
        case GLUT_KEY_LEFT :
            eyepos = eyepos - (eyepos - right) * move_rate*10 ;
            break ;
        case GLUT_KEY_RIGHT :
            eyepos = eyepos + (eyepos + right) * move_rate*10 ;
            break ;
        case GLUT_KEY_UP :
            eyepos = eyepos - (eyepos - look) * move_rate*10 ;
            break ;
        case GLUT_KEY_DOWN :
            eyepos = eyepos + (eyepos -look) * move_rate*10 ;
            break ;
        case GLUT_KEY_PAGE_UP :
            eyepos = eyepos + up * move_rate *100 ;
            break ;
        case GLUT_KEY_PAGE_DOWN :
            eyepos = eyepos - up * move_rate *100 ;
            break ;
        default:
            break ;
    }
}

void simulation(){
    if(MODE == 1) {
        move(1) ;
    }
    glutPostRedisplay() ;

}

int main(int argc, char** argv) {
    glutInit(&argc, argv) ;                      // Initialize GLUT
    glutInitWindowSize(700, 700) ;               // Set the window's initial width & height
    glutInitWindowPosition(0, 0) ;             // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL 3D Drawing-Offline IDC") ;      // Create a window with the given title
    
    init() ;

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display) ; 
    glutIdleFunc(simulation) ;
    glutKeyboardFunc(keyboard) ;                 
    glutSpecialFunc(special) ;                         
    
    
    glutMainLoop() ; 
                            
}