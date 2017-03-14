#ifndef __MARKET_SPREAD_CPP_H__
#define __MARKET_SPREAD_CPP_H__

#include "WallarooCppApi/Data.hpp"
#include "WallarooCppApi/Computation.hpp"
#include "WallarooCppApi/SourceDecoder.hpp"
#include "WallarooCppApi/PartitionFunction.hpp"
#include "WallarooCppApi/State.hpp"
#include "WallarooCppApi/StateChange.hpp"
#include "WallarooCppApi/StateChangeBuilder.hpp"
#include "WallarooCppApi/SinkEncoder.hpp"

#include "Messages.hpp"

extern "C"
{
  extern wallaroo::SourceDecoder *get_order_source_decoder();
  extern wallaroo::SourceDecoder *get_nbbo_source_decoder();
  extern wallaroo::Serializable *w_user_serializable_deserialize(char *bytes_, size_t sz_);
}

//--------------------------------------------------------------------
class SymbolData: public wallaroo::State
{
public:
  bool should_reject_trades;
  double last_bid;
  double last_offer;

  SymbolData(): should_reject_trades(true), last_bid(0), last_offer(0) {}
};



/*
 * KAGR
 */
class SymbolDataStateChange: public wallaroo::StateChange
{
private:
  bool _should_reject_trades;
  double _last_bid;
  double _last_offer;

public:
  SymbolDataStateChange();
  SymbolDataStateChange(uint64_t id_);
  virtual ~SymbolDataStateChange(){}

public:
  void set_should_reject_trades(bool srt_) { _should_reject_trades = srt_; }
  void set_last_bid(double lastb_) { _last_bid=lastb_; }
  void set_last_offer(double lasto_) { _last_offer=lasto_; }
  double get_last_bid() { return _last_bid; }
  double get_last_offer() { return _last_offer; }
  bool get_should_reject_trades(){ return _should_reject_trades; }

public:
  virtual const char *name();
  virtual void apply(wallaroo::State *state_);

  virtual size_t get_log_entry_size() { return 0; }
  virtual void to_log_entry(char *bytes_) { }
  virtual size_t get_log_entry_size_header_size() { return 0; }
  virtual size_t read_log_entry_size_header(char *bytes_) { return 0; }
  virtual bool read_log_entry(char *bytes_) { return true; }

  virtual void update(bool should_reject_trades_, double last_bid_, double last_offer_);

};



class SymbolDataStateChangeBuilder: public wallaroo::StateChangeBuilder
{
public:
  virtual wallaroo::StateChange *build(uint64_t id_);
};

class SymbolPartitionFunction: public wallaroo::PartitionFunctionU64
{
public:
  virtual uint64_t partition(wallaroo::Data *data_);
};

class CheckOrderNoUpdateNoOutput: public wallaroo::StateComputation
{
  const char *name() { return "Check Order against NBBO, no update, no output"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);
  virtual size_t get_number_of_state_change_builders() { return 0;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return NULL; }
};

class CheckOrderNoOutput: public wallaroo::StateComputation
{
  const char *name() { return "Check Order against NBBO, no output"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);
  virtual size_t get_number_of_state_change_builders() { return 0;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return NULL; }
};

class CheckOrder: public wallaroo::StateComputation
{
  const char *name() { return "Check Order against NBBO"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);
  virtual size_t get_number_of_state_change_builders() { return 0;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return NULL; }
};

class UpdateNbboNoUpdateNoOutput: public wallaroo::StateComputation
{
  const char *name() { return "Update NBBO, no update, no output"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);

  virtual size_t get_number_of_state_change_builders() { return 0;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return NULL; }
};

/*
 KAGR
class UpdateNbboNoOutput: public wallaroo::StateComputation
{
  const char *name() { return "Update NBBO, no output"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);

  virtual size_t get_number_of_state_change_builders() { return 1;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return new SymbolDataStateChangeBuilder(); }
};

/* KAGR
class UpdateNbbo: public wallaroo::StateComputation
{
  const char *name() { return "Update NBBO"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);

  virtual size_t get_number_of_state_change_builders() { return 1;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return new SymbolDataStateChangeBuilder(); }
};
*/

class OrderResult: public wallaroo::EncodableData
{
public:
  OrderMessage *order_message;
  double bid;
  double offer;
  uint64_t timestamp;
  OrderResult(OrderMessage *order_message_, double bid_, double offer_, uint64_t timestamp_);
  ~OrderResult();

  virtual size_t encode_get_size();
  virtual void encode(char *bytes);
};

class OrderResultSinkEncoder: public wallaroo::SinkEncoder
{
public:
  virtual size_t get_size(wallaroo::EncodableData *data);
  virtual void encode(wallaroo::EncodableData *data, char *bytes);
};

#endif // __MARKET_SPREAD_CPP_H__
