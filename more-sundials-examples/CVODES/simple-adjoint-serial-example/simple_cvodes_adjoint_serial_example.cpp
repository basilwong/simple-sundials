/*
A simple example with the CVODES library using adjoint backward sensitivity
analysis to solve a simple ODE system.
*/

// 1. Include nessesary header files.
// -----------------------------------------------------------------------------
#include <iostream>
#include <cvodes/cvodes.h> // prototypes for CVODE fcts., consts.
#include <nvector/nvector_serial.h>  // access to serial N_Vector
#include <sunlinsol/sunlinsol_spgmr.h>  //access to SPGMR SUNLinearSolver
#include <cvodes/cvodes_spils.h> // access to CVSpils interface
#include <sundials/sundials_dense.h>  // use generic dense solver in precond
#include <sundials/sundials_types.h>  // defs. of realtype, sunindextype
#include <sundials/sundials_math.h>  // contains the macros ABS, SUNSQR, EXP
// -----------------------------------------------------------------------------

// This macro gives access to the individual components of the data array of an
// N Vector.
#define NV_Ith_S(v,i) ( NV_DATA_S(v)[i] )

static int f(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
static int fb(realtype t, N_Vector y, N_Vector yB, N_Vector yBdot,
              void *user_data);
static int jtv(N_Vector v, N_Vector Jv, realtype t, N_Vector u, N_Vector fu,
               void *user_data, N_Vector tmp);
static int check_flag(void *flagvalue, const char *funcname, int opt);


int main() {
  int flag; // For checking if functions have run properly
  realtype abstol = 1e-5; // real tolerance of system
  realtype reltol = 1e-5; // absolute tolerance of system

  // 2. Initialize parallel or multi-threaded environment, if appropriate.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  /* FORWARD PROBLEM */

  // 3. Set problem dimensions etc. for the forward problem.
  // ---------------------------------------------------------------------------
  sunindextype N_forward = 2;
  // ---------------------------------------------------------------------------

  // 4. Set initial conditions for the forward problem.
  // ---------------------------------------------------------------------------
  N_Vector y_forward; // Problem vector.
  y_forward = N_VNew_Serial(N_forwrad);
  NV_Ith_S(y_forward, 0) = 2.0;
  NV_Ith_S(y_forward, 1) = 1.0;
  // ---------------------------------------------------------------------------

  // 5. Create CVODES object for the forward problem.
  // ---------------------------------------------------------------------------
  void *cvode_mem = NULL; // Problem dedicated memory.
  cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
  // ---------------------------------------------------------------------------

  // 6. Initialize CVODES for the forward problem.
  // ---------------------------------------------------------------------------
  realtype t0 = 0; // Initiale value of time.
  flag = CVodeInit(cvode_mem, f, t0, y);
  if (check_flag(&flag, "CVodeSetUserData", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 7. Specify integration tolerances for the forward problem.
  // ---------------------------------------------------------------------------
  flag = CVodeSStolerances(cvode_mem, reltol, abstol);
  if (check_flag(&flag, "CVodeSStolerances", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 8. Set optional inputs for the forward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 9. Create matrix object for the forward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 10. Create linear solver object for the forward problem.
  // ---------------------------------------------------------------------------
  SUNLinearSolver LS;
  // Here we chose one of the possible linear solver modules. SUNSPMR is an
  // iterative solver that is designed to be compatible with any nvector
  // implementation (serial, threaded, parallel,
  // user-supplied)that supports a minimal subset of operations.
  LS = SUNSPGMR(y, 0, 0);
  if (check_flag((void *)LS, "SUNSPGMR", 0)) return(1);
  // ---------------------------------------------------------------------------

  // 11. Set linear solver optional inputs for the forward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 12. Attach linear solver module for the forwrad problem.
  // ---------------------------------------------------------------------------
  // CVSpilsSetLinearSolver is for iterative linear solvers.
  flag = CVSpilsSetLinearSolver(cvode_mem, LS);
  if (check_flag(&flag, "CVSpilsSetLinearSolver", 1)) return 1;
  // ---------------------------------------------------------------------------

  // 13. Initialize quadrature problem or problems for forward problems, using
  // CVodeQuadInit and/or CVodeQuadSensInit.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 14. Initialize forward sensitivity problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------


  // 15. Specify rootfinding.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 16. Allocate space for the adjoint computation.
  // ---------------------------------------------------------------------------
  // Type of interpolation used depends on CV_POLYNOMIAL or CV_HERMITE.
  flag = CVodeAdjInit(cvode_mem, NSTEPS, CV_HERMITE);
  if (check_flag(&flag, "CVadjInit", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 17. Integrate forward problem.
  // ---------------------------------------------------------------------------
  // Have the solution advance over time, but stop to log 100 of the steps.
  int print_steps = 100;
  realtype tout;
  realtype end_time = 50;
  realtype step_length = 0.5;
  realtype t = 0;
  int ncheck = 0;
  // loop over output points, call CVode, print results, test for error
  for (tout = step_length; tout <= end_time; tout += step_length) {
    // CVodeF is similar to the CVode advance in time operation, but it also
    // stores checkpoint data every Nd integration steps.
    flag = CVodeF(
        cvode_mem, // pointer to the cvodes memory block
        tout, // the next time at which a computed solution is desired
        y, // the computed solution vector y
        &t, // the time reached by the solver (output)
        CV_NORMAL, // CV_NORMAL has the solver take internal steps until it has
                   // reached or just passed the user-speciﬁed tout parameter
        &ncheck //  the number of (internal) checkpoints stored so far
        );
    // std::cout << "t: " << t;
    // std::cout << "\ny:";
    // N_VPrint_Serial(y);
    if (check_flag(&flag, "CVodeF", 1)) break;
  }
  // ---------------------------------------------------------------------------

  /* Backward Problem */

  // 18. Set problem dimensions for the backward problem.
  // ---------------------------------------------------------------------------
  sunindextype N_backward = 2;
  // ---------------------------------------------------------------------------

  // 19. Set initial values for the backward problem.
  // ---------------------------------------------------------------------------
  N_Vector y_backward; // Problem vector.
  y_backward = N_VNew_Serial(N_backward);
  NV_Ith_S(y_backward, 0) = 2.0;
  NV_Ith_S(y_backward, 1) = 1.0;
  // ---------------------------------------------------------------------------

  // 20. Create the backward problem.
  // ---------------------------------------------------------------------------
  int indexB; // contains the identiﬁer assigned by cvodes for the newly
              // created backward problem.
  flag = CVodeCreateB(cvode_mem, CV_BDF, CV_NEWTON, &indexB);
  if (check_flag(&flag, "CVodeCreateB", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 21. Allocate memory for the backward problem.
  // ---------------------------------------------------------------------------
  flag = CVodeInitB(cvode_mem, indexB, fb, end_time, y_backward);
  if (check_flag(&flag, "CVodeInitB", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 22. Specify integration tolerances for the backward problem.
  // ---------------------------------------------------------------------------
  flag = CVodeSStolerancesB(cvode_mem, indexB, reltol, abstol);
  if (check_flag(&flag, "CVodeSStolerancesB", 1)) return(1);
  // ---------------------------------------------------------------------------

  // 23. Set optional inputs for the backward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 24. Create linear solver for the backward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 25. Create linear solver for the backwrad problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 26. Set linear solver interface optional inputs for the backward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 27. initialize quadrature calculation.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------

  // 28. Integrate backward problem.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------


  // 29. Extract quadrature variables.
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------


  // 16. Deallocate memory.
  // ---------------------------------------------------------------------------
  N_VDestroy(y);
  CVodeFree(&cvode_mem);
  // ---------------------------------------------------------------------------

  // 18. Free linear solver and matrix memory for the backward problem.
  // ---------------------------------------------------------------------------
  SUNLinSolFree(LS);
  // ---------------------------------------------------------------------------

  return(0);
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

// The calculates for the right hand side of the backward problem.
static int fb(realtype t, N_Vector y, N_Vector yB, N_Vector yBdot,
              void *user_data) {
  // N_VGetArrayPointer returns a pointer to the data in the N_Vector class.
  realtype *udata  = N_VGetArrayPointer(u); // pointer u vector data
  realtype *dudata = N_VGetArrayPointer(u_dot); // pointer to udot vector data

  dudata[0] = -101.0 * udata[0] - 100.0 * udata[1];
  dudata[1] = udata[0];

  return(0);
}

// Jacobian function vector routine.
static int jtv(N_Vector v, N_Vector Jv, realtype t, N_Vector u, N_Vector fu,
               void *user_data, N_Vector tmp) {
  realtype *udata  = N_VGetArrayPointer(u);
  realtype *vdata  = N_VGetArrayPointer(v);
  realtype *Jvdata = N_VGetArrayPointer(Jv);
  realtype *fudata = N_VGetArrayPointer(fu);

  Jvdata[0] = -101.0 * vdata[0] + -100.0 * vdata[1];
  Jvdata[1] = vdata[0] + 0 * vdata[1];

  fudata[0] = 0;
  fudata[1] = 0;

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