## What is KINSOL?

KINSOL is a solver for nonlinear algebraic systems. It includes a Newton-Krylov solver as well as Picard and fixed point solvers, both of which can be accelerated with Anderson acceleration. KINSOL is based on the previous Fortran package NKSOL of Brown and Saad.

KINSOL’s Newton solver employs the Inexact Newton method. As this solver is intended mainly for large systems, four iterative methods are provided to solve the resulting linear systems–GMRES, Bi-CGStab, TFQMR, and FGMRES. These are Krylov methods, implemented with scaling and preconditioning, and can be used with all versions of the NVECTOR module.

 - https://computation.llnl.gov/projects/sundials/kinsol

## Where do I get the KINSOL package?

To download the libraries go to their software download page:

 - https://computation.llnl.gov/projects/sundials/sundials-software

Appendix A of the CVODE guide gives a pretty good tutorial of how to download, install, and configure all SUNDIALS libraries for use depending on the OS you're currently using. 

 - https://computation.llnl.gov/sites/default/files/public/cv_guide.pdf
 
*Note: The instructions above tell the user to change the generate settings for the install prefix to /usr/casc/sundials/instdir but for the GenericMakeFile it is more simple to leave the ccmake settings as is and keep the generate settings for the install prefix as the default option:

```
/usr/local
```





