#include <iostream>
#include <cstdio>

#include "functions.h"

#include <vector>
#include <cmath>
#include <tuple>
#include <string>
/////////////////////////////
const std::string P_SPACE  = "                                                                    ";
const std::string P_LINE   = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
const std::string P_SHARP  = "####################################################################";
const std::string P_DOT    = "....................................................................";
#ifdef FORPY
#include <pybind11/stl.h>
namespace py = pybind11;
using namespace pybind11::literals;
#endif

void progress_bar(real percent, int length=15){
    /**
     * @brief Prints progresbar for convenience.
     * 
     */
    if (percent > 1.0)
        percent /= 100.0;
    int pass = int(length * percent);

#ifndef FORPY    
    printf("\r");
    printf("%0.2f%%  ", percent*100);
    printf("[");
    std::cout<<P_SHARP.substr(0, pass+1);
    std::cout<<P_DOT.substr(0, length - pass-1);
    printf("]");
#else
     py::print("\r","end"_a="");
    char buf[20];
    sprintf(buf, "%0.2f%%  ", percent*100);

    py::print(buf, "[", P_SHARP.substr(0, pass+1),
        P_DOT.substr(0, length - pass-1), "]", "end"_a="");
#endif
}
///////////////////////////////

inline real round_value(const real &val){
    /**
     * @brief round value in 1e-10 order for numerical stability.
     */
    return round((val) * 10000000000) * 0.0000000001;
}

//          position   velocity  acceleration  time
std::tuple<array2, array2, array2, real> 
    integration_step(array2 &r, array2 &v, array2 &a, real &t, 
        const real &dt, const array2 &mass_matrix, const array1 &mass){
    /**
     * @brief makes single step of integration of motion equation according 
     * to given state and numerical parameters of the integration. This uses 
     * Stormer-Verlet Algorithm.
     * 
     * @param r (3 x N) matrix of positions
     * @param v (3 x N) matrix of velocities
     * @param a (3 x N) matrix of accelerations
     * @param t (real) current time
     * 
     * @param dt  (real) time decrement
     * @param mass_matrix mass 1d matrix calculated from masses
     * @param mass 1d array of masses
     * 
     * @returns tuple(new_r (3 x N), new_v (3 x N), new_a (3 x N), new_t real)
     */

    //////////////////////////////
    // Stormer-Verlet Algorithm
    r += v * dt + 0.5 * a * dt*dt;
    v += 0.5 * a * dt;
    a = calc_acceleration(
        gravity_force(r, mass_matrix), mass);
    v += 0.5 * a * dt;
    t = round_value(t+dt);
    /////////////////////////////   
    return std::make_tuple(r, v, a, dt);
}

std::tuple<std::vector<array2>, std::vector<array2>, std::vector<real>> 
    simulate(
        array2 r, array2 v, real t, 
        const array1 mass,
        const real iteration_time=1.0, 
        const real dt=0.0005, 
        const real record_interval=0.01,
        bool zero_momentum=true,
        bool verbose=true){
    /**
     * @brief Simulation function, which integrates equation of motion for 
     * given duration, time decrement and initial conditions.
     * 
     * @param r (3 x N) matrix of initial positions
     * @param v (3 x N) matrix of initial velocities
     * @param t (real) initial time
     * @param mass 1d array of masses 
     * 
     * @param iteration_time (real) integration time-range
     * @param dt  (real) time decrement
     * @param record_interval (real) period for recording system parameters
     * @param zero_momentum (bool) transfer into zero-momentum-frame
     * 
     * @param verbose  (bool)  use progressbar
     * 
     * 
     * @returns recordings of the system:
     *      tuple(vector<2dArray> positions,
     *            vector<2dArray> velocities,
     *            vector<real> time)
     */
    if (zero_momentum){
        v = zero_momentum_frame(v, mass);
    }
    array2 mass_matrix = get_mass_matrix(mass);

    array2 a = calc_acceleration(gravity_force(r, mass_matrix), mass);
    unsigned int n_iterations = iteration_time/dt;
    std::vector<array2> history_rs{{r}};
    std::vector<array2> history_vs{{v}};
    std::vector<real> history_ts{{t}};
    real last_add_time = t;
    real last_percentage = 0;

    for (unsigned int i=0;i<n_iterations;++i){
        auto ret = integration_step(r, v, a, t, dt, mass_matrix, mass);
        if (t - last_add_time >= record_interval - dt/4){
            last_add_time = round_value(last_add_time+record_interval);
            history_ts.push_back(last_add_time);
            history_vs.push_back(v);
            history_rs.push_back(r);
            if (verbose==true){
                real current_percentage = 1.0*i/n_iterations;
                if (current_percentage > last_percentage + 0.001){
                    progress_bar(current_percentage, 25);
                    last_percentage = current_percentage;
                }
            }
        }
    }
    printf("simulation finished\n");
    return std::forward_as_tuple(history_rs, 
        history_vs, history_ts);

}
