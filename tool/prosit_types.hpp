/*
 * Base data types used in the PROSIT tool.
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef PROSIT_TYPES_HPP
#define PROSIT_TYPES_HPP
#include <map>

namespace PrositCore {
  ///!All deadlines are defiend as integer multiple of 
  ///a basic quantity
  typedef unsigned int DeadlineUnit;
  
  ///!Data structure that associates a deadline with its probability
  typedef std::map<DeadlineUnit,double> DeadlineProbabilityMap; 

};
#endif
