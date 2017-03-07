%module(directors="1") MSPY
%{
  #include "/usr/local/include/WallarooCppApi/Hashable.hpp"
  #include "/usr/local/include/WallarooCppApi/Serializable.hpp"
  #include "/usr/local/include/WallarooCppApi/ManagedObject.hpp"
  #include "/usr/local/include/WallarooCppApi/Data.hpp"
  #include "/usr/local/include/WallarooCppApi/State.hpp"
  #include "/usr/local/include/WallarooCppApi/StateChange.hpp"
  #include "/usr/local/include/WallarooCppApi/StateChangeRepository.hpp"
  #include "/usr/local/include/WallarooCppApi/StateChangeBuilder.hpp"
  #include "/usr/local/include/WallarooCppApi/Computation.hpp"
  #include "Base.hpp"
  extern void printKevin();
%}


// global stuff
%include "std_string.i"
%include "cpointer.i";
%feature("director") StateComputation;


// wallaroo stuff
%include "/usr/local/include/WallarooCppApi/Hashable.hpp"
%include "/usr/local/include/WallarooCppApi/Serializable.hpp"
%include "/usr/local/include/WallarooCppApi/ManagedObject.hpp"
%include "/usr/local/include/WallarooCppApi/Data.hpp"
%include "/usr/local/include/WallarooCppApi/State.hpp"
%include "/usr/local/include/WallarooCppApi/StateChange.hpp"
%include "/usr/local/include/WallarooCppApi/StateChangeRepository.hpp"
%include "/usr/local/include/WallarooCppApi/StateChangeBuilder.hpp"
%include "/usr/local/include/WallarooCppApi/Computation.hpp"


 // this project
%include "Base.hpp"

extern void printKevin();
extern void *w_stateful_computation_get_return(void *state_change_repository_helper_, wallaroo::Data* data_, void *state_change_);
extern void *w_state_change_repository_lookup_by_name(void *state_change_repository_helper_, void *state_change_repository_, const char *name_);
extern wallaroo::StateChange *w_state_change_get_state_change_object(void *state_change_repository_helper_, void *state_change_);

