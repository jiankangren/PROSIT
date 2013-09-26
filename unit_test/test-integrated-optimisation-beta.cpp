#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>

#include "parser.hpp"
#include "standard_distr.hpp"
#include "standard_task.hpp"
#include "standard_qosfun.hpp"
#include "distr_factory.hpp"
#include "task_factory.hpp"


class IntegratedOptimisationTestSuite : public Test::Suite {
public:
  IntegratedOptimisationTestSuite() {
    StandardDistributions::init();
    StandardQoSFun::init();
    StandardTasks::init();
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation1);
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation2);
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation3);
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation4);
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation5);
    TEST_ADD(IntegratedOptimisationTestSuite::optimisation6);
  };
  
  ~IntegratedOptimisationTestSuite() {
    QoSFactory::qos_fun_factory.clean_up_types();
    DistrFactory::distr_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up();
    TaskFactory::task_descriptor_factory.clean_up_types();
  };
private:

  void optimisation1() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-1.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==2,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
  
  void optimisation2() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-2.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==2,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
  void optimisation4() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-4.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==3,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };

        t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task5"));
    assert(t);
    cerr<<endl<<"TASK: task5"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    cout<<"Task 5: Budget: "<<Opt.get_budget("task5")<<endl;
	
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
  void optimisation3() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-3.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==3,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    cout<<"Task 5: Budget: "<<Opt.get_budget("task5")<<endl;
	
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
    void optimisation5() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-5.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==3,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    cout<<"Task 5: Budget: "<<Opt.get_budget("task5")<<endl;
	
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
    void optimisation6() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-beta-6.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::task_descriptor_factory.get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==3,"There are two tasks");
          
#if 0
    ProbPeriodicTaskDescriptor * t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task3"));
    assert(t);
    cerr<<"TASK: task3"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
    
    t = dynamic_cast<ProbPeriodicTaskDescriptor*>(TaskFactory::get_task_descriptor_from_name("task4"));
    assert(t);
    cerr<<endl<<"TASK: task4"<<endl;
    cerr<<"===========Probability=========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", Probability: "<<t->probability(Q)<<endl;
    };
    cerr<<endl<<"=====QoS==========="<<endl;
    for (int Q = 0; Q<=t->get_server_period();Q++) {
      cerr<<"Q: "<<Q<<", QoS: "<<t->QoS(Q)<<endl;
    };
#endif

    InfinityNormBudgetOptimiser Opt(v,p->get_optimisation_epsilon());
    Opt.set_verbose(p->get_verbose());
    Opt.init_target_bounds();
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task4"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith: "<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    cout<<"Task 3: Budget: "<<Opt.get_budget("task3")<<endl;
    cout<<"Task 4: Budget: "<<Opt.get_budget("task4")<<endl;
    cout<<"Task 5: Budget: "<<Opt.get_budget("task5")<<endl;
	
    TEST_ASSERT_MSG(Opt.get_state()==GenericBudgetOptimiser::OK,"Optimisation succeeded");
    delete p;
  }
};
bool run_IntegratedOptimisation_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new IntegratedOptimisationTestSuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_IntegratedOptimisation_tests()) {
      std::cerr<<"Test passed"<<endl;
      return 1;
    }
    else {
      std::cerr<<"Test failed"<<endl;
      return 0;
    }
  }catch (Exc & e) {
    e.what();
  };
  return 0;
}
