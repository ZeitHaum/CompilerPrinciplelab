instr_str = "LW, SW, ADDI, LI, NOP, JR, CALL, MV, JAL, ADD, SUB, MUL, DIV, SLLI"

instrs = instr_str.split(", ")
# print(instrs)

"""
"LW", #op rs2, imm(rs1)
"ADDI", # op rd,rs1,imm
"LI", #op rd, imm
"NOP", #op
"JR" #op rs1
"CALL" #op rd,label
"MV" #op rd, rs2
"JAL", #op label
"ADD" #op, rd, rs1,rs2
"""
family_mask_map = dict()
#rd,rs1,rs2,imm,label
family_mask_map["LW"] = [0,1,1,1,0]
family_mask_map["ADDI"] = [1,1,0,1,0]
family_mask_map["LI"] = [1,0,0,1,0]
family_mask_map["NOP"] = [0,0,0,0,0]
family_mask_map["JR"] = [0,1,0,0,0]
family_mask_map["CALL"] = [1,0,0,0,1]
family_mask_map["MV"] = [1,0,1,0,0]
family_mask_map["JAL"] = [0,0,0,0,1]
family_mask_map["ADD"] = [1,1,1,0,0]

instrs_familiy = [
    "LW", 
    "LW", 
    "ADDI",
    "LI",
    "NOP",
    "JR",
    "CALL",
    "MV",
    "JAL",
    "ADD",
    "ADD",
    "ADD",
    "ADD",
    "LI",
]

assert(len(instrs)==len(instrs_familiy))

#任务一 生成define
f = open("script.cpp","w")
attr_lis = ["rd","rs1","rs2","imm","label"]
for i in range(len(instrs)):
    ins_name = instrs[i].lower()
    ret = "#define dgen_"
    ret+= ins_name
    ret+= "_ins(name, "
    init_str = "rv::rv_inst name; name.op = rv::rvOPCODE::{0}; ".format(instrs[i])
    mask = family_mask_map[instrs_familiy[i]]
    assert(len(mask)==len(attr_lis))
    for j in range(len(mask)):
        if(mask[j]==1):
            ret+= attr_lis[j]
            ret+= "_, "
            init_str+= "name."+attr_lis[j] + " = " + attr_lis[j]+"_; "
    ret = ret[:-2] + ")" + ret[-1]
    ret+= init_str
    if(instrs[i]=="SW"):
       ret+="stack_space+=4; " 
    ret+= "rv_insts.push_back(name)" 
    print(ret,file=f)
    
#任务二  生成 draw函数
#直接在cpp源程序中修改即可

#任务三  生成std::string rv::toString(rvREG r)
ret = """
std::string rv::toString(rvOPCODE r){
"""
key = instrs
val = [insi.lower() for insi in key]
var_name = "r"
for i in range(len(key)):
    if(i==0):
        ret+="\tif({0} == rv::rvOPCODE::{1}){{return \"{2}\";}}\n".format(var_name,key[i],val[i])
    else:
        ret+="\telse if({0} == rv::rvOPCODE::{1}){{return \"{2}\";}}\n".format(var_name,key[i],val[i])
    
ret+="\telse{error();}\n"
ret+="""}
"""
print(ret,file=f)

    
            
            

