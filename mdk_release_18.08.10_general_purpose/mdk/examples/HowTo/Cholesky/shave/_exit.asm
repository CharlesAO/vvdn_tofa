  .version 00.50.01

  .code .text._exit
.nowarn
// void __attribute__((noretun)) _exit();

  .lalign
  .type _exit, @function
_exit:
  bru.swih 0x1f
   nop 16
  bru.bra _exit
   nop 6
.nowarnend
  .size _exit, . - _exit

.nowarnend
