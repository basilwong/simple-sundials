## Simple Adjoint Serial Example

This code is a simple example of what is added onto the CVODE example in the root's src folder in order to do adjoint sensitivity analysis. 

For more examples with CVODES checkout:

 - https://computation.llnl.gov/sites/default/files/public/cvs_examples.pdf

## Makefile

For those who are Makefile beginners, a simple solution is to use the cpp generic Makefile from:

 - https://github.com/mbcrawfo/GenericMakefile
 
Then add:

```
-lsundials_cvodes -lsundials_nvecserial
```

onto the line:

```
LINK_FLAGS = 
```

## Code Structure

The numbered steps indicated by the comments in the code follow the steps in section 6.1 "A skeleton of the user's main program" of the [CVODES guide](https://computation.llnl.gov/sites/default/files/public/cvs_guide.pdf).