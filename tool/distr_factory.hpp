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
  //! root to create hiearchies of parameters
  /*!
   *All parameters of a distribution model should
   * subclass from this root
   */
  struct DistrParameters {
    virtual ~DistrParameters(){};
  };

  //! Instantiation of the generic builder to create factories of distributions
  typedef GenericFactory::FunctorEntityBuilder<pmf,DistrParameters> DistrBuilder;  

    //! Generic factory instantiation to create factories of distributions
  /*! This factory is to create a distribution object.
   * Distributions can take the form of a pmf pr of a cdf
   * The crucial method for a distribution is get(int el), which
   * associates a probability with the integer value el.
   */
  typedef GenericFactory::FunctorEntityFactory<pmf,DistrParameters> DistributionFactory;
  
  //! Global object used to create distributions
  /*!
   *This one should be probably the only object entitled to create qos functions.
   * At least, this is my current idea. However, I do not see particular reasons to
   * overconstrain future developments (e.g., by forcing this factory to be a singleton).
   */
  extern DistributionFactory distr_factory;
};



#endif
