## Simple User Data Example

This is a simple example showing how to use OPEN-MPI to implement parallel computing for integration. 

### Install OPEN-MPI

In order to get parallel computing working, first install OPEN-MPI from the OPEN-MPI website:

 - https://www.open-mpi.org/software/ompi/v3.1/
 
The [FAQ](https://www.open-mpi.org/faq/?category=building#easy-build) page on the OPEN-MPI webisite gives the instruction for how to install the downloaded openmpi-3.x.x.tar.gz file:

```
shell$ gunzip -c openmpi-3.1.1.tar.gz | tar xf -
shell$ cd openmpi-3.1.1
shell$ ./configure --prefix=/usr/local
<...lots of output...>
shell$ make all install
```

### Configure the CVODE install for MPI



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

