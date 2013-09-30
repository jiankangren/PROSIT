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
#ifndef TASK_DESCRIPTOR_HPP
#define TASK_DESCRIPTOR_HPP
#include <vector>
#include <string>
#include <utility>
#include <memory>
using namespace std;
#include "pmf.hpp"
namespace TaskDescriptor {

  //! Generic class descriptor
  /*! It is the root of the hierarchy of task descriptors
   * This descriptor only contains the timing information.
   * The hierarchy is specialised on the basis of: 1) scheduling algorithm
   * 2) solver family
   */
  class GenericTaskDescriptor {
  protected:
    string name; /*!< Name of the task */
    auto_ptr<pmf> C; /*!< Distribution of the computation time */
    auto_ptr<pmf> Z; /*!< Distribution of the interarrival time */
  
    bool verbose; /*!< Flag to print out information while processing */
    bool periodic; /*!< Flag for periodic tasks */
    unsigned int P;  /*!< Period (has meaning onlly if periodic flag is set) */
    
    unsigned int deadline_step; /*!< Granularity to define probabilistic deadlines */
    double qos; /*< Quality level */

    map<unsigned int,double> probabilistic_deadlines; /*!< Map associating deadlines with probabilities */
  public:

    //! Constructor for aperiodic Tasks
    /*! \param nm unique identifier for the task
     *  \param Cd pointer to the distribution of the computation times 
     *  \param Zd distirbution of the interarrival time
     */
    GenericTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, auto_ptr<pmf> Zd) throw(Exc):
      name(nm),
      C(Cd),
      Z(Zd),
      verbose(false),
      periodic(false),
      P(0),
      deadline_step(0),
      qos(0.0)
    {};
    
