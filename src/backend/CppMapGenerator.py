def map(var_name,f):
    """if-else 版本"""
    # for i in range(len(key)):
    #     if(i==0):
    #         print("if({0} == {1}){{return \"{2}\";}}".format(var_name,key[i],val[i]),file = f)
    #     else:
    #         print("else if({0} == {1}){{return \"{2}\";}}".format(var_name,key[i],val[i]),file = f)
    # print("else{{error();}}",file= f)
    """define"""
    for i in range(len(key)):
        print("\t{1} = {2},".format(var_name,val[i],key[i]),file = f)
    
key_str = """ADD, SUB, XOR, OR, AND, SLL, SRL, SRA, SLT, SLTU, ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI, SLTI, SLTIU, LW, SW, BEQ, BNE, BLT, BGE, BLTU, BGEU, JAL, JALR, LA, LI, MOV, J, NOP"""

key = key_str.split(", ")

val = []
for i in range(len(key)):
    val.append(key[i].lower())

key = [
    "X0",
    "X1",
    "X2",
    "X3",
    "X4",
    "X5",
    "X6",
    "X7",
    "X8",
    "X9",
    "X10",
    "X11",
    "X12",
    "X13",
    "X14",
    "X15",
    "X16",
    "X17",
    "X18",
    "X19",
    "X20",
    "X21",
    "X22",
    "X23",
    "X24",
    "X25",
    "X26",
    "X27",
    "X28",
    "X29",
    "X30",
    "X31",
    "null",
]
val = [
    "zero",
    "ra",
    "sp",
    "gp",
    "tp",
    "t0",
    "t1",
    "t2",
    "s0",
    "s1",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7",
    "s2",
    "s3",
    "s4",
    "s5",
    "s6",
    "s7",
    "s8",
    "s9",
    "s10",
    "s11",
    "t3",
    "t4",
    "t5",
    "t6",
    "null",
]

namespace_name = "rv::rvOPCODE::"
# for i in range(len(key)):
#     val[i] = namespace_name + val[i]
#     key[i] = namespace_name + key[i]
    

var_name = "r"
with open("script.cpp","w") as f:
    map(var_name,f)
    