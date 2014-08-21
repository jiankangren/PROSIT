#ifndef PROBABILITY_SOLVER_HPP
#define PROBABILITY_SOLVER_HPP

#include "prosit_types.hpp"

///@file probability_solver.hpp
///
///This file contains the abstract definitions for
///probability solvers. In essence, a probability solver wraps
///the different algorithms used to compute probabilistic deadlines.
///Specific interfaces are also provided for resource reservations and
///fixed priority. A solver for resource reservation is one to one linked
///with a task, whereas a solver for fixed priority considers sets of tasks.

namespace PrositCore{
  ///@brief root of the hiearachy. Essentially this is class defines an interface.
  class ProbabilitySolver {
  public:
    ///@brief Computes the steady state probability of meeting a set of deadlines
    ///
    ///@param pm refernce to the deadline map
    ///@param deadline_step basic deadline step
    virtual void solve(DeadlineProbabilityMap & pm, unsigned int deadline_step) = 0;
    
    ///@brief Adds the task to the set of tasks the solver applies to
    ///
    ///This method is pure virtual so that each solver can verify the appropriateness
    ///of the task. The call to this method resets previous computations.
    ///@param td descriptor of the task
    virtual void register_task(GenericTaskDescriptor * td) = 0;
    virtual ~ProbabilitySolver() {};
  };

  ///@brief Interface for generic solvers that apply to a resource reservation task.
  class ResourceReservationProbabilitySolver : public ProbabilitySolver {
  protected:
    ResourceReservationTaskDescriptor * task_descriptor; /*!< Descriptor of only task managed by this type of solver >*/
  public:
     ///@brief Associates the solver with one task descriptor
    ///
    ///A resource reservation solver applies to a resource reservation task descriptor.
    ///@param td descriptor of the task. An exception is thrown id 
    virtual void register_task(GenericTaskDescriptor * td);
  };
  
}
#endif
