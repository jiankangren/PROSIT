/*
 * abstract class for the function object computing the 
 * QoS function
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef QOS_FUN_HPP
#define QOS_FUN_HPP
class QoSFun {
public:
  virtual double eval (double prob) = 0;
};
#endif
