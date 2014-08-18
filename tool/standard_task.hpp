/*
 * Headers for standard task models
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef STD_TASK_HPP
#define STD_TASK_HPP
#include "probabilistic_optimisation.hpp"
#include "task_factory.hpp"
#include "qos_fun.hpp"
using std::unique_ptr;
namespace StandardTasks {
  void init();
  struct UserQoSPeriodicTaskParameters :  public TaskFactory::PeriodicTaskParameters {
    unique_ptr<QoSFun> qos;
    UserQoSPeriodicTaskParameters(unique_ptr<PeriodicTaskParameters> p, unique_ptr<QoSFun> qosd):
      TaskFactory::PeriodicTaskParameters(std::move(p->c), p->Pd, p->Qd, p->Tsd, p->Deltad, p->epsilond), 
      qos(std::move(qosd)) {};
  };

  class UserQoSPeriodicTaskBuilder: public TaskFactory::PeriodicTaskBuilder {
    public:
    virtual GenericTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc)=0;
    virtual TaskFactory::GenericTaskParameters * parse_parameters(const char * name, XMLElement * task) throw (Exc);
  };

  class UserQoSPeriodicTaskBuilderCR: public UserQoSPeriodicTaskBuilder {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc);
  };
  class UserQoSPeriodicTaskDescriptorCR: public ProbPeriodicTaskDescriptorCR {
  private:
    unique_ptr<QoSFun> qos;
  public:
    UserQoSPeriodicTaskDescriptorCR(const char * nm, 
				    unique_ptr<PrositAux::pmf> c, 
				    int Pd, int Qd, int Tsd, 
				    int Deltad,
				    double epsilond, 
				    unique_ptr<QoSFun> qosd) throw (Exc);
    virtual double QoS_from_prob(double prob);
    ~UserQoSPeriodicTaskDescriptorCR();
  };

  class UserQoSPeriodicTaskBuilderAnalytic: public UserQoSPeriodicTaskBuilder {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc);
  };
  class UserQoSPeriodicTaskDescriptorAnalytic: public ProbPeriodicTaskDescriptorAnalytic {
  private:
    unique_ptr<QoSFun> qos;
  public:
    UserQoSPeriodicTaskDescriptorAnalytic(const char * nm, 
					  unique_ptr<PrositAux::pmf> c, 
			   int Pd, int Qd, int Tsd, 
			   int Deltad,
			   double epsilond, 
			   unique_ptr<QoSFun> qosd) throw (Exc);
    virtual double QoS_from_prob(double prob);
    ~UserQoSPeriodicTaskDescriptorAnalytic();
  };
}

#endif
