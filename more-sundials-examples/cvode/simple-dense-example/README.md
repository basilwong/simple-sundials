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

