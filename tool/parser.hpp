/*
 * Classes and function for XML parsing
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _XMLPARSER_HPP
#define _XMLPARSER_HPP
#include <tinyxml2.h>
#include <iostream>
#include"exc.hpp"
#include "task_factory.hpp"
#include "qos_factory.hpp"
using namespace std;
using namespace tinyxml2;
namespace XMLParser {
  enum ACTIONS {OPTIMISE, SOLVE};
  enum OptType {NO_OPT,INFINITY_NORM};
  class Parser {
  private:
    XMLDocument * doc;
    ACTIONS act;
    OptType ot;
    void optimisation_parse(XMLElement * el) throw (Exc);
    void analysis_parse(XMLElement * el) throw(Exc);
    void task_list_parse(XMLElement * el) throw(Exc);
    void verbose_parse(XMLElement * el) throw (Exc);
    double optim_eps;
    double total_bandwidth;
    bool verbose;
  public:
    static GenericTaskDescriptor * task_parse(XMLElement * task) throw(Exc);
    static auto_ptr<pmf> distr_parse(XMLElement * task) throw(Exc);
    static auto_ptr<QoSFun> qosfun_parse(XMLElement * qosfunElement) throw(Exc);
    Parser(const char * fname) throw(Exc): ot(NO_OPT),optim_eps(1e-6),total_bandwidth(1.0), verbose(false) {
      doc = new XMLDocument();
      
      if(doc->LoadFile(fname) !=  XML_NO_ERROR) {
	delete doc;
	doc = 0;
	EXC_PRINT("cannot open file");
      };
      ot = NO_OPT;
    };
    ACTIONS get_act() const {return act;};
    bool set_verbose(bool verbosed) {
      bool current = verbose;
      verbose = verbosed;
      return current;
    };
    bool get_verbose() const { return verbose;};
    OptType get_optimisation_type() const {
      return ot;
    };
    double get_optimisation_epsilon() const {
      return optim_eps;
    };
    double get_total_bandwidth() const {
      return total_bandwidth;
    };

    //This one parses the file and creates the entries in the task factory
    ACTIONS parse() throw (Exc);
    ~Parser() {
      if (doc)
	delete doc;
    };
  };
};
#endif
