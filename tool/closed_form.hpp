/*
 * Header file for functions required by analytic computation of probabilities
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef CLOSED_FORM_HPP
#define CLOSED_FORM_HPP
#include "pmf.hpp"
#include "exc.hpp"
double closed_form_old_model_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q) throw(Exc);
double closed_form_compute_pi(const PrositAux::pmf & p, unsigned int N, unsigned int Q) throw(Exc);
bool closed_form_set_verbose(bool v);

#endif
