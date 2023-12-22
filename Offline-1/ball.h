#pragma once
#include"point.h"
class Rolling_ball {

public:
    double velocity ;
    double radius ;
    double stacks ;
    double slices ;
    double theta ;
    Rolling_ball() {
        velocity = 1 ;
        radius = 3 ;
        stacks = 20 ;
        slices = 24 ;
    }

} ;
