#include "PyModule.hpp"

#include "stdinc.hpp"
#include "Buffers.hpp"
#include "Messages.hpp"
#include "market-spread-cpp.hpp"
#include "WallarooCppApi/ApiHooks.hpp"


#include <iostream>
#include <cstring>






OrderMessage::OrderMessage(SideType side_, uint32_t account_,
                           string& order_id_, string& symbol_,
                           double order_quantity_, double price_,
                           string& transact_time_):
  _side(side_), _account(account_), _order_id(order_id_),
  _symbol(symbol_), _order_quantity(order_quantity_), _price(price_),
  _transact_time(transact_time_)
{
}

OrderMessage::OrderMessage(OrderMessage *that_):
  _side(that_->_side), _account(that_->_account), _order_id(that_->_order_id),
  _symbol(that_->_symbol), _order_quantity(that_->_order_quantity), _price(that_->_price),
  _transact_time(that_->_transact_time)
{
}

uint64_t partition_from_symbol(string& symbol_)
{
  uint64_t ret = 0;
  for (size_t i = 0; i < symbol_.size(); i++)
  {
    ret += ((unsigned char)symbol_[i]) << (i * 8);
  }

  return ret;
}

uint64_t OrderMessage::get_partition()
{
  return partition_from_symbol(_symbol);
}

NbboMessage::NbboMessage(string& symbol_, string& transit_time_, double bid_price_, double offer_price_):
  _symbol(symbol_), _transit_time(transit_time_), _bid_price(bid_price_), _offer_price(offer_price_)
{
}

uint64_t NbboMessage::get_partition()
{
  return partition_from_symbol(_symbol);
}


wallaroo::StateChange* SymbolDataStateChangeBuilder::build(uint64_t id_)
{
  PyModule& pyMod = PyModule::getInstance();
  stringstream args;
  args<<id_;
  debugPrintFunction(STDMSG, __PRETTY_FUNCTION__, __LINE__);
  wallaroo::StateChange* sc = pyMod.getDerivedPythonClass<StateChange*>("buildSymbolDataStateChange", args.str());
  cout << YEL_STA << "hash: " << sc->hash() << YEL_END <<  endl;
  
  return sc;
}

SymbolDataStateChange::SymbolDataStateChange():
  StateChange((uint64_t)(this)),
  _should_reject_trades(false), 
  _last_bid(0), 
  _last_offer(0)
{
  debugPrintFunction(STDMSG, __PRETTY_FUNCTION__, __LINE__);
}


SymbolDataStateChange::SymbolDataStateChange(uint64_t id_): 
  StateChange(id_), 
  _should_reject_trades(false), 
  _last_bid(0), 
  _last_offer(0)
{
  debugPrintFunction(STDMSG, __PRETTY_FUNCTION__, __LINE__);
  cout << YEL_STA << STDMSG << "SymbolDataStateChange::SymbolDataStateChange(" << id_ << ")" << YEL_END << endl;
}

const char *SymbolDataStateChange::name()
{
  return "symbol data state change";
}

void SymbolDataStateChange::apply(wallaroo::State *state)
{
#ifdef DEBUG
  NOTICE("application of the symbol!");
#endif
  SymbolData *symbol_data = (SymbolData *) state;
  symbol_data->should_reject_trades = _should_reject_trades;
  symbol_data->last_bid = _last_bid;
  symbol_data->last_offer = _last_offer;
}

void SymbolDataStateChange::update(bool should_reject_trades_, double last_bid_, double last_offer_)
{
  debugPrintFunction(STDMSG, __PRETTY_FUNCTION__, __LINE__);

#ifdef DEBUG
  NOTICE("update from SymbolDataStateChange");
#endif
  _should_reject_trades = should_reject_trades_;
  _last_bid = last_bid_;
  _last_offer = last_offer_;
}


uint64_t SymbolPartitionFunction::partition(wallaroo::Data *data_)
{
  PartitionableMessage *pm = (PartitionableMessage *) data_;
  return pm->get_partition();
}

void *CheckOrderNoUpdateNoOutput::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, none);
}

void *CheckOrderNoOutput::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, none);
}

