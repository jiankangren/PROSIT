#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
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


class ProbPeriodicTaskDescriptorTestSuite : public Test::Suite {
public:
  ProbPeriodicTaskDescriptorTestSuite() {
    TEST_ADD(ProbPeriodicTaskDescriptorTestSuite::creation);
    TEST_ADD(ProbPeriodicTaskDescriptorTestSuite::test_QoS);
    auto_ptr<pmf> p (new pmf(1000,0));
    p->set(10,0.5);
    p->set(40,0.5);
    t = new StubProbPeriodicTaskDescriptor("prova",p,120,10,40,10,1e-8);
  };
 
  ~ProbPeriodicTaskDescriptorTestSuite() {
    delete t;
  }
private:
  StubProbPeriodicTaskDescriptor * t;
  void creation() {
    auto_ptr<pmf> p(new pmf(1000,0));
    p->set(10,0.5);
    p->set(40,0.5);
    TEST_ASSERT_MSG(t->get_name()==string("prova"),"Name does not correspond");
    TEST_ASSERT_MSG(t->get_computation_time().get(10)==0.5,"Computation time at 10 does not correspond");
    TEST_ASSERT_MSG(t->get_computation_time().get(40)==0.5,"Computation time at 40 does not correspond");
    TEST_ASSERT_MSG(t->get_budget()==10,"Budget does not correspond");
    TEST_ASSERT_MSG(t->get_server_period()==40,"Server period does not correspond");
    TEST_ASSERT_MSG(t->get_qmin()==0,"Qmin does not correspond");
    TEST_ASSERT_MSG(t->get_qmax()==40,"Qmax does not correspond");
    TEST_ASSERT_MSG(t->get_period()==120,"Period does not correspond");
    TEST_THROWS_MSG(StubProbPeriodicTaskDescriptor t1("prova",p,120,10,40,1000,1e-8),Exc,"Exception not thrown");
  };
  void test_QoS() {
    t->set_qmin(10);
    t->set_qmax(35);
    t->init_bounds();
    int Q;

    TEST_ASSERT_DELTA_MSG(t->QoS(5),0,1e-7,"Lower bound of QoS failed");
    TEST_ASSERT_DELTA_MSG(t->QoS(40),1,1e-7,"Upper bound of QoS failed");
    TEST_ASSERT_DELTA_MSG(t->get_qosmin(),0.14286,1e-5,"QoS min not properly set");
    TEST_ASSERT_DELTA_MSG(t->get_qosmax(),0.85714,1e-5,"QoS max not properly set");;
    TEST_ASSERT_MSG(!t->inv_QoS(10,Q,true),"Inverse QoS should be undefined");
    TEST_ASSERT_MSG(t->inv_QoS(0.85714,Q,true),"Inverse QoS should be defined");
    TEST_ASSERT_MSG(Q==35,"inversion incorrect");
    
  };
};


