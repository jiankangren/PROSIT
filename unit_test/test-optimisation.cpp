#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
#include "task_factory.hpp"

class StubProbPeriodicTaskDescriptor: public ProbPeriodicTaskDescriptor {
  double scale;
public:
  StubProbPeriodicTaskDescriptor(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad, double eps, double scaled): 
    ProbPeriodicTaskDescriptor(nm,c,Pd,Qd,Tsd,Deltad,eps), scale(scaled) {
  };
  double QoS_from_prob(double p) {
    return scale*p;
  };
  double probability(int Q) {
    if ( Q < 0.05*double(Ts) )
      return 0.0;
    if( Q > 0.85*double(Ts))
      return 1.0;
    return (double(Q)-0.05*double(Ts))/(0.85*double(Ts)-0.05*double(Ts));
  }
};

struct StubParameters: public TaskFactory::PeriodicTaskParameters {
  double scale;
  StubParameters(auto_ptr<pmf> cd, int Pdd, int Qdd, int Tsdd, int Deltadd, double epsilondd, double scaled): 
    PeriodicTaskParameters(cd,Pdd,Qdd,Tsdd,Deltadd,epsilondd), scale(scaled) {};
};
class StubProbPeriodicTaskBuilder: public TaskFactory::PeriodicTaskBuilder {
public:
  virtual ProbPeriodicTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw (Exc) {
    StubParameters * t1 = dynamic_cast<StubParameters*>(t);
    if (t1==0)
      throw Exc("StubProbPeriodicTaskBuilder::create_instance wrong parameter type");
    return new StubProbPeriodicTaskDescriptor(name, t1->c, t1->Pd, t1->Qd, t1->Tsd, t1->Deltad, t1->epsilond, t1->scale);
  };
};



class TaskOptimisationSuite : public Test::Suite {
public:
  TaskOptimisationSuite() {
    TaskFactory::register_task_type("StubProbPeriodicTaskDescriptor",new StubProbPeriodicTaskBuilder());
    auto_ptr<pmf> c(new pmf(1000,0));
    auto_ptr<pmf> c1(new pmf(1000,0));
    auto_ptr<pmf> c2(new pmf(1000,0));
    
    c->set(10,0.5);
    c->set(40,0.5);
    *c1 = *c;
    *c2 = *c;
    StubParameters p(c, 1200000, 50, 100000, 1, 1e-8,2.0);
    TaskFactory::create_task_descriptor_instance("StubProbPeriodicTaskDescriptor","task 1",  &p);
    p.scale = 10.0;
    p.Tsd = 200000;
    p.c = c1;
    TaskFactory::create_task_descriptor_instance("StubProbPeriodicTaskDescriptor", "task 2",  &p);
    p.scale = 100.0;
    p.Tsd=400000;
    p.c = c2;
    TaskFactory::create_task_descriptor_instance("StubProbPeriodicTaskDescriptor", "task 3",  &p);
    TEST_ADD(TaskOptimisationSuite::simple_optimisation_1);
    TEST_ADD(TaskOptimisationSuite::simple_optimisation_2);
  };
 
