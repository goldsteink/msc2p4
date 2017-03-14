#include "Messages.hpp"
#include "Buffers.hpp"
#include <iostream>
#include <cstring>
using namespace std;

//--------------------------------------------------------------------
Reader::Reader(unsigned char *bytes_): _ptr(bytes_)
{
}





//--------------------------------------------------------------------
uint8_t Reader::u8_be()
{
  uint8_t ret = *((uint8_t *)_ptr);
  _ptr += 1;
  return ret;
}





//--------------------------------------------------------------------
uint16_t Reader::u16_be()
{
  uint16_t ret = be16toh(*((uint16_t *)_ptr));
  _ptr += 2;
  return ret;
}





//--------------------------------------------------------------------
uint32_t Reader::u32_be()
{
  uint32_t ret = be32toh(*((uint32_t *)_ptr));
  _ptr += 4;
  return ret;
}





//--------------------------------------------------------------------
uint64_t Reader::u64_be()
{
  uint64_t ret = be64toh(*((uint64_t *)_ptr));
  _ptr += 8;
  return ret;
}





//--------------------------------------------------------------------
double Reader::ms_double()
{
  double ret = 0.0;
  char *p = (char *)(&ret) + 8;

  for (int i = 0; i < sizeof(double); i++)
  {
    *(--p) = *(_ptr++);
  }

  return ret;
}





//--------------------------------------------------------------------
string *Reader::ms_string(size_t sz_)
{
  string *ret = new string((char *)_ptr, sz_);
  _ptr += sz_;
  return ret;
}





//--------------------------------------------------------------------
Writer::Writer(unsigned char *bytes_): _ptr(bytes_)
{
}





//--------------------------------------------------------------------
void Writer::u8_be(uint8_t value_)
{
  *(uint8_t *)(_ptr) = value_;
  _ptr += 1;
}





//--------------------------------------------------------------------
void Writer::u16_be(uint16_t value_)
{
  *(uint16_t *)(_ptr) = htobe16(value_);
  _ptr += 2;
}





//--------------------------------------------------------------------
void Writer::u32_be(uint32_t value_)
{
  *(uint32_t *)(_ptr) = htobe32(value_);
  _ptr += 4;
}





//--------------------------------------------------------------------
void Writer::u64_be(uint64_t value_)
{
  *(uint64_t *)(_ptr) = htobe64(value_);
  _ptr += 8;
}





//--------------------------------------------------------------------
void Writer::ms_double(double value_)
{
  char *p = (char *)(&value_) + 8;

  for (int i = 0; i < sizeof(double); i++)
  {
    *(_ptr++) = *(--p);
  }
}





//--------------------------------------------------------------------
void Writer::ms_string(string *str) 
{
  std::memcpy(_ptr, str->c_str(), str->size());
  _ptr += str->size();
}




//--------------------------------------------------------------------
size_t OrderSourceDecoder::header_length()
{
  return 4;
}





//--------------------------------------------------------------------
size_t OrderSourceDecoder::payload_length(char *bytes)
{
  Reader reader((unsigned char*) bytes);
  return reader.u32_be();
}





//--------------------------------------------------------------------
wallaroo::Data *OrderSourceDecoder::decode(char *bytes, size_t sz_)
{
  Reader reader((unsigned char*) bytes);

  uint8_t fix_type = reader.u8_be();
  if (fix_type != FixType::Order)
  {
    std::cerr << "wrong type! expected order, got fix_type=" << (uint16_t)fix_type << std::endl;
    return nullptr;
  }

  SideType side = (SideType) reader.u8_be();
  uint32_t account = reader.u32_be();
  string *order_id = reader.ms_string(6);
  string *symbol = reader.ms_string(4);
  double order_quantity = reader.ms_double();
  double price = reader.ms_double();
  string *transact_time = reader.ms_string(21);

  OrderMessage *order_message = new OrderMessage(side, account, *order_id, *symbol, order_quantity, price, *transact_time);

  delete order_id;
  delete symbol;
  delete transact_time;

  return order_message;
}





//--------------------------------------------------------------------
size_t NbboSourceDecoder::header_length()
{
  return 4;
}

size_t NbboSourceDecoder::payload_length(char *bytes)
{
  Reader reader((unsigned char*) bytes);
  return reader.u32_be();
}

wallaroo::Data *NbboSourceDecoder::decode(char *bytes, size_t sz_)
{
  Reader reader((unsigned char*) bytes);

  uint8_t fix_type = reader.u8_be();
  if (fix_type != FixType::Nbbo)
  {
    std::cerr << "wrong type! expected nbbo, got fix_type=" << fix_type << std::endl;
    return nullptr;
  }

  string *symbol = reader.ms_string(4);
  string *transact_time = reader.ms_string(21);
  double bid_price = reader.ms_double();
  double offer_price = reader.ms_double();

  // std::cerr << "bid_price=" << bid_price << " offer_price=" << offer_price << std::endl;

  NbboMessage *nbbo_message = new NbboMessage(*symbol, *transact_time, bid_price, offer_price);

  delete symbol;
  delete transact_time;

  return nbbo_message;
}









