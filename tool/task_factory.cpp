#include "task_factory.hpp"

#include <map>
#include <utility>
#include "parser.hpp"

using namespace std;
namespace TaskFactory {

  void TaskDescriptorFactory::set_task_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc) {
    map<string,GenericTaskDescriptor*>::iterator it;
    it = name_dictionary.find(string(name));
    if(it==name_dictionary.end())
      EXC_PRINT_2("task not present:", name);
    if (qos_min > qos_max)
      EXC_PRINT_2("Bad qos limits for task:", name);
    if(!(*it).second->identify_bounds(qos_min,qos_max))
      EXC_PRINT_2("QoS bounds infeasible for task", name);
    return;
  } 

  void TaskDescriptorFactory::set_task_target_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc) {
    map<string,GenericTaskDescriptor*>::iterator it;
    it = name_dictionary.find(string(name));
    if(it==name_dictionary.end())
      EXC_PRINT_2("task not present:", name);
    if (qos_min > qos_max)
      EXC_PRINT_2("Bad qos limits for task:", name);
    (*it).second->set_target_qosmin(qos_min);
    (*it).second->set_target_qosmax(qos_max);
    
    return;
  } 

  int TaskDescriptorFactory::get_task_descriptor_vector(vector<GenericTaskDescriptor*> & v) {
    int num = 0;
    v.clear();
    map<string,GenericTaskDescriptor*>::iterator it;
    for (it=name_dictionary.begin();it!=name_dictionary.end();it++) {
      v.push_back((*it).second);
      num ++;
    };
    return num;
  };
  
  GenericTaskParameters * GenericTaskBuilder::parse_parameters(const char * name, XMLElement * taskElement) throw(Exc) {
    int Q;
    int Ts;
    int Delta;
    double epsilon;

    XMLElement * internal;
        
    if(!(internal= taskElement->FirstChildElement("serverPeriod"))) 
      EXC_PRINT_2("server period undefined for task: ",name);
    internal->QueryIntText(&Ts);
    
    if((internal= taskElement->FirstChildElement("serverBudget"))) 
      internal->QueryIntText(&Q);
    else
      Q = 0;
    if(!(internal= taskElement->FirstChildElement("pmf"))) 
      EXC_PRINT_2("pmf undefined for task",name);

    auto_ptr<PrositAux::pmf> c(XMLParser::Parser::distr_parse(internal));
    
    if((internal= taskElement->FirstChildElement("Delta")))
      internal->QueryIntText(&Delta);
    else
      Delta = 1;
    if((internal= taskElement->FirstChildElement("epsilon")))
      internal->QueryDoubleText(&epsilon);
    else
      epsilon = 1e-8;
    
    return new GenericTaskParameters(c,Q,Ts,Delta,epsilon);
  };
  GenericTaskParameters * PeriodicTaskBuilder::parse_parameters(const char * name, XMLElement * taskElement) throw(Exc) {
    int P;
    
    XMLElement * internal;
    GenericTaskParameters * p = GenericTaskBuilder::parse_parameters(name, taskElement);
    
    if(!(internal= taskElement->FirstChildElement("period"))) 
      EXC_PRINT_2("period undefined for task", name);
    internal->QueryIntText(&P);
    
    return new PeriodicTaskParameters(auto_ptr<GenericTaskParameters>(p), P);
    //return new PeriodicTaskParameters(p->c,P,p->Qd,p->Tsd,p->Deltad,p->epsilond);
    };

  
  TaskDescriptorFactory task_descriptor_factory;
};
