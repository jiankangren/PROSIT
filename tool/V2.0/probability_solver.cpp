#include "probability_solver.hpp"
#include "task_descriptor.hpp"
#include "exc.hpp"


namespace PrositCore {

  void ResourceReservationProbabilitySolver::register_task(GenericTaskDescriptor * td) {
    if (! (task_descriptor = dynamic_cast<ResourceReservationTaskDescriptor*>(td)) )
      EXC_PRINT_2("Resource reservation solver udes for improper task ", td->get_name());
    

}
