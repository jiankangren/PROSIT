#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
#include "standard_qosfun.hpp"

class QoSFactorySuite : public Test::Suite {
public:
  QoSFactorySuite() {
    StandardQoSFun::init();
    TEST_ADD(QoSFactorySuite::linear);
    //TEST_ADD(DistrFactorySuite::quadratic);
  };
  
  ~QoSFactorySuite() {
    QoSFactory::qos_fun_factory.clean_up_types();
  };
private:
  void linear() {
    StandardQoSFun::LinearQoSFunParameters pWrong(0.5, 1.3, 0.8);
    StandardQoSFun::LinearQoSFunParameters pWrong1(0.5, 0.3, 1.8);
    TEST_THROWS_MSG(QoSFactory::qos_fun_factory.create_instance("linear", &pWrong), Exc, "Wrong parameter type");
    TEST_THROWS_MSG(QoSFactory::qos_fun_factory.create_instance("linear", &pWrong1), Exc, "Wrong parameter type");
    StandardQoSFun::LinearQoSFunParameters pOk(0.5, 0.3, 0.8);
    auto_ptr<QoSFun> p;
    TEST_THROWS_NOTHING(p = QoSFactory::qos_fun_factory.create_instance("linear",&pOk));
    TEST_ASSERT_DELTA_MSG(p->eval(0.3),0,1e-4,"Left boundary failed");
    TEST_ASSERT_DELTA_MSG(p->eval(0.8),0.25,1e-4,"Right boundary failed");
  };
  void quadratic() {
    StandardQoSFun::LinearQoSFunParameters pWrong(0.5, 1.3, 0.8);
    StandardQoSFun::LinearQoSFunParameters pWrong1(0.5, 0.3, 1.8);
    TEST_THROWS_MSG(QoSFactory::qos_fun_factory.create_instance("quadratic", &pWrong), Exc, "Wrong parameter type");
    TEST_THROWS_MSG(QoSFactory::qos_fun_factory.create_instance("quadratic", &pWrong1), Exc, "Wrong parameter type");
    StandardQoSFun::LinearQoSFunParameters pOk(0.5, 0.3, 0.8);
    auto_ptr<QoSFun> p;
    TEST_THROWS_NOTHING(p = QoSFactory::qos_fun_factory.create_instance("quadratic",&pOk));
    TEST_ASSERT_DELTA_MSG(p->eval(0.3),0,1e-4,"Left boundary failed");
    TEST_ASSERT_DELTA_MSG(p->eval(0.8),0.25*0.25,1e-4,"Right boundary failed");
  };
};




bool run_QoSFactory_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new QoSFactorySuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_QoSFactory_tests()) {
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