void *CheckOrder::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  OrderMessage *order_message = (OrderMessage *) input_;
  SymbolData *symbol_data = (SymbolData *) state_;

  if (symbol_data->should_reject_trades)
  {
#ifdef DEBUG
    NOTICE("Rejecting: " << *(order_message->symbol()));
#endif
    // std::cerr << "rejected" << std::endl;
    // TODO: not getting time here, is this a problem?
    OrderResult *order_result = new OrderResult(order_message, symbol_data->last_bid, symbol_data->last_offer, 0);
    return w_stateful_computation_get_return(state_change_repository_helper_, order_result, none);
  }
  else
  {
#ifdef DEBUG
    NOTICE("NOT Rejecting: " << *(order_message->symbol()));
#endif
  }

  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, none);
}

void *UpdateNbboNoUpdateNoOutput::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, none);
}

/*
 * KAGR
void *UpdateNbboNoOutput::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  NbboMessage *nbbo_message = (NbboMessage *) input_;
  void *state_change_handle = w_state_change_repository_lookup_by_name(state_change_repository_helper_, state_change_repository_, "symbol data state change");
  SymbolDataStateChange *symbol_data_state_change = (SymbolDataStateChange *)w_state_change_get_state_change_object(state_change_repository_helper_, state_change_handle);


  double offer_bid_difference = nbbo_message->offer_price() - nbbo_message->bid_price();
  bool should_reject_trades = (offer_bid_difference >= 0.05) || ((offer_bid_difference / nbbo_message->mid()) >= 0.05);

  symbol_data_state_change->update(should_reject_trades, nbbo_message->bid_price(), nbbo_message->offer_price());
  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, state_change_handle);
}

void *UpdateNbbo::compute(wallaroo::Data *input_, wallaroo::StateChangeRepository *state_change_repository_, void *state_change_repository_helper_, wallaroo::State *state_, void *none)
{
  NbboMessage *nbbo_message = (NbboMessage *) input_;
  void *state_change_handle = w_state_change_repository_lookup_by_name(state_change_repository_helper_, state_change_repository_, "symbol data state change");
  SymbolDataStateChange *symbol_data_state_change = (SymbolDataStateChange *)w_state_change_get_state_change_object(state_change_repository_helper_, state_change_handle);

  double offer_bid_difference = nbbo_message->offer_price() - nbbo_message->bid_price();
  bool should_reject_trades = (offer_bid_difference >= 0.05) || ((offer_bid_difference / nbbo_message->mid()) >= 0.05);
  
  symbol_data_state_change->update(should_reject_trades, nbbo_message->bid_price(), nbbo_message->offer_price());
  return w_stateful_computation_get_return(state_change_repository_helper_, nullptr, state_change_handle);
}*/

OrderResult::OrderResult(OrderMessage *order_message_, double bid_, double offer_, uint64_t timestamp_): bid(bid_), offer(offer_), timestamp(timestamp_)
{
  order_message = new OrderMessage(order_message_);
}

OrderResult::~OrderResult()
{
  delete order_message;
}

size_t OrderResult::encode_get_size()
{
  return 55;
}

void OrderResult::encode(char *bytes)
{
  Writer writer((unsigned char *) bytes);

  switch (order_message->side())
  {
  case SideType::Buy:
    writer.u8_be((uint8_t) SideType::Buy);
    break;
  case SideType::Sell:
    writer.u8_be((uint8_t) SideType::Sell);
    break;
  default:
    std::cerr << "expected side to be buy or sell (1 or 2), but got side=" << order_message->side() << std::endl;
    writer.u8_be(0xFF);
    break;
  }

  writer.u32_be(order_message->account());
  writer.ms_string(order_message->order_id());
  writer.ms_string(order_message->symbol());
  writer.ms_double(order_message->order_quantity());
  writer.ms_double(order_message->price());
  writer.ms_double(bid);
  writer.ms_double(offer);
  writer.u64_be(timestamp);
}

size_t OrderResultSinkEncoder::get_size(wallaroo::EncodableData *data)
{
  //Header (size == 55 bytes)
  return data->encode_get_size();
}

void OrderResultSinkEncoder::encode(wallaroo::EncodableData *data, char *bytes)
{
  Writer writer((unsigned char *) bytes);

  uint32_t message_size = data->encode_get_size();

  writer.u32_be(message_size);

  data->encode(bytes + 4);
}
