#include "distr_factory.hpp"
#include <map>
#include <string>
#include <utility>

namespace DistrFactory {  
  map<string,DistrBuilder*> name_factory;
  bool register_distr_type(const char * type_name, DistrBuilder  * b) {
    pair<string, DistrBuilder*> p(string(type_name), b);

    return (name_factory.insert(p)).second;
  }
  auto_ptr<pmf> create_distribution_instance(const char * type_name,
					     DistrParameters * p) throw(Exc) {
    map<string,DistrBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end())
      EXC_PRINT_2("Distribution descriptor type not registered:",type_name);
    return auto_ptr<pmf>((*it).second->create_instance(p));
  };

  DistrParameters * parse_distribution_parameters(const char * type_name,
						  XMLElement * p) throw(Exc) {
    map<string,DistrBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end())
      EXC_PRINT_2("Distribution type not registered:", type_name);
    return (*it).second->parse_parameters(p);
  };

  void clean_up() {
     map<string,DistrBuilder*>::iterator it1=name_factory.begin();
       
    for (it1=name_factory.begin();it1!=name_factory.end();it1++) {
      DistrBuilder * q = (*it1).second;
      delete q;
    };
    name_factory.erase(name_factory.begin(),name_factory.end());
    
  };

};
