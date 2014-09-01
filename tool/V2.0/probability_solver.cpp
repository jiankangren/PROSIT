#include "probability_solver.hpp"
#include "task_descriptor.hpp"
#include "exc.hpp"


namespace PrositCore {
  void ProbabilitySolver::solve() {
    if ( !check_list() ) {
      if (linked_flag && (verbose_flag)) 
	cerr<<"Probability solver will not execute."<<endl;
      return;
    };
    
    pre_process();
    if (verbose_flag)       
      cout<<"Preprocessing phase completed." <<endl;
          
    pre_process_done = true;
    apply_algorithm();
    if(verbose_flag)
      cout<<"Solver computation completed"<<endl;
    solved = true;
    post_process();
    if(verbose_flag)
      cout<<"Post-processing completed"<<endl;
    post_process_done = true;
    fill_in_probability_map();
  };
  
  void ResourceReservationProbabilitySolver::register_task(GenericTaskDescriptor * td) {
    if (! (task_descriptor = dynamic_cast<ResourceReservationTaskDescriptor*>(td)) )
      EXC_PRINT_2("Resource reservation solver used for improper task ", td->get_name());
    reset();
    verbose_flag = task_descriptor->get_verbose();
    linked_flag = true;
    if( verbose_flag)
      cout<<"Task "<<task_descriptor->get_name()<<" linked to the solver"<<endl;
  };

  void FixedPriorityProbabilitySolver::register_task(GenericTaskDescriptor * td) {
    FixedPriorityTaskDescriptor * task_descriptor;
    std::map<unsigned int, std::list<FixedPriorityTaskDescriptor* > >::iterator it;
    if (! (task_descriptor = dynamic_cast<FixedPriorityTaskDescriptor*>(td)) )
      EXC_PRINT_2("Resource reservation solver udes for improper task ", td->get_name());
    
    if ( (it=task_set.find(task_descriptor->get_priority())) == task_set.end() ) 
      {
	/* not task at current level of priority */
	it = task_set.insert(pair< unsigned int, std::list<FixedPriorityTaskDescriptor*> >(task_descriptor->get_priority(), std::list<FixedPriorityTaskDescriptor*>() )).first;
      };
    (*it).second.push_back(task_descriptor);
    reset();
    //verbose flag equal to true is not overriden by one task for which it is equal to false.
    if (!verbose_flag)
      verbose_flag = task_descriptor->get_verbose();
    linked_flag = true;
    if( verbose_flag)
      cout<<"Task "<<task_descriptor->get_name()<<" linked to the solver"<<endl;
  };
}


