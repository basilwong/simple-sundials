# Simple Parallel Example

This example is a guide for how to setup MPI for a parallel environment on a local computer in addition to showing how to use implement the most basic parallel capabilities of the CVODE package on the original simple cvode example which is implemented serially. 


## What is MPI?

MPI stands for "Message Passing Interface". MPI is a standard defined by a large committee of experts from industry and academia. 

For more information on how to work with MPI

 - http://mpitutorial.com/tutorials/
 
is a good place to start learning. 


## Environment Setup

### Quick Guide for Installing MPI

In order to utilize the parallel tools of CVODE MPI has to be installed on the computer. To be more specifc, this is a guide for installing MPICH2 on a computer. 

### Download 

First download the latest version of MPICH2 from the website here in tar.gz compressed format:

 - http://www.mpich.org/
 
### Uncompress 
 
Next go to the folder where the file was downloaded (or move it to a src location then go there).

Keep in mind that the version in the example is 3.2, but this doens't have to be the case, just download the latest stable version. Once in the directory of the downloaded tar.gz file and run:

```
tar -xzf mpich2-3.2.tar.gz
```

This uncompresses the file. Then go into the directory:

```
cd mpich2-3.2
```

### Configure

*Note: If a c++ compiler is not already downloaded you can download it with:

```
sudo apt-get install g++
```

Next we will be doing a standard configuration the installation:

```
./configure --prefix=/usr/local
```

This sets the prefix to the same path as the CVODE installation done in the README of this repository's root folder. 


```
./configure --disable-fortran
```

You will not be able to make the package without disabling the fortran installation unless you have installed a fortran compiler. 

### Make/Install

Once the package is configured, we will now make and install the package with the following commands:

```
make
```

```
sudo make install
```

### Check for Successful Installation

To make sure the package was installed properly run the following command:

```
mpiexec --version
```


## Compiling/Running MPI Program 

Compiling and running a MPI program requires slightly different commands than a normal C/C++ program. 

### Compiling 

To compile the simple CVODE parallel example the makefile is slightly different. The makefile currently being used is from the [Projectdummies](https://github.com/rkoenigstein/Projectdummies) GitHub repository, which contains makefiles for C/C++ programs including parallel libraries. 

Essentially the main change is that the program uses the ```mpicc``` or ```mpic++``` compiler, which compiles C/C++ MPI programs.

Additionally the CVODE libraries need to be linked on the makefile by adding:

```
-lsundials_cvode -lsundials_nvecparallel
```

to

```
LDFLAGS = -Wall
``` 

### Running

Example of the command for running the MPI program. 

```
mpirun -n 2 ./parallel
```

 - mpirun is the command to run the program.
 - -n is for specifying the number of processes.
 - 8 is the specified number of processes.
 - ./parallel is the path to the executable file



## MPI with CVODE

In this section, the changes from the simple example to the parallel example are 
described. 

### Includes

```
#include <nvector/nvector_parallel.h>  
```

### N_Vector access

The defined macro needs to be changed for N_Vector_Parallel struct. 

```
#define NV_Ith_P(v,i) ( NV_DATA_P(v)[i] )
```

### Initialize MPI Environment

```
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // Setup User Data Pointer so the function for the ODE can know the process
  // rank.
  UserData *data = alloc_user_data(world_rank, world_size);
```

Is added to step 1 in the 'main' function.

In this example we utilize UserData to pass the process rank and the size of the communicator to the function for calculating the derivative. 

For more information on how to utilize UserData with CVODE go to the example in 

```
simple-sundials-example\more-sundials-examples\cvode
```

of this repository. 

### Finalize MPI

```
  // Finalize the MPI environment.
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
```

Is added in step 19 as the last step before finishing the 'main' function. This is used to clean up any memory allocated for the utilized processes.

### In the Derivative Calculation

Here we have the two processes each calculating one of the terms in the size 2 global derivative vector. 

In practice, the size of the problem should be much larger in order to make up for the overhead of parallelizing the problem. 

 

