#pragma once
#include "point.h"
#include <GL/glut.h> 
#include<cmath>
#include "defs.h"

void drawAxes() {
    glLineWidth(4) ;
    glBegin(GL_LINES) ;
        glColor3f(0,0,1) ;
        glVertex3f(0,0,0) ;
        glVertex3f(1,0,0) ;
        
        glColor3f(0,1,0) ; 
        glVertex3f(0,0,0) ;
        glVertex3f(0,1,0) ;

        glColor3f(1,0,0) ;
        glVertex3f(0,0,0) ;
        glVertex3f(0,0,1) ;
    glEnd() ;
}

void drawPlane(GLfloat angle,int color_idx) {
   
    glPushMatrix() ;
    glRotatef(angle,0,1,0) ; //Rotate along y axis
    
    point t = center * scaling ;
    // X = x + t.x , Y = y + t.y , Z = z + t.z
    glTranslatef(t.x,t.y,t.z) ; // translating to change the co-ordinate, Only scaling will scale refering to the center
    glScalef(1-scaling,1-scaling,1-scaling) ; //scale it now
        
        glBegin(GL_TRIANGLES);
            GLfloat *color_code = octahedralPlaneColors[color_idx] ;
            glColor3fv(color_code);

            //Given in question

            glVertex3f(1,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,1);
        glEnd(); 
    glPopMatrix() ;
}

void drawOctaHedral() {
    //8 faces
    //first 4 ---> like pyramid 
    //rotate only 90 degree

    for(int i = 0 ; i < 4 ; i++) {
        drawPlane(GLfloat(i * 90),i) ;
    }

    glPushMatrix() ;
        glRotatef(180,1,0,0) ;
        for(int i = 0 ; i < 4 ; i++) {
            drawPlane(GLfloat(i*90),i+4) ;
        }
    glPopMatrix() ;
}


void drawCylinder() {
    GLfloat height = sqrt(2) *(1-scaling) ; 
    GLfloat radius = (1.0/sqrt(3)) * scaling ;
    int segments = 100 ;

    glTranslatef((1-scaling)/sqrt(2),0,0) ;
    //angle is acos(0.33)
    glRotatef(-70.5287794/2,0,0,1) ;

    //in the beginning
    GLfloat temp_x = radius ; 
    GLfloat temp_y = 0 ;

    GLfloat x, y ;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            GLfloat theta_2 = i * theta / segments ;
            x = radius * cos(theta_2) ;
            y = radius * sin(theta_2)  ;

            glVertex3f(x,y ,height/2) ;
            glVertex3f(x,y ,-height/2) ; 

            glVertex3f(temp_x, temp_y ,-height/2) ;
            glVertex3f(temp_x,temp_y , height/2) ;

            //join the points
            //use the previous x,y
            temp_x = x ;
            temp_y = y ;
        }
    glEnd() ;
}

void drawCylinder_v2(GLfloat angle, GLfloat x , GLfloat y , GLfloat z) {
    for(int i = 0; i < 4; i++) {
        glPushMatrix() ;
        drawCylinder() ;
        glPopMatrix() ;
        glRotatef(angle,x,y,z) ;
    }
}
void drawAllCylinders() {

    
    glColor3f(0.31, 0.365, 0.184);

    glPushMatrix();
    glRotatef(45,0,1,0); //rotate along y axis
    drawCylinder_v2(90,0,1,0) ;
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,1,0,0);  //rotate along x axis
    glRotatef(45,0,1,0); //rotate along y axis
    drawCylinder_v2(90,0,1,0) ;
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,0,1);  //rotate along z axis
    glRotatef(45,0,1,0); //rotate along y axis
    drawCylinder_v2(90,0,1,0) ;
    glPopMatrix();
}


point** generatePoints(int stacks,int slices,double radius) {
    point** points = new point*[stacks+1] ;
    for(int i = 0 ; i <= stacks ; i++) {
        points[i] = new point[slices+1] ;
        double r =  radius * cos(((double)i/(double)stacks)*2*M_PI) ;
        double h =  radius * sin(((double)i/(double)stacks)*2*M_PI) ;
        for(int j = 0 ; j <= slices ; j++) {
            points[i][j].x = r * cos(((double)j/(double)slices)*2*M_PI);
            points[i][j].y = r * sin(((double)j/(double)slices)*2*M_PI);
            points[i][j].z = h ;
        }
    }

    return points ;
}

point** buildUnitPositiveX(int subdivision)
{
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    point **points = new point *[pointsPerRow];
    for (int i = 0; i < pointsPerRow; ++i)
        points[i] = new point[pointsPerRow];

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i) {
    // normal for latitudinal plane
    // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
    // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = M_PI / 4.0 - (M_PI / 2.0 * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-r.)
        for (unsigned int j = 0; j < pointsPerRow; ++j) {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = -M_PI / 4.0 + (M_PI / 2.0 * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float norm_scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= norm_scale;
            v[1] *= norm_scale;
            v[2] *= norm_scale;

            // add a vertex into array
            points[i][j].x = v[0] * 1/sqrt(3);
            points[i][j].y = v[1] * 1/sqrt(3);
            points[i][j].z = v[2] * 1/sqrt(3);
        }
    }
  return points;
}

