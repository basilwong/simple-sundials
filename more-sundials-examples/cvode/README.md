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


