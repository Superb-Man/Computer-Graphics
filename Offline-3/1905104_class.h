#pragma once
#include<bits/stdc++.h>
#include<vector>
#include <GL/glut.h> 
#include <GL/gl.h>
#include<cmath>


#define pi (2*acos(0.0))
int MAX_DEPTH ;
#define REC_REFLECTION 3

class point {
    public:
    double x, y, z ;
    point(double x = 0., double y = 0. , double z = 0.) {
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
    point operator+(point p) const{
        return point(x+p.x , y+p.y , z+p.z) ;
    }

    point operator-(point p) const{
        return point(x - p.x , y - p.y , z - p.z) ;
    }
    point operator*(point p) const{
        return point(y*p.z - z*p.y , z*p.x - x*p.z , x*p.y - y*p.x) ;
    }
    //-operator
    point operator-() const{
        return point(-x,-y,-z) ;
    }
    ~point(){

    }
    friend std::istream& operator>>(std::istream &in, point &p) {
        in >> p.x >> p.y >> p.z;
        return in;
    }


};

//From Slide 
// at first finding a vector which is perpenticular to both of
// axis and vector
// Then v = u sin(alpha) + v cos(alpha)
point rotation(point& axis, point& vector , double angle){
    point u = point(axis.y * vector.z - vector.y * axis.z ,
                    -axis.x * vector.z + vector.x * axis.z, axis.x * vector.y - axis.y * vector.x) ;
    vector = u * sin(angle) + vector * cos(angle) ;

    return vector ;
}

double dotProduct(point a, point b) {
    return a.x * b.x + a.y * b.y + a.z * b.z ;
}

double sqMagnitude(point p) {
    return (p.x * p.x + p.y * p.y + p.z * p.z) ;
}

point crossProduct(point axis,point vector) {
    point u = point(axis.y * vector.z - vector.y * axis.z ,
                    -axis.x * vector.z + vector.x * axis.z, axis.x * vector.y - axis.y * vector.x) ;
    return u ;
}

point normalize(point p) {
    double length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z) ;
    return point(p.x / length , p.y / length , p.z / length) ;
}

class Color {
    public:
    double r, g, b ;
    Color(double r= 0. , double g = 0. , double b = 0.) {
        this->r = r ;
        this->g = g ;
        this->b = b ;
    }
    Color operator*(double s) const{
        return Color(r*s,g*s,b*s) ;
    }
    Color operator/(double s) const{
        return Color(r/s,g/s,b/s) ;
    }
    Color operator+(Color c) const{
        return Color(r+c.r , g+c.g , b+c.b) ;
    }
    Color operator-(Color c) const{
        return Color(r - c.r , g - c.g , b - c.b) ;
    }
    Color operator*(Color c) const{
        return Color(r*c.r , g*c.g , b*c.b) ;
    }
    ~Color() {

    }
};

