

#ifndef __PYMODULE_HPP__
#define __PYMODULE_HPP__

#include <Python.h>

#include <atomic>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <swigpyrun.h>
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
  string printError(string err_, string method_, int line_);

    
public:
  virtual ~PyModule();
  static PyModule& getInstance(string path_="/home/kgoldstein/dev/c++/msc2p4/src/", string module_="PyNbbo");
  
public:
  StateComputation* getNbboCheck() const;
  StateChange* getSymbolDataStateChange(size_t idx_) const;
  
  
  /**
   * the function args MUST be a single string, separated by commas. The Derived classes
   * should expect the appropriate number of incomming strings and then convert them
   * appropriately. In other words - pass one giant string to the derived class and let
   * the derived class parse it.. in Python!
   * 
   * also, only primites should be allowed: strings, int, double
   */
  template<class T> T getDerivedPythonClass(const char* callname_, string funcArgs_="")
  {
    //
    // some basics
    //
    if ( _pyModule == nullptr )
    {
      string errStr = printError(
	string(callname_).append(" called with no module! Called out of order?"),
	__PRETTY_FUNCTION__,
	__LINE__);
      throw runtime_error(errStr); 
      return nullptr;
    }

    cout << "funcArgs: " << funcArgs_ << endl;
    
  
    //
    // determine if the function is there
    //
    PyObject* pFunc = PyObject_GetAttrString(_pyModule, callname_);
    PyObject* pArgs = PyString_FromString(funcArgs_.c_str());
    if ( pFunc && PyCallable_Check(pFunc) )
    {
      PyObject* rv = SWIG_Python_CallFunctor(pFunc, pArgs);
      if ( rv!=nullptr )
      {
	T functionRV = nullptr;
	if ( SWIG_ConvertPtr(rv, (void**)&functionRV, 0, 0)==-1 )
	{
	  printError("Unable to convert pointer", __PRETTY_FUNCTION__,    __LINE__);
	}
	else
	{
	  printDebug("Converted function pointer");
	  return functionRV;
	}
      }
      else
      {
	printError("Function call failed!", __PRETTY_FUNCTION__, __LINE__);
      }
    }
    else
    {
      printError("Unable to call specified function", __PRETTY_FUNCTION__, __LINE__);
    }
    
    //
    // should never have made it here
    //
    cerr << "BAD!" << endl;
    return nullptr;
  }
};



#endif // __PYMODULE_HPP__
