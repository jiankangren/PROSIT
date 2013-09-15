/*
 * Headers for functions for QBD solutions using
 * various algorithm 
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef SMC_FUNC_HPP
#define SMC_FUNC_HPP
#include <Eigen/Dense>
#include "exc.hpp"
#include "pmf.hpp"
using namespace Eigen;

struct qbd_parms {
  bool verbose;
  unsigned int max_iter;
};


struct qbd_cr_parms : public qbd_parms {
  bool shift; /* true = shift mode, false = basic mode */
};

void qbd_cr(const MatrixXd & A0, 
	   const MatrixXd & A1, 
	   const MatrixXd & A2,
	   MatrixXd & G,
	   MatrixXd & R,
	   MatrixXd & U,
	   const struct qbd_cr_parms & parms) throw(Exc);
void qbd_latouche(const MatrixXd & A0, 
	   const MatrixXd & A1, 
	   const MatrixXd & A2,
	   MatrixXd & G,
	   MatrixXd & R,
	   MatrixXd & U) throw(Exc);

struct qbd_latouche_parms : public qbd_parms {
  double epsilon;
};

void qbd_latouche(const MatrixXd & Ap0, 
		  const MatrixXd & Ap1, 
		  const MatrixXd & Ap2,
		  MatrixXd & R,
		  const struct qbd_latouche_parms & parms) throw(Exc);


bool qbd_compute_pi0(const MatrixXd & R,
		     const MatrixXd & B0,
		     const MatrixXd & A0,
		     RowVectorXd & pi0,
		     const qbd_parms & parms) throw (Exc);

#endif
