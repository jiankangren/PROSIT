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
  void init();
  struct LinearQoSFunParameters: public QoSFactory::QoSFunParameters {
    double scale;
    double pmin;
    double pmax;
  public:
    LinearQoSFunParameters(double scaled, double pmind, double pmaxd): scale(scaled), pmin(pmind), pmax(pmaxd) {
      if((pmind > pmaxd)||
	 (pmind <0)||(pmind >1.0)||
	 (pmaxd <0)||(pmaxd >1.0)) 
	EXC_PRINT("wrong probability limits");
      if (scaled < 0)
	EXC_PRINT("wrong scaling constant");
    };
  };
  class LinearQoSFun: public QoSFun {
    double scale;
    double pmin;
    double pmax;
  public:
    LinearQoSFun(double scaled, double pmind, double pmaxd) throw (Exc);
    virtual double eval(double prob);
  };
  class LinearQoSFunBuilder: public QoSFactory::QoSFunBuilder {
    virtual auto_ptr<QoSFun> create_instance(QoSFactory::QoSFunParameters * t) throw(Exc);
    virtual QoSFactory::QoSFunParameters* parse_parameters(XMLElement * qosfunel) throw (Exc);
  };

  class QuadraticQoSFun: public QoSFun {
    double scale;
    double pmin;
    double pmax;
  public:
    QuadraticQoSFun(double scaled, double pmind, double pmaxd) throw (Exc);
    virtual double eval(double prob);
  };

  class QuadraticQoSFunBuilder: public LinearQoSFunBuilder {
    virtual auto_ptr<QoSFun> create_instance(QoSFactory::QoSFunParameters * t) throw(Exc);
  };
};



#endif
