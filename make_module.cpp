#include <iostream>

#define FORPY


#include "simulate.h"

#define FORCE_IMPORT_ARRAY                // numpy C api loading
#include "xtensor-python/pytensor.hpp"
#include "xtensor-python/pyarray.hpp"

#include <pybind11/stl.h>
namespace py = pybind11;


PYBIND11_MODULE(steps, m)
{
    xt::import_numpy();
    m.doc() = "Simulation engine for many-body gravity.";

    m.def("simulate", &simulate, py::arg("r"),py::arg("v"),py::arg("t"),
        py::arg("mass"),py::arg("iteration_time"), py::arg("dt"),
        py::arg("record_interval"),py::arg("zero_momentum"),py::arg("verbose"),
R"mydelimiter(
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
)mydelimiter");

    m.def("integration_step", &integration_step, 
        py::arg("r"),py::arg("v"),py::arg("a"),py::arg("t"),py::arg("dt"),
        py::arg("mass_matrix"), py::arg("mass"),
R"mydelimiter(
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
)mydelimiter");

    m.def("zero_momentum_frame", &zero_momentum_frame, 
        py::arg("v"), py::arg("mass"),
R"mydelimiter(
* @brief transforms velocities linearly such that total momentum of the
* system is ZERO.
* 
* @param v 3 x N matrix of velocities
* @param mass 1d array of masses
* 
* @returns 3 x N matrix of transformed velocities
)mydelimiter");

    m.def("get_mass_matrix", &get_mass_matrix, py::arg("mass"),
R"mydelimiter(
* @brief creates matrix which elements are all
* possible combinations of masses.
* 
* @param mass 1D matrix
)mydelimiter");

    m.def("calc_diff", &calc_diff, py::arg("r"),
R"mydelimiter(
* @brief calculates coordinate difference between all 
* pairs keeping sign
* 
* @param r 3 x N array of coordinates of particles
* @returns 3 x N x N  array
)mydelimiter");

    m.def("gravity_force", &gravity_force, py::arg("r"),py::arg("mass_matrix"),
R"mydelimiter(
* @brief given the positions of particles, calculates gravity force
* between every pairs of objects. *Gravity constant G = 1*
* 
* @param r 3 x N array of coordinates of particles
* @param mass_matrix N x N matrix of mass products.
* 
* @returns  3 x N x N  array of forces.
)mydelimiter");

    m.def("calc_acceleration", &calc_acceleration, 
        py::arg("F"),py::arg("mass"),
R"mydelimiter(
* @brief Given forces, calculates acceleration of the objects
* @param F  3 x N x N  array of forces.
* @param mass  1D array of masses
)mydelimiter");

    m.def("gravity_potential", &gravity_potential, 
        py::arg("r"),py::arg("mass_matrix"),
R"mydelimiter(
* @brief given the positions of particles, calculates gravity potential
* energy between every pairs of objects. *Gravity constant G = 1*
* 
* @param r 3 x N array of coordinates of particles
* @param mass_matrix N x N matrix of mass products.
* 
* @returns  N x N  array of potential energies.  
)mydelimiter");
    
    m.def("kinetic_energy", &kinetic_energy,
        py::arg("v"),py::arg("mass"),
R"mydelimiter(
* @brief given the positions of particles, calculates gravity force
* between every pairs of objects. *Gravity constant G = 1*
* 
* @param v 3 x N array of velocities of particles
* @param mass 1d array of masses.
* 
* @returns 1d array of kinetic energies.
)mydelimiter");

    // m.def("", &, R"mydelimiter()mydelimiter");
}