/*
 * Headers for standard distribution type
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef STD_DISTR_HPP
#define STD_DISTR_HPP
#include "pmf.hpp"
#include "distr_factory.hpp"
namespace StandardDistributions {
  void init();

  struct FileDistrParameters: public DistrFactory::DistrParameters {
    std::string fileName;
    int size;
    FileDistrParameters(const char * nm, int sz = 50000): fileName(nm), size(sz) {};
    FileDistrParameters(const std::string & nm, int sz = 50000): fileName(nm), size(sz) {};
  };
  class FileDistrBuilder: public DistrFactory::DistrBuilder {
  public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };  
  
  struct SyntheticDistrParameters: public DistrFactory::DistrParameters {
    int cmin;
    int cmax;
    int step;
    int size;
    SyntheticDistrParameters(int cmind, int cmaxd, int stepd,int sized): 
      cmin(cmind),
      cmax(cmaxd),
      step(stepd),
      size(sized)
    {};
  };
  
  class SyntheticDistrBuilder : public DistrFactory::DistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc)=0;
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
  
  class UniformDistrBuilder : public SyntheticDistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
  
  struct BetaDistrParameters : public SyntheticDistrParameters {
    double a;
    double b;
    BetaDistrParameters(int cmind, int cmaxd, int stepd,int sized, double ad, double bd):
      SyntheticDistrParameters(cmind, cmaxd, stepd, sized), a(ad), b(bd) {};
  };
  class BetaDistrBuilder: public SyntheticDistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
};



#endif
