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
#include "generic_factory.hpp"
#include <memory>
#include <tinyxml2.h>
using namespace tinyxml2;
namespace QoSFactory {
  struct QoSFunParameters {
    virtual ~QoSFunParameters(){};
  };
  typedef GenericFactory::FunctorEntityBuilder<QoSFun,QoSFunParameters> QoSFunBuilder;  
  typedef GenericFactory::FunctorEntityFactory<QoSFun,QoSFunParameters> QoSFunFactory;
  extern QoSFunFactory qos_fun_factory;
};

#endif