void drawSphere(int subdivision) {
    point** spherePoints = buildUnitPositiveX(6);
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    glBegin(GL_QUADS);
    for(int i = 0; i < pointsPerRow - 1; i++) {
        for(int j = 0; j < pointsPerRow - 1; j++) {
            glVertex3f(spherePoints[i][j].x, spherePoints[i][j].y, spherePoints[i][j].z);
            glVertex3f(spherePoints[i][j+1].x, spherePoints[i][j+1].y, spherePoints[i][j+1].z);
            glVertex3f(spherePoints[i+1][j+1].x, spherePoints[i+1][j+1].y, spherePoints[i+1][j+1].z);
            glVertex3f(spherePoints[i+1][j].x, spherePoints[i+1][j].y, spherePoints[i+1][j].z);
        }
    }
    glEnd();
}

void drawSphere_v2() {
    glTranslatef(1.0-scaling, 0.0, 0.0);
    glScalef(scaling,scaling,scaling);
    drawSphere(6);
}

void drawAllSpheres() {

    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    drawSphere_v2() ;
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(180.0, 0, 1, 0);
    drawSphere_v2() ;
    glPopMatrix();
   
    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix();
    glRotatef(90.0, 0, 1, 0);
    drawSphere_v2() ;
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(270.0, 0, 1, 0);
    drawSphere_v2() ;
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRotatef(270.0, 0, 0, 1);
    drawSphere_v2() ;
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(90.0, 0, 0, 1);
    drawSphere_v2() ;
    glPopMatrix();
}



//Rolliing Sphere offline task-2

void square(double a) {
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();
}

void drawBoundaries(double a) {
	double h = 4;
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f( a,-a,h);
		glVertex3f( a,a,h);
	}glEnd();

    glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f(-a, a,0);
		glVertex3f(-a, a,h);
		glVertex3f( a, a,h);
	}glEnd();

    glBegin(GL_QUADS);{
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
		glVertex3f(-a, a,h);
		glVertex3f(-a,-a,h);
	}glEnd();

	glBegin(GL_QUADS);{
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f( -a,-a,h);
		glVertex3f( a,-a,h);
	}glEnd();
}


void drawRollingSphere(int stacks,int slices,double radius) {
    point** points = generatePoints(stacks,slices,radius) ;
    for(int i = 0; i < stacks ; i++) {
        int c_type = 0 ;
        for(int j = 0; j < slices ; j++) {
            glColor3fv(ballColor[c_type ^ 1]) ;
            glBegin(GL_QUADS);
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z) ;
                glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z) ;
                glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z) ;
                glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z) ;
            glEnd() ;

            glColor3fv(ballColor[c_type]) ;
            glBegin(GL_QUADS) ;
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z) ;
                glVertex3f(points[i][j+1].x, points[i][j+1].y, -points[i][j+1].z) ;
                glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z) ;
                glVertex3f(points[i+1][j].x, points[i+1][j].y, -points[i+1][j].z) ;
            glEnd() ;
            (j%(slices/8) == 0) ? c_type ^= 1 : c_type = c_type ;
        }
    }
    glEnd();
}

void drawArrow(point& currPos , double ball_theta) {
    glColor3f(0,0,1);
    glPushMatrix();
    glTranslatef(currPos.x, currPos.y, currPos.z);
    glRotatef(ball_theta , 0,0,1) ;
    glLineWidth(5);
    glBegin(GL_LINES);{
        glVertex3f(0,0,0);
        glVertex3f(8, 0,0);
    }glEnd();
    glBegin(GL_TRIANGLES);{
        glVertex3f(8,0,0);
        glVertex3f(6, 2, 0);
        glVertex3f(6, -2, 0);
    }glEnd();
    glLineWidth(1);
    glPopMatrix();
}

void drawSquares(double l) {
    glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(0,0,-3);
	glRotatef(0,0,0,1);
	drawBoundaries(l);
	glPopMatrix();
}

void drawBnWGrid(int n ,int l){
	for(int i = 0; i < n ; i++){
        GLfloat x = l * sqrt(2) + i * 2 * l *sqrt(2) ;
		glPushMatrix();
            glTranslatef(x, 0, 0);
            glColor3f(1.,1.,1.);
            for(int j= 0; j < n ; j++){		
                GLfloat y = j * l * 2 ;
                glPushMatrix();
                    glRotatef(45 , 0, 0, 1);
                    glTranslatef(y, y, 0);
                    square(l);
                glPopMatrix();   

                glPushMatrix();
                    glRotatef(45 , 0, 0, 1);
                    glTranslatef(-y,-y, 0);
                    square(l);
                glPopMatrix();   
            }
		glPopMatrix();


		glPushMatrix();
            glTranslatef(-x, 0, 0);
            glColor3f(1.,1.,1.);
            for(int j= 0; j < n ; j++){		
                GLfloat y = j * l * 2 ;
                glPushMatrix();
                    glRotatef(45 , 0, 0, 1);
                    glTranslatef(y, y, 0);
                    square(l);
                glPopMatrix();   

                glPushMatrix();
                    glRotatef(45 , 0, 0, 1);
                    glTranslatef(-y,-y, 0);
                    square(l);
                glPopMatrix();   
            }
		glPopMatrix();
	}
}