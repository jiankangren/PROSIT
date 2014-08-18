/*
 * Classes and function for optimisation
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef OPTIMISATION_HPP
#define OPTIMISATION_HPP
#include <vector>
#include <string>
#include <utility>
#include <memory>
using namespace std;
#include "pmf.hpp"
const int typical_size=1000;


class GenericTaskDescriptor {
protected:
  string name;
  //Computation time
  auto_ptr<PrositAux::pmf> C;
  //Budget
  int Q;
  //Server Period
  int Ts;
  int Qmin;
  int Qmax;
  double QoSmin;
  double QoSmax;
  bool bounds_inited;
  bool verbose;
  double QoSmin_target;
  double QoSmax_target;

public:
  GenericTaskDescriptor(const char * nm, auto_ptr<PrositAux::pmf> c, const int Qd, const int Tsd) throw(Exc):
    name(nm),C(c),
    Q(Qd),
    Ts(Tsd),
    Qmin(0),
    Qmax(Tsd),
    bounds_inited(false),
    verbose(false)
  {
    if (double(Qd)/double(Tsd) > 1.0)
      EXC_PRINT_2("task period too small for", name);
  };
  void init_bounds() {
    QoSmin = QoS(Qmin);
    QoSmax = QoS(Qmax);
    bounds_inited = true;
  };

  virtual bool identify_bounds(double q_min, double q_max) throw(Exc);
  virtual bool identify_target_bounds() throw (Exc);


  bool set_verbose(bool verbosed) {
    bool current=verbose;
    verbose = verbosed;
    return current;
  };
  bool get_bounds_inited() {
    return bounds_inited;
  };
  virtual ~GenericTaskDescriptor() {
  };
  const PrositAux::pmf & get_computation_time() {
    return *C;
  };
  virtual int get_budget() const {
    return Q;
  };
  string get_name() const {
    return name;
  }
  int get_server_period() const {
    return Ts;
  };
  void set_budget(int Qd) throw(Exc) {
    if (double(Qd)/Ts > 1.0)
      EXC_PRINT_2("budget too large for object", name);
    Q=Qd;
  };
  int set_server_period(int Tsd) {
    if (double(Q)/double(Tsd) > 1.0)
      EXC_PRINT_2("server period too small for task ", name);
    Ts=Tsd;
  };
  //Need to redefine this for task specific QoS
  virtual double QoS(int budget)=0;
  virtual bool inv_QoS(double q, int & Q, bool ceil) = 0;
  int get_qmin() const {
    return Qmin;
  };
  int get_qmax() const {
    return Qmax;
  };
  double get_qosmin() const {
    return QoSmin;
  };
  double get_qosmax() const {
    return QoSmax;
  };
  int set_qmin(int Qmind) {
    int Qminold=Qmin;
    Qmin=Qmind;
    bounds_inited = false;
    return Qminold;
  };
  int set_qmax(int Qmaxd) {
    int Qmaxold=Qmaxd;
    Qmax=Qmaxd;
    bounds_inited = false;
    return Qmaxold;
  };
  double get_target_qosmin() const {
    return QoSmin_target;
  };
  double get_target_qosmax() const {
    return QoSmax_target;
  };
  double set_target_qosmax(double q) {
    double current = QoSmax_target;
    QoSmax_target = q;
    bounds_inited = false;
    return current;
  };
  double set_target_qosmin(double q) {
    double current = QoSmin_target;
    QoSmin_target = q;
    bounds_inited = false;
    return current;
  };
};
class AperiodicTaskDescriptor: public GenericTaskDescriptor {
  auto_ptr<PrositAux::pmf> Z;
public:
  PrositAux::pmf &get_interarrival_time() {
    return *Z;
  };
  AperiodicTaskDescriptor(const char * nm, auto_ptr<PrositAux::pmf> c, auto_ptr<PrositAux::pmf> Zd, const int Qd, const int Tsd): GenericTaskDescriptor(nm,c,Qd,Tsd), Z(Zd) {};
  ~AperiodicTaskDescriptor(){};
};

class PeriodicTaskDescriptor: public GenericTaskDescriptor {
protected:
  int P;
public:
  int get_period() const {
    return P;
  };
  void set_period(int Pd) {
    P = Pd;
  };
  PeriodicTaskDescriptor(const char * nm,auto_ptr<PrositAux::pmf> c, int Pd, const int Qd, const int Tsd): GenericTaskDescriptor(nm,c,Qd,Tsd), P(Pd){};
  virtual ~PeriodicTaskDescriptor() {};
};

class GenericBudgetOptimiser {
public:
  typedef enum Err {NOT_EXECUTED, OK, BAD_LOWER_BOUNDS, NO_SOLUTION} ErrCodes;
protected:
  vector<GenericTaskDescriptor*> tasks;
  ErrCodes state;
  unsigned int max_num;
  unsigned int number;
  double optimum;
  bool verbose;
public:
  double eps;
  GenericBudgetOptimiser(int task_num=typical_size, double eps = 1e-6);
  GenericBudgetOptimiser(vector<GenericTaskDescriptor*> v, double eps=1e-6);
  virtual ~GenericBudgetOptimiser() {};
  int insert_task(GenericTaskDescriptor * t, bool re_optimise=false) throw(Exc);
  GenericTaskDescriptor * remove_task(unsigned int i, bool re_optimise=false) throw(Exc);
  GenericTaskDescriptor * remove_task(const string & name, bool re_optmise=false) throw(Exc);
  int task_num() {
    return number;
  };
  bool set_verbose(bool verbosed) {
    bool current=verbosed;
    verbose= verbosed;
    return current;
  };
  int get_budget(unsigned int i) const throw(Exc);
  int get_budget(const string & name) const throw(Exc);
  double get_bandwidth(unsigned int i) const throw(Exc);
  double get_bandwidth(const string & name) const throw(Exc);
  double get_optimum() const {return optimum;};
  enum Err get_state() const {return state;};
  virtual double optimise()=0;
  virtual double optimise(const vector< pair<double,double> > & bounds) throw (Exc);
  void init_target_bounds() throw (Exc);
  double eval();
};

class InfinityNormBudgetOptimiser: public GenericBudgetOptimiser {
public:
  InfinityNormBudgetOptimiser(int task_num=typical_size, double eps = 1e-6): GenericBudgetOptimiser(task_num, eps) 
  {};
  InfinityNormBudgetOptimiser(vector<GenericTaskDescriptor*> v, double eps=1e-6):GenericBudgetOptimiser(v, eps)
  {};
  double optimise();
  double eval();
};

#endif
