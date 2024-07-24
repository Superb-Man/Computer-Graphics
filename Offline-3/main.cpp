#pragma once
#include <GL/glut.h>  
#include <cmath>
#include <iostream>
#include <vector>
#include "1905104_class.h"
#include "bitmap_image.h"

void load() ;
void capture() ;
void clear() ;
void drawAxis() ;
void drawS() ;


Object* obj ;
extern int MAX_DEPTH ;
//import the OBjects , pointlight, spotlight from class.h
extern std::vector<Object*> objects ;
extern std::vector<PointLight*> pointLights ;
extern std::vector<SpotLight*> spotLights ;
int pixelWidth , pixelHeight ;


const double rotate_rate = 1 * pi / 180. ;
const double move_rate   = 1 ;

point eyepos(115. ,115., 50.) ;
point look(-1./sqrt(2) ,-1./sqrt(2), 0.) ;
point right(-1./sqrt(2) ,1./sqrt(2), 0.) ;
point up (0. ,0. ,1.) ;

int captures = 0 ;
int windowHeight = 500. ;
int windowWidth = 500. ;
double viewAngle = 80. ;
int drawGrid = 0 ;
int drawAxes = 1 ;
double angle = 0. ;
double cameraHeight = 40. ;
double cameraAngle = 1.32 ;


void load() {
    
    std::ifstream scene("scene.txt") ;

    if(!scene) {
        std::cout << "scene file not found" << std::endl ;
        return ;
    }

    int totalObjects ;
    Color color ;
    double diffuse, specular, reflection, ambient ;
    int shine ;

    //read if scene file is open
    if(scene.is_open()) {
        scene >> MAX_DEPTH ;
        scene >> pixelWidth ;
        pixelHeight = pixelWidth ;
        scene >> totalObjects ;
        Object* obj ;
        for(int i = 0 ; i < totalObjects ; i++) {
            std::string type ;
            scene >> type ;
            if(type == "sphere") {
                point center ;
                double radius ;
                scene >> center.x >> center.y >> center.z >> radius ;
                // objects.push_back(new Sphere(center , radius)) ;
                obj = new Sphere(center , radius) ;
                // std::cout << "Sphere" << std::endl ;
            }
            else if(type == "triangle") {
                point a, b, c ;
                scene >> a.x >> a.y >> a.z >> b.x >> b.y >> b.z >> c.x >> c.y >> c.z ;
                obj = new Triangle(a, b, c) ;
                // objects.push_back(new Triangle(a, b, c)) ;
            }
            else if(type == "general") {
                obj = new General();
                objects.push_back(obj) ;
                scene >> *((General *)obj);
                continue;   
            }
            scene >> color.r >> color.g >> color.b ;
            scene >> ambient >> diffuse >> specular >> reflection ;
            scene >> shine ;

            obj->setColor(color) ;
            obj->setCoEfficients(ambient, diffuse, specular, reflection) ;
            obj->shine = shine ;

            objects.push_back(obj) ;
        }

        int totPointLights = 0 ;
        scene >> totPointLights ;
        PointLight* pointLight ;

        for(int i = 0 ; i < totPointLights ; i++) {
            point pos ;
            scene >> pos.x >> pos.y >> pos.z ;
            scene >> color.r >> color.g >> color.b ;
            pointLight = new PointLight(pos,color) ;
            pointLight->setColor(color) ;
            pointLights.push_back(pointLight) ;
        }

        int totSpotLights = 0 ;
        scene >> totSpotLights ;
        SpotLight* spotLight ;

        for(int i = 0 ; i < totSpotLights ; i++) {
            point pos ,dir ;
            double cutOffangle, near, far ;

            scene >> pos.x >> pos.y >> pos.z ;
            scene >> color.r >> color.g >> color.b ;
            std::cout << color.r << " " << color.g << " " << color.b << std::endl ;
            scene >> dir.x >> dir.y >> dir.z ;
            scene >> cutOffangle  ;

            pointLight = new PointLight(pos,color) ;
            pointLight->setColor(color) ;
            spotLight = new SpotLight(*pointLight, dir, cutOffangle) ;
            spotLights.push_back(spotLight) ;

        }

        //floor
        obj = new Floor(400., 10.) ;
        obj->setCoEfficients(0.4, 0.4, 0.4, 0.2) ;
        obj->shine = 5 ;
        objects.push_back(obj) ;

        //close
        scene.close() ;

    }
}

