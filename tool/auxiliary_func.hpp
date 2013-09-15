/*
 * Header file for auxiliary mathematical functions
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#ifndef AUX_FUNC_HPP
#define AUX_FUNC_HPP
#include "exc.hpp"
#include <Eigen/SVD>
#include<sys/time.h>

using namespace Eigen;
using namespace std;

inline long long
my_get_time (void)
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return tv.tv_usec + 1000000LL * tv.tv_sec;
};


template < typename _Matrix_Type_ > double
InfinityNorm (const _Matrix_Type_ & a)
{

  return a.cwiseAbs ().rowwise ().sum ().maxCoeff ();
};

//Moore--Penrose Psuedoinverse
template < typename _Matrix_Type_ > void
pseudoInverse (const _Matrix_Type_ & a, _Matrix_Type_ & result,
	       double epsilon = std::numeric_limits < double >::epsilon ())
{

  Eigen::JacobiSVD < _Matrix_Type_ > svd = a.jacobiSvd (Eigen::ComputeFullU |
							Eigen::ComputeFullV);

  //Matrix<typename JacobiSVD< _Matrix_Type_ >::SingularValuesType,Dynamic,Dynamic> sigma = svd.singularValues();
  // MatrixXd sigma = MatrixXd (svd.singularValues ());
  // MatrixXd inv_sigma = sigma;
  _Matrix_Type_ H (a.cols (), a.rows ());
  
  H.setZero ();
  double tolerance =
    double (epsilon * svd.singularValues ().array ().abs ().maxCoeff ());
  int len = svd.singularValues ().array ().rows();
  //typename JacobiSVD< _Matrix_Type_ >::SingularValuesType tolerance = epsilon * double(std::max(a.cols(),a.rows()) * svd.singularValues().array().abs().maxCoeff());

  for (long i = 0; i < len; ++i)
    {
      double sigma = svd.singularValues()[i];
      if (sigma > tolerance)	
	H(i,i) = typename _Matrix_Type_::Scalar(1.0)/typename _Matrix_Type_::Scalar(sigma); 
    }
  
  //H.block (0, 0, lav.rows (), lav.cols ()) = _Matrix_Type_(lav);

  result = svd.matrixV () * H * (svd.matrixU ().adjoint());
  return;
};


inline bool
check_sizes (const MatrixXd & A0, const MatrixXd & A1)
{
  return (A0.rows () == A0.cols ()) && (A0.rows () == A1.rows ())
    && (A0.cols () == A1.cols ());
}

/* returns the eigenvector related to eigenvalue 1 */
RowVectorXd
stat (const MatrixXd & A)
throw (Exc);
#endif
