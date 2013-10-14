/*
 * Header file for probability distribution classes
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef PMF_H
#define PMF_H
#include "exc.hpp"
#include "auxiliary_func.hpp"
//#include "smc_func.hpp"
#include<algorithm>

#include <Eigen/Dense>
#include <Eigen/SVD>
#define EPS 1e-5
#define TYPICAL_SIZE 10000

class distr {
protected:
  VectorXd  elems;
  unsigned int size;
  unsigned int offset;
  int min;
  int max;
public:
  double epsilon;
  distr(unsigned int sz=TYPICAL_SIZE, unsigned int offs=0, double my_epsilon=EPS): elems(sz), size(sz), offset(offs), min(sz-offs), max(-offs), epsilon(my_epsilon) {};
  distr(const distr & p): elems(p.elems),
			  size(p.size),
			  offset(p.offset),
			  min(p.min),
			  max(p.max),
			  epsilon(p.epsilon)
  {
  };
  const distr & operator=(const distr &p) {
    elems=p.elems;
    size=p.size;
    offset=p.offset;
    epsilon=p.epsilon;
    min = p.min;
    max=p.max;
    return *this;
  };
  virtual ~distr() {};
  int get_max() const {
    return max;
  };
  int get_min() const {
    return min;  
  };
  unsigned int get_size() const {return size;};
  unsigned int get_offset() const {return offset;};
  virtual void print() const {
    for (int i=get_min(); i<=get_max();i++)
      cout<<i<<": "<<get(i)<<endl;
  };
  virtual int set(int val, double p) throw(Exc)=0;
  virtual double get(int el) const throw(Exc)  = 0;    
  int load(const char * filename) throw (Exc) {
    return load(string(filename));
  };
  virtual int load(const string & filename) throw(Exc);
};
class pmf;
class cdf;

class pmf : public distr {
  double tail;

public:
  typedef enum ERR_CODES {PMF_OK, PMF_NEG, PMF_SMALLER_ONE, PMF_GREATER_ONE} ERR_CODES;
  pmf(unsigned int sz=TYPICAL_SIZE, unsigned int offs=0, double my_epsilon=EPS);
  pmf(const pmf & p): distr::distr(p){
    tail=p.tail;
  };
  double avg() const;
  double std() const;
  double var() const;
  ~pmf();
  virtual int set(int val, double p) throw(Exc);
  int set_samples(int samples);
  double sum() const;
  ERR_CODES check() const;
  
  virtual double get(int el) const throw(Exc) {
    if(el+offset > size-1) {
      cerr<<"El: "<<el<<" Size: "<<size<<endl;
      cerr<<"min: "<<get_min()<<" Max: "<<get_max()<<endl;
      EXC_PRINT("access out of range");
    }
    else
      return elems(el+offset);
  };
  double get_tail() {
    return tail;
  };
  virtual int load(const string & filename) throw(Exc);
  pmf * resample(int q) const;
  friend void pmf2cdf(const pmf & p, cdf & c);
  friend void cdf2pmf(const cdf & c, pmf & p);
  
};

class cdf : public distr {
  bool just_created;

public:
  typedef enum ERR_CODES {CDF_OK, CDF_BAD_MAX, CDF_BAD_MIN, CDF_NON_INCREASING} ERR_CODES;
  cdf(unsigned int sz=TYPICAL_SIZE, unsigned int offs=0, double epsilon=EPS);
  ~cdf();
  cdf(const cdf & p):
    distr::distr(p) {
    just_created=p.just_created;
  };
  virtual int set(int val, double p) throw(Exc);
  ERR_CODES check() const;
  void finalise() throw(Exc);
  virtual double get(int el) const throw(Exc);

  double get_epsilon() const {return epsilon;};
  virtual int load(const string & filename) throw(Exc);
  friend void pmf2cdf(const pmf & p, cdf & c); 
  friend void cdf2pmf(const cdf & c, pmf & p);
};

void pmf2cdf(const pmf & p, cdf & c);
void cdf2pmf(const cdf & c, pmf & p);
 


#endif
