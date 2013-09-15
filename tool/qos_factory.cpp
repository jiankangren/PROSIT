#include "qos_factory.hpp"
#include <map>
#include <string>
#include <utility>

namespace QoSFactory {
  map<string,QoSFunBuilder*> name_factory;
  
  bool register_qosfun_type(const char * type_name, QoSFunBuilder  * b) {
    pair<string, QoSFunBuilder*> p(string(type_name), b);

    return (name_factory.insert(p)).second;
  }
  auto_ptr<QoSFun> create_qosfun_instance(const char * type_name,
					  QoSFunParameters * p) throw(Exc) {
    map<string,QoSFunBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end())
      EXC_PRINT_2("QoS function descriptor type undefined",type_name);

    return auto_ptr<QoSFun>((*it).second->create_instance(p));
  };

  QoSFunParameters * parse_qosfun_parameters(const char * type_name,
					    XMLElement * p) throw(Exc) {
    map<string,QoSFunBuilder*>::iterator it;
    if ((it=name_factory.find(string(type_name)))== name_factory.end())
      EXC_PRINT_2("QoS function type not registerd:", type_name);

    return (*it).second->parse_parameters(p);
  };

  void clean_up() {
     map<string,QoSFunBuilder*>::iterator it1=name_factory.begin();
       
    for (it1=name_factory.begin();it1!=name_factory.end();it1++) {
      QoSFunBuilder * q = (*it1).second;
      delete q;
    };
    name_factory.erase(name_factory.begin(),name_factory.end());
    
  };
};