class ProbPeriodicTaskDescriptorCRTestSuite: public Test::Suite {
private:
  ProbPeriodicTaskDescriptorCR * t, * t1;
  void probability() {
    TEST_ASSERT_DELTA(t->probability(4), 0.780476, 1e-5);
    TEST_ASSERT_DELTA(t->probability(6), 0.973746, 1e-5);
    TEST_ASSERT_DELTA(t1->probability(4),0.745696, 1e-5);
    TEST_ASSERT_DELTA(t1->probability(6),0.973372, 1e-5);
  };
  void QoS() {
    TEST_ASSERT_DELTA(t->QoS(4), 0.780476*0.780476, 1e-5);
    TEST_ASSERT_DELTA(t->QoS(6), 0.973746*0.973746, 1e-5);
    TEST_ASSERT_DELTA(t1->QoS(4),0.745696*0.745696, 1e-5);
    TEST_ASSERT_DELTA(t1->QoS(6),0.973372*0.973372, 1e-5);
    int Q;
    TEST_ASSERT_MSG(t->inv_QoS( 0.780476*0.780476,Q,true),"QoS has to be invertible");
    TEST_ASSERT_MSG(Q==4,"incorrect value for Q");
    TEST_ASSERT_MSG(!t->inv_QoS( 4,Q,true),"QoS has not to be invertible");
    TEST_ASSERT_MSG(!t->inv_QoS( -10,Q,true),"QoS has not to be invertible");
    // TEST_ASSERT(t1->inv_QoS(0.973372,Q));
    // cout<<Q<<endl;
    // TEST_ASSERT(Q==6);!
  };
public:
  ProbPeriodicTaskDescriptorCRTestSuite() {
    auto_ptr<pmf> p(new pmf(1000,0));
    auto_ptr<pmf> p1(new pmf(1000,0));
    p->set(10,0.05);
    p->set(15,0.05);
    p->set(25,0.45);
    p->set(35,0.2);
    p->set(40,0.15);
    p->set(45,0.0125);
    p->set(50,0.0125);
    p->set(55,0.025);
    p->set(60,0.025);
    p->set(65,0.0125);
    p->set(70,0.00625);
    p->set(80,0.00625);
    *p1=*p;
    t = new StubProbPeriodicTaskDescriptorCR("prova",p,100,4,10,1,1e-8);
    t1 = new StubProbPeriodicTaskDescriptorCR("prova1",p1,100,4,10,2,1e-8);
    TEST_ADD(ProbPeriodicTaskDescriptorCRTestSuite::probability);
    TEST_ADD(ProbPeriodicTaskDescriptorCRTestSuite::QoS);
  };
  ~ProbPeriodicTaskDescriptorCRTestSuite() {
    delete t;
    delete t1;
  };
  
  
};


class ProbPeriodicTaskDescriptorAnalyticTestSuite: public Test::Suite {
private:
  ProbPeriodicTaskDescriptorAnalytic * t, * t1;
  void probability() {
    TEST_ASSERT_DELTA(t->probability(8), 0.35, 1e-5);
    TEST_ASSERT_DELTA(t->probability(12), 0.921053, 1e-5);
    TEST_ASSERT_DELTA(t1->probability(8), 0.511364, 1e-5);
    TEST_ASSERT_DELTA(t1->probability(12),0.511364, 1e-5);
  };
  void QoS() {
    TEST_ASSERT_DELTA(t->QoS(8), 0.35*0.35, 1e-5);
    TEST_ASSERT_DELTA(t->QoS(12),  0.921053* 0.921053, 1e-5);
    int Q;
    TEST_ASSERT_MSG(t->inv_QoS( 0.35*0.35,Q,true),"QoS has to be invertible");
    TEST_ASSERT_MSG(Q==8,"incorrect value for Q");
    TEST_ASSERT_MSG(!t->inv_QoS( 4,Q,true),"QoS has not to be invertible");
    TEST_ASSERT_MSG(!t->inv_QoS( -10,Q,true),"QoS has not to be invertible");
    // TEST_ASSERT(t1->inv_QoS(0.973372,Q));
    // cout<<Q<<endl;
    // TEST_ASSERT(Q==6);!
  };
public:
  ProbPeriodicTaskDescriptorAnalyticTestSuite() {
    auto_ptr<pmf> p(new pmf(1000,0));
    auto_ptr<pmf> p1(new pmf(1000,0));
    p->set(10,0.05);
    p->set(15,0.05);
    p->set(25,0.45);
    p->set(35,0.2);
    p->set(40,0.15);
    p->set(45,0.0125);
    p->set(50,0.0125);
    p->set(55,0.025);
    p->set(60,0.025);
    p->set(65,0.0125);
    p->set(70,0.00625);
    p->set(80,0.00625);
    *p1=*p;
    t = new StubProbPeriodicTaskDescriptorAnalytic("prova",p,100,8,20,4,1e-8);
    t1 = new StubProbPeriodicTaskDescriptorAnalytic("prova1",p1,100,8,20,8,1e-8);
    TEST_ADD(ProbPeriodicTaskDescriptorAnalyticTestSuite::probability);
    TEST_ADD(ProbPeriodicTaskDescriptorAnalyticTestSuite::QoS);
  };
  ~ProbPeriodicTaskDescriptorAnalyticTestSuite() {
    delete t;
    delete t1;
  };
  
  
};


bool run_ProbPeriodicTaskDescriptor_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorTestSuite));
  ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
  ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorAnalyticTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_ProbPeriodicTaskDescriptor_tests()) {
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
