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
  #include "/usr/local/include/WallarooCppApi/ApiHooks.hpp"
  #include "/usr/local/include/WallarooCppApi/SourceDecoder.hpp"
  #include "/usr/local/include/WallarooCppApi/SinkEncoder.hpp"
  #include "/usr/local/include/WallarooCppApi/PartitionFunction.hpp"
  
  #include "market-spread-cpp.hpp"
  #include "Buffers.hpp"
  #include "Messages.hpp"

  using namespace std;

  extern void printName(wallaroo::StateChange* sc_);
  extern void* getNullPtr();
  extern unsigned long long getWallarooHashValue(wallaroo::StateChange* sc_);
  extern NbboMessage* getNbboMessageFromInput(wallaroo::Data* data_);
  extern void* w_stateful_computation_get_return(void *state_change_repository_helper_, 
						 wallaroo::Data* data_, 
						 void *state_change_);
  extern void* w_state_change_repository_lookup_by_name(void *state_change_repository_helper_, 
							void *state_change_repository_, 
							const char *name_);
  extern wallaroo::StateChange* w_state_change_get_state_change_object(void *state_change_repository_helper_, void *state_change_);

%}


// global stuff
%include "std_string.i"
%include "cpointer.i";
//%include "std_sstream.i"
%feature("director") StateComputation;
%feature("director") StateChange;



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
%include "/usr/local/include/WallarooCppApi/ApiHooks.hpp"
%include "/usr/local/include/WallarooCppApi/SourceDecoder.hpp"
%include "/usr/local/include/WallarooCppApi/SinkEncoder.hpp"
%include "/usr/local/include/WallarooCppApi/PartitionFunction.hpp"


 // this project
%include "Buffers.hpp"
%include "Messages.hpp"
%include "market-spread-cpp.hpp"

extern void printName(wallaroo::StateChange* sc_);
extern void* getNullPtr();
extern unsigned long long getWallarooHashValue(wallaroo::StateChange* sc_);
extern NbboMessage* getNbboMessageFromInput(wallaroo::Data* data_);
extern void* w_stateful_computation_get_return(void *state_change_repository_helper_, 
					       wallaroo::Data* data_, 
					       void *state_change_);
extern void* w_state_change_repository_lookup_by_name(void *state_change_repository_helper_, 
						      void *state_change_repository_, 
						      const char *name_);
extern wallaroo::StateChange* w_state_change_get_state_change_object(void *state_change_repository_helper_, void *state_change_);


