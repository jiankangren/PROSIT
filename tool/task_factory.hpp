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
#include"generic_factory.hpp"
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
  

  class GenericTaskBuilder: public GenericFactory::NamedEntityBuilder<GenericTaskDescriptor,GenericTaskParameters> {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, GenericTaskParameters * t) throw(Exc)=0;
    virtual TaskFactory::GenericTaskParameters * parse_parameters(const char * name, XMLElement * task) throw (Exc);
  };
  

  class PeriodicTaskBuilder : public GenericTaskBuilder{
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, GenericTaskParameters * t) throw(Exc)=0;
    virtual TaskFactory::GenericTaskParameters * parse_parameters(const char * name, XMLElement * task) throw (Exc);
  };

  class TaskDescriptorFactory: public GenericFactory::NamedEntityFactory<GenericTaskDescriptor,GenericTaskParameters> {
  public:
    void set_task_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc);
    void set_task_target_qos_bounds(const char * name, double qos_min, double qos_max) throw (Exc);
    int get_task_descriptor_vector(vector<GenericTaskDescriptor*> & v);  
  };
  extern TaskDescriptorFactory task_descriptor_factory;

};


#endif
