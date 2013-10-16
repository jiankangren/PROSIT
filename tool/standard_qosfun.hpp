/*
 * Headers for standard QoS function models
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef STD_QOSFUN_HPP
#define STD_QOSFUN_HPP
#include "qos_fun.hpp"
#include "qos_factory.hpp"
namespace StandardQoSFun {
  //! Initialiser
  /*! 
   * This function registers the standard types for 
   * quality of service function defined in this
   * file. It must be called prior to using any of the
   * types documented here.
   */
  void init();

  //! Parameter structure for the linear Quality of service model.
  struct LinearQoSFunParameters: public QoSFactory::QoSFunParameters {
    double scale;
    double pmin;
    double pmax;
    double offset;
  public:
    LinearQoSFunParameters(double scaled, double pmind, double pmaxd, double offsetd=0): scale(scaled), pmin(pmind), pmax(pmaxd), offset(offsetd) {
      
      if((pmind > pmaxd)||
	 (pmind <0)||(pmind >1.0)||
	 (pmaxd <0)||(pmaxd >1.0)) 
	EXC_PRINT("wrong probability limits");
      if (scaled < 0)
	EXC_PRINT("wrong scaling constant");
    };
  };

  //! Class implementing the linear model for the quality of service
  /*! Le p represent the probability of deadline miss
   * The idea is that for p < pmin the quality is offset;
   * it grows linearly from pmin to pmax with a slope equal to scale,
   * and it saturates for probability grater than pmax
   */
  class LinearQoSFun: public QoSFun {
    double scale; /**< Slope fo the linear mapping */
    double pmin; /**< Lower bound for the probability of deadline hit (quality offset below) */
    double pmax; /**< Upper bound for the probability of deadline hit (quality saturates above) */
    double offset; /**< Minimum value for the quality */
  public:
    LinearQoSFun(double scaled, double pmind, double pmaxd, double offsetd=0) throw (Exc);
    //! Function returning the quality
    /*! \param prob is the probability of respecting the deadline. In
     *  a different implementation could easily be a vector of probabilities.
     */
    virtual double eval(double prob);
  };

  //! Sub class of QoSFun builder used to build a linear QoS function
  class LinearQoSFunBuilder: public QoSFactory::QoSFunBuilder {
    virtual auto_ptr<QoSFun> create_instance(QoSFactory::QoSFunParameters * t) throw(Exc);
    virtual QoSFactory::QoSFunParameters* parse_parameters(XMLElement * qosfunel) throw (Exc);
  };

  //! Class implementing the linear model for the quality of service
  /*! Le p represent the probability of deadline miss
   * The idea is that for p < pmin the quality is zero;
   * it grows as scale times the probability from pmin to pmax,
   * and it saturates for probability grater than pmax
   */
  class QuadraticQoSFun: public QoSFun {
    double scale;
    double pmin;
    double pmax;
  public:
    QuadraticQoSFun(double scaled, double pmind, double pmaxd) throw (Exc);
    virtual double eval(double prob);
  };
  
  //! Sub class of QoSFun builder used to build a quadratic QoS function
  class QuadraticQoSFunBuilder: public LinearQoSFunBuilder {
    virtual auto_ptr<QoSFun> create_instance(QoSFactory::QoSFunParameters * t) throw(Exc);
  };
};



#endif
