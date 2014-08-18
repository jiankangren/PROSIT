#include <iostream>
#include <algorithm>
#include "auxiliary_func.hpp"
#include "matrix_io.hpp"
#include "exc.hpp"
#include "smc_func.hpp"
#include "pmf.hpp"
#include "companion.hpp"
#include <stdio.h>
#include <algorithm>
#include <getopt.h>
#include "matrix.hpp"
#include "closed_form.hpp"
#define Nc 250000
#define Nz 1200

static int Q = 10000;
static int T = 20000;
static int Tp = 0;
static int step=1;
int ps=0;
static int verbose_flag=0;
static int latouche_flag = 0;
static int companion_flag = 0;
static int compress_flag = 0;
static int analytic_flag=0;
static int cr_flag = 0;
static int old_model_flag = 0;

static int iter = 100;
static double eps = 1e-4;


static int opts_parse(int argc, char *argv[]) throw (Exc)
{
  int opt;
  static struct option long_options[16] =
    {
      /* These options set a flag. */
      {"verbose", no_argument,0,'v'},
      {"brief",   no_argument,0,'b'},
      {"latouche",no_argument, 0, 'l'},
      {"cyclic",no_argument, 0, 'c'},
      {"companion", no_argument,0,'o'},
      {"compress",no_argument,0,'m'},
      {"analytic",no_argument,0,'a'},
      {"old_model",no_argument,0,'d'},
      /* These options don't set a flag.
	 We distinguish them by their indices. */
      {"budget",  required_argument,       0, 'q'},
      {"period",  required_argument,       0, 't'},
      {"task_period",required_argument, 0, 'T'},
      {"epsilon", required_argument, 0, 'e'},
      {"max_iterations",  required_argument, 0, 'i'},
      {"pessimistic",no_argument,0,'p'},
      {"step", required_argument, 0, 's'},
      {0, 0, 0, 0},
    };
  verbose_flag = 0;
  while ((opt = getopt_long(argc, argv, "t:q:e:i:T:s:vbdplcom",long_options,0)) != -1) {
    switch (opt) {
    case 'q':
      Q = atoi(optarg);
      break;
    case 't':
      T = atoi(optarg);
      break;
    case 'i':
      iter = atoi(optarg);
      break;
    case 'e':
      eps = atof(optarg);
      break;
    case 'T':
      Tp = atoi(optarg);
      break;
    case 'p':
      ps = 1;
      break;
    case 'c':
      cr_flag=1;
      break;
    case 'l':
      latouche_flag=1;
      break;
    case 'o':
      companion_flag=1;
      break;
    case 'v': 
      verbose_flag=1;
      break;
    case 'b':
      verbose_flag = 0;
    case 's':
      step = atoi(optarg);
      break;
    case 'm':
      compress_flag = 1;
      break;
    case 'a':
      analytic_flag = 1;
      break;
    case 'd':
      old_model_flag = 1;
      break;
    default: /* ?~@~Y??~@~Y */
      throw Exc("opts_parse parameters incorrect");
    }
  }  
  return optind;
}




