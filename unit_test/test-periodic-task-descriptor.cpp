#include "optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
class StubPeriodicTaskDescriptor: public PeriodicTaskDescriptor {
public:
  StubPeriodicTaskDescriptor(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd): 
    PeriodicTaskDescriptor(nm,c,Pd,Qd,Tsd) {
  };
  double QoS(int budget) {
    return double(budget);
  };
  bool inv_QoS(double q, int & Q, bool ceil) throw (Exc){
    Q = int(q);
    return true;
  };
  bool identify_bounds(double qos_min, double qos_max) throw (Exc) {
    Qmin=0;
    Qmax=Ts;
    return true;
  };
};

class PeriodicTaskDescriptorTestSuite : public Test::Suite {
public:
  PeriodicTaskDescriptorTestSuite() {
    TEST_ADD(PeriodicTaskDescriptorTestSuite::creation);
  }; 
private:
  void creation() {
    auto_ptr<pmf> p(new pmf(1000,0));
    p->set(1,0.5);
    p->set(4,0.5);
    StubPeriodicTaskDescriptor t("prova",p,120,10,40);
    t.init_bounds();
    TEST_ASSERT(t.get_name()==string("prova"));
    TEST_ASSERT(t.get_computation_time().get(1)==0.5);
    TEST_ASSERT(t.get_computation_time().get(4)==0.5);
    TEST_ASSERT(t.get_budget()==10);
    TEST_ASSERT(t.get_server_period()==40);
    TEST_ASSERT(t.get_qmin()==0);
    TEST_ASSERT(t.get_qmax()==40);
    TEST_ASSERT(t.get_period()==120);
  };
};
bool run_PeriodicTaskDescriptor_tests()
{
  PeriodicTaskDescriptorTestSuite sts;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return sts.run(output, true); // Note the 'false' parameter
}

int main() {
  if(run_PeriodicTaskDescriptor_tests()) {
    std::cerr<<"Test passed"<<endl;
    return 1;
  }
  else
    std::cerr<<"Test failed"<<endl;
  return 0;
}
