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
  };
  
  ~IntegratedOptimisationTestSuite() {
    QoSFactory::clean_up();
    DistrFactory::clean_up();
    TaskFactory::clean_up();
  };
private:
  void optimisation1() {
    XMLParser::Parser * p;
    TaskFactory::task_clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-1.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==2,"There are two tasks");
    InfinityNormBudgetOptimiser Opt(v);
    Opt.set_verbose(true);
    double opt = Opt.optimise();
    
    double Btot;  
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task3"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith"<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    TEST_ASSERT_MSG(opt>=0,"Infinity norm has to be positive");
    delete p;
  };
    void optimisation2() {
    XMLParser::Parser * p;
    TaskFactory::task_clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-optimisation-2.xml"));
    TEST_THROWS_NOTHING(p->parse());
    vector<GenericTaskDescriptor*> v;
    int num = TaskFactory::get_task_descriptor_vector(v);
    TEST_ASSERT_MSG(num==2,"There are two tasks");
    InfinityNormBudgetOptimiser Opt(v);
    double opt = Opt.optimise();
    double Btot;
    TEST_THROWS_NOTHING(Btot = Opt.get_bandwidth("task3")+ Opt.get_bandwidth("task3"));
    TEST_ASSERT_MSG(Btot <1.0 +1e-5,"bandwdith must be smaller than 1");
    cout <<"Total bandwdith"<<Btot<<endl;
    cout<<"Optimal value"<<opt<<endl;
    TEST_ASSERT_MSG(opt>=0,"Infinity norm has to be positive");
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
