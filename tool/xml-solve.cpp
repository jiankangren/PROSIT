#include "probabilistic_optimisation.hpp"
#include "pmf.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include "parser.hpp"
#include "standard_distr.hpp"
#include "standard_task.hpp"
#include "standard_qosfun.hpp"
#include "distr_factory.hpp"
#include "task_factory.hpp"
#include <getopt.h>

bool verbose_flag;
bool verbose_set;
bool help_flag;
long long  t_start=0, t_xml_parse_end = 0;

static int opts_parse(int argc, char *argv[]) throw (Exc)
{
  int opt;
  static struct option long_options[4] =
    {
      /* These options set a flag. */
      {"verbose", no_argument,0,'v'},
      {"silent", no_argument,0,'s'},
      {"help",   no_argument,0,'h'},
      {0, 0, 0, 0},
    };
  verbose_flag = false;
  verbose_set = false;
  help_flag = false;
  while ((opt = getopt_long(argc, argv, ":vhs",long_options,0)) != -1) {
    switch (opt) {
    case 'v': 
      verbose_flag=true;
      verbose_set = true;
      break;
    case 's':
      verbose_set = true;
      verbose_flag = false;
      break;
    case 'h':
      help_flag = true;
      break;
    default: /* ?~@~Y??~@~Y */
      EXC_PRINT("opts_parse parameters incorrect");
    }
  }  
  return optind;
}


static int solve_core(vector<GenericTaskDescriptor*> & v, vector<double> & probability, vector<double> & quality, vector<long long> & time) {
  long long t_solution_start_i=0, t_solution_end_i=0;
  int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
  if(verbose_flag)
    cout<<"Number of tasks parsed: "<<num<<endl;
  
  int i = 0;
  for (vector<GenericTaskDescriptor*>::iterator it = v.begin() ; (it != v.end()); ++it) {
    ProbPeriodicTaskDescriptor * p = dynamic_cast<ProbPeriodicTaskDescriptor *>(*it);
    if(!p) {
      cerr<<"Sorry. Analysis works only for probabilistic periodic tasks as yet."<<endl;
      return 0;
    }
    t_solution_start_i = my_get_time();
    probability[i] = p->probability(p->get_budget());
    t_solution_end_i = my_get_time();
    quality[i] = p->QoS_from_prob(probability[i]);
    time[i] = t_solution_end_i - t_solution_start_i;
    i++;
  };
  return v.size();
};


static int solve_display_results(vector<GenericTaskDescriptor*> & v, const vector<double> & probability, const vector<double> & quality, const vector<long long> & time, bool show_time) {
  cout<<"Analysis results."<<endl;
  cout<<"=========================================================================================================="<<endl;
  cout<<"=                                                Results                                                 ="<<endl;
  cout<<"=========================================================================================================="<<endl;
  double Btot=0;
  int i = 0;
  double inf_norm=1e38;

  for (vector<GenericTaskDescriptor*>::iterator it = v.begin() ; (it != v.end()); ++it) {
    printf("   Task: %8s. Budget: %10d. Bandwidth: %10f. Probability %10f. Quality: %10f.", (*it)->get_name().c_str(), (*it)->get_budget(),double((*it)->get_budget())/double((*it)->get_server_period()),probability[i], quality[i]);
    if(show_time)
      printf(" Time: %*llu\n",25,time[i]);
    else
      printf("\n");
    Btot += double((*it)->get_budget())/double((*it)->get_server_period());
    inf_norm=min<double>(quality[i],inf_norm);	  
    i++;
  }
  cout<<"=========================================================================================================="<<endl;
  printf("\tTotal bandwidth: \t\t%25f\n",Btot);
  printf("\tInfinity norm value: \t\t%25f\n",inf_norm);
  
  return v.size();
}

