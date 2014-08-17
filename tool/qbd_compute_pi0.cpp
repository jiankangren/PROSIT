#include <iostream>
#include <Eigen/Dense>
#include "exc.hpp"
#include "auxiliary_func.hpp"
#include "smc_func.hpp"
#include<algorithm>

bool qbd_compute_pi0(const MatrixXd & R,
		     const MatrixXd & B0,
		     const MatrixXd & A0,
		     RowVectorXd & pi0,
		     const qbd_parms & parms) throw (Exc) {
  if(R.rows()!=R.cols())
    EXC_PRINT("R had to be square");

  if (!PrositAux::check_sizes(A0,R) || !PrositAux::check_sizes(A0,B0)) 
    EXC_PRINT("A0, A1, A2 matrixes have to be square and equal size");

  if ((R.minCoeff()<0)&&parms.verbose)
    cerr<<"QBD_COMPUTE_PI0: Warning: R has negative coeeficients"<<endl;
  SelfAdjointEigenSolver<MatrixXd> eigensolver(R);
  if (eigensolver.info() != Success) {
    if (parms.verbose)
      cerr<<"QBD_COMPUTE_PI0: cannot compute eigenvalues of R"<<endl;
    return false;
  }
  if ((ArrayXd(eigensolver.eigenvalues()).abs().maxCoeff()>1)&&parms.verbose)
    cerr<<"QBD_COMPUTE_PI0: Warning: R has spectral radius greater than 1"<<endl;
  int n = R.rows();
  MatrixXd Id = MatrixXd::Identity(n,n);
  VectorXd u(n);
  u.setOnes();
  MatrixXd M(n,n+1);
  M.block(0,0,n,n)= B0+R*A0-Id;
  M.block(0,n,n,1)= (Id-R).inverse()*u;

  FullPivLU<MatrixXd> lu_decomp(M);
  if(lu_decomp.rank()<n) {
    if (parms.verbose)
      cerr<<"QBD_COMPUTE_PI0: No unique solution"<<endl;
    return false;
  }

  RowVectorXd work(n+1);
  work.setZero();
  work(n)=1;
  MatrixXd W1;
  PrositAux::pseudoInverse<MatrixXd>(M,W1);
  pi0 = work*W1;
  if ((pi0.minCoeff()<0)&&parms.verbose)
    cerr<<"QBD_COMPUTE_PI0: Warning: x0 has negative elements"<<endl;
  return true;
}
