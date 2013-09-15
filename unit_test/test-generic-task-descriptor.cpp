#include "optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
class StubGenericTaskDescriptor: public GenericTaskDescriptor {
public:
  StubGenericTaskDescriptor(const char * nm, auto_ptr<pmf> c, const int Qd, const int Tsd): 
    GenericTaskDescriptor(nm,c,Qd,Tsd) {
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

class GenericTaskDescriptorTestSuite : public Test::Suite {
public:
  GenericTaskDescriptorTestSuite() {
    TEST_ADD(GenericTaskDescriptorTestSuite::creation);
    TEST_ADD(GenericTaskDescriptorTestSuite::set_budget);
  }; 
private:
  void creation() {
    auto_ptr<pmf> p(new pmf(1000,0));
    p->set(1,0.5);
    p->set(4,0.5);
    StubGenericTaskDescriptor t("prova",p,10,40);
    t.init_bounds();
    TEST_ASSERT(t.get_name()==string("prova"));
    TEST_ASSERT(t.get_computation_time().get(1)==0.5);
    TEST_ASSERT(t.get_computation_time().get(4)==0.5);
    TEST_ASSERT(t.get_budget()==10);
    TEST_ASSERT(t.get_server_period()==40);
    TEST_ASSERT(t.get_qmin()==0);
    TEST_ASSERT(t.get_qmax()==40);
  };
  void set_budget() {
    auto_ptr<pmf>  p(new pmf(1000,0));
    p->set(1,0.25);
    p->set(4,0.5);
    p->set(5,0.25);
    StubGenericTaskDescriptor t("prova1",p,20,40);
    TEST_ASSERT(!t.get_bounds_inited());
    t.init_bounds();
    TEST_ASSERT(t.get_bounds_inited());
    t.set_budget(25);
    TEST_ASSERT(t.get_budget()==25);
    TEST_THROWS(t.set_budget(45),Exc);
    TEST_THROWS(StubGenericTaskDescriptor t1("prova1",p,45,40), Exc);
  };
};
bool run_GenericTaskDescriptor_tests()
{
  GenericTaskDescriptorTestSuite sts;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return sts.run(output, true); // Note the 'false' parameter
}

int main() {
  if(run_GenericTaskDescriptor_tests()) {
    std::cerr<<"Test passed"<<endl;
    return 1;
  }
  else
    std::cerr<<"Test failed"<<endl;
  return 0;
}
