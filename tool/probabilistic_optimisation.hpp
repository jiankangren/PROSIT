/*
 * Headers containing classes and functions to
 * specialise optimisation to a probabilistic setup
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef PROBABILISTIC_OPTIMISATION_HPP
#define PROBABILISTIC_OPTIMISATION_HPP
#include "optimisation.hpp"
#include "pmf.hpp"

class ProbPeriodicTaskDescriptor: public PeriodicTaskDescriptor {
protected:
  double eps;
  int Delta;
  int sampledQ;
  auto_ptr<pmf> sampledCpmf;
  auto_ptr<cdf> sampledCcdf;

public:
  virtual double QoS_from_prob(double prob)=0;
  
  ProbPeriodicTaskDescriptor(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Delta, double epsilon=1e-38) throw(Exc);
  virtual double QoS(int Q) {
    return QoS_from_prob(probability(Q));
  };
  virtual double probability(int Q)=0;
  virtual bool inv_QoS(double q, int & Q,bool ceil);
  virtual ~ProbPeriodicTaskDescriptor() {
  };
};

class ProbPeriodicTaskDescriptorCR: public ProbPeriodicTaskDescriptor {
  int max_iter;
  bool shift;
public:
  ProbPeriodicTaskDescriptorCR(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad,double epsilon) throw (Exc);
  virtual double probability(int Q);
  virtual ~ProbPeriodicTaskDescriptorCR() {};
  int get_max_iter() { return max_iter;};
  int set_max_iter(int max) {
    int current = max_iter;
    max_iter = max;
    return current;
  };
  bool set_shift(bool sh) {
    bool current = shift;
    shift = sh;
    return current;
  };
};


class ProbPeriodicTaskDescriptorAnalytic: public ProbPeriodicTaskDescriptor {
public:
  ProbPeriodicTaskDescriptorAnalytic(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad,double epsilon) throw(Exc);
  virtual double probability(int Q);
  virtual ~ProbPeriodicTaskDescriptorAnalytic() {};
};





class ProbPeriodicTaskDescriptorCompanion: public ProbPeriodicTaskDescriptor {
public:
  ProbPeriodicTaskDescriptorCompanion(const char * nm, auto_ptr<pmf> c, int Pd, int Qd, int Tsd, int Deltad,double epsilon) throw(Exc);
  virtual double probability(int Q);
  virtual ~ProbPeriodicTaskDescriptorCompanion() {};
};




#endif
