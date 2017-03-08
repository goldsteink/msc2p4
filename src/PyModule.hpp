

#ifndef __PYMODULE_HPP__
#define __PYMODULE_HPP__

#include <Python.h>
#include <atomic>
#include <mutex>
#include <string>

#include <WallarooCppApi/Computation.hpp>
#include "market-spread-cpp.hpp"

using namespace std;
using namespace wallaroo;

class PyModule
{
private:
  // the module
  PyObject* _pyModule;
  
  
private:
  PyModule(string path_, string file_);
  bool isValidPath(string path_) const;
  void printDebug(string str_) const;
    
public:
  virtual ~PyModule();
  static PyModule& getInstance(string path_="/home/kgoldstein/dev/c++/msc2p4/src/", string module_="PyNbbo");
  
public:
  StateComputation* getNbboCheck() const;
};



#endif // __PYMODULE_HPP__
