/*
--------------------------------------------------------------------
file  : stm32h7_linkerScript.ld
brief : Cortex M7 linker file for STM32H743ZI.
--------------------------------------------------------------------
*/

/* Global entry point */
ENTRY(IsrReset)

/* Flash */
FLASH_START     =   0x08000000;
FLASH_SIZE      =   0x200000;   /* 2097152 B */

/* FLASH vector table */
FLASHVTABLE_START =  FLASH_START;  /* addr 0x08000000 */
FLASHVTABLE_SIZE =  0x300;        /* 768 B */

/* Flash ID + checksum area */
CHECKSUM_SIZE          = 0x4;
IDAREA_SIZE            = 0x100 - CHECKSUM_SIZE;   /* 252 B */
IDAREA_START           = FLASHVTABLE_START + FLASHVTABLE_SIZE;
CHECKSUM_START         = IDAREA_START + IDAREA_SIZE - CHECKSUM_SIZE;

/* Flash Application */
APP_SIZE               = FLASH_SIZE - FLASHVTABLE_SIZE - IDAREA_SIZE;         
APP_START              = FLASH_START + FLASHVTABLE_SIZE + IDAREA_SIZE + CHECKSUM_SIZE; 
APP_END                = APP_START + APP_SIZE;

/* RAM */

DTCMRAM_SIZE    =   0x20000;     /* 1310720 B DTCM = Data Tightly-coupled mem*/
ITCMRAM_SIZE    =   0x10000;     /* 65536 B ITCM = Instruction Tightly-coupled mem*/        
RAM_D1_SIZE     =   0x80000;     /* 524288 B */
RAM_D2_SIZE     =   0x48000;     /* 294912 B */
RAM_D3_SIZE     =   0x10000;     /* 65536 B */

DTCMRAM_START   =   0x20000000;
ITCMRAM_START   =   0x00000000;  
RAM_D1_START    =   0x24000000;             
RAM_D2_START    =   0x30000000;             
RAM_D3_START    =   0x38000000;             


/* RAM vector table */
RAMVTABLE_START =   RAM_D1_START; /* addr 0x24000000 */
RAMVTABLE_SIZE  =   0x300;        /* 768 B */

/* NOINIT Area */
NOINIT_SIZE            = 0x100;                  /* 256 B */
NOINIT_START           = RAMVTABLE_START + RAMVTABLE_SIZE; /* addr 0x20000300 */

RAM_D1_STACKSIZE       = 0x800;                 /* 2048 B */     
RAM_D1_STACKSTART      = RAM_D1_START + RAM_D1_SIZE - RAM_D1_STACKSIZE;   

RAM_D1_APPSIZE         = RAM_D1_SIZE - RAMVTABLE_SIZE - NOINIT_SIZE - RAM_D1_STACKSIZE;
RAM_D1_APPSTART        = NOINIT_START + NOINIT_SIZE;


/* Specify the memory areas */
MEMORY
{
  FLASHVTABLE (r)     : ORIGIN = FLASHVTABLE_START,   LENGTH = FLASHVTABLE_SIZE
  IDAREA (r)          : ORIGIN = IDAREA_START,        LENGTH = IDAREA_SIZE
  CHECKSUM (r)        : ORIGIN = CHECKSUM_START,      LENGTH = CHECKSUM_SIZE
  FLASH (rx)          : ORIGIN = APP_START,           LENGTH = APP_SIZE
  
  /* Always use 512kB of RAM */
  RAMVTABLE (rwx)     : ORIGIN = RAMVTABLE_START,     LENGTH = RAMVTABLE_SIZE
  NOINIT (rw)         : ORIGIN = NOINIT_START,        LENGTH = NOINIT_SIZE                       
  RAM_D1 (rwx)        : ORIGIN = RAM_D1_APPSTART,     LENGTH = RAM_D1_APPSIZE
  STACK  (rwx)        : ORIGIN = RAM_D1_STACKSTART,   LENGTH = RAM_D1_STACKSIZE  
  RAM_D2    (rwx)     : ORIGIN = RAM_D2_START,        LENGTH = RAM_D2_SIZE
  RAM_D3    (rwx)     : ORIGIN = RAM_D3_START,        LENGTH = RAM_D2_SIZE             
}

