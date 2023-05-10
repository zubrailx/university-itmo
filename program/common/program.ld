OUTPUT_ARCH(riscv)

/**** Start point ****/
hwreset = 0x0;
ENTRY(hwreset)

SECTIONS
{
  .text : {
      *(.text)
  }

  /DISCARD/ : {
    *(.note.gnu.build-id)
    *(.riscv.attributes)
    /* *(.dynsym) */
    /* *(.dynstr) */
    /* *(.gnu.hash) */
    /* *(.dynamic) */
    /* *(.got) */
  }
}
