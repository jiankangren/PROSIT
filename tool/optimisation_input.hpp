#ifndef OPTMISATION_HPP
#define OPTMISATION_HPP
#include "exc.hpp"
#include "probabilistic_optimisation.hpp"
#include <vector>
//FILE FORMAT
//[METHOD]
//CYCLIC||ANALYTIC
//[TASKS]
//task_name task_type pmf_file_name server_period task_period qos_min qos_max weight

void load_optimisation_file(const char *fname) throw (Exc);

#endif
