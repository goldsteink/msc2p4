

#ifndef __BASE_HPP__
#define __BASE_HPP__

#include <WallarooCppApi/Data.hpp>
#include <WallarooCppApi/ApiHooks.hpp>
//using namespace wallaroo;

enum FixType
{
  Order = 1,
  Nbbo = 2
};

enum SideType
{
  Buy = 1,
  Sell = 2
};


class Base
{
public:
  Base();
  virtual ~Base();
};



class CheckOrder: public wallaroo::StateComputation
{
  virtual const char *name() { return "Check Order against NBBO"; }
  virtual size_t get_number_of_state_change_builders() { return 0;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return NULL; }
  virtual void *compute(wallaroo::Data *input_, 
		wallaroo::StateChangeRepository *state_change_repository_, 
		void *state_change_repository_helper_, 
		wallaroo::State *state_, 
		void *none);
};



#endif // __BASE_HPP__