void capture() {
    bitmap_image* image = new bitmap_image(pixelWidth, pixelHeight) ;


    for(int j = 0 ; j < (int)pixelHeight ; j++) {
        for(int i = 0 ; i < (int) pixelWidth ; i++) {
            image->set_pixel(j,i,0,0,0) ;
        }
    }

    double palneDistance = ((double) windowHeight/2.0) /tan(viewAngle / 2.0 * pi / 180.0) ;
    std::cout << palneDistance << std::endl ;
    point topleft = eyepos + look * palneDistance - right * ((double) windowWidth/2.0) + up * ((double) windowHeight/2.0) ;
    std::cout << topleft.x << " " << topleft.y << " " << topleft.z << std::endl ;
    double du = (double) windowWidth / (double) pixelWidth ;
    double dv = (double) windowHeight / (double) pixelHeight ;

    topleft = topleft + right*(du*0.5) - up*(dv*0.5) ;
    std::cout << "Output_1" << captures << ".bmp"  << std::endl ;
    double tMin, t ;
    for(int j = 0 ; j < pixelHeight ; j++) {
        for(int i = 0 ; i < pixelWidth ; i++) {
            point currPixel = topleft + right * (j * du) - up * (i * dv) ;
            Ray ray(eyepos, currPixel - eyepos) ;
            ray.dir = normalize(ray.dir) ;

            tMin = -1 ;
            Color color(0.,0.,0.) ;
            int idx = 0 , objNum = -1 ;

            for(auto* obj : objects) {
                t = obj->intersect(ray,color) ;
                // std::cout <<t <<'\n' ;
                if(t > 0. && (objNum == -1 || (tMin-t) > 1e-5)) {
                    tMin = t ;
                    objNum = idx ;
                }
                ++idx ;
            }
            // std::cout << objNum << std::endl ;
            if(objNum != -1) {
                objects[objNum]->intersect(ray, color,1) ;
                color.r = std::min(std::max(color.r,0.0),1.0) ;
                color.g = std::min(std::max(color.g,0.0),1.0) ;
                color.b = std::min(std::max(color.b,0.0),1.0) ;
                image->set_pixel(j,i,color.r*255.,color.g*255.,color.b*255.) ;
            }
        }
    }
    image->save_image("Output_1" + std::to_string(captures) + ".bmp") ;
    delete image ;

    std::cout << "<<-----------Toriqe_104------------>>"  << std::endl ;

}

void clear() {
    std::cout << "clear called" <<'\n' ;
    for(auto* obj : objects) {
        delete obj ;
    }
    objects.clear() ;

    for(auto* pointLight : pointLights) {
        delete pointLight ;
    }
    pointLights.clear() ;

    for(auto* spotLight : spotLights) {
        delete spotLight ;
    }
    spotLights.clear() ;
}

void drawAxis() {
    if(drawAxes == 1) {
        glBegin(GL_LINES) ;
        glColor3f(1.,0.,0.) ;
        glVertex3f(100.,0.,0.) ;
        glVertex3f(-100.,0.,0.) ;
        glColor3f(0.,1.,0.) ;
        glVertex3f(0.,-100,0.) ;
        glVertex3f(0.,100,0.) ;
        glColor3f(0.,0.,1.) ;
        glVertex3f(0.,0.,100) ;
        glVertex3f(0.,0.,-100) ;
        glEnd() ;
    }
}
void drawS() {
    for(Object* obj : objects) {
        obj->draw() ;
    }
    for(PointLight* pointLight : pointLights) {
        pointLight->draw() ;
    }
    for(SpotLight* spotLight : spotLights) {
        spotLight->draw() ;
    }
}

void init()
{   

    //load the data at initialization
    load() ;
    glClearColor(0,0,0,0) ;
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective(80, 1, 1, 1000.) ;

}

void display() {      

    // glEnable(GL_DEPTH_TEST) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glClearColor(0.,0.,0.,0.) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glMatrixMode(GL_MODELVIEW) ;             // To operate on Model-View matrix

    glLoadIdentity() ;                       
    // // camera

    double d = sqrt(eyepos.x * eyepos.x +eyepos.y
                * eyepos.y + eyepos.z*eyepos.z) ;
    gluLookAt(eyepos.x, eyepos.y, eyepos.z,eyepos.x + d*look.x,
                eyepos.y+d*look.y , eyepos.z + d*look.z,
                up.x,up.y,up.z) ;
    // gluLookAt(eyepos.x, eyepos.y, eyepos.z,eyepos.x + look.x,
    //             eyepos.y+look.y , eyepos.z + look.z,
    //             up.x,up.y,up.z) ;

    glMatrixMode(GL_MODELVIEW) ;
    drawAxis() ;
    drawS() ;

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
    case 'v':
        captures++;
        capture();
        break;
    case '1' :
        //rotate left
        rotation(up, look, rotate_rate) ;
        rotation(up, right, rotate_rate) ;
        break ;
    case '2' :
        //rotate right
        rotation(up, look, -rotate_rate) ;
        rotation(up, right, -rotate_rate) ;
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
        eyepos = eyepos + up * move_rate;
        break ;
    case GLUT_KEY_PAGE_DOWN :
        eyepos = eyepos - up * move_rate;
        break ;
    }
    glutPostRedisplay() ;
}

void animate() {
    glutPostRedisplay() ;
}

int main(int argc, char** argv) {
    atexit(clear) ;
    glutInit(&argc, argv) ;                      // Initialize GLUT
    glutInitWindowSize(500, 500) ;               // Set the window's initial width & height
    glutInitWindowPosition(0, 0) ;             // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; // Enable double buffering
    glutCreateWindow("OpenGL 3D Drawing-Offline IDC") ;      // Create a window with the given title
    
    init() ;
    glEnable(GL_DEPTH_TEST) ;

    glutDisplayFunc(display) ; 
    glutIdleFunc(animate) ;
    glutKeyboardFunc(keyboard) ;                 
    glutSpecialFunc(special) ;                         
    
    
    glutMainLoop() ; 
                            
}