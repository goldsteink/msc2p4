
#ifndef __BUFFERS_HPP__
#define __BUFFERS_HPP__

#include <WallarooCppApi/SourceDecoder.hpp>
#include <WallarooCppApi/SinkEncoder.hpp>

using namespace wallaroo;






//--------------------------------------------------------------------
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





//--------------------------------------------------------------------
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




//--------------------------------------------------------------------
class OrderSourceDecoder: public SourceDecoder
{
public:
  virtual size_t header_length();
  virtual size_t payload_length(char *bytes);
  virtual wallaroo::Data *decode(char *bytes, size_t sz_);
};




//--------------------------------------------------------------------
class NbboSourceDecoder: public SourceDecoder
{
public:
  virtual size_t header_length();
  virtual size_t payload_length(char *bytes);
  virtual wallaroo::Data *decode(char *bytes, size_t sz_);
};





#endif// __BUFFERS_HPP__
