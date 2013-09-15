#include"standard_task.hpp"
#include"qos_factory.hpp"
#include "parser.hpp"
#include<assert.h>
using namespace XMLParser;
namespace StandardTasks {
  GenericTaskDescriptor * UserQoSPeriodicTaskBuilderCR::create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc) {
    UserQoSPeriodicTaskParameters * p;
    if(!(p=dynamic_cast<UserQoSPeriodicTaskParameters*>(t)))
      EXC_PRINT_2("wrong parameter type for task", name);
    return new UserQoSPeriodicTaskDescriptorCR(name, p->c, p->Pd, p->Qd, p->Tsd, p->Deltad, p->epsilond, p->qos); 
  };

  TaskFactory::GenericTaskParameters * UserQoSPeriodicTaskBuilder::parse_parameters(const char * name, XMLElement * task) throw (Exc) {
    TaskFactory::PeriodicTaskParameters * p;
    p = dynamic_cast<TaskFactory::PeriodicTaskParameters*>(PeriodicTaskBuilder::parse_parameters(name, task));
    assert(p);
    XMLElement * internal;
    if (! (internal= task->FirstChildElement("qosfun") )) 
      EXC_PRINT_2("Qos function undefined for task:", name);

    auto_ptr<QoSFun> f = Parser::qosfun_parse(internal);
    return new UserQoSPeriodicTaskParameters(auto_ptr<TaskFactory::PeriodicTaskParameters>(p), f);
  };
  GenericTaskDescriptor * UserQoSPeriodicTaskBuilderAnalytic::create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc) {
    UserQoSPeriodicTaskParameters * p;
    if(!(p=dynamic_cast<UserQoSPeriodicTaskParameters*>(t)))
      EXC_PRINT_2("Wrong parameter type for task:", name);

    return new UserQoSPeriodicTaskDescriptorAnalytic(name, p->c, p->Pd, p->Qd, p->Tsd, p->Deltad, p->epsilond, p->qos); 
  };
  
  UserQoSPeriodicTaskDescriptorCR::UserQoSPeriodicTaskDescriptorCR(const char * nm, 
						   auto_ptr<pmf> c, 
						   int Pd, int Qd, int Tsd, 
						   int Deltad,
						   double epsilond, 
						   auto_ptr<QoSFun> qosd) throw (Exc):
    ProbPeriodicTaskDescriptorCR(nm,c,Pd,Qd,Tsd,Deltad,epsilond), qos(qosd)
  {};
  double UserQoSPeriodicTaskDescriptorCR::QoS_from_prob(double prob) {
    return qos->eval(prob);
  }
  
  UserQoSPeriodicTaskDescriptorCR::~UserQoSPeriodicTaskDescriptorCR(){};
 
  UserQoSPeriodicTaskDescriptorAnalytic::UserQoSPeriodicTaskDescriptorAnalytic(const char * nm, 
						   auto_ptr<pmf> c, 
						   int Pd, int Qd, int Tsd, 
						   int Deltad,
						   double epsilond, 
									       auto_ptr<QoSFun> qosd) throw (Exc):
    ProbPeriodicTaskDescriptorAnalytic(nm,c,Pd,Qd,Tsd,Deltad,epsilond), qos(qosd)
  {};
  double UserQoSPeriodicTaskDescriptorAnalytic::QoS_from_prob(double prob) {
    return qos->eval(prob);
  }
  UserQoSPeriodicTaskDescriptorAnalytic::~UserQoSPeriodicTaskDescriptorAnalytic(){};
  void init() {
    TaskFactory::register_task_type("UserQoSCR",new UserQoSPeriodicTaskBuilderCR);
    TaskFactory::register_task_type("UserQoSAnalytic",new UserQoSPeriodicTaskBuilderAnalytic);
  };


};
