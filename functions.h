
#include <iostream>
#include <cstdio>
#include "xtensor/xarray.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xindex_view.hpp"
#include "xtensor/xio.hpp"
#include <vector>
#include <cmath>
#include <tuple>

typedef double real;

#ifndef FORPY
using array3 = xt::xtensor<real, 3>;
using array2 = xt::xtensor<real, 2>;
using array1 = xt::xtensor<real, 1>;
#else
#define FORCE_IMPORT_ARRAY                // numpy C api loading
#include "xtensor-python/pytensor.hpp"
#include "xtensor-python/pyarray.hpp"
using array3 = xt::pytensor<real, 3>;
using array2 = xt::pytensor<real, 2>;
using array1 = xt::pytensor<real, 1>;

#endif


// template<std::size_t SIZE>
// xt::xtensor<double, SIZE-1> norm(const xt::xtensor<double, SIZE> &r){
//     return xt::sqrt(xt::sum(xt::square(r), 0));
// }

const array2 get_mass_matrix(const array1 &mass){
    /**
     * @brief creates matrix which elements are all
     * possible combinations of masses.
     * 
     * @param mass 1D matrix
     */

    int N = mass.shape()[0];
    array2 D = (
        xt::reshape_view(mass, { N, 1 }) *
        xt::reshape_view(mass, { 1, N }) );
    return D;
}

const array3 calc_diff(const array2 &r){
    /**
     * @brief calculates coordinate difference between all 
     * pairs keeping sign
     * 
     * @param r 3 x N array of coordinates of particles
     * @returns 3 x N x N  array
     */

    int N = r.shape()[1];
    array3 D = (
        xt::reshape_view(r, { 3, N, 1 }) - 
        xt::reshape_view(r, { 3, 1, N }) );
    return D;
}

const array3 gravity_force(const array2 &r, const array2 &mass_matrix){
    /**
     * @brief given the positions of particles, calculates gravity force
     * between every pairs of objects. *Gravity constant G = 1*
     * 
     * @param r 3 x N array of coordinates of particles
     * @param mass_matrix N x N matrix of mass products.
     * 
     * @returns  3 x N x N  array of forces.
     */
    
    int N = r.shape()[1];
    
    array3 diff = calc_diff(r);
    auto dist_square = xt::sum(xt::square(diff), 0);

    array2 comon_part = mass_matrix / (dist_square * xt::sqrt(dist_square));
    array3 F = -comon_part * diff;

    for (std::size_t i=0;i<N;++i){
        F(0,i,i)=0;
        F(1,i,i)=0;
        F(2,i,i)=0;
    }
    return F;
}
const array2 gravity_potential(const array2 &r, const array2 &mass_matrix){
    /**
     * @brief given the positions of particles, calculates gravity potential
     * energy between every pairs of objects. *Gravity constant G = 1*
     * 
     * @param r 3 x N array of coordinates of particles
     * @param mass_matrix N x N matrix of mass products.
     * 
     * @returns  N x N  array of potential energies.
     */
    
    int N = r.shape()[1];
    
    array3 diff = calc_diff(r);
    auto dist_square = xt::sum(xt::square(diff), 0);

    array2 potential_energy = -mass_matrix / (xt::sqrt(dist_square));

    for (std::size_t i=0;i<N;++i){
        potential_energy(i,i)=0;
    }
    return potential_energy;
}

const array1 kinetic_energy(const array2 &v, const array1 &mass){
    /**
     * @brief given the positions of particles, calculates gravity force
     * between every pairs of objects. *Gravity constant G = 1*
     * 
     * @param v 3 x N array of velocities of particles
     * @param mass 1d array of masses.
     * 
     * @returns 1d array of kinetic energies.
     */
    
    array1 kinetic_energy = 0.5 * mass * xt::sum(xt::square(v), 0);
    return kinetic_energy;
}

inline array2 calc_acceleration(const array3 &F, const array1 &mass){
    /**
     * @brief Given forces, calculates acceleration of the objects
     * @param F  3 x N x N  array of forces.
     * @param mass  1D array of masses
     */
    return xt::sum(F, 2)/mass;
}

array2 zero_momentum_frame(const array2 &v, const array1 &mass){
    /**
     * @brief transforms velocities linearly such that total momentum of the
     * system is ZERO.
     * 
     * @param v 3 x N matrix of velocities
     * @param mass 1d array of masses
     * 
     * @returns 3 x N matrix of transformed velocities
     */
    auto pc = xt::sum(v * mass, 1);
    auto vc = pc / xt::sum(mass);
    return v -  xt::reshape_view(vc, { 3, 1 });
}