static int solve_execute() {
  long long t_solution_start=0, t_solution_end=0;
  t_solution_start = my_get_time();
  vector<GenericTaskDescriptor*> v;
  int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);

  vector<double> probability(num);
  vector<double> quality(num);
  vector<long long> time(num);


  solve_core(v,probability, quality, time);
  t_solution_end = my_get_time();
  solve_display_results(v, probability, quality, time,true);
  cout<<"=========================================================================================================="<<endl;
  cout<<"=                                              Computation time                                         ="<<endl;
  cout<<"=========================================================================================================="<<endl;
  printf("\tParsing time: \t\t\t%*llu \n",25,t_xml_parse_end  - t_start);
  printf("\tSolution time: \t\t\t%*llu \n",25,t_solution_end-t_solution_start);
  printf("\tTotal time: \t\t\t%*llu \n", 25,t_solution_end-t_start);
  cout<<"=========================================================================================================="<<endl;
  return 1;
}
static int opt_execute(XMLParser::Parser * p) {
  long long t_optimisation_start=0, t_optimisation_set_up = 0, t_optimisation_end=0;

  t_optimisation_set_up = my_get_time();
  vector<GenericTaskDescriptor*> v;
  int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
  if(verbose_flag)
    cout<<"Number of tasks parsed: "<<num<<endl;
    
  InfinityNormBudgetOptimiser Opt(v, p->get_optimisation_epsilon(),p->get_total_bandwidth());
  Opt.init_target_bounds();
  if(verbose_set)
    Opt.set_verbose(verbose_flag);
  else
    Opt.set_verbose(p->get_verbose());
    
  t_optimisation_start=my_get_time();
    
  Opt.optimise();
    
  t_optimisation_end=my_get_time();
  vector<double> probability(num);
  vector<double> quality(num);
  vector<long long> time(num);
  if (Opt.get_state() != GenericBudgetOptimiser::OK) {
    cerr<<"Optimisation failed"<<endl;
    return 0;
  };
  
  solve_core(v, probability, quality, time);
  
  cout<<"Optimisation succeeded."<<endl;
  solve_display_results(v, probability, quality, time, false);

  cout<<"=========================================================================================================="<<endl;
  cout<<"=                                              Computation time                                         ="<<endl;
  cout<<"=========================================================================================================="<<endl;
  printf("\tParsing time: \t\t\t%*llu \n",25,t_xml_parse_end  - t_start);
  printf("\tOptimisation setup: \t\t%*llu \n",25,t_optimisation_start-t_optimisation_set_up);
  printf("\tOptimisation time: \t\t%*llu \n",25,t_optimisation_end-t_optimisation_start);
  printf("\tTotal time: \t\t\t%*llu \n", 25,t_optimisation_end-t_start);
  cout<<"=========================================================================================================="<<endl;
    
  return 1;
}

int main(int argc, char *argv[])
{
  try {
    StandardDistributions::init();
    StandardQoSFun::init();
    StandardTasks::init();
    int opt = opts_parse(argc, argv);
    if (help_flag) {
      cout<<"usage: xml-solve [flags] input_file.xml"<<endl;
      cout<<"flags:"<<endl;
      cout<<"    -h (--help): prints this screen and returns"<<endl;
      cout<<"    -v (--verbose flag): sets verbose mode (overrides settings in xml file)"<<endl;
      cout<<"    -s (--verbose flag): sets verbose mode (overrides settings in xml file)"<<endl;
      return 1;
    };
    if( argc-optind!=1 )
      EXC_PRINT("one input file requested");
    
    t_start = my_get_time();
    
    XMLParser::Parser * p = new XMLParser::Parser(argv[opt]);
    p->parse();
    if(verbose_flag)
      cout<<"XML file succesfully parsed"<<endl;
   
    t_xml_parse_end = my_get_time();
    int res;
    switch(p->get_act()) {
    case  XMLParser::OPTIMISE: 
      res = opt_execute(p); 
      break;
    case XMLParser::SOLVE:
      res = solve_execute();
      break;
    default:
      EXC_PRINT("action not currently recognised");
    };

    delete p;
    QoSFactory::qos_fun_factory.clean_up_types();
    DistrFactory::distr_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up();
    //TaskFactory::clean_up();
    return res;
  }
  catch (Exc & e) {
    QoSFactory::qos_fun_factory.clean_up_types();
    DistrFactory::distr_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up();
  
    //QoSFactory::clean_up();
    //DistrFactory::clean_up();
    //TaskFactory::clean_up();
    e.what();
    return 0;
  };
};


