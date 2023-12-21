DEF_EXP(MUL, "mul")
DEF_EXP(DIV, "div")
DEF_EXP(ADD, "add")
DEF_EXP(SUB, "sub")

#define COND_JMP(type, cmd)                            \
    DEF_EXP(type,                                      \
    "\n" "; Operation " #type                          \
    "\n" cmd " " #type "_JMP_%d"                       \
    "\n" "push 0"                                      \
    "\n" "jump " #type "_JMP_EXT_%d"                   \
    "\n"  #type "_JMP_%d:"                             \
    "\n" "push 1"                                      \
    "\n"  #type "_JMP_EXT_%d:", index, index, index, index)

COND_JMP(GR,  "ja")
COND_JMP(LS,  "jb")
COND_JMP(GRE, "jae")
COND_JMP(LSE, "jbe")
COND_JMP(EQ,  "je")
COND_JMP(NEQ, "jne")