

#include "market-spread-cpp.hpp"
#include "PyModule.hpp"
#include "Messages.hpp"
#include "Buffers.hpp"
#include "stdinc.hpp"



extern "C"
{
  extern wallaroo::SourceDecoder *get_order_source_decoder()
  {
    return new OrderSourceDecoder();
  }

  extern wallaroo::SourceDecoder *get_nbbo_source_decoder()
  {
    return new NbboSourceDecoder();
  }

  extern wallaroo::PartitionFunctionU64 *get_partition_function()
  {
    return new SymbolPartitionFunction();
  }

  extern wallaroo::StateComputation *get_check_order_no_update_no_output()
  {
    return new CheckOrderNoUpdateNoOutput();
  }

  extern wallaroo::StateComputation *get_update_nbbo_no_update_no_output()
  {
    return new UpdateNbboNoUpdateNoOutput();
  }

  extern wallaroo::StateComputation *get_check_order_no_output()
  {
    return new CheckOrderNoOutput();
  }

  extern wallaroo::StateComputation *get_update_nbbo_no_output()
  {
    PyModule& pyMod = PyModule::getInstance();
    return pyMod.getDerivedPythonClass<StateComputation*>("updateNbboNoOutputChecker");
    //return new UpdateNbboNoOutput();
  }

  extern wallaroo::StateComputation *get_check_order()
  {
    return new CheckOrder();
  }

  extern wallaroo::StateComputation *get_update_nbbo()
  {
    PyModule& pyMod = PyModule::getInstance();
    return pyMod.getDerivedPythonClass<StateComputation*>("getNbboChecker", "test");
  }

  extern wallaroo::SinkEncoder *get_order_result_sink_encoder()
  {
    return new OrderResultSinkEncoder();
  }

  extern wallaroo::State *get_symbol_data()
  {
    return new SymbolData();
  }

  extern wallaroo::Serializable *w_user_serializable_deserialize(char *bytes_, size_t sz_)
  {
    return nullptr;
  }
}
