#include<math.h>
#include "pmf.hpp"
#include<fstream>
#include<sstream>
#include<string>
#include <stdlib.h>
#include<iostream>

using namespace std;
//const double epsilon = 1e-10;



int distr::load(const string & filename) throw (Exc) {
  ifstream myfile (filename.c_str());
  string line;
  stringstream sstr;
  long size;
  double d; double i;
  if (!myfile.is_open()) 
    EXC_PRINT_2("unable to open file", filename);

  getline(myfile,line);
  sstr<<line;
  size = 0;
  while(!sstr.eof()) {
    if(!(sstr>>d))
      EXC_PRINT("Incorrect file format");
    size++;
  };
 
  if (size > 2)
    EXC_PRINT_2("format unknown for file",filename);
 
  sstr.clear();
  sstr<<line;
  int j = 1;
  if(!(sstr>>i))
    throw Exc("Incorrect file format");

  if(!(sstr>>d))
    throw Exc("Incorrect file format");

  set(i,d);
  while ( myfile.good() )
    {
      getline (myfile,line);
      if (line.find_first_not_of(' ') == std::string::npos)
	break;
      sstr.clear();
      sstr<<line;

      if(!(sstr>>i))
	EXC_PRINT_2("Incorrect format for file", filename);

      if(!(sstr>>d))
	EXC_PRINT_2("Incorrect format for file", filename);

      set(i,d);
      j++;
    }
  myfile.close();
  return j; 
};

double pmf::avg() const
{
  unsigned int i;
  double avg = 0;

  for (i = 0; i < size; i++) {
    avg += ((i - offset) * elems(i));
  }

  return avg;
}


double pmf::std() const 
{
  double var = pmf::var();

  return sqrt(var);
}

double pmf::var() const
{
   unsigned int i;
   double var = 0;
   double avg = pmf::avg();

  for (i = 0; i < size; i++) {
    var += (pow(((i - offset)-avg),2) * elems(i));
  };

  return var;
}

pmf::pmf(unsigned int sz, unsigned int offs, double epsilon): distr(sz, offs, epsilon), tail(0.0)
{
  elems.setZero();
  return;
}

pmf::~pmf(){};

int pmf::set(int val, double p) throw(Exc)
{
  if (val > (int)size - (int)offset) {
    EXC_PRINT("access out of range");
  }
  
  if (p > 1e-10) {
    if (val > max) {
      max = val;
      //cerr<<"setting max to"<<max<<endl;
    }
    if (val < min) {
      min = val;
      //cerr<<"setting min to"<<min<<endl;
    }
  }

  elems(val + offset) = p;
  //cerr<<"SET: Min: "<<get_min()<<", Max: "<<get_max()<<endl; 
  
  return 1;
}

int pmf::set_samples(int samples)
{
  unsigned int i;

  if (tail > 0) {
    return -1;
  }

  tail = 1.0 / samples;		/* epsilon = 1 / samples */
  for (i = 0; i < size; i++) {
    if (elems(i) > epsilon) {
      elems(i) = elems(i)* samples / (samples + 1);
    }
  }

  return 1;
}

double pmf::sum() const
{
  double p;

  p = tail;
  p+=elems.sum();
  
  return p;
}

pmf::ERR_CODES pmf::check() const
{
  double p;

  p = sum();
  int i;

  for (i = get_min(); i<= get_max(); i++)
    if (get(i) < -epsilon)
      return PMF_NEG;
  if (p < 1.0 - epsilon) {
    cerr<<"warning: pmf::check, sum:"<<p<<endl;
    return PMF_SMALLER_ONE;
  }
  if (p > 1.0 + epsilon) {
    cerr<<"warning: pmf::check, sum:"<<p<<endl;
    return PMF_GREATER_ONE;
  }
    
  return PMF_OK;
}

int pmf::load(const string &filename) throw(Exc)
{
  int j=distr::load(filename);
  ERR_CODES e = check();
  switch(e) {
  case PMF_NEG: 
    EXC_PRINT_2("ill formed pmf: negative value, file", filename);
    break;
  case PMF_SMALLER_ONE:
    EXC_PRINT_2("ill formed pmf: sum smaller than one, file", filename);
    break;
  case PMF_GREATER_ONE:
    EXC_PRINT_2("ill formed pmf: sum greater than one, file", filename);
    break;
  default:;
  };

  return j;
}

pmf * pmf::resample(int q) const {
  double sum;
  int i;
  pmf *clone = new pmf(this->get_size(), this->get_offset()/q);
  if (q!=1) {
    sum = 0;
    for (i = get_min(); i <=(get_max() / q) * q + q; i++) {
      sum += get(i);
      if (i % q == 0) {
	clone->set(i/q, sum);
	sum = 0;
      }
    };
  }
  else
    *clone = *this;
  return clone;
};  

void pmf2cdf(const pmf & p, cdf & c) {
  double sum=0.0;
  c.elems = p.elems;
  c.elems.setZero();
  c.size =p.size;
  c.offset=p.offset;
  c.min = c.size-c.offset;
  c.max = -c.offset;
  for (int i = p.get_min();  i <= p.get_max(); i++) {
    sum += p.get(i);
    c.set(i, sum);
  }
  
  if ( c.get(c.get_max()) < 1.0 -c.get_epsilon() ) {
    cerr<<"PMF2CDF warning: pdf does not sum to 1"<<endl;
    c.set(c.get_max(),1.0);
  }

  c.check();
  return;
}

void cdf2pmf(const cdf & c, pmf & p) {
   double sum=0;
   p.elems = c.elems;
   p.elems.setZero();
   p.size =c.size;
   p.offset=c.offset;
   p.min = p.size-p.offset;
   p.max = -p.offset;
  
   for (int i = c.get_min();  i <= c.get_max(); i++) {
     p.set(i,c.get(i)-sum);
     sum += c.get(i);
   }
   return;
}
