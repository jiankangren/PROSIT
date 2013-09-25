/*
 * Header file for files implementing factories of probability distributions
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef DISTR_FACTORY_HPP
#define DISTR_FACTORY_HPP
#include "pmf.hpp"
#include "exc.hpp"
#include "generic_factory.hpp"
#include <memory>
#include <tinyxml2.h>
using namespace tinyxml2;
namespace DistrFactory {
  struct DistrParameters {
    virtual ~DistrParameters(){};
  };

  // class DistrBuilder {
  // public:
  //   virtual auto_ptr<pmf> create_instance(DistrParameters * t) throw(Exc)=0;
  //   virtual DistrParameters* parse_parameters(XMLElement * distr) throw (Exc)=0;
  // };

  typedef GenericFactory::FunctorEntityBuilder<pmf,DistrParameters> DistrBuilder;  
  typedef GenericFactory::FunctorEntityFactory<pmf,DistrParameters> DistributionFactory;
  
  extern DistributionFactory distr_factory;
};



#endif
