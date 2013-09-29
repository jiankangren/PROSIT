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
    string name; /*!< Name of the task */
    auto_ptr<pmf> C; /*!< Distribution of the computation time */
    auto_ptr<pmf> Z; /*!< Distribution of the interarrival time */
  
    bool verbose; /*!< Flag to print out information while processing */
    bool periodic; /*!< Flag for periodic tasks */
    int P;  /*!< Period (has meaning onlly if periodic flag is set) */
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
      P(0)
    {};
    
    //! Constructor for periodic tasks
    /*! \param nm unique identifier for the task
     *  \param Cd pointer to the distribution of the computation times 
     *  \param Pd task period
     */
    GenericTaskDescriptor(const char * nm, auto_ptr<pmf> Cd, int Pd) throw(Exc):
      name(nm),
      C(Cd),
      Z(0),
      verbose(false),
      periodic(true),
      P(Pd)
    {};
    
    //! Set verbose flag to a specified value
    /*! 
     * \param verbosed desidered value for the verbose flag
     * \return current value for the verbose flag
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

    //! Probability of respecting a deadline
    /*! This probability is computed for a given configuration
     * of the scheduling parameters.
     * \param deadline is the deadline we want to test
     * \return the probability of respecting the deadlien
     */
    virtual double probability(int deadline)=0;

    //! Probability of respecting a vectro of deadlines
    /*! This probability is computed for a given configuration
     * of the scheduling parameters.
     * \param dmin minimal deadline we want to test
     * \param dmax maximal deadline we want to test
     * \param step step between the deadlines
     * \param probs probabilities related to the different deadlines
     */
    virtual void probability(vector<double> & probs, int dmin, int dmax, int step) = 0;
    
  };
  

  
  class FixedPriorityTaskDescriptor : public GenericTaskDescriptor {

  };
  
}
