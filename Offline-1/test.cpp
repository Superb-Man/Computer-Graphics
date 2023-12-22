
// #pragma once
// #include<bits/stdc++.h>
// #include<cmath>
// #include"ball.h"
// #include"geometry.h"
// #include"defs.h"
// #include"point.h"
// int MODE =  0 ;
// point prevPos(0,0,0) ;
// point currPos(0,0,0) ;

// double boundary = 5*sqrt(2)*6 ;
// int rotate = 0 ;
// double ball_theta = 45 ;
// double initangle = 45 ;
// int prevColision = -1 ;
// int prevForward = 0 ;

// point normals[5] ;

// void getNormals() {
//     //for xy plane
//     point a  ={24,boundary-24.0,1} ;
//     point b = {16.5,boundary-16.5,2} ;
//     point c = {32,boundary-32.0,1} ;

//     normals[1] = crossProduct(c-a,b-a) ; 
//     double magnitude = sqrt(normals[1].x*normals[1].x + normals[1].y*normals[1].y + normals[1].z*normals[1].z) ;
//     normals[1] = normals[1]/magnitude ;

//     //for -xy
//     c = {-11,boundary+11.0,1} ;
//     a = {-27,boundary+27.0,1} ;
//     b = {-20,boundary+20.0,2} ;

//     normals[2] = crossProduct(c-a,b-a) ;
//     magnitude = sqrt(normals[2].x*normals[2].x + normals[2].y*normals[2].y + normals[2].z*normals[2].z) ;
//     normals[2] = normals[2]/magnitude ;

//     //for -x-y
//     // b = {-20,-20,2} ;
//     // a = {-25,-13,1} ;
//     // c = {-9,-30,1} ;

//     // normals[3] = crossProduct(b-a,c-a) ;
//     // magnitude = sqrt(normals[3].x*normals[3].x + normals[3].y*normals[3].y + normals[3].z*normals[3].z) ;
//     // normals[3] = normals[3]/magnitude ;
//     normals[3].x = -normals[1].y ;
//     normals[3].y = -normals[1].x ;

//     normals[4].x = normals[2].y ;
//     normals[4].y = normals[2].x ;

// }

// Rolling_ball ball ;

// int check_boundary(double x , double y) {
//     double a = x + y - boundary ;
// 	double b = -x + y -boundary ;
// 	double c = x - y - boundary ;
// 	double d = -x - y - boundary ;

//     // printf("%lf, %lf,%lf, %lf, %lf, %lf, %lf\n",x,y, a, b, c,d, boundary);
//     if(a*boundary > 0)	{
//         std::cout << "Collison here1\n" ;
//         return 1 ;
//     }
//     if(d*boundary > 0)	{
//         std::cout << "Collison here3\n" ;
//         return 3 ;
//     }
//     if(b*boundary > 0)	{
//         std::cout <<"Collison in wall 2\n" ;
//         return 2 ;
//     }
//     if(c*boundary > 0)	{
//         std::cout <<"Collison 4\n" ;
//         return 4 ;
//     }

// 	return 0 ;
// } 
// int check(point& currPos){
//     int a = check_boundary(currPos.x+ball.radius,currPos.y) ;
// 	if(a) return a ; 
// 	int b = check_boundary(currPos.x-ball.radius,currPos.y) ;
// 	if(b)	return b ;
// 	int c = check_boundary(currPos.x,currPos.y-ball.radius) ;
// 	if(c)	return c ;
// 	int d = check_boundary(currPos.x,currPos.y+ball.radius) ;
// 	if(d)	return d ;

// 	return 0 ;
// }

// int checkCoOrdinates(GLfloat x, GLfloat y) {
//     if(x>=0 && y >=0) return 1 ;
//     if(x<0 && y >=0) return 2 ;
//     if(x<0 && y <0) return 3 ;
//     if(x>=0 && y <0) return 4 ;
// }

// double updateAngle(int flag) {

//     point temp = prevPos ;
//     prevPos = currPos ;

//     point vec_b = currPos -temp ;
//     point reflection_axes = normals[flag] ;
//     point lol(0,0,0) ;
//     if(flag == 1 || flag ==3) {
//         lol.x = -currPos.y ;
//         lol.y = -currPos.x ;
//     } 

