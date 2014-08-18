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
#include <memory>
using namespace tinyxml2;

namespace TaskFactory {

  //!Root for hiearchy of parameters
  struct GenericTaskParameters {
    unique_ptr<PrositAux::pmf> c; //!>
    int Qd; 
    int Tsd;
    int Deltad;
    double epsilond;
    GenericTaskParameters(unique_ptr<PrositAux::pmf> cd, int Qdd, int Tsdd, int Deltadd, double epsilondd): c(std::move(cd)), Qd(Qdd),Tsd(Tsdd),Deltad(Deltadd),epsilond(epsilondd) {};
    virtual ~GenericTaskParameters(){};
  };

  //!Parameters for a periodic task
  struct PeriodicTaskParameters: public GenericTaskParameters {
    int Pd;
    PeriodicTaskParameters(unique_ptr<GenericTaskParameters> pt, int Pdd):
      GenericTaskParameters(std::move(pt->c), pt->Qd, pt->Tsd, pt->Deltad, pt->epsilond), Pd(Pdd) {};
    PeriodicTaskParameters(unique_ptr<PrositAux::pmf> cd, int Pdd, int Qdd, int Tsdd, int Deltadd, double epsilondd): GenericTaskParameters(std::move(cd),Qdd,Tsdd,Deltadd,epsilondd), Pd(Pdd) {};
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
