#ifndef LATOUCHE_SOLVER_HPP
#define LATOUCHE_SOLVER_HPP

///@file qbd_rr_solver.hpp
///
///Family of solvers for resource reservation tasks based on algorithms for numeric solution of
///quasi-brith death processes.
///In particular: 
///ResourceReservationLatoucheSolver applies the Latouche Algorithm (1) for the solution. It applies to both periodic and aperiodic tasks
///(1) G. Latouche and V. Ramaswami. A logarithmic reduction algorithm for quasi-birth-and-death processes. J. Appl. Probab., 30:650--674, 1993.

#include "probability_solver.hpp"

#include <Eigen/Dense>

#include "auxiliary_func.hpp"
//#include "smc_func.hpp"
#include <memory>


namespace PrositCore {
  class QBDResourceReservationProbabilitySolver: public ResourceReservationProbabilitySolver {
  protected:
    Eigen::MatrixXd B0, A0, A1, A2, R;
    unsigned int granularity; /*!< Granularity for resampling computation time */
    
    bool compress_flag; /*!< Should we compress all the states smaller than the minimum interarrival time ? */
    ///@brief Generates matrices used by QBD methods.
    void generate_matrices();
  public:
    ///@brief Default constructor
    ///
    ///@param granularity grnaularity for resampling the distribution of computation time
    QBDResourceReservationProbabilitySolver(unsigned int grand):  ResourceReservationProbabilitySolver(), B0(), A0(), A1(), A2(), granularity(grand), compress_flag(false) {};
    
    ///@brief Sets granularity to a desired value
    ///
    ///The distribution of computation time is resampled at the graularity parameter
    ///to reduce the size of the system maintaining a conservative compuation.
    ///@param grand desired value
    void set_granularity(unsigned int grand);
    
    ///@brief Set compress flag
    ///
    ///The system produces a model where all states corresponding to states smaller than
    ///the minimum interarrival time are compressed into a single state.
    void set_compress_flag() {
      compress_flag = true;
      reset();
    };
    virtual ~QBDResourceReservationProbabilitySolver() {};
   

  };
  class ResourceReservationLatoucheSolver : public QBDResourceReservationProbabilitySolver {
    ///@brief Default constructor
    ///
    ///Desired value of the granularity for resampling the distribution of computation time
    ResourceReservationLatoucheSolver(unsigned int grand):  QBDResourceReservationProbabilitySolver(grand) {};
    
    ///@brief Destructor
    virtual ~ResourceReservationLatoucheSolver() {};
    
    ///@brief Computation of the probability.
    virtual void solve();


  };
}


#endif
