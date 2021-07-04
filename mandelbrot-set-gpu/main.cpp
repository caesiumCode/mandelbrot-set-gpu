//
//  main.cpp
//  mandelbrot-set-gpu
//
//  Created by Stephen Jaud on 03/07/2021.
//

#include "Program.hpp"

int main(int argc, const char * argv[]) {
    
    Program program(2000, 1500);
    
    program.run();
    
    return 0;
}
