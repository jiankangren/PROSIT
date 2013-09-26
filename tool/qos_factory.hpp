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
  //! root to create hiearchies of parameters
  /*!
   *All parameters of a model for qos function should
   * subclass from this root
   */
  struct QoSFunParameters {
    virtual ~QoSFunParameters(){};
  };

  //! Instantiation of the generic builder to create factories of QoS function
  typedef GenericFactory::FunctorEntityBuilder<QoSFun,QoSFunParameters> QoSFunBuilder;
  
  //! Generic factory instantiation to create factories of QoS functions.
  /*! This factory is to create a functor object that, given a distribution of
   * delays or,simply, the probability of deadline miss returns the quality.
   * This is done via a method called eval (we could as well redefine (), but
   * at the moment we simply do not do this :=)
   */
  typedef GenericFactory::FunctorEntityFactory<QoSFun,QoSFunParameters> QoSFunFactory;
  
  //! Global object used to create factories
  /*!
   *This one should be probably the only object entitled to create qos functions.
   * At least, this is my current idea. However, I do not see particular reasons to
   * overconstrain future developments (e.g., by forcing this factory to be a singleton).
   */
  extern QoSFunFactory qos_fun_factory;
};

#endif