int main(int argc, char *argv[])
{
  //double avgc;
  int maxv,i,j;
  int forward,back;
  long long t_start=0, t_matrix_preparation_end=0, t_solve_end=0, t_probability_compute_end=0;
  MatrixXd B0, A0, A1, A2;// matp, A0p, A1p, A2p, Rp;

  //  VectorXd *X0,*X0p,*X1;
#ifdef DEBUG
  VectorXd tmp;
#endif
  int opt;
  try {
    opt = opts_parse(argc, argv);
    if (ps == 1)
      throw (Exc("pessimistic version not implemented"));
    if ((analytic_flag+cr_flag+latouche_flag+companion_flag)==0)
      latouche_flag = 1;
    if ((analytic_flag+cr_flag+latouche_flag+companion_flag)!=1)
      throw (Exc("Ambiguous choice of the algorithm"));
    if (old_model_flag) {
      if ((analytic_flag+companion_flag)==0)
	cerr<<"Warning::Old model option ignored"<<endl;
      else
	if (step!=1) {
	  cerr<<"Warning step ignored for old model"<<endl;
	  step = 1;
	}
    };
    PrositAux::pmf c(Nc,0);
    PrositAux::pmf u(Nz,0);
    
    if (companion_flag) 
      {
	if (argc-optind !=1)
	  throw(Exc("one file parameter requested for companion form"));
      };
    
    c.load(argv[opt]);
    //TODO: we can easily apply to aperiodic
    if (companion_flag) {
      if (Tp==0)
	throw(Exc("Task period needs to be set for companion"));
    }
    else {
      if (argc-optind ==2)
	  u.load(argv[opt+1]);
      else {
	if (Tp!=0)
	  u.set(Tp/T,1.0);
	else
	  throw(Exc("Two file parameters requested"));
      }
    }
    t_start = PrositAux::my_get_time();
    PrositAux::pmf * h;
    if (step !=1) {
      if (verbose_flag) { 
	cout<<"WCET: "<<c.get_max()<<endl;
	cout<<"BCET: "<<c.get_min()<<endl;
	cout<<"Now resampling the distribution"<<endl;
      }
      h = c.resample(step);
      if (verbose_flag) {
	cout<<"Distribution resampled"<<endl;
	cout<<"WCET: "<<h->get_max()<<endl;
	cout<<"BCET: "<<h->get_min()<<endl;
      }
    }	else
      h = &c;
    if (analytic_flag) {
      if (verbose_flag) {
	cout<<"CLosed Form Bound Selected"<<endl;
	closed_form_set_verbose(true);
      };
      double prob;
      if (old_model_flag)
	prob = closed_form_old_model_compute_pi(c, Tp/T, Q);
      else
	prob = closed_form_compute_pi(*h, Tp/T, Q/step);
      if (verbose_flag) {
	cout<<"Closed Form Bound Computation finished"<<endl;
      };
      t_probability_compute_end=PrositAux::my_get_time();
      if ((prob < 0)||(prob > 1.0+0.0001))
	prob = 0;
      cout<<"P{d < "<<Tp<<"} "<<prob<<endl;
    }
    else
      {      
	if (companion_flag) {
	  RowVectorXd p;
	  if (verbose_flag) {
	    cout<<"Companion Strategy Selected"<<endl;
	    companion_set_verbose(true);
	  };
      
	  double prob;
	  if (old_model_flag)
	    prob = companion_old_model_compute_pi(c, Tp/T, Q);
	  else
	    prob = companion_compute_pi(*h, Tp/T, Q/step,p, false);
	  
	  if (verbose_flag) {
	    cout<<"Companion Strategy computation finished"<<endl;
	  };
	  t_probability_compute_end=PrositAux::my_get_time();
	  if ((prob < 0)&&(prob > 1+0.0001))
	    prob = 0;
	  cout<<"P{d < "<<Tp<<"} "<<prob<<endl;
	}
	else
	  {
	    PrositAux::cdf cdfc(h->get_size(),0);
	    //1. compute cdf of U
	    PrositAux::pmf2cdf(*h,cdfc);
	    Q = Q/step;
	    if (compress_flag) {
	      if(verbose_flag)
		cout<<"Computing matrix in compressed form"<<endl;
	      if (u.get_min()*Q > h->get_max()) 
		throw Exc("Compressed mode not usable: task always finishes before its next activation");
	      back = h->get_max()+1-u.get_min()*Q;
	      forward = u.get_min()*Q-h->get_min ()+1;
	      //2. compute maxvalue
	    }
	    else {
	      if(verbose_flag)
		cout<<"Computing matrix in Extended form"<<endl;

	      forward = u.get_min()*Q+1;
	      back = h->get_max()+1;
	    }
	    //2. compute maxvalue
	    maxv=max(forward,back);
	    if (maxv<=0) maxv=1;
	    if (verbose_flag)
	      cout<<"Computing Matrix. Size: "<<maxv<<endl;
	    MatrixXd mat(3*maxv,3*maxv);

	    //3. compute matrix
	    for (i=0; i<maxv*3; i++){
	      for (j=0; j<maxv*3; j++){
		if (compress_flag)
		  mat(i,j) = matrix_prob_ts_compressed(i,j,Q,cdfc,u);
		else
		  mat(i,j) = matrix_prob_ts(i,j,Q,cdfc,u);
	      }
	    }
	    if (verbose_flag)
	      cout<<"Matrix computed. Now extracting submatrixes "<<endl;
	
	    //4. Extract submatrix
	    MatrixXd A0(maxv,maxv), B0(maxv,maxv), A1(maxv,maxv),A2(maxv,maxv);
	    compute_matrixes(mat,maxv,B0,A0,A1,A2);
	    if (verbose_flag)
	      cout<<"Submatrices extracted "<<endl;

	    MatrixXd R(A0.rows(),A0.cols());

	    if (cr_flag) {
	      if (verbose_flag)
		cout<<"Cyclic Reduction Selected"<<endl;
	      struct qbd_cr_parms parms;
	      parms.shift =false;
	      if (!verbose_flag)
		parms.verbose=false;
	      else
		parms.verbose=true;
	      parms.max_iter=iter;
	      MatrixXd G(A0.rows(),A0.cols());
	      MatrixXd U(A0.rows(),A0.cols());
	      t_matrix_preparation_end = PrositAux::my_get_time();
	      qbd_cr(A2, A1, A0, G, R, U, parms);
	      if (verbose_flag)
		cout<<"Cyclic reduction computation finished"<<endl;
	      t_solve_end=PrositAux::my_get_time();
	    };


	    if (latouche_flag) {
	      if (verbose_flag)
		cout<<"Latouche Selected"<<endl;
	      struct qbd_latouche_parms p1;
	      p1.max_iter=iter;
	      p1.epsilon=eps;
	      if (!verbose_flag)
		p1.verbose=false;
	      else
		p1.verbose=true;
  
     
	      t_matrix_preparation_end = PrositAux::my_get_time();
	      qbd_latouche(A2, A1, A0, R, p1);
	      if (verbose_flag)
		cout<<"Latouche computation finished"<<endl;
	      t_solve_end=PrositAux::my_get_time();
	      // if (ps){
	      //   Rp=m_get(maxv,maxv);
	      //   Rp=computeR(Rp,A0p,A1p,A2p,0.001);
	      // }
	    };

#ifdef DEBUG
	    cerr<<"R = "<<R<<endl;
#endif


	    //6. Compute X0 from R
	    RowVectorXd pi0;
	    struct qbd_parms parms;
	    if(verbose_flag)
	      parms.verbose=true;
	    else
	      parms.verbose=false;
	    parms.max_iter=iter;
	    qbd_compute_pi0(R,B0,A2,pi0,parms);
	    t_probability_compute_end=PrositAux::my_get_time();
#ifdef DEBUG
	    cerr<<"pi0 :"<<pi0<<endl;
	    cerr<<"Probability: "<<pi0.sum()<<endl;
#endif
	    if (compress_flag) {
	      double prob = 0.0; int delta=0;
	      cout<<"P{d < "<<T*u.get_min()<<"} "<<pi0(0)<<endl;
	      for (i = 0; i<pi0.size(); i++) {
		prob += pi0(i);
		if (i%Q == 0)
		  cout<<"P{d < "<<u.get_min()*T+T*(++delta)<<"} "<<prob<<endl;
	      };
	    }
	    else
	      {
		double prob = 0.0; int delta=0;
		for (i = 0; i<pi0.size(); i++) {
		  prob += pi0(i);
		  if (i%Q == 0)
		    cout<<"P{d < "<<T*(delta++)<<"} "<<prob<<endl;
		};
	      }
	  };
      }
    if (step != 1)
      delete h;

  } catch (Exc &e) {
    e.what();
  };
  cout<<"Total time: "<<t_probability_compute_end-t_start<<endl;
  if (latouche_flag || cr_flag) 
    cout<<"Qbd solve: "<<t_solve_end-t_matrix_preparation_end<<endl;
  

  return 0;
}

