#include"standard_task.hpp"
#include"qos_factory.hpp"
#include "parser.hpp"
#include<assert.h>
#include <memory>
using std::unique_ptr;

using namespace XMLParser;
namespace StandardTasks {
  GenericTaskDescriptor * UserQoSPeriodicTaskBuilderCR::create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc) {
    UserQoSPeriodicTaskParameters * p;
    if(!(p=dynamic_cast<UserQoSPeriodicTaskParameters*>(t)))
      EXC_PRINT_2("wrong parameter type for task", name);
    return new UserQoSPeriodicTaskDescriptorCR(name, std::move(p->c), p->Pd, p->Qd, p->Tsd, p->Deltad, p->epsilond, std::move(p->qos)); 
  };

  TaskFactory::GenericTaskParameters * UserQoSPeriodicTaskBuilder::parse_parameters(const char * name, XMLElement * task) throw (Exc) {
    TaskFactory::PeriodicTaskParameters * p;
    p = dynamic_cast<TaskFactory::PeriodicTaskParameters*>(PeriodicTaskBuilder::parse_parameters(name, task));
    assert(p);
    XMLElement * internal;
    if (! (internal= task->FirstChildElement("qosfun") )) 
      EXC_PRINT_2("Qos function undefined for task:", name);

    unique_ptr<QoSFun> f = Parser::qosfun_parse(internal);
    return new UserQoSPeriodicTaskParameters(unique_ptr<TaskFactory::PeriodicTaskParameters>(p), std::move(f));
  };
  GenericTaskDescriptor * UserQoSPeriodicTaskBuilderAnalytic::create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc) {
    UserQoSPeriodicTaskParameters * p;
    if(!(p=dynamic_cast<UserQoSPeriodicTaskParameters*>(t)))
      EXC_PRINT_2("Wrong parameter type for task:", name);

    return new UserQoSPeriodicTaskDescriptorAnalytic(name, std::move(p->c), p->Pd, p->Qd, p->Tsd, p->Deltad, p->epsilond, std::move(p->qos)); 
  };
  
  UserQoSPeriodicTaskDescriptorCR::UserQoSPeriodicTaskDescriptorCR(const char * nm, 
								   unique_ptr<PrositAux::pmf> c, 
								   int Pd, int Qd, int Tsd, 
								   int Deltad,
								   double epsilond, 
								   unique_ptr<QoSFun> qosd) throw (Exc):
    ProbPeriodicTaskDescriptorCR(nm,std::move(c),Pd,Qd,Tsd,Deltad,epsilond), qos(std::move(qosd))
  {};
  double UserQoSPeriodicTaskDescriptorCR::QoS_from_prob(double prob) {
    return qos->eval(prob);
  }
  
  UserQoSPeriodicTaskDescriptorCR::~UserQoSPeriodicTaskDescriptorCR(){};
 
  UserQoSPeriodicTaskDescriptorAnalytic::UserQoSPeriodicTaskDescriptorAnalytic(const char * nm, 
									       unique_ptr<PrositAux::pmf> c, 
									       int Pd, int Qd, int Tsd, 
									       int Deltad,
									       double epsilond, 
									       unique_ptr<QoSFun> qosd) throw (Exc):
    ProbPeriodicTaskDescriptorAnalytic(nm,std::move(c),Pd,Qd,Tsd,Deltad,epsilond), qos(std::move(qosd))
  {};
  double UserQoSPeriodicTaskDescriptorAnalytic::QoS_from_prob(double prob) {
    return qos->eval(prob);
  }
  UserQoSPeriodicTaskDescriptorAnalytic::~UserQoSPeriodicTaskDescriptorAnalytic(){};
  void init() {
    TaskFactory::task_descriptor_factory.register_type("UserQoSCR",new UserQoSPeriodicTaskBuilderCR);
    TaskFactory::task_descriptor_factory.register_type("UserQoSAnalytic",new UserQoSPeriodicTaskBuilderAnalytic);
  };


};