_estack = RAM_D1_STACKSTART + RAM_D1_STACKSIZE;        /* end of RAM_D1 */
_Min_Heap_Size = 0x200;                    /* required minimum amount of heap, currently this is just a placeholder  */
_Min_Stack_Size = RAM_D1_STACKSIZE;        /* required amount of system stack */


/* Output sections */
SECTIONS
{
  /* The startup code goes into "FLASH" (Rom type MEMORY) */
  .isr_vector :
  {
   . = ALIGN(4);
   _sflash = .;
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASHVTABLE

  .idarea :
  {
   . = ALIGN(4);
    KEEP(*(.idarea))
    KEEP(*(.idarea*))
    . = ALIGN(4);
  } >IDAREA

  .checksum :
  {
    . = ALIGN(4);
    KEEP (*(.checksum))
    KEEP (*(.checksum*))
    . = ALIGN(4);
  } > CHECKSUM

  /* The program code and other data goes into ROM */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbol at end of code */
  } >FLASH

  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  /* Cpp arm exception table */
  .ARM.extab   : { 
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  /* Arm exception index */
  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;  /* exception index section start */
      *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >FLASH

  /* pre initialization */
  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* initialization of function pointers at program startup */
  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    PROVIDE_HIDDEN (_ctors_begin = . );
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    PROVIDE_HIDDEN (_ctors_end = . );
    . = ALIGN(4);
  } >FLASH

  /* finalization destructor pointers */  
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* ############ RAM sections ############## */

  /* copy of interrupt vectors in ram init manually, modifiable at runtime */
  .isr_ramvector (NOLOAD):
  {
   . = ALIGN(512);          /* On Cortex-M, the vector table has to be 512 byte aligned! */
    KEEP(*(.isr_ramvector)) /* Startup code */
    . = ALIGN(4);
  } >RAMVTABLE
  
  /* Noinit area of RAM */
  .noinit (NOLOAD):
  {
      . = ALIGN(4);
      _snoinit = .;
      KEEP(*(.noinit))
      KEEP(*(.noinit*))
      . = ALIGN(4);
      _enoinit = .;
  } >NOINIT

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM_D1, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    /* Functions that have to be executed from RAM */
    . = ALIGN(4);
    *(.ramfunc);
    *(.ramfunc*);

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM_D1 AT> FLASH

  /* Uninitialized data section into RAM_D1 */
  .bss :
  {
    . = ALIGN(4);
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM_D1

  /* User_heap section, used to check that there is enough RAM_D1 left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );      /* point to the start of heap section */
    PROVIDE ( _end = . );     /* twice symbols because support different libs */
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1

  /* used by the startup to initialize data */
  _siRAM_D2 = LOADADDR(.RAM_D2);

  /* Initialized data sections goes into RAM_D2, load LMA copy after code */
  .RAM_D2 : 
  {
    . = ALIGN(4);
    _sRAM_D2 = .;        /* create a global symbol at data start */
    *(.RAM_D2)           /* .data sections */
    *(.RAM_D2*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D2 = .;        /* define a global symbol at data end */
  } >RAM_D2 AT> FLASH 

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss_RAM_D2 :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss_RAM_D2 = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss_RAM_D2;
    *(.bss_RAM_D2)
    *(.bss_RAM_D2*)
    *(COMMON)

    . = ALIGN(4);
    _ebss_RAM_D2 = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss_RAM_D2;
  } >RAM_D2


  /* used by the startup to initialize data */
  _siRAM_D3 = LOADADDR(.RAM_D3);

  /* Initialized data sections goes into RAM_D3, load LMA copy after code */
  .RAM_D3 : 
  {
    . = ALIGN(4);
    _sRAM_D3 = .;        /* create a global symbol at data start */
    *(.RAM_D3)           /* .data sections */
    *(.RAM_D3*)          /* .data* sections */

    . = ALIGN(4);
    _eRAM_D3 = .;        /* define a global symbol at data end */
  } >RAM_D3 AT> FLASH 

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss_RAM_D3 :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss_RAM_D3 = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss_RAM_D3;
    *(.bss_RAM_D3)
    *(.bss_RAM_D3*)
    *(COMMON)

    . = ALIGN(4);
    _ebss_RAM_D3 = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss_RAM_D3;
  } >RAM_D3

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  .RAM_D1 : {*(.RAM_D1)} >RAM_D1
  .RAM_D2 : {*(.RAM_D2)} >RAM_D2
  .RAM_D3 : {*(.RAM_D3)} >RAM_D3
}