    //! Constructor for periodic tasks
    /*! \param nm unique identifier for the task
     *  \param Cd pointer to the distribution of the computation times 
     *  \param Pd task period
     */
    GenericTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, unsigned int Pd) throw(Exc):
      name(nm),
      C(Cd),
      Z(0),
      verbose(false),
      periodic(true),
      P(Pd),
      deadline_step(0),
      qos(0.0)
    {};
    
    //! Set verbose flag to a specified value
    /*! 
     * \param verbosed desidered value for the verbose flag
     * \return old value for the verbose flag
     */
    bool set_verbose(bool verbosed) {
      bool current=verbose;
      verbose = verbosed;
      return current;
    };
    
    //! Verify if the task is periodic
    bool is_periodic() const {
      return periodic;
    };
    
    //! Returns the task period
    /*! 
     * \return the task period (Exception raised if not periodic)
     */
     int get_period() const throw (Exc){
       if(!periodic)
	 EXC_PRINT_2("Period wrongly required for aperiodic task ", name);
       return P;
    };
    
    //! Returns a copy of the computation time distribution
    /*!
     * \return a copy of the pmf related to the computation time
     */
    int get_computation_time() const {
      return *C;
    };

    //! Returns a copy of the interarrival time distribution
    /*!
     * \return a copy of the pmf related to the interarrival time (Exceprion of the task is periodic)
     */
    int get_interarrival_time() const throw (Exc){
      if(periodic)
	 EXC_PRINT_2("Interarrival time wrongly required for periodic task ", name);
      return *Z;
    };    


    //! Define a deadline
    /*! \param deadline has to be a multiple of deadline_step
     * \return Exception thrown for multiple entries and for non-multiple
     * of the deadline_step
     */
    void insert_deadline(unsigned int deadline) throw (Exc) {
      if (deadline%deadline_step)
	EXC_PRINT_2("Wrong deadline values set for task ", name);
      pair<unsigned int, double> entry(deadline,0.0);
      if(!probabilistic_deadlines.insert(entry))
	EXC_PRINT_2("cannot create deadline for task ", name);
      return;
    };
    

    //! Computes the probability of respecting the deadline
    /*! These probabilities are computed for a given configuration
     * of the scheduling parameters.
     *      
     */
    virtual void compute_probability(int deadline)=0;
 
    double get_probablity(int deadline) const throw(Exc);

    //!Quality of service function 
    /*! \return Quality of Service associated with the current
     * configuration of scheduling parameters.
     */
    virtual double compute_QoS()=0;

    //!Inverse quality functions
    /*! 
     *  Compute the scheduling parameters required to attain the
     * level of quality in qos
     * \param ceil should we approximate the desired value from above (true),
     *   or from below (false)?
     */
    virtual void invert_QoS(bool ceil)=0;

    //!Returns the current level of the quality of service
    /*! Makes sense after calling compute_QoS()
     * \return current value of quality
     */
    double get_qos() const {return qos;};

    //! Set quality of service level
    /*! \param qosd desired quality level
     * It needs to be non-negative.
     * \return Previous level of the quality of service
     */
    double set_qos(double qosd) throw (Exc) {
      double old_qos = qos;
      qos = qosd;
      if (qosd < 0)
	EXC_PRINT_2("Negative value of QoS set for task ", name);
      return old_qos;
    };

    //! Destruct, which is virtual, being the class polymorphic
    virtual ~GenericTaskDescriptor() {};
  };
  

  //! Class for fixed priority task descriptors
  class FixedPriorityTaskDescriptor : public GenericTaskDescriptor {
    unsigned int priority; /*!<Scheduling priority of the task */
  public:
    //! Constructor for aperiodic Fixed Priority Tasks
    /*! \param nm unique identifier for the task
     *  \param Cd pointer to the distribution of the computation times 
     *  \param Zd distirbution of the interarrival time
     *  \param priorityd scheduling priority (in the range 0..99)
     */
    FixedPriorityTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, auto_ptr<pmf> Zd, unsigned int priorityd) throw(Exc):
      GenericTaskDescriptor(nm,Cd,Zd)
    {
      if (priorityd>99)
	EXC_PRINT_2("Priority out of range for task ", nm);
      priority = priorityd;
    };
    
    //! Constructor for Fixed Priority periodic tasks
    /*! \param nm unique identifier for the task
     *  \param Cd pointer to the distribution of the computation times 
     *  \param Pd task period
     *  \param priorityd scheduling priority (in the range 0..99)
     */
    FixedPriorityTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, unsigned int Pd, unsigned int priorityd) throw(Exc):
      GenericTaskDescriptor(nm,Cd,Pd)
    {
      if (priorityd>99)
	EXC_PRINT_2("Priority out of range for task ", nm);
      priority = priorityd;
    };
    
    //!Returns the task priority
    unsigned int get_priority() const {return priority;}; 

    //! Sets the task priority
    /*! \param priority new task priority
     * \return old priority
     */
    unsigned int set_priority(unsigned int priorityd) {
      unsigned int old_prio = priority;
      priority = priorityd;
      return old_prio;
    };
  };
  
  class ResourceReservationTaskDescriptor: public GenericTaskDescriptor {
    int Q; /*!< Reservation budget */
    int Ts; /*!< Reservation period */
    int Qmin; /*!< Minimum value for budget */
    int Qmax; /*!< Maximum value for the budget */
    double QoSmin; /*!< Minimum value for quality*/
    double QoSmax; /*!< Maximum value for quality */
    bool bounds_inited;
    double QoSmin_target;
    double QoSmax_target;
  public:
    ResourceReservationTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, auto_ptr<pmf> Zd, const int Qd, const int Tsd) throw(Exc):
      GenericTaskDescriptor(nm, Cd, Zd, Qd, Tsd), 
      Q(Qd),
      Ts(Tsd),
      Qmin(0),
      Qmax(Tsd),
      bounds_inited(false)
    {
      if (double(Qd)/double(Tsd) > 1.0)
	EXC_PRINT_2("task period too small for", name);
    };

    void init_bounds() throw(Exc) {
      set_budget(Qmin);
      QoSmin = QoS();
      set_budget(Qmax);
      QoSmax = QoS();
      bounds_inited = true;
    };

    virtual bool identify_bounds(double q_min, double q_max) throw(Exc);
    virtual bool identify_target_bounds() throw (Exc);
    bool get_bounds_inited() const {
      return bounds_inited;
    };
    virtual ~ResourceReservationTaskDescriptor() {
    };
    
    int get_budget() const {
      return Q;
    };
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
  }
};