//     else if(flag == 2 || flag == 4) {
//         lol.x = currPos.y ;
//         lol.y = currPos.x ;
//     } 
//     reflection_axes = lol - currPos ;
//     reflection_axes = normalize(reflection_axes) ;


//     point reflection_vector ;
//     reflection_vector = vec_b- reflection_axes * (2*vec_b.x*reflection_axes.x + 2*vec_b.y*reflection_axes.y) ;
//     float eng = acos(reflection_vector.x/(sqrt(reflection_vector.x*reflection_vector.x + reflection_vector.y*reflection_vector.y))) ;
//     eng = (180*eng)/M_PI ;

//     int c = checkCoOrdinates(reflection_vector.x,reflection_vector.y) ;
//     if(c == 3 || c == 4) eng = 360 - eng ;
//     return eng ;
// }

// void move(int forward) {
//     prevForward = forward ;
//     point temp = prevPos ;
//     int flag = check(currPos) ;
//     double temp_theta = ball_theta ;

//     if(prevForward == -1) temp_theta = temp_theta + 180 ;
//     if(temp_theta > 360) temp_theta = temp_theta - 360 ;

//     if(flag)
//         temp_theta = updateAngle(flag) ;

    
//     double x = ball.velocity * cos(temp_theta * M_PI / 180.0) ;
//     double y = ball.velocity * sin(temp_theta * M_PI / 180.0) ;
//     currPos = currPos + point(x,y,0) ;
//     rotate = rotate + forward * 30 ;
//     ball_theta = temp_theta ;
//     if(prevForward == -1) ball_theta = ball_theta + 180 ;
//     if(ball_theta > 360) ball_theta = ball_theta - 360 ;
// }

// void init()
// {
//     glMatrixMode(GL_PROJECTION) ;
//     gluPerspective(80, 1, 1, 1000) ;
//     eyepos.x = 0 ; eyepos.y = 80 ; eyepos.z = 60 ;
//     look.x = 0 ; look.y = -40 ; look.z = -40 ;
//     up.x = 0 ; up.y =  0 ; up.z = 1 ;
//     right.x = 1 ; right.y = 0 ; right.z = 0 ;

// }

// void display() {      

//     // glEnable(GL_DEPTH_TEST) ;
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
//     glClearColor(0,0,0,0) ;
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
//     glMatrixMode(GL_MODELVIEW) ;             // To operate on Model-View matrix

//     glLoadIdentity() ;                       
//     // camera

//     // GLfloat d = sqrt(eyepos.x * eyepos.x +eyepos.y
//     //             * eyepos.y + eyepos.z*eyepos.z) ;
//     // gluLookAt(eyepos.x, eyepos.y, eyepos.z,look.x,
//     //             look.y , look.z,
//     //             up.x,up.y,up.z) ;
//     GLfloat d = sqrt(eyepos.x * eyepos.x +eyepos.y
//                 * eyepos.y + eyepos.z*eyepos.z) ;
//     gluLookAt(eyepos.x, eyepos.y, eyepos.z,eyepos.x + d*look.x,
//                 eyepos.y+d*look.y , eyepos.z + d*look.z,
//                 up.x,up.y,up.z) ;

//     glMatrixMode(GL_MODELVIEW) ;
//     double sq_length = 5 ;

//     glRotatef(angle,0,0,1) ;
//     // glPushMatrix() ;

// 	// grid
// 	glPushMatrix();
// 	glTranslatef(0,0,-3);
// 	drawBnWGrid(20,sq_length) ;
// 	glPopMatrix();

// 	// // square on grid
//     glPushMatrix() ;
//     drawSquares(6*sq_length) ;
//     glPopMatrix() ;

//     //Ball drawing
//     glPushMatrix() ;
//     glTranslatef(currPos.x, currPos.y,currPos.z);
// 	glRotatef(-rotate%360, sin(ball_theta*M_PI /180),-cos(ball_theta*M_PI/180),0);
// 	drawRollingSphere(20,24,3) ;
//     glPopMatrix();
//     boundary = 6*sq_length*sqrt(2);


//     drawArrow(currPos, ball_theta) ;

