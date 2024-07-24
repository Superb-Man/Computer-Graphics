#pragma once

#include<bits/stdc++.h>
#include"matrix.h"
#include"utils.h"
#include"zbuffer.h"
#include"helper.h"
#include"bitmap_image.hpp"

using namespace std;

//model view projection transformation
std::vector<Matrix> mvpTransformation(){


    std::stack<Matrix> st;
    std::vector<Matrix> matrices;
    Matrix cur(4,4);
    st.push(cur);
    Transformation transformation;

    FILE *fp = freopen("scene.txt", "r",stdin);
    FILE *fpo = freopen("stage1.txt", "w",stdout);

    std::cin >> transformation.eye.x >> transformation.eye.y >> transformation.eye.z;
    std::cin >> transformation.look.x >> transformation.look.y >> transformation.look.z;
    std::cin >> transformation.up.x >> transformation.up.y >> transformation.up.z;
    std::cin >> transformation.fovY >> transformation.aspectRatio >> transformation.near >> transformation.far;

    //modeling transformation

    std::string cmd;
    while(std::cin >> cmd) {
        if(cmd == "triangle") {
            point p1, p2, p3;
            std::cin >> p1.x >> p1.y >> p1.z;
            std::cin >> p2.x >> p2.y >> p2.z;
            std::cin >> p3.x >> p3.y >> p3.z;
            Matrix m = triangleMatrix(p1, p2, p3);
            m = cur * m;
            matrices.push_back(m);
            print(m,3,3,true) ;
        }
        else if(cmd == "translate") {
            point p;
            std::cin >> p.x >> p.y >> p.z;
            Matrix m = translete(p);
            cur = cur * m;
        }
        else if(cmd == "scale") {
            point p;
            std::cin >> p.x >> p.y >> p.z;
            Matrix m = scale(p);
            cur = cur * m;
        }
        else if(cmd == "rotate") {
            point p;
            double angle;
            std::cin >> angle >> p.x >> p.y >> p.z ;
            double pi = (double)acos((double)-1.0) ;
            angle = (angle * pi) /180.0 ;
            Matrix m = transformation.rotate(p, angle);
            cur = cur * m;
        }
        else if(cmd == "push") {
            st.push(cur);
        }
        else if(cmd == "pop") {
            if(!st.empty()) 
                cur = st.top();
                st.pop();
        }
        else if(cmd == "end") {
            break;
        }
    }
    fclose(fp);
    fclose(fpo);

    fpo = freopen("stage2.txt", "w",stdout);
    //view transformation
    transformation.viewTransformation(matrices);
    fclose(fpo);

    fpo = freopen("stage3.txt", "w",stdout);
    //projection transformation
    transformation.projectionTransformation(matrices);
    fclose(fpo);

    return matrices;        
}

int main() {

    std::vector<Matrix>matrices = mvpTransformation();
    FILE* fp = freopen("config.txt", "r",stdin);
    double width = 500, height = 500;
    std::cin >> width >> height;
    z_bufferApply(width, height, matrices);
    fclose(fp);
}