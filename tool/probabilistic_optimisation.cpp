#include "probabilistic_optimisation.hpp"
#include "matrix.hpp"
#include "closed_form.hpp"
#include "smc_func.hpp"
#define Nc 250000
#define Nz 1200
ProbPeriodicTaskDescriptor::ProbPeriodicTaskDescriptor(const char * nm,
						       auto_ptr<pmf> c, 
						       int Pd, 
						       int Qd, 
						       int Tsd, 
						       int Deltad, 
						       double epsilon) throw (Exc): 
  PeriodicTaskDescriptor(nm, c, Pd, Qd, Tsd), 
  eps(epsilon),
  Delta(Deltad)
{
  if ((Delta>Pd))
    EXC_PRINT_2("illegal delta for task",nm);
  int newDelta = (Delta > 0) ? Delta : 1;
  
  sampledQ = Qd/newDelta;

  auto_ptr<pmf> p (C->resample(newDelta));
  sampledCpmf = p;
  auto_ptr<cdf> q (new cdf(Nc,0));
  sampledCcdf = q;
  pmf2cdf(*sampledCpmf, *sampledCcdf);
  
}

bool ProbPeriodicTaskDescriptor::inv_QoS(double p, int &Q, bool ceil) {
  //  cerr<<"evaluating inverse QoS of"<<p<<" for task:"<<name<<endl;

  if (QoS(Ts)<p) {
    if (ceil) {
      cerr<<"ProbPeriodicTaskDescriptor::inv_QoS unfeasible QoS "<<p<<" for task "<<name;
      return false;
    }
    else {
      Q = Ts;
      return true;
    };
  };
  if (QoS(0)>p) {
    if(!ceil) {
      cerr<<"ProbPeriodicTaskDescriptor::inv_QoS unfeasible QoS"<<p<<" for task "<<name;
      return false;
    }
    else {
      Q = 0;
      return false;
    };
  };
  int Qm=0, QM=Ts;
  while(QM > Qm + 1) {
    // cerr<<"QM:"<<QM<<", Qm>"<<Qm<<endl;
    if (QoS((Qm+QM)/2)< p-eps)
      Qm = (Qm+QM)/2;
    else
      QM = (QM+Qm)/2;
  };
  if(ceil) {
    Q=QM;
  }
  else {
    if (QoS(QM)==p)
      Q = QM;
    else
      Q = Qm;
  };
  return true;
}


ProbPeriodicTaskDescriptorCR::ProbPeriodicTaskDescriptorCR(const char * nm,
							   auto_ptr<pmf>  c, 
							   int Pd, 
							   int Qd, 
							   int Tsd, 
							   int Deltad, 
							   double epsilon) throw (Exc):
  ProbPeriodicTaskDescriptor(nm,c,Pd,Qd,Tsd,Deltad,epsilon), max_iter(10000), shift(true)
{};
  
double ProbPeriodicTaskDescriptorCR::probability(int Q) {
  if(Delta > 0) 
    Q = Q/Delta;
  
  if (P*Q > sampledCpmf->get_max()*Ts)
    return 1.0;
  
  int back = sampledCpmf->get_max()+1-P/Ts*Q;
  int forward = P/Ts*Q-sampledCpmf->get_min ()+1;
  
  //2. compute maxvalue
  int maxv=max(forward,back);
  if (maxv<=0) maxv=1;
  
  MatrixXd mat(3*maxv,3*maxv);
  pmf u(Nz,0);
  u.set(P/Ts,1.0);
  //3. compute matrix
  for (int i=0; i<maxv*3; i++)
    for (int j=0; j<maxv*3; j++)
      mat(i,j) = matrix_prob_ts_compressed(i,j,Q,*sampledCcdf,u);
   
  //4. Extract submatrix
  MatrixXd A0(maxv,maxv), B0(maxv,maxv), A1(maxv,maxv),A2(maxv,maxv);
  compute_matrixes(mat,maxv,B0,A0,A1,A2);
  
  MatrixXd R(A0.rows(),A0.cols());
  struct qbd_cr_parms parms;
  parms.shift =shift;
  parms.verbose=verbose;
	
  parms.max_iter=max_iter;
  MatrixXd G(A0.rows(),A0.cols());
  MatrixXd U(A0.rows(),A0.cols());
  qbd_cr(A2, A1, A0, G, R, U, parms);
  RowVectorXd pi0;
    
  if (qbd_compute_pi0(R,B0,A2,pi0,parms))
    return pi0(0);
  else
    return 0.0;
}
ProbPeriodicTaskDescriptorAnalytic::ProbPeriodicTaskDescriptorAnalytic(const char * nm,
								       auto_ptr<pmf> c, 
								       int Pd, 
								       int Qd, 
								       int Tsd, 
								       int Deltad, 
								       double epsilon) throw (Exc):
  ProbPeriodicTaskDescriptor(nm,c,Pd,Qd,Tsd,Deltad,epsilon)
{};
  
double ProbPeriodicTaskDescriptorAnalytic::probability(int Q) {
  //Adaptation of the Delta
  if ( Delta < 0) {
    int newDelta = max(1,Q/2);
    auto_ptr<pmf> newSampledPmf(C->resample(newDelta));
    return closed_form_compute_pi(*newSampledPmf,P/Ts,Q/newDelta);
  }
  else
    return closed_form_compute_pi(*sampledCpmf,P/Ts,Q/Delta);
}

