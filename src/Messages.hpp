
#ifndef __MESSAGES_HPP__
#define __MESSAGES_HPP__


#include <WallarooCppApi/Data.hpp>

using wallaroo::Data;






//--------------------------------------------------------------------
enum FixType
{
  Order = 1,
  Nbbo = 2
};




//--------------------------------------------------------------------
enum SideType
{
  Buy = 1,
  Sell = 2
};




//--------------------------------------------------------------------
class PartitionableMessage: public Data
{
public:
  virtual uint64_t get_partition() = 0;
};




//--------------------------------------------------------------------
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




//--------------------------------------------------------------------
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
  string get_symbol() { return _symbol; }
  double bid_price() { return _bid_price; }
  double offer_price() { return _offer_price; }
  double mid() {return (_bid_price + _offer_price) / 2.0; }
};






#endif
