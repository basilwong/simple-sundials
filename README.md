I found there were few simple examples while trying to learn how to use the SUNDIALS libraries. Here are a few simple examples for those starting to utilize the SUNDIALS libraries. 

Within `src` is an example using the CVODE library to solve a simple 2d ODE, treating it as a stiff system. The system is based off of boost's stiff system example at:

 - https://www.boost.org/doc/libs/1_67_0/libs/numeric/odeint/doc/html/boost_numeric_odeint/tutorial/stiff_systems.html
 
for comparison purposes.

## What is SUNDIALS?

SUNDIALS is a SUite of Nonlinear and DIfferential/ALgebraic equation Solvers released by the Lawrence Livermore National Laboratory.

 - https://computing.llnl.gov/projects/sundials

## How do I download/install the SUDIALS libraries?

To download the libraries go to their software download page:

 - https://computing.llnl.gov/projects/sundials/sundials-software

Libraries such as CVODE can be downloaded individually on the above page. There is also a link on that page to download the [entire SUNDIALS library](https://github.com/LLNL/sundials/releases/download/v6.5.0/sundials-6.5.0.tar.gz).
 
The online documentation gives a good tutorial for how to install the software depending on OS: 

 - https://sundials.readthedocs.io/en/latest/Install_link.html
 
*Note: The instructions above tell the user to change the generate settings for the install prefix to /usr/casc/sundials/instdir but for the GenericMakeFile it is more simple to leave the ccmake settings as is and keep the generate settings for the install prefix as the default option:

```
/usr/local
```

## Sundials Examples (CVODE, CVODES, KINSOL)

More examples for SUNDIALS libraries can be found in the more-sundials-examples folder.

### KINSOL

 - Simple rootfinding example.

### CVODE

 - Example of how to implement user data for the system. Building block for implementing solver for complex systems. 
 - Example of how to setup a parallel environment (MPICH2) and utilize CVODE's integration with the MPI protocol(N_Vector_Parallel).

### CVODES

 - Simple serial example with adjoint sensitivity analysis for stiff systems. 

