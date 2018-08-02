I found when learning to use the CVODE libraries, that there were few simple examples to start off with for learning from scratch how to use the libraries. So here is a simple example using the CVODE library to solve a simple 2d ODE, treating it as a stiff system. The system is based off of boost's stiff system example at:

 - https://www.boost.org/doc/libs/1_67_0/libs/numeric/odeint/doc/html/boost_numeric_odeint/tutorial/stiff_systems.html
 
for comparison purposes.

## What is CVODE?

CVODE is a solver for stiff and nonstiff ordinary differential equation (ODE) systems (initial value problem) given in explicit form y’ = f(t,y).

 - https://computation.llnl.gov/projects/sundials/cvode 

## Where do I get the CVODE solver?

To download the libraries go to their software download page:

 - https://computation.llnl.gov/projects/sundials/sundials-software

Appendix A of the CVODE guide gives a pretty good tutorial of how to download, install, and configure the libraries for use depending on the OS you're currently using. 

 - https://computation.llnl.gov/sites/default/files/public/cv_guide.pdf
 
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

### CVODES

 - Simple serial example with adjoint sensitivity analysis for stiff systems. 

