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


class ParserTestSuite : public Test::Suite {
public:
  ParserTestSuite() {
    StandardDistributions::init();
    StandardQoSFun::init();
    StandardTasks::init();
    TEST_ADD(ParserTestSuite::file_access);
    TEST_ADD(ParserTestSuite::no_optimisation_method);
    TEST_ADD(ParserTestSuite::no_task_section);
    TEST_ADD(ParserTestSuite::no_task_name);
    TEST_ADD(ParserTestSuite::no_registered_task_type);
    TEST_ADD(ParserTestSuite::no_server_period);
    TEST_ADD(ParserTestSuite::no_pmf);
    TEST_ADD(ParserTestSuite::no_pmf_file_name);
    TEST_ADD(ParserTestSuite::no_pmf_wrong_file_name);
    TEST_ADD(ParserTestSuite::no_uniform_pmf_missing_cmax);
    TEST_ADD(ParserTestSuite::no_uniform_wrong_step);
    TEST_ADD(ParserTestSuite::no_uniform_missing_qosfun_type);
    TEST_ADD(ParserTestSuite::no_uniform_missing_qosfun_parameter);
    TEST_ADD(ParserTestSuite::no_uniform_wrong_qosfun_parameter);
    TEST_ADD(ParserTestSuite::ok_uniform);
    TEST_ADD(ParserTestSuite::ok_uniform_1);
    TEST_ADD(ParserTestSuite::ok_uniform_2);
    TEST_ADD(ParserTestSuite::no_beta_1);
    TEST_ADD(ParserTestSuite::no_beta_2);
    TEST_ADD(ParserTestSuite::ok_beta_3);
  };
  
  ~ParserTestSuite() {
    QoSFactory::qos_fun_factory.clean_up_types();
    DistrFactory::distr_factory.clean_up_types();
    TaskFactory::task_descriptor_factory.clean_up();
    TaskFactory::task_descriptor_factory.clean_up_types();

  };
private:
  void file_access() {
    XMLParser::Parser * p;
    TEST_THROWS_MSG(p = new  XMLParser::Parser("NotExists.xml"), Exc, "File does not exists");
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-1-wrong.xml"));
    delete p;
  };
  void no_optimisation_method() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-1-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing optimisation method");
    delete p;
  };
  void no_task_section() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-2-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing task section");
    delete p;
  };
  void no_task_name() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-3-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing task name");
    delete p;
  };
  void no_registered_task_type() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-4-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Task type not registered");
    delete p;
  };
  void no_server_period() {
     XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-5-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Server Period missing");
    delete p;
  };
  void no_pmf() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-6-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"PmF missing");
    delete p;
  };
  void no_pmf_file_name() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-7-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"PmF file name missing");
    delete p;
  };
  void no_pmf_wrong_file_name() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-8-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"PmF wrong file name");
    delete p;
  };
  void no_uniform_pmf_missing_cmax() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-9-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Uniform PmF missing cmax");
    delete p;
  };
  void no_uniform_wrong_step() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-10-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Wrong step size");
    delete p;
  };
  void no_uniform_missing_qosfun_type() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-11-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing qosfun typee");
    delete p;
  };
  void no_uniform_missing_qosfun_parameter(){
     XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-12-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing qosfun scale parameter");
    delete p;
  };
  void no_uniform_wrong_qosfun_parameter(){
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-13-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"Missing qosfun scale parameter");
    delete p;
  };
  void ok_uniform() {
    XMLParser::Parser * p;
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-14-ok.xml"));
    
    TEST_THROWS_NOTHING(p->parse());
    delete p;
  };
  void ok_uniform_1() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();

    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-15-ok.xml"));
    TEST_THROWS_NOTHING(p->parse());
    delete p;
  };
    void ok_uniform_2() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();

    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-16-ok.xml"));
//     try {
//       p->parse();
//     } catch(Exc & e) {
//       e.what();
//     };
    TEST_THROWS_NOTHING(p->parse());
    delete p;
    };
  void no_beta_1() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();

    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-17-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"missing parameter in beta");
    delete p;
  };
  void no_beta_2() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-18-wrong.xml"));
    TEST_THROWS_MSG(p->parse(),Exc,"missing parameter in beta");
    delete p;
  };
    void ok_beta_3() {
    XMLParser::Parser * p;
    TaskFactory::task_descriptor_factory.clean_up();
    
    TEST_THROWS_NOTHING(p = new  XMLParser::Parser("./xml-tests/test-19-ok.xml"));
    TEST_THROWS_NOTHING(p->parse());
    delete p;
  };
};




bool run_Parser_tests()
{
  Test::Suite ts;
  ts.add(auto_ptr<Test::Suite>(new ParserTestSuite));
  //ts.add(auto_ptr<Test::Suite>(new  ProbPeriodicTaskDescriptorCRTestSuite));
     
  Test::TextOutput output(Test::TextOutput::Verbose);
  return ts.run(output);
}

int main() {
  try {
    if(run_Parser_tests()) {
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
