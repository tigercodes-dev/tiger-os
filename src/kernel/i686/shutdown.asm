bits 32

global halt
halt:
    cli ; Disable interrupts
    hlt ; Stop processor