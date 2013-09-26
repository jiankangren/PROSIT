#include "probabilistic_optimisation.hpp"
#include <cpptest.h>
#include "pmf.hpp"
#include <string>
#include <iostream>
#include "standard_distr.hpp"

class DistrFactorySuite : public Test::Suite {
public:
  DistrFactorySuite() {
    StandardDistributions::init();
    TEST_ADD(DistrFactorySuite::file);
    TEST_ADD(DistrFactorySuite::uniform);
    TEST_ADD(DistrFactorySuite::beta);
  };
  
  ~DistrFactorySuite() {
    DistrFactory::distr_factory.clean_up_types();
  };
private:
  void file() {
    StandardDistributions::SyntheticDistrParameters pWrong(10,100,1,1000);
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("file", &pWrong), Exc, "Wrong parameter type");
    StandardDistributions::FileDistrParameters pWrong1("NotExists.txt");
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("file", &pWrong1),Exc,"File does not exist");
    StandardDistributions::FileDistrParameters pOk("./pmf-tests/beta.txt");
    auto_ptr<pmf> p;
    TEST_THROWS_NOTHING(p = DistrFactory::distr_factory.create_instance("file", &pOk));
    TEST_ASSERT_DELTA_MSG(p->get(5),1.8067801e-01, 1e-4,"First sample does not coincide");
    TEST_ASSERT_MSG(p->check()==pmf::PMF_OK,"pmf ill formed");
  };
  void uniform() {
    StandardDistributions::SyntheticDistrParameters pWrong(100,10,1,1000);
    StandardDistributions::SyntheticDistrParameters pWrong2(10,20,3,1000);
    StandardDistributions::SyntheticDistrParameters pOk(10,20,2,1000);
    //StandardDistributions::UniformDistrBuilder builder;
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("uniform", &pWrong), Exc, "cmax smaller than cmin");
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("uniform", &pWrong2), Exc, "step has to be integer submultiple of the interval");
    auto_ptr<pmf> p;
    TEST_THROWS_NOTHING(p = DistrFactory::distr_factory.create_instance("uniform", &pOk));
    //p->print();
    TEST_ASSERT_MSG(p->check()==pmf::PMF_OK,"pmf ill formed");
  };
  void beta() {
    StandardDistributions::SyntheticDistrParameters pWrong(100,10,1,1000);
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("beta", &pWrong), Exc, "Wrong parameter type");
    
    StandardDistributions::BetaDistrParameters pWrong1(10,20,3,1000,-3.0,-4.0);

    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("beta", &pWrong1), Exc, "Wrong parameter type 2");
    
    StandardDistributions::BetaDistrParameters pWrong2(10,20,2,1000,-3.0,4.0);
    TEST_THROWS_MSG(DistrFactory::distr_factory.create_instance("beta", &pWrong2), Exc, "Wrong parameter type 3");

    StandardDistributions::BetaDistrParameters pOk(10,100,2,1000,3.0,4.0);
    auto_ptr<pmf> p;
    TEST_THROWS_NOTHING(p = DistrFactory::distr_factory.create_instance("beta", &pOk));
    TEST_ASSERT_MSG(p->check()==pmf::PMF_OK,"pmf ill formed");
    //    p->print();
 };
};




bool run_Distribution_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new DistrFactorySuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_Distribution_tests()) {
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