void drawSphere(point p,double rr, Color color) {
        glPushMatrix();

        glTranslatef(p.x, p.y, p.z);
        double radius = rr ;

        point points[100][100];
        double h,r;
        int stacks = 30, slices=20;
        //generate points
        for(int i = 0;i <= stacks; i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(int j = 0;j <= slices; j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        glColor3f(color.r, color.g,color.b);

        for(int i = 0;i < stacks;i++)
        {
            for(int j = 0;j < slices ; j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }
        glPopMatrix();

}

struct Ray{
    point start, dir ;
    //default constructor
    Ray() {

    }
    Ray(point start, point dir) {
        this->start = start ;
        this->dir = dir ;
        //need to normalize the direction
        this->dir = normalize(this->dir) ;
    }
    ~Ray() {

    }
};

struct  PointLight{
    point light_pos ;
    Color color ;
    //default constructor
    PointLight() {

    }
    PointLight(point light_pos,Color color) {
        this->light_pos = light_pos ;
        setColor(color) ;
    }

    void setColor(Color color) {
        // std::cout << "Color: " << color.r << " " << color.g << " " << color.b << std::endl ;
        this->color = color ;
    }
    void draw() {
        drawSphere(light_pos,0.5,this->color) ;
    }
    ~PointLight() {

    }

};

class SpotLight{
    public:
    PointLight pointLight ;
    point lightDirection ;
    double cutoffAngle ;

    //default constructor
    SpotLight() {

    }
    SpotLight(PointLight pointLight, point lightDirection, double cutoffAngle) {
        this->pointLight = pointLight ;
        this->lightDirection = lightDirection ;
        this->cutoffAngle = cutoffAngle ;
    }

    bool exceedCutOffAngle(Ray ray) {
        double angle = fabs(acos(dotProduct(ray.dir,this->lightDirection)/sqrt(sqMagnitude(ray.dir) * sqMagnitude(this->lightDirection)))) * 180. / pi ;
        if(angle > this->cutoffAngle) {
            return true ;
        }
        return false ;
    }

    void draw() {
        this->pointLight.draw() ;
    }

    ~SpotLight() {

    }


};

class Object{
    public:

    point referencePoint ; // should have x, y, z
    double height, width, length ;
    Color color ;
    double coEfficients[4] ;
    int shine ;
    Object() {

    }
    Object(point referencePoint,Color color , double height = 0.0, double width = 0.0 , double length = 0.0,int shine =0) {
        this->referencePoint = referencePoint ;
        this->height = height ;
        this->width = width ;
        this->length = length ;
        setShine(shine) ;
        setColor(color) ;
    }
    void setColor(Color color) {
        this->color = color ;
    }
    void setShine(double shine) {
        this->shine = shine ;
    }

    void setCoEfficients(double ambient, double diffuse, double specular, double reflection) {
        this->coEfficients[0] = ambient ;
        this->coEfficients[1] = diffuse ;
        this->coEfficients[2] = specular ;
        this->coEfficients[3] = reflection ;
    }



    ~Object() {

    }
    double intersect(Ray ray ,Color& color,int level) ;
    virtual void draw() = 0 ;
    virtual Color getColor(point p) = 0 ;
    virtual double getIntersectingT(Ray ray) = 0 ;
    virtual point getNormal(Ray ray ,point pointOfIntersection) = 0 ;
    void getColorByModel(double tCurrent, Ray ray, Color& finalColor, point intersectionPoint,
                                Color& intersectionColor, point normal) ;
};



std::vector<Object*> objects ;
std::vector<PointLight*> pointLights ;
std::vector<SpotLight*> spotLights ;

double Object::intersect(Ray ray,Color& color,int level = 0){
    // ray.dir = normalize(ray.dir) ;
    double t = this->getIntersectingT(ray) ;

    //if level is 0 return t
    if(t < 0 ) {
        return -1 ;
    }
    if(level == 0) {
        return t ;
    }

    // intersectionPoint = r->start + r->dir*t
    point intersectionPoint = ray.start + ray.dir * t ;
    // calculate normal at intersectionPoint
    Color intersectionColor = this->getColor(intersectionPoint) ;

    color = (intersectionColor * this->coEfficients[0]) ;
    //setting in range 0 to 1
    color.r = std::min(std::max(color.r,0.0),1.0) ;
    color.g = std::min(std::max(color.g,0.0),1.0) ;
    color.b = std::min(std::max(color.b,0.0),1.0) ;
    //fixRnage
    for(auto* pointLight : pointLights) {
        Ray ray_l(pointLight->light_pos,intersectionPoint - pointLight->light_pos) ;
        ray_l.dir = normalize(ray_l.dir) ;
        //get the normal at intersection point
        point normal = this->getNormal(ray_l,intersectionPoint) ;

        double tmin = sqrt(sqMagnitude(intersectionPoint - pointLight->light_pos));

        if(tmin < 1e-5) continue;
        bool oo = false ;
        for(Object *o: objects)
        {
            double tnow = o->getIntersectingT(ray_l);
            if(tnow > 0.0 && (tmin - tnow)  > 1e-5)
            {
                oo = true;
                break;
            }
        }
        if(!oo) {
            double lambert = dotProduct(normal,ray_l.dir) ;
            Ray ray_r(intersectionPoint,ray_l.dir - (normal * 2.0) * lambert) ;
            ray_r.dir = normalize(ray_r.dir) ;

            double phong = pow(std::max(dotProduct(ray.dir,-ray_r.dir),0.),this->shine) ;
            color = color + pointLight->color * coEfficients[1] * std::max(lambert,0.0) * intersectionColor ;
            //setting 0 to 1 range
            // color.r = std::min(std::max(color.r,0.0),1.0) ;
            // color.g = std::min(std::max(color.g,0.0),1.0) ;
            // color.b = std::min(std::max(color.b,0.0),1.0) ;
            color = color + pointLight->color * coEfficients[2] * phong * intersectionColor  ;
            //
            color.r = std::min(std::max(color.r,0.0),1.0) ;
            color.g = std::min(std::max(color.g,0.0),1.0) ;
            color.b = std::min(std::max(color.b,0.0),1.0) ;
        }
    }

    for(auto* spotLight : spotLights) {
        //do the same as pointlight loop
        Ray ray_l(spotLight->pointLight.light_pos,intersectionPoint - spotLight->pointLight.light_pos) ;
        ray_l.dir = normalize(ray_l.dir) ;
        point normal = this->getNormal(ray_l,intersectionPoint) ;

        if(spotLight->exceedCutOffAngle(ray_l)) {
            continue ;
        }
        double tmin = sqrt(sqMagnitude(intersectionPoint - spotLight->pointLight.light_pos));
        if(tmin < 1e-5) continue;
        bool oo = false ;
        for(Object *o: objects)
        {
            double tnow = o->getIntersectingT(ray_l);
            if(tnow > 0.0 && (tmin - tnow)  > 1e-5)
            {
                oo = true;
                break;
            }
        }
        if(!oo) {
            double lambert = dotProduct(normal,ray_l.dir) ;
            Ray ray_r(intersectionPoint,ray_l.dir - (normal * 2.0) * lambert) ;
            ray_r.dir = normalize(ray_r.dir) ;

            double phong = pow(std::max(dotProduct(ray.dir,-ray_r.dir),0.),this->shine) ;
            color = color + spotLight->pointLight.color * coEfficients[1] * std::max(lambert,0.0) * intersectionColor ;
            //
            // color.r = std::min(std::max(color.r,0.0),1.0) ;
            // color.g = std::min(std::max(color.g,0.0),1.0) ;
            // color.b = std::min(std::max(color.b,0.0),1.0) ;
            color = color + spotLight->pointLight.color * coEfficients[2] * phong * intersectionColor  ;
            //
            color.r = std::min(std::max(color.r,0.0),1.0) ;
            color.g = std::min(std::max(color.g,0.0),1.0) ;
            color.b = std::min(std::max(color.b,0.0),1.0) ;
        }
    }
    //reflection-recursive
    // if level ≥ recursion_level, return tmin
    if(level < MAX_DEPTH) {
        // std::cout <<"here\n" ; 
        point normal = this->getNormal(ray,intersectionPoint) ;
        // construct reflected ray from intersection point

        Ray reflectedRay(intersectionPoint,ray.dir - normal * 2.0 * dotProduct(ray.dir,normal)) ;

        // actually slightly forward from the point (by moving the
        // start a little bit towards the reflection direction)
        // to avoid self intersection
        reflectedRay.start = reflectedRay.start + reflectedRay.dir * (1e-5) ;
        reflectedRay.dir = normalize(reflectedRay.dir) ;


        Color reflected ;
        double tMin = 1e9 , t = -1.0  ;
        int idx = 0, num = -1 ;
        for(auto* object : objects) {
            // find tmin from the nearest intersecting object, using
            // intersect() method, as done in the capture() method
            // if found, call intersect(rreflected, colorreflected, level+1)

            t = object->intersect(reflectedRay,reflected,level+1) ;
            if(t > 0.0 and (tMin - t) >1e-5 ) {
                tMin = t ;
                num = idx ;
            }
            idx++ ;
        }

        if(num != -1) {
            // colorreflected will be updated while in the subsequent call
            // update color using the impact of reflection
            double t = objects[num]->intersect(reflectedRay,reflected,level+1) ;
            color = color + reflected * coEfficients[REC_REFLECTION] ;
            //fixing range
            color.r = std::min(std::max(color.r,0.0),1.0) ;
            color.g = std::min(std::max(color.g,0.0),1.0) ;
            color.b = std::min(std::max(color.b,0.0),1.0) ;
        }
    }



    return t ;


}

double det(double ara[3][3]) {
    double ret = 0.0 ;
    for(int i=0;i<3;i++) {
        double temp = 1.0 ;
        if(i%2 == 1) temp = -1.0 ;
        ret = ret + temp * (ara[0][i] * (ara[1][(i+1)%3] * ara[2][(i+2)%3] - ara[1][(i+2)%3] * ara[2][(i+1)%3])) ;
    }
    return ret ;
}

class Triangle : public Object {
public:
    point p1,p2,p3 ;
    //default Constructor
    Triangle() {

    }
    Triangle(point p1,point p2,point p3) {
        this->p1 = p1 ;
        this->p2 = p2 ;
        this->p3 = p3 ;
    }
    Color getColor(point p) {
        return this->color ;
    }

    void draw() {
        glPushMatrix() ;
        glColor3f(this->color.r,this->color.g,this->color.b) ;
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();

    }

    double getIntersectingT(Ray ray) {
        //find detBeta,detGamma,detT
        double detA = (p1.x - p2.x) * ((p1.y - p3.y) * ray.dir.z - (p1.z-p3.z) * ray.dir.y) - (p1.y - p2.y) * ((p1.x - p3.x) * ray.dir.z - (p1.z - p3.z) * ray.dir.x)
                                                +(p1.z - p2.z) * ((p1.x - p3.x) * ray.dir.y - (p1.y - p3.y) * ray.dir.x);

        if(abs(detA) < (1e-5)) {
            return -1.0;
        }

        double detBeta = (p1.x - ray.start.x) * ((p1.y - p3.y) * ray.dir.z - (p1.z-p3.z) * ray.dir.y) - (p1.y - ray.start.y) * ((p1.x - p3.x) * ray.dir.z - (p1.z - p3.z) * ray.dir.x) 
                                                + (p1.z - ray.start.z) * ((p1.x - p3.x) * ray.dir.y - (p1.y - p3.y) * ray.dir.x);

        double detGamma = (p1.x - p2.x) * ((p1.y - ray.start.y) * ray.dir.z - (p1.z-ray.start.z) * ray.dir.y)- (p1.y - p2.y) * ((p1.x - ray.start.x) * ray.dir.z - (p1.z - ray.start.z) * ray.dir.x)
                                                +(p1.z - p2.z) * ((p1.x - ray.start.x) * ray.dir.y - (p1.y - ray.start.y) * ray.dir.x);

        double detT = (p1.x - p2.x) * ((p1.y - p3.y) * (p1.z - ray.start.z) - (p1.z-p3.z) * (p1.y - ray.start.y)) - (p1.y - p2.y) * ((p1.x - p3.x) * (p1.z - ray.start.z) - (p1.z - p3.z) * (p1.x - ray.start.x))
                                                +(p1.z - p2.z) * ((p1.x - p3.x) * (p1.y - ray.start.y) - (p1.y - p3.y) * (p1.x - ray.start.x));

        double beta = detBeta / detA;
        double gamma = detGamma / detA;
        double t = detT / detA;

        if(beta < 0.0 || (beta-1.0) > (1e-5) || gamma < 0.0 || (gamma - 1.0) > (1e-5) || t < 0.0 || (beta+gamma-1.0)>(1e-5))
        {
            return -1;
        }

        return t;
    }

    point getNormal(Ray ray, point intersectionPoint) {
        point normal = normalize(crossProduct(p2-p1,p3-p1)) ;

        return dotProduct(ray.dir,normal) < 0. ? -normal : normal ;
    }


} ;

class Sphere : public Object{
public:
    Sphere() {

    }
    Sphere(point center , double radius) {
        this->referencePoint = center ;
        this->length = this->width = this->height = radius ;
    }

    Color getColor(point p) {
        return this->color ;
    }

    void draw(){
        drawSphere(this->referencePoint,this->length,this->color) ;
    }

    double getIntersectingT(Ray ray) {
        //from slide 
        point R0 = ray.start - this->referencePoint ;
        ray.dir = normalize(ray.dir) ;

        //Now find tP
        double tP = dotProduct(-R0,ray.dir) ;
        if(tP < 0.0){
            return -1.0 ;
        }
        //sqrt flaoting point prblem
        double d = sqMagnitude(R0) - tP*tP ;
        if((d - this->length * this->length) > 1e-5) {
            return -1.0 ;
        }
        double tPrime = sqrt(this->length * this->length - d) ;

        double t1 = tP + tPrime ;
        double t2 = tP - tPrime ;

        if((sqMagnitude(R0) - this->length * this->length) > 1e-5) {
            return (t2 >= 0.0) ? t2 : -1.0 ; 
        }
        else {
            return (t1 >= 0.0) ? t1 : -1.0 ;
        }  
    }

    point getNormal(Ray ray, point intersectionPoint) {
        point normal = normalize(intersectionPoint - this->referencePoint) ; 
        return normal * (-1.) ; 
    }
} ;

class Floor: public Object {

public:
    double fWidth,tWidth ;
    Color Xcolor ;

    //constructor
    Floor() {

    }
    Floor(double fWidth, double  tWidth) {
        this->fWidth = fWidth ;
        this->tWidth = tWidth ;
        this->referencePoint = point(-fWidth/2.0,-fWidth/2.0,0) ;
        this->length = this->fWidth ;

        //set color
        this->color = Color(1.,1.,1.) ;
        this->Xcolor = Color(0.,0.,0.0) ;
    }

    Color getColor(point p) {
        int x = (p.x - this->referencePoint.x) /  tWidth;
        int y = (p.y - this->referencePoint.y) / tWidth ;

        return ((x+y) % 2 == 0) ? this->color : this->Xcolor ;
    }

    void draw() {
        glPushMatrix();
        int n = this->fWidth / this->tWidth;
        for(int i=0;i<n;i++) {
            for(int j=0;j<n;j++)
            {
                double x = this->referencePoint.x + tWidth * i;
                double y = this->referencePoint.y + tWidth * j;

                (((i+j) & 1) == 0) ? glColor3f(color.r, color.g, color.b) :  glColor3f(Xcolor.r, Xcolor.g, Xcolor.b);

                glBegin(GL_QUADS); {
                    glVertex3f(x,y, 0);
					glVertex3f(referencePoint.x + (i + 1) * tWidth , referencePoint.y + j * tWidth, 0);
					glVertex3f(referencePoint.x + (i + 1) * tWidth, referencePoint.y + (j + 1) * tWidth, 0);
					glVertex3f(x, referencePoint.y + (j + 1) * tWidth, 0);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

    //ovrride the getIntersectingT
    double getIntersectingT(Ray ray) {
        point normal = point(0.,0.,1.) ;
        if(round(dotProduct(ray.dir,normal) *100) == 0.) 
            return -1. ;
        double t = dotProduct(-ray.start,normal) / dotProduct(ray.dir,normal) ;
        point p = ray.start + ray.dir * t ;
        if(p.x <= referencePoint.x || p.x >= abs(referencePoint.x) && p.y <= referencePoint.y && p.y >= abs(referencePoint.y)){
            return -1;
        }
        
        return t;
    }

    point getNormal(Ray ray, point intersectionPoint) {
        return (ray.start.z < 0.) ? point(0.,0.,-1.) :  point(0.,0.,1.) ;
    }
};
struct CoEff{
    double A,B,C,D,E,F,G,H,I,J ;
};

class General : public Object {

public:
    double A , B,C,D ,E,F,G,H,I,J ;
    General() {

    }
    General(double length,double width,double height,point referencePoint) {
        this->length = length ;
        this->width = width ;
        this->height = height ;
        this->referencePoint = referencePoint ;
    }
    point getNormal(Ray ray, point intersectionPoint) {
        point normal = point(2.0*A*intersectionPoint.x + D*intersectionPoint.y + E*intersectionPoint.z + G,
                            2.0*B*intersectionPoint.y + D*intersectionPoint.x + F*intersectionPoint.z + H,
                            2.0*C*intersectionPoint.z + E*intersectionPoint.x + F*intersectionPoint.y + I) ;
        return normalize(normal) ;
    }

    Color getColor(point p) {
        return this->color ;
    }

    bool check(point p) {
        if(abs(length) > 1e-5) {
            if(p.x < referencePoint.x || p.x > referencePoint.x + length) {
                return false ;
            }
        }
        if(abs(width) > 1e-5) {
            if(p.y < referencePoint.y || p.y > referencePoint.y + width) {
                return false ;
            }
        }
        if(abs(height) > 1e-5) {
            if(p.z < referencePoint.z || p.z > referencePoint.z + height) {
                return false ;
            }
        }

        return true ;
    }

    double getIntersectingT(Ray ray) {
        double a = A*ray.dir.x*ray.dir.x + B*ray.dir.y*ray.dir.y + C*ray.dir.z*ray.dir.z + D*ray.dir.x*ray.dir.y + E*ray.dir.x*ray.dir.z + F*ray.dir.y*ray.dir.z ;
        double b = 2.0*A*ray.start.x*ray.dir.x + 2.0*B*ray.start.y*ray.dir.y + 2.0*C*ray.start.z*ray.dir.z + D*ray.start.x*ray.dir.y + D*ray.start.y*ray.dir.x + E*ray.start.x*ray.dir.z + E*ray.start.z*ray.dir.x + F*ray.start.y*ray.dir.z + F*ray.start.z*ray.dir.y + G*ray.dir.x + H*ray.dir.y + I*ray.dir.z ;
        double c = A*ray.start.x*ray.start.x + B*ray.start.y*ray.start.y + C*ray.start.z*ray.start.z + D*ray.start.x*ray.start.y + E*ray.start.x*ray.start.z + F*ray.start.y*ray.start.z + G*ray.start.x + H*ray.start.y + I*ray.start.z + J ;

        double d = b*b - 4.0*a*c ;
        if(d < 0.0) {
            return -1.0 ;
        }
        double t1 = (-b + sqrt(d)) / (2.0*a) ;
        double t2 = (-b - sqrt(d)) / (2.0*a) ;

        if(t1 < 0.0 && t2 < 0.0) {
            return -1.0 ;
        }
        if(t2 < t1) {
            std::swap(t1,t2) ;
        }
        if(t1 > 0) {
            if(check(ray.start + ray.dir * t1)) {
                return t1 ;
            }
        }
        if(t2 > 0) {
            if(check(ray.start + ray.dir * t2)) {
                return t2 ;
            }
        }
        return -1.0 ;
    }

    void draw() {
        //draw the object
    }

    friend std::istream& operator>>(std::istream &in, General &g) {
        in >> g.A >> g.B >> g.C >> g.D >> g.E >> g.F >> g.G >> g.H >> g.I >> g.J;
        in >> g.referencePoint >> g.length >> g.width >> g.height;

        in >> g.color.r >> g.color.g >> g.color.b; // color
        for(int i = 0; i < 4; i++) in >> g.coEfficients[i] ;
        in >> g.shine;
        return in;

    }
};


