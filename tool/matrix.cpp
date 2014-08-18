#include <iostream>
#include <algorithm>
#include "auxiliary_func.hpp"
#include "matrix_io.hpp"
#include "exc.hpp"
#include "smc_func.hpp"
#include "pmf.hpp"
#include "companion.hpp"
#include <stdio.h>
#include <algorithm>
#include<getopt.h>
#define Nc 25000
#define Nz 12
#include"matrix.hpp"

//Computes the transition matrix for a 
//reservation with computation time p and interarrival time u
//It uses the model we have used in JSS12
double matrix_prob_ts(int i, int j, int q, const PrositAux::cdf &p, 
		      const PrositAux::pmf &u)
{

  int z = 0; 
#ifdef DEBUG
  cerr<<"matrix_prob_ts called"<<endl;
  cerr<<"i = "<<i<<endl;
  cerr<<"j = "<<j<<endl;
  cerr<<"u.get_min() "<<u.get_min()<<endl;
  cerr<<"u.get_max() "<<u.get_max()<<endl;
  cerr<<"u: "<<endl;
  u.print();
  cerr<<"p: "<<endl;
  p.print();
#endif
  double prob = 0;
  int l = u.get_max();
  if ( i < u.get_min() * q)
    if (j== 0)
      prob = p.get(j);
    else
      prob=p.get(j) -p.get(j-1);
  else
    for (z = u.get_min();z <= l; z++)
      if ((j-i+z*q)<= p.get_min())
	prob = prob + u.get(z)*(p.get(j-i+z*q));
      else
	prob = prob + u.get(z)*(p.get(j-i+z*q)-p.get(j-i+z*q-1));
#ifdef DEBUG
  cerr<<"Probability Returned = "<<prob<<endl;
#endif
  return prob;
}


//Computes the transition matrix for a 
//reservation with computation time p and interarrival time u
//It uses the model we have used in JSS12 but it compresses the states
//smaller than the minimum interarrival time
double matrix_prob_ts_compressed(int i, int j, int q, const PrositAux::cdf &p, 
				 const PrositAux::pmf &u)
{
#ifdef DEBUG
  cerr<<"matrix_prob_ts called"<<endl;
  cerr<<"i = "<<i<<endl;
  cerr<<"j = "<<j<<endl;
  cerr<<"u.get_min() "<<u.get_min()<<endl;
  cerr<<"u.get_max() "<<u.get_max()<<endl;
  cerr<<"u: "<<endl;
  u.print();
  cerr<<"p: "<<endl;
  p.print();
#endif
  double prob = 0;
  int zmin = u.get_min();
  int zmax = u.get_max();
  if (i == 0) {
    if (j == 0)
      //P(v_h+1 <= zmin *Q|v_h <= z_min*Q)=P(c <=zmin*Q))
      prob = p.get(zmin*q);
    else
      //P(v_h+1 = zmin *Q+j |v_h <= z_min*Q)=P(c =zmin*Q+j))
      prob = p.get(zmin*q+j)-p.get(zmin*q+j-1);
  }
  else
    {
      if (j == 0) {
	//P(v_h+1 <= zmin *Q|v_h = z_min*Q+i)=P(c -zQ = -i)+ P(c -zQ = -i-1)+...+P(c -zQ = -i-zmin*Q)  
	for (int h = zmin; h <= zmax; h++)
	  prob += u.get(h)*(p.get(h*q-i)-p.get(h*q-i-zmin*q));
      }
      else
	//P(v_h+1 = zmin *Q+j|v_h = z_min*Q+i)=P(c -zQ = -i)+ P(c -zQ = -i-1)+...+P(c -zQ = -i-zmin
	for (int h = zmin; h <= zmax; h++)
	  prob += u.get(h)*(p.get(h*q+j-i)-p.get(h*q+j-i-1));
    }
#ifdef DEBUG
  cerr<<"Probability Returned = "<<prob<<endl;
#endif
  return prob;
}

void compute_matrixes(const MatrixXd & mat, int dim, MatrixXd & B, MatrixXd & A0,
		      MatrixXd & A1, MatrixXd & A2)
{
    int i, j, h, k;
    for (i = 0; i < dim; i++) {
	for (j = 0; j < dim; j++)
	  B(i,j) = mat(i,j);
	for (j = dim, h = 0; j < dim * 2; j++, h++)
	  A0(i,h) =mat(i,j);
    }
    for (i = dim, k = 0; i < dim * 2; i++, k++) {
	for (j = 0; j < dim; j++)
	  A2(k,j) = mat(i,j);
	for (j = dim, h = 0; j < dim * 2; j++, h++)
	  A1(k,h) = mat(i,j);
    }
}


