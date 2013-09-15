/*
 * Header file for the exception mechanism
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef EXC_HPP
#define EXC_HPP
#include<iostream>
#include<string>
using namespace std;
class Exc {
  string msg;
public:
  Exc(const char *ms): msg(ms){};
  Exc(string s): msg(s){};
  virtual void what() 
  {
    cerr<<msg<<endl;
  };
};

#define EXC_PRINT(msg)\
 do {\
 cerr<<"file:"<<__FILE__<<", line:"<<__LINE__<<", function: "<<__PRETTY_FUNCTION__<<". "<<msg<<endl; \
 throw Exc(string(__PRETTY_FUNCTION__)+string(": ")+string(msg));\
} while(0)

#define EXC_PRINT_2(msg,msg1)\
 do {\
 cerr<<"file:"<<__FILE__<<", line:"<<__LINE__<<", function: "<<__PRETTY_FUNCTION__<<". "<<msg<<" "<<msg1<<endl; \
 throw Exc(string(__PRETTY_FUNCTION__)+string(": ")+string(msg)+string(" ")+string(msg1));\
} while(0)
    
#endif
