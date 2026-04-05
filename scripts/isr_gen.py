# Generation script for the ISRs
from os import makedirs

ISR_DIR = "src/kernel/i686/isr_gen"
ISR_ASM = ISR_DIR + "/isr_gen.inc"
ISR_C = ISR_DIR + "/isr_gen.c"

ISR_ERRCODE = [ 0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x11, 0x15, 0x1D, 0x1E ]

def main():
    # Create directory if it does not exist
    makedirs(ISR_DIR, exist_ok=True)

    gen_asm(ISR_ASM)
    gen_c(ISR_C)

def gen_asm(filepath):
    with open(filepath, "w") as file:
        file.write("; ! This is an auto generated file by isr_gen.py !\n")
        for i in range(256):
            if i in ISR_ERRCODE:
                file.write(f"ISR_ERRCODE 0x{format(i, "02X")}\n")
            else:
                file.write(f"ISR 0x{format(i, "02X")}\n")

def gen_c(filepath):
    with open(filepath, "w") as file:
        file.write("// ! This is an auto generated file by isr_gen.py !\n"
                   "#include \"../idt.h\"\n"
                   "#include \"../gdt.h\"\n\n")
        
        for i in range(256):
            file.write(f"void __attribute__((cdecl)) ISR0x{format(i, "02X")}();\n")

        file.write("\nvoid initialize_gates_IDT() {\n")
        
        for i in range(256):
            file.write(f"\tset_gate_IDT(0x{format(i, "02X")}, ISR0x{format(i, "02X")}, 0x08, ATTRIB_RING_0 | ATTRIB_TYPE_32BIT_INT); \n")
        
        file.write("}\n")

if __name__ == "__main__":
    main()