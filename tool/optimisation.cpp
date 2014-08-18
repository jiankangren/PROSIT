#include "optimisation.hpp"
#include <algorithm>
#include <cassert>


bool GenericTaskDescriptor::identify_bounds(double q_min,double q_max) throw(Exc) {
  bool res1, res2;  

  if(q_min > q_max)
    EXC_PRINT_2("Bad bounds made identify_bounds fail for task",name);
  res1 = inv_QoS(q_min,Qmin,true);
  res2 = inv_QoS(q_max,Qmax,false);
  
  if(Qmin>Qmax) 
    return false;

  QoSmin = QoS(Qmin);

  QoSmax = QoS(Qmax);
  
  
  bounds_inited = true;
  return res1 && res2;
}

bool GenericTaskDescriptor::identify_target_bounds() throw (Exc) {
  return identify_bounds(QoSmin_target, QoSmax_target);
}


GenericBudgetOptimiser::GenericBudgetOptimiser(int task_num, double epsd):tasks(),state(NOT_EXECUTED),max_num(task_num), number(0), optimum(-1e38), eps(epsd) {
  tasks.clear();
};
GenericBudgetOptimiser::GenericBudgetOptimiser(vector<GenericTaskDescriptor*> v, double epsd): tasks(v),state(NOT_EXECUTED),max_num(typical_size), number(0), optimum(-1e38), eps(epsd) {
  number = v.size();
};

int GenericBudgetOptimiser::insert_task(GenericTaskDescriptor * t, bool re_optimise) throw(Exc) {
  bool found = false;
  int pos = 0;
  if (number == max_num -1)
    EXC_PRINT("max size exceeded");

  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; !found && (it != tasks.end()); ++it)
    if ((*it)->get_name() == t->get_name()) {
      found = true;
    } else
      pos++;
  if (found)     
    cerr<<"Warning: GenericBudgetOptimiser::insert_task insertion of a task alredy present"<<endl;
  else
    tasks.push_back(t);
  if (re_optimise)
    optimise();
  number++;
  return pos;
};


GenericTaskDescriptor * GenericBudgetOptimiser::remove_task(unsigned int i,bool re_optimise) throw(Exc) {
  GenericTaskDescriptor * v;
  if (i>=tasks.size())
    EXC_PRINT("position out of range");

  if (number == 0)
    EXC_PRINT("removal from empty structure");

  v = tasks[i];
  vector<GenericTaskDescriptor*>::iterator p = tasks.begin()+i;

  tasks.erase(p);
  if (re_optimise)
    optimise();
  number--;
  return v;
};

GenericTaskDescriptor * GenericBudgetOptimiser::remove_task(const string & name, bool re_optimise) throw(Exc) {
  bool found = false;
  GenericTaskDescriptor * t=0;
  if (number == 0)
    EXC_PRINT("removal from empty structure");

  vector<GenericTaskDescriptor*>::iterator it;
  for (tasks.begin() ; !found && (it != tasks.end()); ++it)
    if (*it != 0) 
      if (name == (*it)->get_name()) {
	found = true;
	t=*it;
	*it = 0;
      } 
  if (!found) {
    cerr<<"WARNING: BudgetOptimiser::remove_task task "<<name<<" not found";
  }
  tasks.erase(it);
  if (re_optimise)
    optimise();
  number --;
  return t;
};

int GenericBudgetOptimiser::get_budget(unsigned int i) const throw(Exc) {
  if (i>=tasks.size())
    EXC_PRINT("access out of range");
  
  return tasks[i]->get_budget();
}

double GenericBudgetOptimiser::get_bandwidth(unsigned int i) const throw(Exc) {
  if (i>=tasks.size())
    EXC_PRINT("access out of range");
  
  return double(tasks[i]->get_budget())/double(tasks[i]->get_server_period());
}



double GenericBudgetOptimiser::get_bandwidth(const string & name) const throw(Exc) {
  bool found = false;
  double bandwidth=0;
  for (vector<GenericTaskDescriptor*>::const_iterator it = tasks.begin() ; !found && (it != tasks.end()); ++it)
    if (*it != 0) 
      if (name == (*it)->get_name()) {
	found = true;
	bandwidth=double((*it)->get_budget())/double((*it)->get_server_period());
      } 
      
  if (!found) {
    EXC_PRINT_2("task not found",name);
  };
  
  return bandwidth;
};


int GenericBudgetOptimiser::get_budget(const string & name) const throw(Exc) {
  bool found = false;
  int budget;
  for (vector<GenericTaskDescriptor*>::const_iterator it = tasks.begin() ; !found && (it != tasks.end()); ++it)
    if (*it != 0) 
      if (name == (*it)->get_name()) {
	found = true;
	budget=(*it)->get_budget();
      } 
      
  if (!found) {
    EXC_PRINT_2("task not found",name);
  };
  
  return budget;
};

