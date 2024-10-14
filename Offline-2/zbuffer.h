#pragma once 

#include<bits/stdc++.h>
#include"utils.h"
#include"matrix.h"
#include"helper.h"
#include"bitmap_image.hpp"

std::vector<Triangle> makeTriangles(std::vector<Matrix>& matrices) {
    std::vector<Triangle>triangles ;
    for(Matrix v  : matrices){
        std::vector<point> points;
        for(int i = 0; i < 3; i++){
            double x[3] ;
            for(int j = 0; j<3 ;j++){
                x[j] = v.get(j,i) ;
            }
            // std::cout << x[0] << " " << x[1] << " " << x[2] << std::endl ;
            points.push_back({x[0], x[1], x[2]});
        }
        rgb_t color = make_colour(random1()%256, random1()%256, random1()%256);
        triangles.push_back({points[0], points[1], points[2], color});
    }

    return triangles;
}

void z_bufferApply(double width, double height,std::vector<Matrix>&matrices) {
    // FILE *zbf = freopen("z_buffer.txt","w",stdout) ;
    // std::cout << "z_buffer\n" ;
    //Ekhane kichu triangle ashbe
    std::vector<Triangle> triangles = makeTriangles(matrices);
    
    double** z_buffer = new double*[(int)height];
    for(int i = 0; i < (int)height; i++) {
        z_buffer[i] = new double[(int)width];
        for(int j = 0; j < (int) width; j++) {
            z_buffer[i][j] = 1.0 ;
        }
    }

    bitmap_image image((int)width, (int)height);
    image.set_all_channels(0, 0, 0);

    double x_right = 1.0;
    double x_left = -1.0;
    double y_top = 1.0;
    double y_bottom = -1.0;
    double dx = 1.0*(x_right - x_left) / width; //(1 -(-1))/width
    double dy = 1.0*(y_top - y_bottom) / height;//(1 -(-1))/height

    double top_y = y_top - dy / 2.0;
    double left_x = x_left + dx / 2.0;

    for(Triangle triangle : triangles) {
        //take the three points of the triangle

        Line l1 = {triangle.a, triangle.b - triangle.a};
        Line l2 = {triangle.b, triangle.c - triangle.b};
        Line l3 = {triangle.c, triangle.a - triangle.c};

        //uporer boundarir jonno
        double mx_y = std::max(std::max(triangle.a.y, triangle.b.y), triangle.c.y);
        mx_y = std::min(mx_y, 1.0);
        // std::cout << "max is " << mx_y <<' '<<fabs(mx_y-1.0)*1./dy<<'\n' ;
        //koto tomo index theke iteration shuru hobe
        int top_row =  round(fabs(mx_y -1.)/dy) ; 

        //nicer boundarir jonno
        double mn_y = std::min(std::min(triangle.a.y, triangle.b.y), triangle.c.y);
        mn_y = std::max(mn_y, -1.0);
        //koto tomo index e iteration shesh hobe
        int bottom_row = floor(std::abs(mn_y -1.0)/dy) ;
        // std::cout << "toprow :" << top_row << " Bottom_row : " <<bottom_row <<'\n' ;
        for (int i = top_row ; i < bottom_row ; i++) {

            double new_y = top_y - i * dy;

            //tinta line er kon duitar shathe intersection ache ta ber kora
            std::pair<point,double> scanline_p1 = intersection_t(l1, new_y,1);
            std::pair<point,double> scanline_p2 = intersection_t(l2, new_y,1);

            if(scanline_p1.second < 0 || scanline_p1.second >1) {
                scanline_p1 = intersection_t(l3, new_y,1);
            }
            else if(scanline_p2.second < 0 || scanline_p2.second >1) {
                scanline_p2 = intersection_t(l3, new_y,1);
            }
            //intersection peye gechi
            //uWu
            //left to right scanning er idx khujtesi
            int left_idx = floor(((std::max(std::min(scanline_p1.first.x, scanline_p2.first.x),x_left)) + 1.0 )/ dx);
            int right_idx = floor(((std::min(std::max(scanline_p1.first.x, scanline_p2.first.x),x_right))+1.0)/ dx);
            // std::cout << left_idx << " " << right_idx << std::endl ;

            Line l3 ;
            l3.u = scanline_p1.first ;
            l3.v = scanline_p2.first - scanline_p1.first ;
            for(int j = left_idx ;j <=right_idx ; j++) {
                double new_x = left_x + j * dx;
                std::pair<point,double> temp = intersection_t(l3, new_x,0);
                point p = temp.first;

                if(p.z < z_buffer[i][j] && p.z > -1.0) {
                    z_buffer[i][j] = p.z;
                    //color
                    image.set_pixel(j, i, triangle.color.blue, triangle.color.green, triangle.color.red);
                }

            }

        }





    }
    // std::cout << "z_buffer\n" ;

    FILE* zbf = freopen("z_buffer.txt","w",stdout) ;
    for(int i = 0 ; i < (int)height ; i++){
        for(int j = 0 ; j < (int)width ; j++){
           if(z_buffer[i][j]== 1) continue;
           else{
                std::cout << std::fixed << std::setprecision(6) << z_buffer[i][j] << '\t';
           }
        }
        std::cout << "\n" ;
    }
    fclose(zbf) ;

    for(int i = 0; i < (int)height; i++) {
        delete[] z_buffer[i];
    }
    delete[] z_buffer;
    

    image.save_image("z_buffer.bmp") ;

}