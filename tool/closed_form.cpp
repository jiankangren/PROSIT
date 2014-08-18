#include <iostream>
#include "exc.hpp"
#include "auxiliary_func.hpp"
#include "smc_func.hpp"
#include<algorithm>
#include"pmf.hpp"
#include<math.h>
#include<complex>
#include "closed_form.hpp"
#include <Eigen/Dense>

using namespace std;

/* This implementation is only for periodic tasks managed by a resource reservation*/
// double trunccdfU(const pmf & c, int i) {
//   return (i <= c.get_min()) ? c.get(i) : 1;
// }
static bool verbose = false;
bool closed_form_set_verbose(bool v) {
  bool old = verbose;
  verbose = v;
  return old;
};


double closed_form_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q) throw(Exc) {
  PrositAux::cdf c(p.get_size(),p.get_offset());
  PrositAux::pmf2cdf(p, c);

  double a0p = c.get(N*Q-1);
  unsigned WCET = c.get_max();
  double pi_0;
  //  cerr<<"a0p:"<<a0p<<endl;
  if (verbose)
    cout<<"now preparing analytic form"<<endl;
  if (N*Q>WCET) {
    if (verbose)
      cout<<"Bandwidth grater than Worst case requirements"<<endl;
    return 1.0;
  };
  if (abs(a0p) < 1e-10) {
    if (verbose)
      cout<<"Bandwidth too small"<<endl;
    return 0.0;
  };

  pi_0 = 1.0;
  for(unsigned int i = N*Q+1; i <= WCET; i++)
    {
// #ifdef DEBUG
//       cerr<<"i = "<<i<<endl;
//       cerr<<"Probability = "<<(c.get(i)-c.get(i-1))<<endl;
//       cerr<<"Ratio = "<<(c.get(i)-c.get(i-1))/a0p<<endl;
//       cerr<<"Multiplier = "<<(i-N*Q)<<endl;
//       cerr<<"Subtracting:"<<(i-N*Q)*(c.get(i)-c.get(i-1))/a0p<<endl;
// #endif
      pi_0 = pi_0 - (i-N*Q)*(c.get(i)-c.get(i-1))/a0p;
    }
 
  if (verbose)
    cout<<"Analytic computation completed"<<endl;
  if (pi_0 < 0)
    pi_0 = 0;
  return pi_0;
}
double closed_form_old_model_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q) throw(Exc) {
  PrositAux::cdf c(p.get_size(),p.get_offset());
  PrositAux::pmf2cdf(p, c);
  
  double a0p = c.get((N-1)*Q);
  unsigned WCET = c.get_max();
  unsigned Nmax = ceil(double(WCET)/double(Q))-N+1;
  double pi_0;
  //  cerr<<"a0p:"<<a0p<<endl;
  if (verbose)
    cout<<"now preparing analytic form"<<endl;
  if (N*Q>WCET) {
    if (verbose)
      cout<<"Bandwidth grater than Worst case requirements"<<endl;
    return 1.0;
  };
  pi_0 = 1.0;
  for(unsigned int i = N+1; i <= N+Nmax; i++)
    {
      // //#ifdef DEBUG
      // cerr<<"i = "<<i<<endl;
      // cerr<<"Probability = "<<(c.get(i*Q)-c.get((i-1)*Q))<<endl;
      // cerr<<"Ratio = "<< (c.get(i*Q)-c.get((i-1)*Q))/a0p<<endl;
      // cerr<<"Multiplier = "<<(i-N)<<endl;
      // cerr<<"Subtracting:"<<(i-N)*(c.get(i*Q)-c.get((i-1)*Q))/a0p<<endl;
      // 				   //#endif
      pi_0 = pi_0 - (i-N)*(c.get(i*Q)-c.get((i-1)*Q))/a0p;
    }
 
  if (verbose)
    cout<<"Analytic computation completed"<<endl;
  return pi_0;
}
