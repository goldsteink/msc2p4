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

extern "C"
{
  extern wallaroo::SourceDecoder *get_order_source_decoder();
  extern wallaroo::SourceDecoder *get_nbbo_source_decoder();
  extern wallaroo::Serializable *w_user_serializable_deserialize(char *bytes_, size_t sz_);
}

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

// Buffer

class Reader
{
private:
  unsigned char *_ptr;
public:
  Reader(unsigned char *bytes_);
  uint8_t u8_be();
  uint16_t u16_be();
  uint32_t u32_be();
  uint64_t u64_be();
  double ms_double();
  string *ms_string(size_t sz_);
};

class Writer
{
private:
  unsigned char *_ptr;
public:
  Writer(unsigned char *bytes_);
  void u8_be(uint8_t value_);
  void u16_be(uint16_t value_);
  void u32_be(uint32_t value_);
  void u64_be(uint64_t value_);
  void ms_double(double value_);
  void ms_string(string *str);
};

class OrderSourceDecoder: public wallaroo::SourceDecoder
{
public:
  virtual size_t header_length();
  virtual size_t payload_length(char *bytes);
  virtual wallaroo::Data *decode(char *bytes, size_t sz_);
};

class NbboSourceDecoder: public wallaroo::SourceDecoder
{
public:
  virtual size_t header_length();
  virtual size_t payload_length(char *bytes);
  virtual wallaroo::Data *decode(char *bytes, size_t sz_);
};

class PartitionableMessage: public wallaroo::Data
{
public:
  virtual uint64_t get_partition() = 0;
};

class OrderMessage: public PartitionableMessage
{
private:
  SideType _side;
  uint32_t _account;
  string _order_id;
  string _symbol;
  double _order_quantity;
  double _price;
  string _transact_time;
public:
  OrderMessage(SideType side_, uint32_t account_, string& order_id_, string& symbol_, double order_quantity_, double price_, string& transact_time_);
  OrderMessage(OrderMessage *that_);
  virtual uint64_t get_partition();

  SideType side() { return _side; }
  uint32_t account() { return _account; }
  string *order_id() { return &_order_id; }
  string *symbol() { return &_symbol; }
  double order_quantity() { return _order_quantity; }
  double price() { return _price; }
};

class NbboMessage: public PartitionableMessage
{
private:
  string _symbol;
  string _transit_time;
  double _bid_price;
  double _offer_price;
public:
  NbboMessage(string& symbol_, string& transit_time_, double bid_price_, double offer_price_);
  virtual uint64_t get_partition();

  double bid_price() { return _bid_price; }
  double offer_price() { return _offer_price; }
  double mid() {return (_bid_price + _offer_price) / 2.0; }
};

class SymbolData: public wallaroo::State
{
public:
  bool should_reject_trades;
  double last_bid;
  double last_offer;

  SymbolData(): should_reject_trades(true), last_bid(0), last_offer(0) {}
};

class SymbolDataStateChange: public wallaroo::StateChange
{
private:
  bool _should_reject_trades;
  double _last_bid;
  double _last_offer;

public:
  SymbolDataStateChange(uint64_t id_);

  virtual const char *name();
  virtual void apply(wallaroo::State *state_);

  virtual size_t get_log_entry_size() { return 0; }
  virtual void to_log_entry(char *bytes_) { }
  virtual size_t get_log_entry_size_header_size() { return 0; }
  virtual size_t read_log_entry_size_header(char *bytes_) { return 0; }
  virtual bool read_log_entry(char *bytes_) { return true; }

  void update(bool should_reject_trades_, double last_bid_, double last_offer_);
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

class UpdateNbboNoOutput: public wallaroo::StateComputation
{
  const char *name() { return "Update NBBO, no output"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);

  virtual size_t get_number_of_state_change_builders() { return 1;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return new SymbolDataStateChangeBuilder(); }
};

class UpdateNbbo: public wallaroo::StateComputation
{
  const char *name() { return "Update NBBO"; }
  void *compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none);

  virtual size_t get_number_of_state_change_builders() { return 1;}
  virtual wallaroo::StateChangeBuilder *get_state_change_builder(size_t idx_) { return new SymbolDataStateChangeBuilder(); }
};


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
