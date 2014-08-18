#include <iostream>
#include <Eigen/Dense>
#include "exc.hpp"
#include "auxiliary_func.hpp"
#include "smc_func.hpp"
#include<algorithm>
#include"pmf.hpp"
#include"companion.hpp"
#include<math.h>
#include<complex>
using namespace Eigen;
using namespace std;

/* This implementation is only for periodic tasks managed by a resource reservation*/
// double trunccdfU(const pmf & c, int i) {
//   return (i <= c.get_min()) ? c.get(i) : 1;
// }
static bool verbose = false;

complex<double>  GammaFunctionsTotal(const complex<double> & b, int dim, const RowVectorXd & Alpha)
{
  int i;
  complex<double> gamma = pow<double>(b,dim);
  for (i = 1; i<= dim; i++)
    gamma += pow<double>(b, int(dim)-i)*Alpha(i+2);
  return gamma;
};


bool companion_set_verbose(bool v) {
  bool old_verbose=verbose;
  verbose = v;
  return old_verbose;
}

double companion_old_model_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q) throw(Exc) {
  PrositAux::cdf c(p.get_size(),p.get_offset());
  PrositAux::pmf2cdf(p, c);
  unsigned BCET = c.get_min();
  unsigned WCET = c.get_max();
  unsigned h0 = N - ceil(double(BCET)/(double(Q)));
  unsigned n = ceil(double(WCET)/(double(Q)))+h0-N;
   
  double a0;
  double pi_0;
  a0=c.get((N-h0)*Q);
  RowVectorXd v(n);
  
  if (verbose)
    cout<<"now preparing the companion form"<<endl;
  if (N*Q>WCET) {
    if (verbose)
      cout<<"Bandwidth grater than Worst case requirements"<<endl;
    return 1.0;
  };
  for (unsigned int i = 0; i < n; i++) {
    double ai = c.get(((N-h0+i+1)*Q))-c.get(((N-h0+i)*Q));
    v(n-i-1)=-ai/a0;
  };

  v(n-h0) = v(n-h0)+1/a0;
  //  cerr<<"v =" <<v<<endl;
  if (verbose)
    cout<<"last row of the comapnion form has size "<<v.size()<<endl;

  VectorXd z(n-1);
  z.setZero();
  MatrixXd I = MatrixXd::Identity(n-1, n-1);
  MatrixXd M(n,n);
  M.block(0,0,n-1,1) = z;
  M.block(0,1,n-1,n-1)=I;
  M.block(n-1,0,1,n)=v;
  //cerr<<"Matrix M: "<<M<<endl;
  if (verbose)
    cout<<"Companion form matrix size "<<M.size()<<endl;

  EigenSolver<MatrixXd> eigensolver(M,false);
 if (verbose)
   cout<<"Companion form eigenvalues found "<< endl;

  if (eigensolver.info() != Success) 
    EXC_PRINT("cannot compute eigenvalues of m");
  //cerr<<"eigenvalues "<<eigensolver.eigenvalues()<<endl;
  complex<double> product(1,0);
  unsigned unst_num=0;
  VectorXcd unstable(n);
  for (unsigned int i = 0; i < n; i++) {
    complex<double> p_i = eigensolver.eigenvalues()[i];
    if(abs(p_i)<1-0.000000001)
      product = product*(complex<double>(1,0)-p_i);
    else
      unstable(unst_num++) = p_i;
  }
  if (product.imag()>0.00000001)
    cerr<<"Warning the probability did not reslt into a real number"<<endl;
  pi_0 = product.real();
  if ((pi_0 < 0)||(pi_0 > 1.0000001))
    cerr<<"ERROR: companion did not produce the correct result"<<endl;
  return pi_0;
}

