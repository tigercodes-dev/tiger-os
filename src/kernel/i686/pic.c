#include "pic.h"
#include "io.h"

#define PIC1_CMD  0x20
#define PIC1_DAT  0x21
#define PIC2_CMD  0xA0
#define PIC2_DAT  0xA1

enum ICW1 {
    ICW1_ICW4       = 0x01,
    ICW1_SINGLE     = 0x02,
    ICW1_INTERVAL4  = 0x04,
    ICW1_LEVEL      = 0x08,
    ICW1_INIT       = 0x10,
};

enum ICW4 {
    ICW4_8086       = 0x01,
    ICW4_AUTO_EOI   = 0x02,
    ICW4_BUF_MASTER = 0x0C,
    ICW4_BUF_SLAVE  = 0x08,
    ICW4_SFN_MODE   = 0x10, // Specially Fully Nested Mode
};

#define END_OF_INTERRUPT 0x20
#define READ_IRR         0x0A
#define READ_ISR         0x0B

void config_PIC(uint8_t offset_pic1, uint8_t offset_pic2) {
    // Send ICW1
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // Send ICW2 (offsets)
    outb(PIC1_DAT, offset_pic1);
    io_wait();
    outb(PIC2_DAT, offset_pic2);
    io_wait();

    // Send ICW3 (slave)
    outb(PIC1_DAT, 1 << 2); // IRQ2 connected to slave
    io_wait();
    outb(PIC2_DAT, 2); // Cascade identity
    io_wait();

    // Send ICW4
    outb(PIC1_DAT, ICW4_8086);
    io_wait();
    outb(PIC2_DAT, ICW4_8086);
    io_wait();

    // Unmask PIC
    outb(PIC1_DAT, 0x00);
    io_wait();
    outb(PIC2_DAT, 0x00);
    io_wait();
}

void end_of_interrupt_PIC(int irq) {
    if (irq >= 8) {
        outb(PIC2_CMD, END_OF_INTERRUPT);
    }
    outb(PIC1_CMD, END_OF_INTERRUPT);
}

void disable_PIC() {
    outb(PIC1_DAT, 0xFF);
    io_wait();
    outb(PIC2_DAT, 0xFF);
    io_wait();
}

void mask_PIC(int irq) {
    uint8_t port;

    if (irq < 8) {
        port = PIC1_DAT;
    } else {
        port = PIC2_DAT;
        irq -= 8;
    }

    uint8_t mask = inb(port) | (1 << irq);
    outb(port, mask);
}

void unmask_PIC(int irq) {
    uint8_t port;

    if (irq < 8) {
        port = PIC1_DAT;
    } else {
        irq -= 8;
        port = PIC2_DAT;
    }

    uint8_t mask = inb(port) & ~(1 << irq);
    outb(port, mask);
}

uint16_t get_irr_PIC() {
    outb(PIC1_CMD, READ_IRR);
    outb(PIC2_CMD, READ_IRR);
    return (uint16_t)inb(PIC1_CMD) | ((uint16_t)inb(PIC2_CMD) << 8);
}

uint16_t get_isr_PIC() {
    outb(PIC1_CMD, READ_ISR);
    outb(PIC2_CMD, READ_ISR);
    return (uint16_t)inb(PIC1_CMD) | ((uint16_t)inb(PIC2_CMD) << 8);
}
