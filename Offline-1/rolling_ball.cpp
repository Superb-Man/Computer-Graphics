
#pragma once
#include<bits/stdc++.h>
#include<ctime>
#include<cmath>
#include"ball.h"
#include"geometry.h"
#include"defs.h"
#include"point.h"
int MODE =  0 ;
int next_time = 2000 ;

point prevPos(0,0,0) ;
point currPos(0,0,0) ;
double prevDistance = 0 ;

double boundary = 5*10 ;
int rotate = 0 ;
double ball_theta = 10 ;
double initangle = 0 ;
int prevColision = -1 ;
int prevForward = 0 ;

clock_t t1,t2 ;



int determine(double x, double y , double angle) ;
int getTime(double x , double y , double angle) ;

point normals[5] ;

Rolling_ball ball ;

int check_boundary(double x , double y) {
	if(x >= boundary || x <=-boundary) {
		if(x>=boundary) std::cout << "Wall 1 colide\n" ;
		else std::cout << "Wall 3 colide\n" ;
		return 1 ;
	}
	if(y <= -boundary || y >= boundary) {
		if(y>=boundary) std::cout << "Wall 2 colide\n" ;
		else std::cout << "Wall 4 colide\n" ;
		return 1 ;
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

int checkCoOrdinates(GLfloat x, GLfloat y) {
    if(x>=0 && y >=0) return 1 ;
    if(x<=0 && y >=0) return 2 ;
    if(x<0 && y <=0) return 3 ;
    if(x>=0 && y <=0) return 4 ;

	return 5 ;
}

double updateAngle(int flag) {
	t2 = clock() ;
    point temp = prevPos ;
    prevPos = currPos ;

    point vec_a = currPos -temp ;
    point reflection_axes = normals[flag] ;
    point n_opposite(0,0,0) ;
	n_opposite = currPos ;
	int c  = checkCoOrdinates(currPos.x,currPos.y) ;
	if(c){
		if(currPos.x+ball.radius > boundary || 
			currPos.x-ball.radius < -boundary)
				n_opposite.x = -currPos.x ;
		else n_opposite.y = -currPos.y ;
	}

    reflection_axes = n_opposite - currPos ;
    reflection_axes = normalize(reflection_axes) ;


    point reflection_vector ;
    reflection_vector = vec_a- reflection_axes * (2*vec_a.x*reflection_axes.x + 2*vec_a.y*reflection_axes.y) ;
    
    float eng = acos(reflection_vector.x/(sqrt(reflection_vector.x*reflection_vector.x + reflection_vector.y*reflection_vector.y))) ;
    eng = (180*eng)/M_PI ;
    
    c = checkCoOrdinates(reflection_vector.x,reflection_vector.y) ;
    if(c == 3 || c == 4) eng = 360 - eng ;

	t1 = t2 ;

    
    return eng ;
}

void move(int forward) {

    prevForward = forward ;
    point temp = prevPos ;
    int flag = check(currPos) ;
	if(int(ball_theta)%45 == 0 && (int(ball_theta)/45)%2) ball_theta+=1 ;
    double temp_theta = ball_theta ;

    if(prevForward == -1) temp_theta = temp_theta + 180 ;
    if(temp_theta >= 360) temp_theta = temp_theta - 360 ;

    if(flag)
        temp_theta = updateAngle(flag) ;

    
    double x = ball.velocity * cos(temp_theta * M_PI / 180.0) ;
    double y = ball.velocity * sin(temp_theta * M_PI / 180.0) ;

    currPos = currPos + point(x,y,0) ;

    double distance = sqrt((currPos.x - temp.x)*(currPos.x - temp.x) + (currPos.y - temp.y)*(currPos.y - temp.y)) ;

    rotate = (360*1.0*distance)/(2*M_PI*ball.radius) ;

    rotate*=forward ;
    
    ball_theta = temp_theta ;
    
    if(prevForward == -1) ball_theta = ball_theta + 180 ;
    if(ball_theta >= 360) ball_theta = ball_theta - 360 ;
}


//EVENT-DRIVEN 
//Bismillah

int determine(double x, double y , double angle){
	double vx = ball.velocity * cos(angle*M_PI/180) ;
	double vy = ball.velocity * sin(angle*M_PI/180) ;
	double b_x = boundary ;
	double b_y = boundary ;
	
    if(vx < 0) b_x = -boundary ;
	if(vy < 0) b_y = -boundary ;
	int wall = 0 ;
	
    double time_x =  abs(b_x-x)/abs(vx) ;
	double time_y =  abs(b_y-y)/abs(vy) ;
	
    if(time_x > time_y) {
		if(vy>0) wall = 2 ;
		else wall = 4 ;
	}
	else{
		if(vx>0) wall = 1 ;
		else wall = 3 ;
	}
	std::cout <<"Will hit on wall " << wall <<'\n' ;

	return wall ;

}

int getTime(double x , double y , double angle) {

    point dd = currPos - prevPos ;
    prevDistance = sqrt(dd.x*dd.x + dd.y*dd.y) ;

	double x2 = x + cos(angle*M_PI/180) ;
	double y2 = y + sin(angle*M_PI/180) ;
	int wall = determine(x,y,angle) ;
	
    double s_x = boundary ;
	double s_y = boundary ;
	
    if(wall == 1) s_x = boundary ;
	if(wall == 3) s_x = -boundary ;
	if(wall == 2) s_y = boundary ;
	if(wall == 4) s_y = -boundary ;
	if(wall ==1 || wall ==3) {
		s_y = -(x*y2 - y*x2 - s_x*sin(angle*M_PI/180))/cos(angle*M_PI/180);
	}
	else {
		s_x = (x*y2 - y*x2 + s_y*cos(angle*M_PI/180))/sin(angle*M_PI/180) ;
	
	}


    double per_Unit_d = next_time/prevDistance ;

	next_time = (sqrt((s_x-x)*(s_x-x) + (s_y-y)*(s_y-y))/ball.velocity) * per_Unit_d ;

	std::cout << time << "time\n" ;

	std::cout << "will colide at point near to " << s_x <<" " << s_y << '\n' ; 
	return 0 ;
}

void init()
{
	glMatrixMode(GL_PROJECTION) ;
    gluPerspective(80, 1, 1, 1000) ;
    eyepos.x = 0 ; eyepos.y = 80 ; eyepos.z = 60 ;
    look.x = 0 ; look.y = -40 ; look.z = -40 ;
    up.x = 0 ; up.y =  0 ; up.z = 1 ;
    right.x = 1 ; right.y = 0 ; right.z = 0 ;

}

void display() {      

    // glEnable(GL_DEPTH_TEST) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glClearColor(0,0,0,0) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glMatrixMode(GL_MODELVIEW) ;             // To operate on Model-View matrix

    glLoadIdentity() ;                       
    // camera
    GLfloat d = sqrt(eyepos.x * eyepos.x +eyepos.y
                * eyepos.y + eyepos.z*eyepos.z) ;
    gluLookAt(eyepos.x, eyepos.y, eyepos.z,eyepos.x + d*look.x,
                eyepos.y+d*look.y , eyepos.z + d*look.z,
                up.x,up.y,up.z) ;

    glMatrixMode(GL_MODELVIEW) ;
    double sq_length = 10 ;

    glRotatef(angle,0,0,1) ;
    // glPushMatrix() ;

	// grid
	glPushMatrix();
	glTranslatef(0,0,-3);
	drawBnWGrid(20,sq_length) ;
	glPopMatrix();

	// // square on grid
    glPushMatrix() ;
    drawSquares(5*sq_length) ;
    glPopMatrix() ;

    //Ball 
    glPushMatrix() ;
    glTranslatef(currPos.x, currPos.y,currPos.z);
	glRotatef(-rotate%360, sin(ball_theta*M_PI /180),-cos(ball_theta*M_PI/180),0);
	drawRollingSphere(ball.stacks,ball.slices,ball.radius) ;
    glPopMatrix();
    boundary = 5*sq_length ;


    drawArrow(currPos, ball_theta) ;

    glutSwapBuffers() ;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'j': case 'J':
            ball_theta = int(ball_theta + 20) % 360;
            prevPos = currPos ;
            if(MODE == 1)   getTime(currPos.x,currPos.y,ball_theta) ;
            break;

        case 'l': case 'L':
            ball_theta = int((ball_theta - 20+360))%360;
            prevPos = currPos ;
            if(MODE == 1)   getTime(currPos.x,currPos.y,ball_theta) ;
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
			getTime(currPos.x,currPos.y,ball_theta) ;
			t1 = clock() ;
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
        default:
            break ;
    }
    glutPostRedisplay() ;
    
}

void special(int key, int x, int y) {
    switch(key) {
    
        case GLUT_KEY_LEFT :
            eyepos = eyepos + right * move_rate*10 ;
            break ;
        case GLUT_KEY_RIGHT :
            eyepos = eyepos -  right * move_rate*10 ;
            break ;
        case GLUT_KEY_UP :
            eyepos = eyepos + look * move_rate ;
            break ;
        case GLUT_KEY_DOWN :
            eyepos = eyepos - look * move_rate ;
            break ;
        case GLUT_KEY_PAGE_UP :
            eyepos = eyepos + up * move_rate*100 ;
            break ;
        case GLUT_KEY_PAGE_DOWN :
            eyepos = eyepos - up * move_rate*100  ;
            break ;
        default:
            break ;
    }
    glutPostRedisplay() ;
}

void simulation(){
    if(MODE == 1) {
        move(1) ;
    }
    glutPostRedisplay() ;

}

void timer(int value) {
    updateAngle(1) ;
	getTime(currPos.x,currPos.y,ball_theta) ;
    //But doesnot working perfectly.
    //So for now avoiding this.
	std::cout << "TEL" << '\n' ;
	glutTimerFunc(next_time, timer, 0) ;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv) ;                      // Initialize GLUT
    glutInitWindowSize(700, 700) ;               // Set the window's initial width & height
    glutInitWindowPosition(0, 0) ;             // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("OpenGL 3D IDC n_opposite") ;      // Create a window with the given title
    
    init() ;

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display) ; 
    glutIdleFunc(simulation) ;
    glutKeyboardFunc(keyboard) ;                 
    glutSpecialFunc(special) ;                         
    timer(0) ;
    glutMainLoop() ; 
                            
}