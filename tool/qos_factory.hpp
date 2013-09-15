/*
 * classes and functions for creating factories of 
 * user defined QoS metrics
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef QOS_FACTORY_HPP
#define QOS_FACTORY_HPP
#include "qos_fun.hpp" 
#include "exc.hpp"
#include <memory>
#include <tinyxml2.h>
using namespace tinyxml2;
namespace QoSFactory {
  struct QoSFunParameters {
    virtual ~QoSFunParameters(){};
  };

  class QoSFunBuilder {
  public:
    virtual auto_ptr<QoSFun> create_instance(QoSFunParameters * t) throw(Exc)=0;
    virtual QoSFunParameters* parse_parameters(XMLElement * qosfunel) throw (Exc)=0;
  };
  
  bool register_qosfun_type(const char * type_name, QoSFunBuilder  * b);
  auto_ptr<QoSFun> create_qosfun_instance(const char * type_name,
					  QoSFunParameters * p) throw(Exc);
  QoSFunParameters * parse_qosfun_parameters(const char * type_name,
					    XMLElement * p) throw(Exc);
  void clean_up();  
};

#endif
