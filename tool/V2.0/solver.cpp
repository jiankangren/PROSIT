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

///@file Command line solver for computation of stady state probabilities



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
static int max_deadline = 0;
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
      /* These options don't set a flag.
	 We distinguish them by their indices. */
      {"budget",  required_argument,       0, 'q'},
      {"period",  required_argument,       0, 't'},
      {"task_period",required_argument, 0, 'T'},
      {"epsilon", required_argument, 0, 'e'},
      {"max_iterations",  required_argument, 0, 'i'},
      {"pessimistic",no_argument,0,'p'},
      {"step", required_argument, 0, 's'},
      {"max_deadline", required_argument,0,'M'},
      {0, 0, 0, 0},
    };
  verbose_flag = 0;
  while ((opt = getopt_long(argc, argv, "t:q:e:i:T:s:M:vbplcom",long_options,0)) != -1) {
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
    case 'M':
      max_deadline = atoi(optarg);
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
    default: /* ?~@~Y??~@~Y */
      throw Exc("opts_parse parameters incorrect");
    }
  }  
  return optind;
}




int main(int argc, char *argv[])
{
  int maxv,i,j;
  int forward,back;
  long long t_start=0, t_matrix_preparation_end=0, t_solve_end=0, t_probability_compute_end=0;
  PrositAux::pmf * c = new PrositAux::pmf(Nc,0);
  PrositAux::pmf * u = new PrositAux::pmf(Nz,0);

#ifdef DEBUG
  VectorXd tmp;
#endif
  int opt;
  try {
    opt = opts_parse(argc, argv);
    if (ps == 1)
      EXC_PRINT("pessimistic version not implemented");
    if ((analytic_flag+cr_flag+latouche_flag+companion_flag)==0)
      latouche_flag = 1;
    if ((analytic_flag+cr_flag+latouche_flag+companion_flag)!=1)
      EXC_PRINT("Ambiguous choice of the algorithm");
    if (max_deadline <= 0)
      EXC_PRINT("Maximum deadline not properly set");
      
    c->load(argv[opt]);
 
    
    if (companion_flag) {
      //TODO: we can easily apply to aperiodic
      if (argc-optind !=1)
	EXC_PRINT("Only one file parameter requested for companion form");
      if (Tp==0)
	EXC_PRINT("Task period needs to be set for companion"));
    }
    else {
      if (argc-optind ==2)
	u->load(argv[opt+1]);
      else {
	if (Tp!=0)
	  u.set(Tp,1.0);
	else
	  EXC_PRINT("Two file parameters requested");
      }
    }
    t_start = PrositAux::my_get_time();
    
    PrositCore::ResourceReservationTaskDescriptor task_des("task", std::move(c), std::move(u), Q, T);
    task_des->set_deadline_step(T);
    for (int i = 0; i < max_deadline; i++)
      task_des->insert_deadline(i);


    if (analytic_flag) {
      EXC_PRINT("Analytic solver TBD");
      // if (verbose_flag) {
      // 	cout<<"CLosed Form Bound Selected"<<endl;
      // 	closed_form_set_verbose(true);
      // };
      // double prob;
      // if (old_model_flag)
      // 	prob = closed_form_old_model_compute_pi(c, Tp/T, Q);
      // else
      // 	prob = closed_form_compute_pi(*h, Tp/T, Q/step);
      // if (verbose_flag) {
      // 	cout<<"Closed Form Bound Computation finished"<<endl;
      // };
      // t_probability_compute_end=PrositAux::my_get_time();
      // if ((prob < 0)||(prob > 1.0+0.0001))
      // 	prob = 0;
      // cout<<"P{d < "<<Tp<<"} "<<prob<<endl;
    }
    else
      {      
	if (companion_flag) {
	  EXC_PRINT("Companion Solver TBD");
	  // RowVectorXd p;
	  // if (verbose_flag) {
	  //   cout<<"Companion Strategy Selected"<<endl;
	  //   companion_set_verbose(true);
	  // };
      
	  // double prob;
	  // if (old_model_flag)
	  //   prob = companion_old_model_compute_pi(c, Tp/T, Q);
	  // else
	  //   prob = companion_compute_pi(*h, Tp/T, Q/step,p, false);
	  
	  // if (verbose_flag) {
	  //   cout<<"Companion Strategy computation finished"<<endl;
	  // };
	  // t_probability_compute_end=PrositAux::my_get_time();
	  // if ((prob < 0)&&(prob > 1+0.0001))
	  //   prob = 0;
	  // cout<<"P{d < "<<Tp<<"} "<<prob<<endl;
	}
	else
	  {
	    if(!latouche_flag)
	      EXC_PRINT("CR to be implemented yet");
	    cout<<"Total time: "<<t_probability_compute_end-t_start<<endl;
	    LatoucheResourceReservationProbabilitySolver solver(step,eps, iter);
	    solver.register_task(task_des);
	    task_des.compute_probability();
	    
	  }
      }

  return 0;
}

