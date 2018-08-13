## Simple User Data Example

This example builds on the original "Simple CVODE Example" by adding a simple example of allocating memory for user data to be used in the ODE rhs function. 

 - The UserData struct defines the variables required by the ODE problem. 

 - The alloc_user_data function is used to initialize the variables to be used. 

 - Setting the user data is part of the optional inputs(7) step. 
 
 - Remember to free the pointer used by the user data as shown in step 18 of the code. 
 
 - In this example it is the function for the problem's differential equation that uses the variables defined in the user data pointer. Since the structure of the function (as defined by CVODE) requires that user_data be inputted as a void pointer, you have to cast the pointer variable to the defined struct as shown in the code. 

## Makefile

For those who are Makefile beginners, a simple solution is to use the cpp generic Makefile from:

 - https://github.com/mbcrawfo/GenericMakefile
 
Then add:

```
-lsundials_cvode -lsundials_nvecserial
```

onto the line:

```
LINK_FLAGS = 
```

## Code Structure

The numbered steps indicated by the comments in the code follow the steps in section 4.4 "A skeleton of the user's main program" of the [CVODE guide](https://computation.llnl.gov/sites/default/files/public/cv_guide.pdf).

