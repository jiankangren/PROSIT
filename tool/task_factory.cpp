#include "task_factory.hpp"

#include <map>
#include <utility>
#include "parser.hpp"

using namespace std;
namespace TaskFactory {
  map<string,GenericTaskDescriptor*> name_dictionary;
  map<string,GenericTaskBuilder*> name_factory;
  
  bool register_task_type (const char * type_name, GenericTaskBuilder * b) {
    pair<string, GenericTaskBuilder*> p(string(type_name), b);

    return (name_factory.insert(p)).second;
  };
  GenericTaskDescriptor * create_task_descriptor_instance(const char * type_name, const char * task_name, GenericTaskParameters * p) throw (Exc) {
    if (name_dictionary.find(string(task_name))!= name_dictionary.end())
      EXC_PRINT_2("Duplicate name for task descriptor",task_name);

    map<string,GenericTaskBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end()) 
      EXC_PRINT_2("Decriptor type not registered",type_name);

    GenericTaskDescriptor * td = (*it).second->create_instance(task_name, p);
    pair<string, GenericTaskDescriptor* > b(string(task_name), td);

    name_dictionary.insert(b);
    return td;
  };
  GenericTaskDescriptor * get_task_descriptor_from_name(const char * name) {
     map<string,GenericTaskDescriptor*>::iterator it;
     it = name_dictionary.find(string(name));
     if (it != name_dictionary.end())
       return (*it).second;
     else
       return 0;
  };
  bool remove_task_descriptor_from_name(const char * name) {
    map<string,GenericTaskDescriptor*>::iterator it;
    it = name_dictionary.find(string(name));
    
    if (it != name_dictionary.end()) {
      name_dictionary.erase(string(name));
      return true;
    }
    else
      return false;
  };

  GenericTaskParameters * parse_task_descriptor_parameters(const char * type_name, const char * task_name, XMLElement * e) throw (Exc) {
    map<string,GenericTaskBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end()) 
      EXC_PRINT_2("Descriptor type not registered:", type_name);
    return (*it).second->parse_parameters(task_name, e);
  };

  void set_task_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc) {
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

  void set_task_target_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc) {
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

  int get_task_descriptor_vector(vector<GenericTaskDescriptor*> & v) {
    int num = 0;
    v.clear();
    map<string,GenericTaskDescriptor*>::iterator it;
    for (it=name_dictionary.begin();it!=name_dictionary.end();it++) {
      v.push_back((*it).second);
      num ++;
    };
    return num;
  };
  void clean_up() {
    task_clean_up();
    map<string,GenericTaskBuilder*>::iterator it1=name_factory.begin();
    
    for (it1=name_factory.begin();it1!=name_factory.end();it1++) {
      GenericTaskBuilder * q = (*it1).second;
      delete q;
    };
    name_factory.erase(name_factory.begin(),name_factory.end());
  };
  void task_clean_up() {
    map<string,GenericTaskDescriptor*>::iterator it; 
    for (it=name_dictionary.begin();it!=name_dictionary.end();it++) {
      GenericTaskDescriptor * p = (*it).second;
      delete p;
    };
    name_dictionary.erase(name_dictionary.begin(), name_dictionary.end());
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

    auto_ptr<pmf> c(XMLParser::Parser::distr_parse(internal));
    
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


};
