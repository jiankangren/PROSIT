#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
#include "task_factory.hpp"
#include<assert.h>
class StubProbPeriodicTaskDescriptor: public ProbPeriodicTaskDescriptor {
public:
  StubProbPeriodicTaskDescriptor(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad, double eps): 
    ProbPeriodicTaskDescriptor(nm,c,Pd,Qd,Tsd,Deltad,eps) {
  };
  double QoS_from_prob(double p) {
    return p;
  };
  double probability(int Q) {
    if ( Q < 5 )
      return 0.0;
    if( Q > 40 )
      return 1.0;
    return (double(Q)-double(5))/(double(40)-double(5));
  }
};

class StubProbPeriodicTaskDescriptorCR: public ProbPeriodicTaskDescriptorCR {
  public:
  StubProbPeriodicTaskDescriptorCR(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad, double eps): 
    ProbPeriodicTaskDescriptorCR(nm,c,Pd,Qd,Tsd,Deltad,eps) {
  };
  double QoS_from_prob(double p) {
    return p*p;
  };
};

class StubProbPeriodicTaskDescriptorAnalytic: public ProbPeriodicTaskDescriptorAnalytic {
  public:
  StubProbPeriodicTaskDescriptorAnalytic(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad, double eps): 
    ProbPeriodicTaskDescriptorAnalytic(nm,c,Pd,Qd,Tsd,Deltad,eps) {
  };
  double QoS_from_prob(double p) {
    return p*p;
  };
};

class StubProbPeriodicTaskBuilder: public TaskFactory::PeriodicTaskBuilder {
public:
  virtual ProbPeriodicTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw (Exc) {
    TaskFactory::PeriodicTaskParameters * p1;
    p1 = dynamic_cast<TaskFactory::PeriodicTaskParameters*>(t);
    assert(p1);
    return new StubProbPeriodicTaskDescriptor(name, p1->c, p1->Pd, p1->Qd, p1->Tsd, p1->Deltad, p1->epsilond);
  };
};

class StubProbPeriodicTaskBuilderCR: public TaskFactory::PeriodicTaskBuilder {
public:
  virtual ProbPeriodicTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw (Exc) {
    TaskFactory::PeriodicTaskParameters * p1;
    p1 = dynamic_cast<TaskFactory::PeriodicTaskParameters*>(t);
    assert(p1);
    return new StubProbPeriodicTaskDescriptorCR(name, p1->c, p1->Pd, p1->Qd, p1->Tsd, p1->Deltad, p1->epsilond);
  }
};


class StubProbPeriodicTaskBuilderAnalytic: public TaskFactory::PeriodicTaskBuilder {
public:
  virtual ProbPeriodicTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw (Exc){
    TaskFactory::PeriodicTaskParameters * p1;
    p1 = dynamic_cast<TaskFactory::PeriodicTaskParameters*>(t);
    assert(p1);
    return new StubProbPeriodicTaskDescriptorAnalytic(name, p1->c, p1->Pd, p1->Qd, p1->Tsd, p1->Deltad, p1->epsilond);
  };
};



class TaskFactoryTestSuite : public Test::Suite {
public:
  TaskFactoryTestSuite() {
    TEST_ADD(TaskFactoryTestSuite::registration);
    TEST_ADD(TaskFactoryTestSuite::creation);
  };
 
  ~TaskFactoryTestSuite() {
    TaskFactory::task_descriptor_factory.clean_up();
  };
private:
  void registration() {
    TEST_THROWS_NOTHING_MSG(TaskFactory::task_descriptor_factory.register_type("StubProbPeriodicTaskDescriptor",new StubProbPeriodicTaskBuilder()),"Cannot create task type");
     TEST_THROWS_NOTHING_MSG(TaskFactory::task_descriptor_factory.register_type("StubProbPeriodicTaskDescriptorCR",new StubProbPeriodicTaskBuilderCR()),"Cannot create task type");
     TEST_THROWS_NOTHING_MSG(TaskFactory::task_descriptor_factory.register_type("StubProbPeriodicTaskDescriptorAnalytic",new StubProbPeriodicTaskBuilderAnalytic()),"Cannot create task type");
     TEST_THROWS_MSG(TaskFactory::task_descriptor_factory.register_type("StubProbPeriodicTaskDescriptorAnalytic",new StubProbPeriodicTaskBuilderAnalytic()), Exc,"Should create existing task type");
  };
  void creation() {
    auto_ptr<pmf> c (new pmf(1000,0));
    auto_ptr<pmf> c1 (new pmf(1000,0));
    c->set(10,0.5);
    c->set(40,0.5);
    *c1=*c;
    auto_ptr<TaskFactory::GenericTaskParameters> pt(new TaskFactory::GenericTaskParameters(c,5,10,1,1e-8));
    TaskFactory::PeriodicTaskParameters p(pt, 120);
    TaskFactory::task_descriptor_factory.create_instance("StubProbPeriodicTaskDescriptor", "task 1",  &p);
    GenericTaskDescriptor * h = TaskFactory::task_descriptor_factory.get_from_name("task 1");
    TEST_ASSERT_MSG(h!=0,"The task should exist");
    TEST_ASSERT_MSG(h->get_name()=="task 1","The task name is incorrect");
    
    //C has been consumed!!!!
    p.c = c1;
    TEST_THROWS_MSG(TaskFactory::task_descriptor_factory.create_instance("StubProbPeriodicTaskDescriptor2","task 2",  &p), Exc, "task type does not exist");
    TEST_THROWS_MSG(TaskFactory::task_descriptor_factory.create_instance("StubProbPeriodicTaskDescriptor", "task 1",  &p), Exc, "task name exists already");
    TaskFactory::task_descriptor_factory.create_instance("StubProbPeriodicTaskDescriptorCR", "task 3",  &p);
    h = TaskFactory::task_descriptor_factory.get_from_name("task 3");
    TEST_ASSERT_MSG(dynamic_cast<StubProbPeriodicTaskDescriptorCR*>(h)!=0,"The task should be of type CR");
  }
};




bool run_TaskFactory_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new  TaskFactoryTestSuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_TaskFactory_tests()) {
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
