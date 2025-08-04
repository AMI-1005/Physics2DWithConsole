#pragma once
#include <iostream>

class MemMaster:
{
  private:
  static int ObjectCount;
  unsigned int Memsize;
  unsigned int MemMax;
  bool ConstructApprover;
  public:
  MemMaster();
  MemMaster(unsigned int MaximumOfMemory, bool AmIAllowedToConstwuctYetDaddyUwu); //I added the bool in the constructor for later development  with the UI 
  virtual int& RaiseObjcount();
  virtual unsigned int& Memcount();
  bool SwitchConstructor();//I will Xor this to avoid redundancy
  unsigned int SetMemMax(unsigned int NewVal);//Too lazy to overload the equal operator
  void  MemReport();
  
};
//Literally not much to comment. It be very basic methods and very basic indexing. Very necessary tho.
//In case this sucks I will sign myself to make it easy to remove later on.
//AMI
