# Gravity Simulation Engine
This is Simulation Engine for Classical Gravitational Force (inverse square depencece). 

Python is a wonderful language, but it has a major disadvantage: it is extremely slow.
We used C++ backend for faster simulation. By using python bindings, we created a module and could be able to use inside python interpreter.

For the tensor algebra [xtensor](https://xtensor.readthedocs.io/) C++ framework, which uses the whole power of the CPU to make fast computations. It has good documentation and a quite similar API to [NumPy](numpy.org).

Then we used [PyBind11](https://pybind11.readthedocs.io/en/stable/) to create python bindings, to use the C++ code inside python. As python is interpreted language, it provides much more freedom and saves time during experimenting. However, it is not guaranteed that our compiled python module will work on other machines, so you may need to compile it yourself.

Finally, we illustrate our simulation engine in Jupyter notebooks.

### Requirements
* xtensor 0.21.2
* xsimd 7.4.5
* xtensor-python 0.24.1
* xtl 0.6.11
* PyBind11 2.4.3
* Python 3.7.4
* MinGW-w64 8.1.0

Operating system: Windows 10 64bit
### File descriptions
* `functions.h` contains all core functions essential for the simulation.
* `simulate.h` contains *integration_step* function, wich uses **Stormer-Verlet** algorithm. *simulate* function makes simulation.
* `test_simulate.cpp` checks integrity.
* `make_module.cpp` creates python module containing ALL defined functions.
* `utils.py` contains frequently used function, e.g. for visualization.
* `build\steps.pyd` compiled python module.
* `StampMe.cmd` renames module file inside 'build' folder. Windows sometimes locks files.

For more, see docstrings inside files.
### Compilation
To compile the python module, just run:
```bash
> .\compile_module.cmd
```
this creates python module `./build/steps.pyd`. You need to change directories of the libraries. For different operation system, refer to [PyBind11](https://pybind11.readthedocs.io/en/stable/) documentation.

You can freely use the simulator with C++ code. For example see `test_simulate.cpp`. to compile it, run
```bash
>.\compile_exe.cmd
```

All compile-time optimization are enabled.
### Examples
The simulator is demonstrated in Jupyter notebooks.
