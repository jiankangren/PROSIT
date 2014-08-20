/*
 * Classes and function for qos optimisation. It just applies to Resource Reservation.
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef QOS_OPTIMISATION_HPP
#define QOS_OPTIMISATION_HPP
#include "task_descriptor.hpp"

namespace PrositCore {
  ///! Specialisation of Resource Reservation task descriptor that is suitable for QoS optimisation. 
  /*! This specialisation adds data and methods for QOS management.
   */
  class RRQosTaskDescriptor: public ResourceReservationsTaskDescriptor {
    double qos; /*< Quality level */
     int Qmin; /*!< Minimum value for budget */
    int Qmax; /*!< Maximum value for the budget */
    double QoSmin; /*!< Minimum value for quality*/
    double QoSmax; /*!< Maximum value for quality */
    bool bounds_inited;
    double QoSmin_target;
    double QoSmax_target;

  public:
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

}


#endif
