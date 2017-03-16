#include <Python.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <exception>

#include <PyModule.hpp>
//#include <swigpyrun.h>



#include <sys/stat.h>
#include <libgen.h>

#include <chrono>


using namespace std;





/**
 * load the specified python module
 */
PyModule::PyModule(string path_, string file_) : _pyModule(nullptr)
{
  //
  // basics
  //
  if ( !isValidPath(path_.c_str()) )
  {
    stringstream errStr;
    errStr << "Not a valid path:";
    errStr << path_.c_str();
    cerr << errStr.str() << endl;
    throw runtime_error(errStr.str());
  }
  else
  {
    printDebug("Path is valid");
    cout << "Path is valid" << endl;
  }

  //
  // load hte python lib
  //
  Py_SetProgramName(const_cast<char*>(string("MSC2P4").c_str()));
  Py_Initialize();


  //
  // append the  path
  //
  PyObject *sys = PyImport_ImportModule("sys");
  PyObject *path = PyObject_GetAttrString(sys, "path");
  PyList_Append(path, PyString_FromString(path_.c_str()));


  //
  // load the module
  //
  PyObject* pModName = PyString_FromString(file_.c_str());
  _pyModule = PyImport_Import(pModName);
  Py_DECREF(pModName);
  if ( _pyModule != nullptr )
  {
    stringstream status;
    status << "Module loaded:";
    status << file_;
    printDebug(status.str());
  }
  else
  {
    stringstream errStr;
    errStr << "Failed to load module!, ModuleName:";
    errStr << file_;
    cerr << errStr.str() << endl;
    //throw runtime_error(errStr.str()); 
  }
}



PyModule::~PyModule()
{
  Py_Finalize();
}



bool PyModule::isValidPath(string path_) const
{
  char *myDir = dirname(const_cast<char*>(path_.c_str()));
  struct stat myStat;
  if ((stat(myDir, &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR)) 
  {
    return true;
  }
  return false;
}



void PyModule::printDebug(string str_) const
{
  #ifdef DEBUG
  cout << str_ << endl;
  #endif
}


string PyModule::printError(string err_, string method_, int line_)
{
  stringstream out;
  out << err_;
  out << " @ ";
  out << method_;
  out << ":";
  out << line_;
  cerr << out.str() << endl;
  return out.str();
}



PyModule& PyModule::getInstance(string path_, string module_)
{
  static PyModule instance(path_, module_);
  return instance;
}






/**
 * get a state computation object
 * basically, I am instantiating a Python Class that extends
 * my state computation object
 */

StateComputation* PyModule::getNbboCheck() const
{
  //
  // some basics
  //
  if ( _pyModule == nullptr )
  {
    stringstream errStr;
    errStr << "getNbboCheck called with no module! Called out of order?";
    errStr << "\n";
    errStr << __PRETTY_FUNCTION__;
    errStr << ":";
    errStr << __LINE__;
    cerr << errStr.str() << endl; 
    throw runtime_error(errStr.str()); 
    return nullptr;
  }


  //
  // determine if the function is there
  //
  string getNbboCheckFunctionName = "getNbboChecker";
  //string getNbboCheckFunctionName = "printMe";
  PyObject* pFunc = PyObject_GetAttrString(_pyModule, getNbboCheckFunctionName.c_str());
  if ( pFunc && PyCallable_Check(pFunc) )
  {
    PyObject* rv = PyObject_CallObject(pFunc, NULL);
    StateComputation* functionRV = nullptr;
    if ( rv!=nullptr )
    {
      if ( SWIG_ConvertPtr(rv, (void**)&functionRV, 0, 0)==-1 )
      {
	cerr << "Unable to convert pointer" << endl;
	cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
      }
      else
      {
	printDebug("Converted function pointer");
	return functionRV;
      }
    }
    else
    {
      cerr << "Function call failed!" << __PRETTY_FUNCTION__ << ":"<< __LINE__ <<  endl;
    }
  }
  else
  {
    cerr << "Unable to call specified function" << endl;
  }

  //
  // should never get here
  //
  cerr << "should never get here!" << endl;
  return nullptr;
}





/**
 * get a state change object
 * basically, I am instantiating a Python Class that extends
 * my state change object
 */
StateChange* PyModule::getSymbolDataStateChange(size_t idx_) const
{
  //
  // some basics
  //
  if ( _pyModule == nullptr )
  {
    stringstream errStr;
    errStr << "getNbboCheck called with no module! Called out of order?";
    errStr << "\n";
    errStr << __PRETTY_FUNCTION__;
    errStr << ":";
    errStr << __LINE__;
    cerr << errStr.str() << endl; 
    throw runtime_error(errStr.str()); 
    return nullptr;
  }


  //
  // determine if the function is there
  //
  string getNbboCheckFunctionName = "getStateChangeBuilder";
  PyObject* pFunc = PyObject_GetAttrString(_pyModule, getNbboCheckFunctionName.c_str());
  if ( pFunc && PyCallable_Check(pFunc) )
  {
    PyObject* rv = PyObject_CallObject(pFunc, NULL);
    StateChange* functionRV = nullptr;
    if ( rv!=nullptr )
    {
      if ( SWIG_ConvertPtr(rv, (void**)&functionRV, 0, 0)==-1 )
      {
	cerr << "Unable to convert pointer" << endl;
	cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
      }
      else
      {
	printDebug("Converted function pointer");
	return functionRV;
      }
    }
    else
    {
      cerr << "Function call failed!" << __PRETTY_FUNCTION__ << ":"<< __LINE__ <<  endl;
    }
  }
  else
  {
    cerr << "Unable to call specified function" << endl;
  }
}




void printName(wallaroo::StateChange* sc_)
{
  if ( sc_!=nullptr )
    cout << sc_->name() << endl;
  return;
}



void* getNullPtr()
{
  return nullptr;
}



#include <market-spread-cpp.hpp>
NbboMessage* getNbboMessageFromInput(wallaroo::Data* data_)
{
  if ( data_ == nullptr ) 
    return nullptr;
  return static_cast <NbboMessage*>(data_);;
}



unsigned long long getWallarooHashValue(wallaroo::StateChange* sc_)
{
  if ( sc_==nullptr ) 
    return 0;
  return sc_->hash();
}