double GenericBudgetOptimiser::optimise(const vector< pair<double, double> > & bounds) throw (Exc){
  unsigned int i=0;
  if (bounds.size() !=number)
    EXC_PRINT("wrong size of the bounds vector");

  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
    if (*it != 0) {
      assert (i<number);
      if (!(*it)->identify_bounds(bounds[i].first,bounds[i].second))
	EXC_PRINT_2("cannot set bounds for task ",(*it)->get_name());
      i++;
    }
  }
  return optimise();
}
void GenericBudgetOptimiser::init_target_bounds() throw(Exc) {
   for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
     if (*it != 0) {
      if (!(*it)->get_bounds_inited()) {
	if(verbose)
	  cerr<<"Initialising bounds for task "<<(*it)->get_name()<<endl;
	if(!(*it)->identify_target_bounds())
	  EXC_PRINT_2("cannot set bounds for task ",(*it)->get_name());
      };
     }
   }
}

double InfinityNormBudgetOptimiser::eval() {
  double inf_norm_1 = 1e38;
  
  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
    if (*it != 0) {    
      inf_norm_1 = min((*it)->QoS((*it)->get_budget()), inf_norm_1);
    };
  };
  return inf_norm_1;
}
double InfinityNormBudgetOptimiser::optimise() {
  vector<int> working_budget(number);
  vector<bool> checked(number);

  double inf_norm_1 = 1e38, inf_norm_2 = 1e38, inf_norm=0.0;
  unsigned int i = 0;
  
  double Btot = 0;

  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
    if (*it != 0) {
      if (!(*it)->get_bounds_inited()) {
	if(verbose)
	  cerr<<"Initialising bounds for task "<<(*it)->get_name()<<endl;
	(*it)->init_bounds();
      };
      (*it)->set_verbose(verbose);
      inf_norm_1 = min((*it)->get_qosmax(), inf_norm_1);
      inf_norm_2 = min((*it)->get_qosmin(), inf_norm_2);
      Btot += double((*it)->get_qmin())/double((*it)->get_server_period());
    };
  };
  if (Btot > 1.0) {
    state = BAD_LOWER_BOUNDS;
    return -1e38;
  };
  Btot = 0;
  i = 0;
  double Breserved=0.0;
  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) 
    if (*it != 0) {
      assert(i<number);
      assert((*it)->inv_QoS(inf_norm_1, working_budget[i],true));
      if (working_budget[i]< (*it)->get_qmin()) {
	working_budget[i]= (*it)->get_qmin();
	checked[i]=true;
	Breserved += double(working_budget[i])/double((*it)->get_server_period());
      } else
	checked[i]=false;
      working_budget[i]=max((*it)->get_qmin(),working_budget[i]);
      Btot+=double(working_budget[i])/double((*it)->get_server_period());
      i++;
    }
  int counter = 0;
  if (Btot > 1.0) {
    while ( inf_norm_1 - inf_norm_2> GenericBudgetOptimiser::eps) {
      inf_norm = (inf_norm_2 +  inf_norm_1)/2;
      i = 0;
      Btot = 0;
      if(verbose && (counter%100))
	cerr<<"Iteration #: "<<counter<<" reached, "<<"difference between the bounds of the norms: "<<inf_norm_1-inf_norm_2<<endl;
      for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
	if (!checked[i]) {
	  assert((*it)->inv_QoS(inf_norm, working_budget[i],false));
	  if (working_budget[i]< (*it)->get_qmin()) {
	    working_budget[i]= (*it)->get_qmin();
	    checked[i]=true;
	    Breserved += double(working_budget[i])/double((*it)->get_server_period());
	  }
	  Btot+=double(working_budget[i])/double((*it)->get_server_period());
	};
	i++;
      }
      if (Btot+Breserved < 1.0)
	inf_norm_2 = inf_norm;
      else
	inf_norm_1 = inf_norm;
      counter ++;
    } 
  }
  else
    inf_norm = inf_norm_1;
  i = 0;
  for (vector<GenericTaskDescriptor*>::iterator it = tasks.begin() ; (it != tasks.end()); ++it) {
    if (*it != 0) {
      (*it)->set_budget(working_budget[i]);
      inf_norm = min(inf_norm, (*it)->QoS(working_budget[i]));
      cerr<<"Setting budget to "<<working_budget[i]<< " with QoS "<<(*it)->QoS(working_budget[i])<<endl;
      if ((working_budget[i]<(*it)->get_qmin())||(working_budget[i]>(*it)->get_qmax())) {
	state=NO_SOLUTION;
	return -1e38;
      };
      i++;
    }
  }
  state = OK;
  optimum = inf_norm;
  return inf_norm;
}
