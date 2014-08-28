#include <iostream>
#include <Eigen/Dense>
#include "exc.hpp"
#include "auxiliary_func.hpp"
#include "smc_func.hpp"
#include<algorithm>

using namespace Eigen;
using namespace std;
/* This implementation is only for discrete--time */
void qbd_latouche(const MatrixXd & Ap0, 
		  const MatrixXd & Ap1, 
		  const MatrixXd & Ap2,
		  MatrixXd & R,
		  const struct qbd_latouche_parms & parms) throw(Exc) {

  if (!PrositAux::check_sizes(Ap0,Ap1) || !PrositAux::check_sizes(Ap1,Ap2)||!PrositAux::check_sizes(Ap1,R)) 
    EXC_PRINT("A0, A1, A2 matrixes have to be square and equal size");
  MatrixXd Rnew(R.rows(), R.cols());
  unsigned long it=0;
  bool cont=true;
  R.setZero();

  cerr<<"Ap0: "<<Ap0<<endl;
  cerr<<"Ap1: "<<Ap1<<endl;
  cerr<<"Ap2: "<<Ap2<<endl;
  
  while (cont) {
    Rnew = Ap2+R*Ap1+R*R*Ap0;
    cerr<<"||R-Rnew||"<<PrositAux::InfinityNorm(R-Rnew)<<endl;
    if(PrositAux::InfinityNorm(R-Rnew)<parms.epsilon) 
      cont = false;
    else
      R=Rnew;

    if(it++ > parms.max_iter) {
      cont = false;
      cerr<<"QBD_LATOUCHE:Maximum number of iterations reached"<<endl;
    };

    if((it%100==0)&&(parms.verbose)) 
      cerr<<"QBD_LATOUCHE: Iteration "<<it<<" reached."<<endl;
  };
  R = Rnew;
};
