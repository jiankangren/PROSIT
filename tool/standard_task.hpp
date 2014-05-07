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
namespace StandardTasks {
  void init();
  struct UserQoSPeriodicTaskParameters :  public TaskFactory::PeriodicTaskParameters {
    auto_ptr<QoSFun> qos;
    UserQoSPeriodicTaskParameters(auto_ptr<PeriodicTaskParameters> p, auto_ptr<QoSFun> qosd):
      TaskFactory::PeriodicTaskParameters(*p), qos(qosd) {};
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
    auto_ptr<QoSFun> qos;
  public:
    UserQoSPeriodicTaskDescriptorCR(const char * nm, 
			   auto_ptr<pmf> c, 
			   int Pd, int Qd, int Tsd, 
			   int Deltad,
			   double epsilond, 
			   auto_ptr<QoSFun> qosd) throw (Exc);
    virtual double QoS_from_prob(double prob);
    ~UserQoSPeriodicTaskDescriptorCR();
  };

  class UserQoSPeriodicTaskBuilderAnalytic: public UserQoSPeriodicTaskBuilder {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc);
  };
  class UserQoSPeriodicTaskDescriptorAnalytic: public ProbPeriodicTaskDescriptorAnalytic {
  private:
    auto_ptr<QoSFun> qos;
  public:
    UserQoSPeriodicTaskDescriptorAnalytic(const char * nm, 
			   auto_ptr<pmf> c, 
			   int Pd, int Qd, int Tsd, 
			   int Deltad,
			   double epsilond, 
			   auto_ptr<QoSFun> qosd) throw (Exc);
    virtual double QoS_from_prob(double prob);
    ~UserQoSPeriodicTaskDescriptorAnalytic();
  };

  class UserQoSPeriodicTaskBuilderCompanion: public UserQoSPeriodicTaskBuilder {
  public:
    virtual GenericTaskDescriptor * create_instance(const char * name, TaskFactory::GenericTaskParameters * t) throw(Exc);
  };
  class UserQoSPeriodicTaskDescriptorCompanion: public ProbPeriodicTaskDescriptorCompanion {
  private:
    auto_ptr<QoSFun> qos;
  public:
    UserQoSPeriodicTaskDescriptorCompanion(const char * nm, 
			   auto_ptr<pmf> c, 
			   int Pd, int Qd, int Tsd, 
			   int Deltad,
			   double epsilond, 
			   auto_ptr<QoSFun> qosd) throw (Exc);
    virtual double QoS_from_prob(double prob);
    ~UserQoSPeriodicTaskDescriptorCompanion();
  };
}

#endif
