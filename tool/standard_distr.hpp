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
  
  //!Parameters for file based distributions
  struct FileDistrParameters: public DistrFactory::DistrParameters {
    std::string fileName;/*!< Name of the file */
    int size; /*!< Number of points of the distribution */
    FileDistrParameters(const char * nm, int sz = 50000): fileName(nm), size(sz) {};
    FileDistrParameters(const std::string & nm, int sz = 50000): fileName(nm), size(sz) {};
  };

  //!Builder class for file based distributions
  /*! A file based distribution is supplied
   * by a set of points: <value, probability >
   * stored in a file
   */
  class FileDistrBuilder: public DistrFactory::DistrBuilder {
  public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };  
  
  //! Parameters for synthetic distributions
  struct SyntheticDistrParameters: public DistrFactory::DistrParameters {
    int cmin; /*!< left extreme of the distribution */
    int cmax; /*!< right extreme of the distribution */
    int step; /*!< Step size */
    int size; /*!< Number of points of the distribution */
    bool dump; /*!< Dump the distribution aftet generating it */
    std::string dump_file; /*!< Name of the file to dump the distribution */

    SyntheticDistrParameters(int cmind, int cmaxd, int stepd,int sized, bool dumpd, const std::string & dump_file_d): 
      cmin(cmind),
      cmax(cmaxd),
      step(stepd),
      size(sized),
      dump(dumpd),
      dump_file(dump_file_d)
    {};
  };
  
  //! Root class for the builder of synthetic distributios
  /*! All synthetic distributions will be subclasses of
   * this one. For simplicity we require that step be
   * a sub-multiple of cmax-cmin.
   */
  class SyntheticDistrBuilder : public DistrFactory::DistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc)=0;
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
  
  //! Builder of uniform distributions
  /*! We do not need parameters other than the ones contained in
   * SyntheticDistrParameters
   */
  class UniformDistrBuilder : public SyntheticDistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
  
  //! Parameters for beta distribution
  struct BetaDistrParameters : public SyntheticDistrParameters {
    double a; /*!< alpha parameter */
    double b; /*!< beta parameter */
    BetaDistrParameters(int cmind, int cmaxd, int stepd,int sized, double ad, double bd, bool dumpd, const string & dump_file_d):
      SyntheticDistrParameters(cmind, cmaxd, stepd, sized, dumpd, dump_file_d), a(ad), b(bd) {};
  };

  //! Builder for beta distributions
  /*! The distribution is P(x) \proportional (1-x)^(b-1)x^(a-1)
   * (see http://mathworld.wolfram.com/BetaDistribution.html)
   * Both a and b are assumed greater than 1
   */
  class BetaDistrBuilder: public SyntheticDistrBuilder {
      public:
    virtual auto_ptr<pmf> create_instance(DistrFactory::DistrParameters * t) throw(Exc);
    virtual DistrFactory::DistrParameters* parse_parameters(XMLElement * distr) throw (Exc);
  };
};



#endif
