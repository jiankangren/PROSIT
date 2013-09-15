/*
 * MATRIX IO functions
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef MATRIX_IO_HPP
#define MATRIX_IO_HPP
#include <Eigen/Dense>
#include "exc.hpp"
using namespace Eigen;

void square_matrix_input(const char * filename,
		  MatrixXd & res) throw (Exc);

void column_vector_input(const char * fname,
			 VectorXd &column) throw (Exc);


#endif
