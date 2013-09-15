/*
 * Header file for functions required by computation of probabilities
 * using the companion technique
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef COMPANION_HPP
#define COMPANION_HPP
#include "pmf.hpp"
#include "exc.hpp"
double companion_old_model_compute_pi(const pmf & p, unsigned int N, unsigned int Q) throw(Exc);
double companion_compute_pi(const pmf & p, unsigned int N, unsigned int Q, RowVectorXd & pi, bool compute_vector) throw(Exc);
bool companion_set_verbose(bool v);

#endif
