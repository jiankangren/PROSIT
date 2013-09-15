#include <iostream>
#include "auxiliary_func.hpp"
#include "matrix_io.hpp"
#include"exc.hpp"
#include "smc_func.hpp"

using namespace std;

int main()
{
  try {
    MatrixXd A0,A1,A2,B0,G,R,U;
    long long t1, t2;
    square_matrix_input("Am1.mat" ,A0);
    square_matrix_input("A0.mat" ,A1);
    square_matrix_input("A1.mat" ,A2);
    square_matrix_input("B0.mat",B0);
    //cerr<<"A0= "<<A0<<endl<<endl;
    //cerr<<(A0+A1+A2).rowwise().sum()<<endl<<endl;
    struct qbd_cr_parms parms;
    parms.shift =false;
    parms.verbose=false;
    parms.max_iter=50;
    t1=my_get_time();
    qbd_cr(A0, A1, A2, G, R, U, parms);
    t2=my_get_time();
    cerr<<t2-t1<<endl;
    //cout<<"G= "<<G<<endl<<endl;
    //cout<<"R= "<<R<<endl<<endl;
    //cout<<"U= "<<U<<endl<<endl;
    RowVectorXd pi0;
    
    qbd_compute_pi0(R,B0,A0,pi0);

    cout<<pi0<<endl<<endl;
    
    struct qbd_latouche_parms p1;
    p1.max_iter=100;
    p1.epsilon=1e-14;
    p1.verbose=false;
    t1=my_get_time();
    qbd_latouche(A0, A1, A2, R, p1);
    t2=my_get_time();
    cerr<<t2-t1<<endl;
    cout<<R<<endl<<endl;
    
    qbd_compute_pi0(R,B0,A0,pi0);

    cout<<pi0<<endl<<endl;
    VectorXd prova;
    column_vector_input("v.mat",prova);
    cout<<prova<<endl;
    

  } catch (Exc & e) {
    e.what();
    return -1;
  };
  return 1;

}
