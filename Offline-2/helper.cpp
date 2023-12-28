#pragma once
#include<bits/stdc++.h>
#include"matrix.h"
#include"utils.h"

point eye, look , up;
double fovY, aspectRatio, near, far;

Matrix triangleMatrix(point& p1 , point& p2, point& p3) {
    Matrix m(4,3);
    // m.set(0,0,p1.x);
    // m.set(0,1,p1.y);
    // m.set(0,2,p1.z);
    // m.set(1,0,p2.x);
    // m.set(1,1,p2.y);
    // m.set(1,2,p2.z);
    // m.set(2,0,p3.x);
    // m.set(2,1,p3.y);
    // m.set(2,2,p3.z);
    // m.set(3,0,1);
    // m.set(3,1,1);
    // m.set(3,2,1);
    //pajn
    m.makeColumn(0,{p1.x,p1.y,p1.z,1});
    m.makeColumn(1,{p2.x,p2.y,p2.z,1});
    m.makeColumn(2,{p3.x,p3.y,p3.z,1});

    return m;
}

Matrix translete(point& p1){
    Matrix m(4,4);
    // m.set(0,0,1);
    // m.set(0,1,0);
    // m.set(0,2,0);
    // m.set(0,3,p1.x);
    // m.set(1,0,0);
    // m.set(1,1,1);
    // m.set(1,2,0);
    // m.set(1,3,p1.y);
    // m.set(2,0,0);
    // m.set(2,1,0);
    // m.set(2,2,1);
    // m.set(2,3,p1.z);
    // m.set(3,0,0);
    // m.set(3,1,0);
    // m.set(3,2,0);
    // m.set(3,3,1);
    m.makeRow(0,{1,0,0,p1.x});
    m.makeRow(1,{0,1,0,p1.y});
    m.makeRow(2,{0,0,1,p1.z});
    m.makeRow(3,{0,0,0,1});

    return m;
}

Matrix scale(point& p1){
    Matrix m(4,4);
    m.set(0,0,p1.x);
    m.set(1,1,p1.y);
    m.set(2,2,p1.z);

    return m;
}

point R(point p, point a, double angle){
    //𝑅(𝑥⃗, 𝑎⃗, 𝜃) =𝑐𝑜𝑠 𝑐𝑜𝑠 𝜃 𝑥⃗ + (1 −𝑐𝑜𝑠 𝑐𝑜𝑠 𝜃 )(𝑎⃗ ∙ 𝑥⃗)𝑎⃗ +𝑠𝑖𝑛 𝑠𝑖𝑛 𝜃 (𝑎⃗ × 𝑥⃗)
    point r1 ;
    r1 =  p * cos(angle);
    r1 = r1 + (a * dotProduct(a,p)) * (1 - cos(angle));
    r1 = r1 + crossProduct(a,p) * sin(angle);
    return r1;
}

Matrix rotate(point& p1 , double angle){
    // a.normalize()
    // c1=R(i,a,angle)
    // c2=R(j,a,angle)
    // c3=R(k,a,angle)
    point x = normalize(p1);
    point c1 = R(point(1,0,0),x,angle);
    point c2 = R(point(0,1,0),x,angle);
    point c3 = R(point(0,0,1),x,angle);
    Matrix m(4,4);

    m.makeColumn(0,{c1.x,c1.y,c1.z,0});
    m.makeColumn(1,{c2.x,c2.y,c2.z,0});
    m.makeColumn(2,{c3.x,c3.y,c3.z,0});
    //m.makeColumn(3,{0,0,0,1});

    return m ;
}

// l = look - eye
// l.normalize()
// r = l X up
// r.normalize()
// u = r X l


void viewTransformation() {
    point l = look - eye;
    l = normalize(l);
    point r = crossProduct(l,up);
    r = normalize(r);
    point u = crossProduct(r,l);
    Matrix T(4,4);

    //Apply the following translation T to move the eye/camera to origin.
    T.makeColumn(3,{-eye.x,-eye.y,-eye.z,1});
    Matrix R(4,4);
    //Apply the following rotation R such that the l aligns with the -Z axis, r with X axis, and u with Y axis.
    //Remember that, the rows of the rotation matrix contain the unit vectors that align with the unit vectors
    //along the principal axes after transformation.

    R.makeRow(0,{r.x,r.y,r.z,0});
    R.makeRow(1,{u.x,u.y,u.z,0});
    R.makeRow(2,{-l.x,-l.y,-l.z,0});
    //now multiply R and T to get the view transformation matrix V.
    Matrix V = R * T;
    //need to add
    
}


// fovX = fovY * aspectRatio
// t = near * tan(fovY/2)
// r = near * tan(fovX/2)
void projectionTransformation(){
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY/2) * (3.1416/180) ;
    double r = near * tan(fovX/2) * (3.1416/180) ;
    Matrix P(4,4);

    P.makeRow(0,{near/r,0,0,0});
    P.makeRow(1,{0,near/t,0,0});
    P.makeRow(2,{0,0,-(far+near)/(far-near),-(2*far*near)/(far-near)});
    P.makeRow(3,{0,0,-1,0});
    
    //add new 
}