class Insn<bits <4> MajOpc, bit MinOpc>{
    bits<32> insnEncoding;
    let insnEncoding{15-12}=MajOpc;
    let insnEncoding{11}=MinOpc;
}

multiclass RegAndImmInsn<bits <4> opcode>{
    def rr: Insn<opcode,0>;
    def ri: Insn<opcode,1>;
}
def SUB : Insn<0x00,0>;
defm ADD : RegAndImmInsn<0x01>;