//     glutSwapBuffers() ;
// }
//             //             up
//             //              |    / (look at the point)
//             //              |   /
//             //              |  /
//             //              | /
//             //              0----------(right)
//             //             /
//             //            /
//             //           /
//             //          /     
//             //
// void keyboard(unsigned char key, int x, int y) {
//     switch (key) {
//     // case '.':
//     //     scaling = std::min(scaling + 0.01, 1.0);
//     //     break;
//     // case ',':
//     //     scaling = std::max(0.0, scaling - 0.01);
//         // break;
//         case 'j': case 'J':
//             ball_theta = int(ball_theta + 20) % 360;
//             prevPos = currPos ;
//             std::cout << prevPos.x << " " << prevPos.y << " " << prevPos.z << '\n' ;
//             break;

//         case 'l': case 'L':
//             ball_theta = int((ball_theta - 20+360))%360;
//             prevPos = currPos ;
//             std::cout << ball_theta <<"angel of ball\n" ;
//             break;

//         case 'i': case 'I':	

//             if(!MODE) { 
//                 if(prevForward != 1) prevPos = currPos ;
//                 move(1) ;
//             }
//             break;

//         case 'k': case 'K':
//             if(!MODE)  {
//                 if(prevForward != -1) prevPos = currPos ;
//                 move(-1) ;
//             }
//             break;

//         case ' ':
//             MODE^= 1 ;
//             prevPos = currPos ;
//             break ;
        
//         case '1' :
//         //rotate left
//         rotation(up, look, -rotate_rate) ;
//         rotation(up, right, -rotate_rate) ;
//         break ;
//         case '2' :
//             //rotate right
//             rotation(up, look, rotate_rate) ;
//             rotation(up, right, rotate_rate) ;
//             break ;
//         case '3' :
//             //rotate up
//             rotation(right, look, rotate_rate) ;
//             rotation(right, up, rotate_rate) ;
//             break ;
//         case '4' :
//             //rotate down
//             rotation(right, look, -rotate_rate) ; 
//             rotation(right, up, -rotate_rate) ;
//             break ;
//         case '5':
//             //clockwise
//             rotation(look, right, rotate_rate) ;
//             rotation(look, up, rotate_rate) ;
//             break ;
//         case '6' :
//             //anticlock
//             rotation(look, right, -rotate_rate) ;
//             rotation(look, up, -rotate_rate) ;
//             break ;
//         default:
//             break ;
//     }
//     glutPostRedisplay() ;
    
// }

// void special(int key, int x, int y) {
//     switch(key) {
    
//         case GLUT_KEY_LEFT :
//             eyepos = eyepos + right * move_rate*10 ;
//             break ;
//         case GLUT_KEY_RIGHT :
//             eyepos = eyepos -  right * move_rate*10 ;
//             break ;
//         case GLUT_KEY_UP :
//             eyepos = eyepos + look * move_rate ;
//             break ;
//         case GLUT_KEY_DOWN :
//             eyepos = eyepos - look * move_rate ;
//             break ;
//         case GLUT_KEY_PAGE_UP :
//             eyepos = eyepos + up * move_rate*100 ;
//             break ;
//         case GLUT_KEY_PAGE_DOWN :
//             eyepos = eyepos - up * move_rate*100  ;
//             break ;
//         default:
//             break ;
//     }
//     glutPostRedisplay() ;
// }

// void simulation(){
//     if(MODE == 1) {
//         move(1) ;
//     }
//     glutPostRedisplay() ;

// }

// int main(int argc, char** argv) {
//     getNormals() ;
//     glutInit(&argc, argv) ;                      // Initialize GLUT
//     glutInitWindowSize(700, 700) ;               // Set the window's initial width & height
//     glutInitWindowPosition(0, 0) ;             // Position the window's initial top-left corner
//     glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
//     glutCreateWindow("OpenGL 3D Drawing-Offline IDC") ;      // Create a window with the given title
    
//     init() ;

//     glEnable(GL_DEPTH_TEST);

//     glutDisplayFunc(display) ; 
//     glutIdleFunc(simulation) ;
//     glutKeyboardFunc(keyboard) ;                 
//     glutSpecialFunc(special) ;                         
    
    
//     glutMainLoop() ; 
                            
// }