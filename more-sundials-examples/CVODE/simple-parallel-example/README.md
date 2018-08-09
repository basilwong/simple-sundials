# Simple Parallel Example

Showing how to use implement the parallel capabilities of the CVODE package on the original simple cvode example which is implemented serially. 

## What is MPI?

MPI stands for "Message Passing Interface". MPI is a standard defined by a large committee of experts from industry and academia. 


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

Essentially the main change is that the program uses the ```mpicc``` or ```mpic++``` compiler, which compile C/C++ MPI programs.

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
mpirun -n 8 ./parallel
```

 - mpirun is the command to run the program.
 - -n is for specifying the number of processes.
 - 8 is the specified number of processes.
 - ./parallel is the path to the executable file




