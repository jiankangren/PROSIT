#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>

#include "tinyxml2.h"
using namespace tinyxml2;
class ParserLikeClass {
  XMLDocument * d;
  int i;
public:
  ParserLikeClass(const char * f): i(0) {
    d = new XMLDocument();
    if(d->LoadFile(f) !=  XML_NO_ERROR) {
      delete d;
      throw 123;
    }
  };
  ~ParserLikeClass() {
    delete d;
  };
};
class ParserLikeClass1 {
  XMLDocument * d;
  int i;
public:
  ParserLikeClass1(const char * f):i(0) {
    d = new XMLDocument();
    if(d->LoadFile(f) !=  XML_NO_ERROR) {
      delete d;
      throw (Exc("Cannot open file"));
    }
  };
  ~ParserLikeClass1() {
    delete d;
  };
};
class TinySuite : public Test::Suite {
public:
  TinySuite() {
    TEST_ADD(TinySuite::ill_formed);
    TEST_ADD(TinySuite::ill_formed1);
    TEST_ADD(TinySuite::ill_formed2);
    //TEST_ADD(DistrFactorySuite::quadratic);
  };
  
  ~TinySuite() {
  };
private:
  void ill_formed() {
    XMLDocument * d = new XMLDocument();
    TEST_ASSERT(d->LoadFile("NotExists.xml") !=  XML_NO_ERROR);
    TEST_THROWS_NOTHING(delete d);    
  };
  void ill_formed1() {
    ParserLikeClass * p;
    TEST_THROWS_MSG(p = new  ParserLikeClass("NotExists.xml"), int, "File does not exists");
  };
  void ill_formed2() {
    ParserLikeClass1 * p;
    TEST_THROWS_MSG(p = new  ParserLikeClass1("NotExists.xml"), Exc, "File does not exists");
  };
  // void linear() {
//     StandardQoSFun::LinearQoSFunParameters pWrong(0.5, 1.3, 0.8);
//     StandardQoSFun::LinearQoSFunParameters pWrong1(0.5, 0.3, 1.8);
//     TEST_THROWS_MSG(QoSFactory::create_qosfun_instance("linear", &pWrong), Exc, "Wrong parameter type");
//     TEST_THROWS_MSG(QoSFactory::create_qosfun_instance("linear", &pWrong1), Exc, "Wrong parameter type");
//     StandardQoSFun::LinearQoSFunParameters pOk(0.5, 0.3, 0.8);
//     auto_ptr<QoSFun> p;
//     TEST_THROWS_NOTHING(p = create_qosfun_instance("linear",&pOk));
//     TEST_ASSERT_DELTA_MSG(p->eval(0.3),0,1e-4,"Left boundary failed");
//     TEST_ASSERT_DELTA_MSG(p->eval(0.8),0.25,1e-4,"Right boundary failed");
//   };
//   void quadratic() {
//     StandardQoSFun::LinearQoSFunParameters pWrong(0.5, 1.3, 0.8);
//     StandardQoSFun::LinearQoSFunParameters pWrong1(0.5, 0.3, 1.8);
//     TEST_THROWS_MSG(QoSFactory::create_qosfun_instance("quadratic", &pWrong), Exc, "Wrong parameter type");
//     TEST_THROWS_MSG(QoSFactory::create_qosfun_instance("quadratic", &pWrong1), Exc, "Wrong parameter type");
//     StandardQoSFun::LinearQoSFunParameters pOk(0.5, 0.3, 0.8);
//     auto_ptr<QoSFun> p;
//     TEST_THROWS_NOTHING(p = create_qosfun_instance("quadratic",&pOk));
//     TEST_ASSERT_DELTA_MSG(p->eval(0.3),0,1e-4,"Left boundary failed");
//     TEST_ASSERT_DELTA_MSG(p->eval(0.8),0.25*0.25,1e-4,"Right boundary failed");
//   };
};




bool run_TINY_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new TinySuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_TINY_tests()) {
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
