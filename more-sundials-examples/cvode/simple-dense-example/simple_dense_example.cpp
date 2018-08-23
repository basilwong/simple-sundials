/*
A simple example using the CVODE library to solve a simple 2d ODE, treating it
as a stiff system. This version utilizes CVODE's implementation of a direct
solver using dense matrices.
*/

#include <iostream>
#include <cvode/cvode.h> // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>  // access to serial N_Vector
#include <sunmatrix/sunmatrix_dense.h> // access to dense SUNMatrix
#include <sunlinsol/sunlinsol_dense.h> // access to dense SUNLinearSolver
#include <cvode/cvode_direct.h> // access to CVDls interface
#include <sundials/sundials_types.h>  // defs. of realtype, sunindextype
#include <sundials/sundials_math.h>  // contains the macros ABS, SUNSQR, EXP

// These macro gives access to the individual components of the data array of an
// N Vector (NV_Ith_S) and SUNMatrix (IJth).
#define NV_Ith_S(v,i) ( NV_DATA_S(v)[i] )
#define IJth(A,i,j) SM_ELEMENT_D(A,i,j)


static int f(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
static int jtv (realtype t, N_Vector y, N_Vector fy, SUNMatrix Jac,
                void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
static int check_flag(void *flagvalue, const char *funcname, int opt);


int main() {
  int flag; // For checking if functions have run properly
  realtype abstol = 1e-5; // real tolerance of system
  realtype reltol = 1e-5; // absolute tolerance of system

  // 1. Initialize parallel or multi-threaded environment, if appropriate.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 2. Defining the length of the problem.
  // ---------------------------------------------------------------------------
  sunindextype N = 2;
  // ---------------------------------------------------------------------------

  // 3. Set vector of initial values.
  // ---------------------------------------------------------------------------
  N_Vector y; // Problem vector.
  y = N_VNew_Serial(N);
  NV_Ith_S(y, 0) = 2.0;
  NV_Ith_S(y, 1) = 1.0;
  // ---------------------------------------------------------------------------

  // 4. Create CVODE Object.
  // ---------------------------------------------------------------------------
  void *cvode_mem = NULL; // Problem dedicated memory.
  cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
  // ---------------------------------------------------------------------------

  // 5. Initialize CVODE solver.
  // ---------------------------------------------------------------------------
  realtype t0 = 0; // Initiale value of time.
  flag = CVodeInit(cvode_mem, f, t0, y);
  if(check_flag(&flag, "CVodeSetUserData", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 6. Specify integration tolerances.
  // ---------------------------------------------------------------------------
  flag = CVodeSStolerances(cvode_mem, reltol, abstol);
  if (check_flag(&flag, "CVodeSStolerances", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 7. Set Optional inputs.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 8. Create Matrix Object.
  // ---------------------------------------------------------------------------
  // Need to create a dense matrix for the dense solver.
  SUNMatrix A = SUNDenseMatrix(N, N);
  if(check_flag((void *)A, "SUNDenseMatrix", 0)) return(1);
  // ---------------------------------------------------------------------------

  // 9. Create Linear Solver Object.
  // ---------------------------------------------------------------------------
  // Dense linear solver object instead of the iterative one in the original
  // simple example.
  SUNLinearSolver LS = SUNDenseLinearSolver(y, A) ;
  if(check_flag((void *)LS, "SUNDenseLinearSolver", 0)) return(1);
  // ---------------------------------------------------------------------------

  // 10. Set linear solver optional inputs.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 11. Attach linear solver module.
  // ---------------------------------------------------------------------------
  // Call CVDlsSetLinearSolver to attach the matrix and linear solver this
  // function is different for direct solvers.
  flag = CVDlsSetLinearSolver(cvode_mem, LS, A);
  if(check_flag(&flag, "CVDlsSetLinearSolver", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 12. Set linear solver interface optional inputs.
  // ---------------------------------------------------------------------------
  // Sets the jacobian-times-vector function.
  flag = CVDlsSetJacFn(cvode_mem, jtv);
  if(check_flag(&flag, "CVDlsSetJacFn", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 13. Specify rootfinding problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 14. Advance solution in time.
  // ---------------------------------------------------------------------------
  // Have the solution advance over time, but stop to log 100 of the steps.
  int print_steps = 100;
  realtype tout;
  realtype end_time = 50;
  realtype step_length = 0.5;
  realtype t = 0;
  // loop over output points, call CVode, print results, test for error
  for (tout = step_length; tout <= end_time; tout += step_length) {
    flag = CVode(cvode_mem, tout, y, &t, CV_NORMAL);
    std::cout << "t: " << t;
    std::cout << "\ny:";
    N_VPrint_Serial(y);
    if(check_flag(&flag, "CVode", 1)) break;
  }
  // ---------------------------------------------------------------------------

  // 15. Get optional outputs.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 16. Deallocate memory for solution vector.
  // ---------------------------------------------------------------------------
  N_VDestroy(y);
  // ---------------------------------------------------------------------------

  // 17. Free solver memory.
  // ---------------------------------------------------------------------------
  CVodeFree(&cvode_mem);
  // ---------------------------------------------------------------------------

  // 18. Free linear solver and matrix memory.
  // ---------------------------------------------------------------------------
  SUNLinSolFree(LS);
  // ---------------------------------------------------------------------------

  // return(0);
}

// Simple function that calculates the differential equation.
static int f(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
  // N_VGetArrayPointer returns a pointer to the data in the N_Vector class.
  realtype *udata  = N_VGetArrayPointer(u); // pointer u vector data
  realtype *dudata = N_VGetArrayPointer(u_dot); // pointer to udot vector data

  dudata[0] = -101.0 * udata[0] - 100.0 * udata[1];
  dudata[1] = udata[0];

  return(0);
}

// Jacobian function vector routine.
static int jtv (realtype t, N_Vector y, N_Vector fy, SUNMatrix Jac,
                void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3) {

  IJth(Jac, 0 , 0 ) = -101.0;
  IJth(Jac, 0 , 1 ) = -100.0;
  IJth(Jac, 1 , 0 ) = 1.0;
  IJth(Jac, 1 , 1 ) = 0.0;

  return(0);
}

// check_flag function is from the cvDiurnals_ky.c example from the CVODE
// package.
/* Check function return value...
     opt == 0 means SUNDIALS function allocates memory so check if
              returned NULL pointer
     opt == 1 means SUNDIALS function returns a flag so check if
              flag >= 0
     opt == 2 means function allocates memory so check if returned
              NULL pointer */
static int check_flag(void *flagvalue, const char *funcname, int opt) {
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return(1); }

  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *) flagvalue;
    if (*errflag < 0) {
      fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n",
              funcname, *errflag);
      return(1); }}

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return(1); }

  return(0);
}
