CPSC 410/611: MP3 -- README.TXT

This file describes the content of this directory.
A file marked with (*) may be of some interest to the 
student, and he/she may want to have a look at it.
A file marked with (**) is important for this MP, and the
student is encouraged to study the contents of this file.
Files marked with (**) may need to be modified in this MP.

BOCH Environment:
=================

FILE: 			DESCRIPTION:

BIOS-bochs-latest	BIOS file.
VGABIOS-lgpl-latest	BIOS file for the graphics system.
dev_kernel_grub.img	Image file of the boot floopy.
bochsrc.bxrc		Description file for the system. (double-click file to 
			start emulation).

COMPILATION:
===========

FILE: 			DESCRIPTION:

makefile       (**)	Makefile for win32 environment. Type "make" to generate the kernel.
makefile.linux (**)	Makefile for Linux environment. overwrite makefile with this file
                        if you are in a Linux environment.
makefile.linux64(**)	Makefile for Linux 64 bit environment.
linker.ld		the linker script.


OS COMPONENTS:
=============

FILE: 			DESCRIPTION:

start.asm (*)		The bootloader starts code in this file, which in turn
			jumpts to the main entry in File "kernel.C".
kernel.C (**)		Main file, where the OS components are set up, and the
                        system gets going.
                        NOTE: Uncomment the definition of _USES_SCHEDULER_ to
                        turn on use of scheduler in "kernel.C".
			NOTE2: Uncomment the definition of _TERMINATING_FUNCTIONS_ to
        		allow (and test) for terminating functions.

assert.h/c		Implements the "assert()" utility.
utils.h/c		Various utilities (e.g. memcpy, strlen, etc..)

gdt.h/c			Global Descriptor Table.
gdt_low.asm		Low-level GDT code, included in "start.asm".
idt.h/c			Interrupt Descriptor Table.
idt_low.asm		Low-level IDT code, included in "start.asm".
irq.h/c                 Re-mapping of IRQ's into the IDT.
irq_low.asm		Low-level IRQ stuff. (Primarily the interrupt service
			routines and the routine stub that branches out to the
			interrupt dispatcher in "interrupts.C". Included in
  			"start.asm".
exceptions.H/C (*)	The exception dispatcher.
interrupts.H/C		The interrupt dispatcher.

console.H/C		Routines to print to the screen.

machine.h/c (*)		Definitions of some system constants and low-level
			machine operations. (Primarily enable/disable interrupts)

simple_timer.H/C (**)	Routines to control the periodic interval timer.


frame_pool.H/C (*)	Definition and implementation of a vanilla free frame pool
                        manager. 

thread.H/C (**)		Tread Control Block (TCB), creation and low-level dispatching
                        of threads. 

threads_low.asm (**)	Low-level tread context switch code.

scheduler.H (**)	Definition of a simple scheduler.


UTILITIES:
==========

FILE: 			DESCRIPTION:

copykernel.bat (**)	Simple script to copy the kernel onto the floppy image.
                        The script mounts the floppy image, copies the kernel
			image onto it, and then unmounts the floppy image again.
  			The paths in the file need to be edited to make them
			reflect the environment. 
copykernel.sh (**)	Same for a Linux environment.



