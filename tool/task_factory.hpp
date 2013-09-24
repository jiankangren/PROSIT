/*
 * Header file for task factories
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef TASK_FACTORY
#define TASK_FACTORY
#include "probabilistic_optimisation.hpp"
#include "exc.hpp"
#include <tinyxml2.h>
using namespace tinyxml2;

namespace TaskFactory {
  struct GenericTaskParameters {
    auto_ptr<pmf> c; 
    int Qd; 
    int Tsd;
    int Deltad;
    double epsilond;
    GenericTaskParameters(auto_ptr<pmf> cd, int Qdd, int Tsdd, int Deltadd, double epsilondd): c(cd), Qd(Qdd),Tsd(Tsdd),Deltad(Deltadd),epsilond(epsilondd) {};
    virtual ~GenericTaskParameters(){};
  };

  struct PeriodicTaskParameters: public GenericTaskParameters {
    int Pd;
    PeriodicTaskParameters(auto_ptr<GenericTaskParameters> pt, int Pdd):
      GenericTaskParameters(*pt), Pd(Pdd) {};
    PeriodicTaskParameters(auto_ptr<pmf> cd, int Pdd, int Qdd, int Tsdd, int Deltadd, double epsilondd): GenericTaskParameters(cd,Qdd,Tsdd,Deltadd,epsilondd), Pd(Pdd) {};
    virtual ~PeriodicTaskParameters(){};
  };
  

  class GenericTaskBuilder {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, GenericTaskParameters * t) throw(Exc)=0;
    virtual TaskFactory::GenericTaskParameters * parse_parameters(const char * name, XMLElement * task) throw (Exc);
  };
  

  class PeriodicTaskBuilder : public GenericTaskBuilder{
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, GenericTaskParameters * t) throw(Exc)=0;
    virtual TaskFactory::GenericTaskParameters * parse_parameters(const char * name, XMLElement * task) throw (Exc);
  };

  
  bool register_task_type(const char * type_name, GenericTaskBuilder  * b);
  GenericTaskDescriptor * create_task_descriptor_instance(const char * type_name,
							  const char * task_name,
							  GenericTaskParameters * p) throw(Exc);
  GenericTaskParameters * parse_task_descriptor_parameters(const char * type_name,
						    const char * task_name,
						    XMLElement * p) throw(Exc);
  GenericTaskDescriptor * get_task_descriptor_from_name(const char * name);
  void clean_up();
  void task_clean_up();
  bool remove_task_descriptor_from_name(const char * name);
  void set_task_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc);
  void set_task_target_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc);
  int get_task_descriptor_vector(vector<GenericTaskDescriptor*> & v);
};


#endif
