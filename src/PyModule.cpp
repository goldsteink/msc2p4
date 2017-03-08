#include <Python.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <exception>

#include <PyModule.hpp>
#include <swigpyrun.h>



#include <sys/stat.h>
#include <libgen.h>

#include <chrono>


using namespace std;

using Clock = chrono::steady_clock;
using chrono::time_point;
using chrono::duration_cast;
using chrono::milliseconds;
using chrono::microseconds;
using chrono::nanoseconds;
using namespace literals::chrono_literals;






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



PyModule& PyModule::getInstance(string path_, string module_)
{
  static PyModule instance(path_, module_);
  return instance;
}




StateComputation* PyModule::getNbboCheck() const
{
  //
  // some basics
  //
  if ( _pyModule == nullptr )
  {
    stringstream errStr ("getNbboCheck called with no module! Called out of order?");
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
}



void printKevin()
{
  cout << "Kevin" << endl;
  return;
}



void* getNullPtr()
{
  return nullptr;
}



#include <market-spread-cpp.hpp>
NbboMessage* getNbboMessageFromInput(wallaroo::Data* data_)
{
  NbboMessage* msg = dynamic_cast<NbboMessage*>(data_);
  if ( msg!=nullptr )
    cout << "NBBO PRC:(" << msg->bid_price() << "x"<<msg->offer_price() << ")" << endl;
  return msg;
}



/*
int main(int argc_, char** argv_)
{
  PyObject *pName, *pModule, *pDict, *pFunc, *pFuncName;
  PyObject *pArgs, *pValue;
  char *cstrret;

  string modname = "call_function";
  //string funcname = "multiply";
  string funcname = "testBase";

  Py_SetProgramName("Kevin");
  Py_Initialize();

  PyObject *sys = PyImport_ImportModule("sys");
  PyObject *path = PyObject_GetAttrString(sys, "path");
  PyList_Append(path, PyString_FromString("/home/kgoldstein/dev/c++/msc2p3/2"));
  pName = PyString_FromString(modname.c_str());
  pFuncName = PyString_FromString(funcname.c_str());
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  if ( pModule != nullptr )
  {
    pFunc = PyObject_GetAttrString(pModule, funcname.c_str());
    if ( pFunc && PyCallable_Check(pFunc))
    {
      PyObject* rv = PyObject_CallObject(pFunc, NULL);
      if ( rv!=nullptr )
      {

	try
	{
	  cout << "RV is NOT NULL" << endl;
	  void* desc = 0;
	  desc = SWIG_TypeQuery("Base *");
	  cout << "DESC!:" << desc << endl;
	  
	  Base* b;
	  if (SWIG_ConvertPtr(rv, (void**)&b, 0, 0) == -1 )
	  {
	    cout << "CONVERSION FAILED!" << endl;
	  }
	  else
	  {
	    cout << "GOOD!" << endl;
            time_point<Clock> start;
            time_point<Clock> end;
size_t sz=0;
while(true){
sz+=1;
start = Clock::now();
b->compute(); 
end = Clock::now();
nanoseconds diff = duration_cast<nanoseconds>(end-start);
cout << diff.count() << endl;
//if (sz%50==0 ) cout << endl;
}
	    //PyObject_CallMethod(rv,"compute", "");
	  }
	  
	}
	catch (exception& e_)
	{
	  cout << e_.what() << endl;
	}
      }
      else
      {
	cout << "RV is null" << endl;
      }
    }
  }
  else
  {
    cout << "module not found!" << endl;
  }



  Py_Finalize();
  return 0;
}

*/
