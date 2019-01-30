#include <iostream>
#include <map>

#include <stdint.h>
#include <stdlib.h>

std::map<unsigned, unsigned> instr_map;
int branch_count[2];

// REF: https://github.com/llvm-mirror/llvm/blob/master/lib/IR/Instruction.cpp#L290-L371
const char *mapCodeToName(unsigned Op) {
    if (Op == 1)
      return "ret";
    if (Op == 2)
      return "br";
    if (Op == 3)
      return "switch";
    if (Op == 4)
      return "indirectbr";
    if (Op == 5)
      return "invoke";
    if (Op == 6)
      return "resume";
    if (Op == 7)
      return "unreachable";
    if (Op == 8)
      return "cleanupret";
    if (Op == 9)
      return "catchret";
    if (Op == 10)
      return "catchswitch";
    if (Op == 11)
      return "add";
    if (Op == 12)
      return "fadd";
    if (Op == 13)
      return "sub";
    if (Op == 14)
      return "fsub";
    if (Op == 15)
      return "mul";
    if (Op == 16)
      return "fmul";
    if (Op == 17)
      return "udiv";
    if (Op == 18)
      return "sdiv";
    if (Op == 19)
      return "fdiv";
    if (Op == 20)
      return "urem";
    if (Op == 21)
      return "srem";
    if (Op == 22)
      return "frem";
    if (Op == 23)
      return "shl";
    if (Op == 24)
      return "lshr";
    if (Op == 25)
      return "ashr";
    if (Op == 26)
      return "and";
    if (Op == 27)
      return "or";
    if (Op == 28)
      return "xor";
    if (Op == 29)
      return "alloca";
    if (Op == 30)
      return "load";
    if (Op == 31)
      return "store";
    if (Op == 32)
      return "getelementptr";
    if (Op == 33)
      return "fence";
    if (Op == 34)
      return "cmpxchg";
    if (Op == 35)
      return "atomicrmw";
    if (Op == 36)
      return "trunc";
    if (Op == 37)
      return "zext";
    if (Op == 38)
      return "sext";
    if (Op == 39)
      return "fptoui";
    if (Op == 40)
      return "fptosi";
    if (Op == 41)
      return "uitofp";
    if (Op == 42)
      return "sitofp";
    if (Op == 43)
      return "fptrunc";
    if (Op == 44)
      return "fpext";
    if (Op == 45)
      return "ptrtoint";
    if (Op == 46)
      return "inttoptr";
    if (Op == 47)
      return "bitcast";
    if (Op == 48)
      return "addrspacecast";
    if (Op == 49)
      return "cleanuppad";
    if (Op == 50)
      return "catchpad";
    if (Op == 51)
      return "icmp";
    if (Op == 52)
      return "fcmp";
    if (Op == 53)
      return "phi";
    if (Op == 54)
      return "call";
    if (Op == 55)
      return "select";
    if (Op == 56)
      return "<Invalid operator>";
    if (Op == 57)
      return "<Invalid operator>";
    if (Op == 58)
      return "va_arg";
    if (Op == 59)
      return "extractelement";
    if (Op == 60)
      return "insertelement";
    if (Op == 61)
      return "shufflevector";
    if (Op == 62)
      return "extractvalue";
    if (Op == 63)
      return "insertvalue";
    if (Op == 64)
      return "landingpad";

    return "<Invalid operator>";
}

// For section 2
// num: the number of unique instructions in the basic block. It is the length of keys and values.
// keys: the array of the opcodes of the instructions
// values: the array of the counts of the instructions
extern "C" __attribute__((visibility("default")))
void updateInstrInfo(unsigned num, uint32_t * keys, uint32_t * values) {
  int i;
  uint32_t key;
  uint32_t value;

  for (i=0; i<num; i++) {
    key = keys[i];
    value = values[i];
    if (instr_map.count(key) == 0)
    	instr_map.insert(std::pair<uint32_t, uint32_t>(key, value));
    else
    	instr_map[key] = instr_map[key] + value;
  }
  return;
}

// For section 3
// If taken is true, then a conditional branch is taken;
// If taken is false, then a conditional branch is not taken.
extern "C" __attribute__((visibility("default")))
void updateBranchInfo(bool taken) {

	if (taken)
		branch_count[0] ++;
	branch_count[1] ++;
  return;
}

// For section 2
extern "C" __attribute__((visibility("default")))
void printOutInstrInfo() {

  for (std::map<uint32_t, uint32_t>::iterator it=instr_map.begin(); it!=instr_map.end(); ++it)
    std::cerr << mapCodeToName(it->first) << '\t' << it->second << '\n';

  instr_map.clear();

  return;
}

// For section 3
extern "C" __attribute__((visibility("default")))
void printOutBranchInfo() {

	std::cerr << "taken\t" << branch_count[0] << '\n';
	std::cerr << "total\t" << branch_count[1] << '\n';

	branch_count[0] = 0;
	branch_count[1] = 0;

  return;
}
