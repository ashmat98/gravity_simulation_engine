#include <iostream>
#include <chrono>



#include "simulate.h"



int main(int argc, char *argv[]){
    real t = 1;
    real dt=0.01;
    real record_dt=0.1;
    int N = 2;
    array1 mass = {100, 1};
    array2 r0 = {{0,1},
                {0,0},
                {0,0}};
    array2 v0 = {{0,0},
                {0,10},
                {0,0}};
    printf("particles: %d\n", N);
    printf("Time:      %0.6f\n", t);
    printf("Step:      %0.6f\n", dt);
    printf("Record:    %0.6f\n", record_dt);

    std::cout<<"Starting..."<<std::endl;
    auto sim_start = std::chrono::steady_clock::now();

    
    auto [h_rs, h_vs, h_ts] = simulate(
        r0, v0, 0.0, mass, 0.5, 0.000000500, 0.001);
    printf("%d %d %d\n", h_rs.size(),h_vs.size(),h_ts.size());

    std::cout<< h_rs[10] <<std::endl;
    std::cout<< h_vs[10] <<std::endl;
    std::cout<< h_ts[10] <<std::endl;



    auto sim_end = std::chrono::steady_clock::now();
    std::cout<<"Finished!"<<std::endl;
    
    real time = (std::chrono::duration<real, std::milli>
        (sim_end - sim_start).count());
    printf("Time: %0.3fs\n", time/1000);
    return 0;
}