import numpy as np
from matplotlib import pyplot as plt
import build.steps as steps

def total_energy_over_time(rs, vs, mass):
    mass_matrix = steps.get_mass_matrix(mass)
    energies = []
    for r, v in zip(rs, vs):
        W = (np.sum(steps.gravity_potential(r, mass_matrix))/2 +
            np.sum(steps.kinetic_energy(v, mass)))
        energies.append(W)
    return np.array(energies)

def plot_energy_evolution(rs, vs, ts, mass):
    plt.title("Energy evolution")
    plt.xlabel("Time");plt.ylabel("Energy")
    plt.plot(ts, total_energy_over_time(rs, vs, mass))
    
def plot_trajectories(rs, ts, iteration_time):
    sz = 0.5
    norm = plt.Normalize(0, iteration_time)

    plt.set_cmap("gist_rainbow")

    xs, ys = rs[:, 0], rs[:, 1]
    
    r0 = rs[0]
    plt.scatter(r0[0], r0[1], marker='x', s=50)
    for i in range(rs.shape[2]):
        plt.scatter(xs[:, i], ys[:, i], s=0.5*(i+1),c=ts, norm=norm)
        plt.plot(xs[:, i], ys[:, i], lw=sz,c="k");
    plt.gca().set_aspect("equal")