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
  }

}


#endif
