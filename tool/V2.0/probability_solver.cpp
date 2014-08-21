#include "probability_solver.hpp"
#include "task_descriptor.hpp"
#include "exc.hpp"


namespace PrositCore {

  void ResourceReservationProbabilitySolver::register_task(GenericTaskDescriptor * td) {
    if (! (task_descriptor = dynamic_cast<ResourceReservationTaskDescriptor*>(td)) )
      EXC_PRINT_2("Resource reservation solver udes for improper task ", td->get_name());
    reset();
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
};
}

