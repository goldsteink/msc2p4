
#include "Messages.hpp"


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