  ~TaskOptimisationSuite() {
    TaskFactory::clean_up();
  };
private:  
  void simple_optimisation_1() {
    vector<GenericTaskDescriptor*> v(3);
    TaskFactory::set_task_qos_bounds("task 1",0.15*2, 0.85*2);
    TaskFactory::set_task_qos_bounds("task 2",0.25*10, 0.75*10);
    TaskFactory::set_task_qos_bounds("task 3",0.05*100, 0.9*100);
    TEST_ASSERT_MSG(TaskFactory::get_task_descriptor_vector(v)==3,"There are three tasks");
    InfinityNormBudgetOptimiser Opt(v);
    TEST_ASSERT_MSG(Opt.task_num()==3,"There are three tasks in the optimiser");
    Opt.optimise();
    double Btot  = double(Opt.get_budget(0))/100000+double(Opt.get_budget(1))/200000+double(Opt.get_budget(2))/400000;
    TEST_ASSERT_DELTA_MSG(Btot,1.0,1e-4,"Bandwidth beyond the limit");

    
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"optimisation failed");
    TEST_ASSERT_MSG((v[0]->QoS(Opt.get_budget(0))>=0.15*2)&&(v[0]->QoS(Opt.get_budget(0))<=0.85*2),"Boundary failed for task 1");
    TEST_ASSERT_MSG((v[1]->QoS(Opt.get_budget(1))>=0.25*10)&&(v[1]->QoS(Opt.get_budget(1))<=0.75*10),"Boundary failed for task 2");
    TEST_ASSERT_MSG((v[2]->QoS(Opt.get_budget(2))>=5)&&(v[2]->QoS(Opt.get_budget(2))<=90),"Boundary failed for task 3");

    
    cerr<<"v[0]->QoS(Opt.get_budget(0))"<<v[0]->QoS(Opt.get_budget(0))<<endl;
    cerr<<"v[1]->QoS(Opt.get_budget(1))"<<v[1]->QoS(Opt.get_budget(1))<<endl;
    cerr<<"v[2]->QoS(Opt.get_budget(2))"<<v[2]->QoS(Opt.get_budget(2))<<endl;
  };
  void simple_optimisation_2() {
    vector<GenericTaskDescriptor*> v(3);
    TaskFactory::set_task_qos_bounds("task 1",0.15*2, 0.90*2);
    TaskFactory::set_task_qos_bounds("task 2",0.2, 0.75*10);
    TaskFactory::set_task_qos_bounds("task 3",0.05*100, 0.9*100);
    TEST_ASSERT_MSG(TaskFactory::get_task_descriptor_vector(v)==3,"There are three tasks");
    InfinityNormBudgetOptimiser Opt(v);
    TEST_ASSERT_MSG(Opt.task_num()==3,"There are three tasks in the optimiser");
    Opt.optimise();
    double Btot  = double(Opt.get_budget(0))/100000+double(Opt.get_budget(1))/200000+double(Opt.get_budget(2))/400000;
        TEST_ASSERT_MSG((v[0]->QoS(Opt.get_budget(0))>=0.15*2)&&(v[0]->QoS(Opt.get_budget(0))<=0.9*2),"Boundary failed for task 1");
    TEST_ASSERT_MSG((v[1]->QoS(Opt.get_budget(1))>=0.2)&&(v[1]->QoS(Opt.get_budget(1))<=0.75*10),"Boundary failed for task 2");
    TEST_ASSERT_MSG((v[2]->QoS(Opt.get_budget(2))>=5)&&(v[2]->QoS(Opt.get_budget(2))<=90),"Boundary failed for task 3");

    TEST_ASSERT_DELTA_MSG(Btot,1.0,1e-4,"Bandwidth beyond the limit");

    cerr<<"v[0]->QoS(Opt.get_budget(0))"<<v[0]->QoS(Opt.get_budget(0))<<endl;
    cerr<<"v[1]->QoS(Opt.get_budget(1))"<<v[1]->QoS(Opt.get_budget(1))<<endl;
    cerr<<"v[2]->QoS(Opt.get_budget(2))"<<v[2]->QoS(Opt.get_budget(2))<<endl;

    TEST_ASSERT_DELTA_MSG(v[0]->QoS(Opt.get_budget(0)),v[1]->QoS(Opt.get_budget(1)),1e-2,"QoS has to be equal at the optimum");
  }
};




bool run_Optimisation_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new  TaskOptimisationSuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_Optimisation_tests()) {
      std::cerr<<"Test passed"<<endl;
      return 1;
    }
    else
      std::cerr<<"Test failed"<<endl;
  }catch (Exc & e) {
    e.what();
  };
  return 0;
}
