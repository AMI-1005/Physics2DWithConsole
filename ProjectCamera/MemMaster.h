#pragma once
#include <iostream>

class MemMaster:
{
  private:
  static int ObjectCount = 0;
  unsigned int Memcount =0;
  unsigned int MemMax = 1250; // around 10KB, it does not make sense to have a bigger default value
  bool ConstructApprover = true;
  public:
  MemMaster();
  MemMaster(unsigned int MaximumOfMemory, bool AmIAllowedToConstwuctYetDaddyUwu); //I added the bool in the constructor for later development  with the UI 
  virtual int& RaiseObjcount();
  virtual unsigned int& Memcount();
  bool SwitchConstructor();//I will Xor this to avoid redundancy
  unsigned int SetMemMax();//Too lazy to overload the equal operator
  
};
//Literally not much to comment. It be very basic methods and very basic indexing. Very necessary tho.
//In case this sucks I will sign myself to make it easy to remove later on.
//AMI
