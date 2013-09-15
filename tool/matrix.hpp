/*
 * MATRIX COMPUTATION FOR RESERVATION BASED SYSTEMS
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef MATRIX_HPP
#define MATRIX_HPP


double matrix_prob_ts(int i, int j, int q, const cdf &p, 
		      const pmf &u);
double matrix_prob_ts_compressed(int i, int j, int q, const cdf &p, 
		      const pmf &u);

void compute_matrixes(const MatrixXd & mat, int dim, MatrixXd & B, MatrixXd & A0,
		      MatrixXd & A1, MatrixXd & A2);



#endif
