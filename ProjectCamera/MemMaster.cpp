#include "MemMaster.h"

MemMaster::MemMaster()
{
  static int ObjectCount = 0;
  unsigned int Memcount =0;
  unsigned int MemMax = 1250; // around 10KB, it does not make sense to have a bigger default value
  bool ConstructApprover = true;
  //Also very ugly way of writing this constructor
}
MemMaster::MemMaster(unsigned int MaximumOfMemory, bool AmIAllowedToConstwuctYetDaddyUwu) : MaximumOfMemory(MemMax), AmIAllowedToConstwuctYetDaddyUwu(ConstructApprover){}
bool MemMaster:SwitchConstructor()
{
  ConstructApprover = ConstructApprover ^ 1; //If 0 turns one if 1 turns 0. Duh.
}
unsigned int MemMaster::SetMemMax(unsigned int NewVal)
{
  MemMax = NewVal;
}
//the other two are virtual
//AMI
