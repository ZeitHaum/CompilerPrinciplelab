inst_list = [
    "def",
    "fdef",
    "mov",
    "fmov",
    "add",
    "addi",
    "fadd",
    "sub",
    "subi",
    "fsub",
    "mul",
    "fmul",
    "div",
    "fdiv",
    "mod",
    "lss",
    "flss",
    "leq",
    "fleq",
    "gtr",
    "fgtr",
    "geq",
    "fgeq",
    "eq",
    "feq",
    "neq",
    "fneq",
    "_not",
    "_and",
    "_or",
    "alloc",
    "load",
    "store",
    "getptr",
    "cvt_i2f",
    "cvt_f2i",
    "return",
    # "call",
    "goto",
    "unuse",
]

#op1,op2,des "def:0,2", "add, 0,1,2", "return 0", "unuse, null"
avaible_lis = [
    "def",
    "def",
    "def",
    "def",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "add",
    "def",
    "add",
    "add",
    "def",
    "add",
    "add",
    "add",
    "def",
    "def", 
    "return",
    #call
    "def",
    "unuse",
]


avaible_matrix = []
dic = dict()
dic["def"] = [True,False,True]
dic["add"] = [True,True,True]
dic["return"] = [True,False,False]
dic["unuse"] = [False,False,False]
for xi in avaible_lis:
    avaible_matrix.append(dic[xi])

out = open("out.txt","w");

for i in range(len(avaible_lis)):
    generor_dic = dict()
    generor_dic["ins_name"] = inst_list[i]
    generor_dic["ins_name_up"] = inst_list[i].upper()
    ai = avaible_matrix[i]
    if(ai[0]):
        generor_dic["0_"] = ",op1_"
        generor_dic["0_str"] = " name->op1 = op1_; " 
    else:
        generor_dic["0_"] = ""
        generor_dic["0_str"] = "" 
    if(ai[1]):
        generor_dic["1_"] = ",op2_"
        generor_dic["1_str"] = " name->op2 = op2_; "
    else:
        generor_dic["1_"] = ""
        generor_dic["1_str"] = "" 
    if(ai[2]):
        generor_dic["2_str"] = " name->des = des_; "
        generor_dic["2_"] = ",des_"
    else:
        generor_dic["2_"] = ""
        generor_dic["2_str"] = "" 
    ret_string = "#define ADD_INST_{ins_name_up}(name{0_}{1_}{2_}) Instruction* name = new Instruction(); {0_str} {1_str} {2_str}  name->op = ir::Operator::{ins_name};ADD_INST(name)".format(**generor_dic)
    print(ret_string,file=out)
    