double companion_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q, RowVectorXd & pi, bool compute_vector) throw(Exc) {
  PrositAux::cdf c(p.get_size(),p.get_offset());
  PrositAux::pmf2cdf(p, c);
  unsigned BCET = c.get_min();
  unsigned WCET = c.get_max();
  unsigned size = (WCET-BCET);
  double a0;
  double pi_0;
  a0=c.get(BCET);
  RowVectorXd v(size);
  RowVectorXd Alfa(size+1);
  Alfa(0) = 1;
  if (verbose)
    cout<<"now preparing the companion form"<<endl;
  if (N*Q>WCET) {
    if (verbose)
      cout<<"Bandwidth grater than Worst case requirements"<<endl;
    return 1.0;
  };
  for (unsigned int i = 0; i < size; i++) {
    double ai = c.get(i+BCET+1)-c.get(i+BCET);
    v(size-i-1)=-ai/a0;
    Alfa(i+1)=ai/a0;
  };

  v(WCET-N*Q) = v(WCET-N*Q)+1/a0;
  //  cerr<<"v =" <<v<<endl;
  if (verbose)
    cout<<"last row of the comapnion form has size "<<v.size()<<endl;

  VectorXd z(size-1);
  z.setZero();
  MatrixXd I = MatrixXd::Identity(size-1, size-1);
  MatrixXd M(size,size);
  M.block(0,0,size-1,1) = z;
  M.block(0,1,size-1,size-1)=I;
  M.block(size-1,0,1,size)=v;
  //cerr<<"Matrix M: "<<M<<endl;
  if (verbose)
    cout<<"Companion form matrix size "<<M.size()<<endl;

  EigenSolver<MatrixXd> eigensolver(M,false);
 if (verbose)
   cout<<"Companion form eigenvalues found "<< endl;

  if (eigensolver.info() != Success) 
    EXC_PRINT("cannot compute eigenvalues of m");
  //cerr<<"eigenvalues "<<eigensolver.eigenvalues()<<endl;
  complex<double> product(1,0);
  unsigned unst_num=0;
  VectorXcd unstable(size);
  for (unsigned int i = 0; i < size; i++) {
    complex<double> p_i = eigensolver.eigenvalues()[i];
    if(abs(p_i)<1-0.000000001)
      product = product*(complex<double>(1,0)-p_i);
    else
      unstable(unst_num++) = p_i;
  }
  if (product.imag()>0.00000001)
    cerr<<"Warning the probability did not reslt into a real number"<<endl;
  pi_0 = product.real();
  if ((pi_0 < 0)||(pi_0 > 1.0000001)) {
    cerr<<"ERROR: companion did not produce the correct result"<<endl;
    pi_0 = 0.0;
  };
  if (!compute_vector)
    return pi_0;
  VectorXcd b(unst_num);
  MatrixXcd A(unst_num, unst_num);
  RowVectorXcd Ah(unst_num);
  for (unsigned int h = 0; h<unst_num;h++) {
    complex<double> betah = unstable(h);
    complex<double> bh = 0;
    for (unsigned int q1 = 0; q1<=unst_num; q1++)
      bh = bh + GammaFunctionsTotal(betah, q1, Alfa);
    b(h-1) = -bh*pi_0;
    for (unsigned int k = 1; k<=unst_num;k++) {
      complex<double> ah = 0;
      for (unsigned int q1=0; q1<= unst_num-k; q1++)
	ah += GammaFunctionsTotal(betah,q1,Alfa);
      Ah(k-1) = ah;
    }
    A.block(h,0,h,unst_num-1)=Ah;
  }
  RowVectorXd pi_final(unst_num+1);
  pi_final(0) = pi_0;
  MatrixXcd Pinv;
  VectorXcd res;
  PrositAux::pseudoInverse<MatrixXcd>(A,Pinv);
  res = Pinv*b;
  
  for (unsigned int i = 1; i<unst_num+1; i++) {
    complex<double> el = res(i);
     if (el.imag()>0.00000001)
       cerr<<"Warning the probability did not reslt into a real number"<<endl;
     pi_final(i) = el.real();
  }
  pi = pi_final;
  return pi_0;
}



// double companion(const pmf &p, unsigned int step, unsigned int q) throw(Exc) {
//   if (q%step) 
//     throw Exc("QBD_COMPANION: q must be a multiple of step");
//   q = q/step;
//   cerr<<"Check0"<<endl;
//   pmf * c = p.resample(step);
//   cerr<<"check1"<<endl;
//   cerr<<"COMPANION: max "<<c->get_max()<<", min "<<c->get_min()<<endl;
//   cerr<<"COMPANION: tail: "<<c->get_tail()<<endl;
//   cerr<<"COMPANION: sum: "<<c->sum()<<endl;

//   c->print();
//   cerr<<"check1a"<<endl;
//   unsigned size = (c->get_max()-c->get_min());
//   cerr<<"check2 size: " <<size<<endl;
//   MatrixXd m(size,size);
//   unsigned i;
//   m.setZero();
  
//   for (i = 0; i < size; i++) {
//     if(i>0) {
//       int j = i-1;
//       m(j,i)=1;
//     };
//     if((size-i)*step+c->get_min() == q)
//       m(size-1,i) = (1-c->get((size-i)*step+c->get_min()))/c->get_min();
//     else
//       m(size-1,i) = (-c->get((size-i)*step+c->get_min()))/c->get_min();
//   };
  
//   SelfAdjointEigenSolver<MatrixXd> eigensolver(m,false);
//   if (eigensolver.info() != Success) 
//     throw Exc("QBD_COMPANION: cannot compute eigenvalues of m");
//   complex<double> product(1,0);
//   for (i = 0; i < size; i++) {
//     complex<double> p_i = eigensolver.eigenvalues()[i];
//     if(abs(p_i)<1-0.000000001)
//       product = product*(complex<double>(1,0)-p_i);
//   }
//   delete c;
//   if (product.imag()>0.00000001)
//     cerr<<"Warning the probability did not reslt into a real number"<<endl;
//   return product.real();
// }
