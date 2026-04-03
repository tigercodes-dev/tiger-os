bits 32

global halt_system
halt_system:
    cli ; Disable interrupts
    hlt ; Stop processor