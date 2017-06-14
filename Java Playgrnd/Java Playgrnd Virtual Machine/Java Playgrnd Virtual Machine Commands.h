

DEFINE_COMMAND
(
  PUSHN, 
  0x10,
  {
    getPUSHNCommand()
  }
)



DEFINE_COMMAND /* 0x11 - 0x17 */
(
  PUSHR,
  0x11,
  {
    getPUSHRCommand(AX) 
  }
)

DEFINE_COMMAND
(
  PUSHR,
  0x12,
  {
    getPUSHRCommand(BX) 
  }
)


DEFINE_COMMAND
(
  PUSHR,
  0x13,
  {
    getPUSHRCommand(CX) 
  }
)


DEFINE_COMMAND
(
  PUSHR,
  0x14,
  {
    getPUSHRCommand(DX) 
  }
)


DEFINE_COMMAND
(
  PUSHR,
  0x15,
  {
    getPUSHRCommand(BP) 
  }
)


DEFINE_COMMAND
(
  PUSHR,
  0x16,
  {
    getPUSHRCommand(SP) 
  }
)


DEFINE_COMMAND
(
  PUSHR,
  0x17,
  {
    getPUSHRCommand(IP) 
  }
)



DEFINE_COMMAND /* 0x19 - 0x1f */
(
  POP,
  0x19,
  { 
    getPOPCommand(AX)
  }
)


DEFINE_COMMAND
(
  POP,
  0x1a,
  {
    getPOPCommand(BX)
  }
)

DEFINE_COMMAND
(
  POP,
  0x1b,
  {
    getPOPCommand(CX)
  }
)

DEFINE_COMMAND
(
  POP,
  0x1c,
  {
    getPOPCommand(DX)
  }
)

DEFINE_COMMAND
(
  POP,
  0x1d,
  {
    getPOPCommand(BP)
  }
)

DEFINE_COMMAND
(
  POP,
  0x1e,
  {
    getPOPCommand(SP)
  }
)

DEFINE_COMMAND
(
  POP,
  0x1f,
  {
    getPOPCommand(IP)
  }
)



DEFINE_COMMAND
(
  ADD,
  0x20,
  {
    RAX += RDX;
  }
)

DEFINE_COMMAND
(
  SUB,
  0x21,
  {
    RAX -= RDX;
  }
)

DEFINE_COMMAND
(
  MUL,
  0x22,
  {
    RAX *= RDX;
  }
)

DEFINE_COMMAND
(
  DIV,
  0x23,
  {
    int64_t t = RAX;
    RAX = RAX / RDX; 
    RDX = t - RAX * RDX;
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x24,
  {
    getMOVRCommand(AX)
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x25,
  {
    getMOVRCommand(BX)
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x26,
  {
    getMOVRCommand(CX)
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x27,
  {
    getMOVRCommand(DX)
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x28,
  {
    getMOVRCommand(BP)
  }
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x29,
  {
    getMOVRCommand(SP)
  }
)


DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2a,
  {
    getMOVNCommand(AX)
  }
)

DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2b,
  {
    getMOVNCommand(BX)
  }
)

DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2c,
  {
    getMOVNCommand(CX)
  }
)

DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2d,
  {
    getMOVNCommand(DX)
  }
)

DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2e,
  {
    getMOVNCommand(BP)
  }
)

DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2f,
  {
    getMOVNCommand(SP)
  }
)





DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x30,
  {
    getMOVDataCommand(AX, unsigned char)
  }
)

DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x31,
  {
    getMOVDataCommand(BX, unsigned char)
  }
)

DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x32,
  {
    getMOVDataCommand(CX, unsigned char)
  }
)


DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x33,
  {
    getMOVDataCommand(DX, unsigned char)
  }
)

DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x34,
  {
    getMOVDataCommand(BP, unsigned char)
  }
)

DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x35,
  {
    getMOVDataCommand(SP, unsigned char)
  }
)




DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x36,
  {
    getMOVDataCommand(AX, short)
  }
)

DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x37,
  {
    getMOVDataCommand(BX, short)
  }
)

DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x38,
  {
    getMOVDataCommand(CX, short)
  }
)

DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x39,
  {
    getMOVDataCommand(DX, short)
  }
)


DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x3a,
  {
    getMOVDataCommand(BP, short)
  }
)

DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x3b,
  {
    getMOVDataCommand(SP, short)
  }
)



DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x3c,
  {
    getMOVDataCommand(AX, int)
  }
)

DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x3d,
  {
    getMOVDataCommand(BX, int)
  }
)

DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x3e,
  {
    getMOVDataCommand(CX, int)
  }
)

DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x3f,
  {
    getMOVDataCommand(DX, int)
  }
)

DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x40,
  {
    getMOVDataCommand(BP, int)
  }
)

DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x41,
  {
    getMOVDataCommand(SP, int)
  }
)


DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x42,
  {
    getMOVDataCommand(AX, int64_t)
  }
)

DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x43,
  {
    getMOVDataCommand(BX, int64_t)
  }
)

DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x44,
  {
    getMOVDataCommand(CX, int64_t)
  }
)


DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x45,
  {
    getMOVDataCommand(DX, int64_t)
  }
)

DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x46,
  {
    getMOVDataCommand(BP, int64_t)
  }
)

DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x47,
  {
    getMOVDataCommand(SP, int64_t)
  }
)


DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x48,
  {
    getWRTDataCommand(AX, unsigned char)
  }
)

DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x49,
  {
    getWRTDataCommand(BX, unsigned char)
  }
)

DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x4a,
  {
    getWRTDataCommand(CX, unsigned char)
  }
)

DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x4b,
  {
    getWRTDataCommand(DX, unsigned char)
  }
)

DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x4c,
  {
    getWRTDataCommand(BP, unsigned char)
  }
)

DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x4d,
  {
    getWRTDataCommand(SP, unsigned char)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x4e,
  {
    getWRTDataCommand(AX, short)
  }
)

DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x4f,
  {
    getWRTDataCommand(BX, short)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x50,
  {
    getWRTDataCommand(CX, short)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x51,
  {
    getWRTDataCommand(DX, short)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x52,
  {
    getWRTDataCommand(BP, short)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x53,
  {
    getWRTDataCommand(SP, short)
  }
)


DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x54,
  {
    getWRTDataCommand(AX, int)
  }
)

DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x55,
  {
    getWRTDataCommand(BX, int)
  }
)

DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x56,
  {
    getWRTDataCommand(CX, int)
  }
)

DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x57,
  {
    getWRTDataCommand(DX, int)
  }
)

DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x58,
  {
    getWRTDataCommand(BP, int)
  }
)

DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x59,
  {
    getWRTDataCommand(SP, int)
  }
)


DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5a,
  {
    getWRTDataCommand(AX, int64_t)
  }
)

DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5b,
  {
    getWRTDataCommand(BX, int64_t)
  }
)

DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5c,
  {
    getWRTDataCommand(CX, int64_t)
  }
)

DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5d,
  {
    getWRTDataCommand(DX, int64_t)
  }
)

DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5e,
  {
    getWRTDataCommand(BP, int64_t)
  }
)

DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5f,
  {
    getWRTDataCommand(SP, int64_t)
  }
)

DEFINE_COMMAND
(
  IN,
  0x60,
  {
    if ( 1 == scanf("%x", &input_number) )
      RAX = input_number;
    else
      onError(WRONG_INPUT_ERROR);
  }
)

DEFINE_COMMAND
(
  OUT,
  0x61,
  {
    printf("%x\n", RAX);
  }
)

DEFINE_COMMAND
(
  RET,
  0x62,
  {
    getPOPCommand(IP)
  }
)

DEFINE_COMMAND
(
  END,
  0x63,
  {
    return 0;
  }
)

DEFINE_COMMAND
(
  CALL,
  0x64,
  {
    RSP -= sizeof(int64_t);
    *(int64_t*)(VRAM + RSP) = RIP + sizeof(int64_t);
    RIP = *(int64_t*)(VRAM + RIP);
  }
)

DEFINE_COMMAND
(
  JMP,
  0x65,
  {
    RIP = *(int64_t*)(VRAM + RIP);
  }
)

DEFINE_COMMAND
(
  JE,
  0x66,
  {
    getOffsetCommand(==)
  }
)

DEFINE_COMMAND
(
  JNE,
  0x67,
  {
    getOffsetCommand(!=)
  }
)

DEFINE_COMMAND
(
  JA,
  0x68,
  {
    getOffsetCommand(>)
  }
)

DEFINE_COMMAND
(
  JAE,
  0x69,
  {
    getOffsetCommand(>=)
  }
)

DEFINE_COMMAND
(
  JB,
  0x6a,
  {
    getOffsetCommand(<)
  }
)

DEFINE_COMMAND
(
  JBE,
  0x6b,
  {
    getOffsetCommand(<=)
  }
)
  
DEFINE_COMMAND
(
  INC,
  0x70,
  {
    RAX++;
  }
)

DEFINE_COMMAND
(
  INC,
  0x71,
  {
    RBX++;
  }
)

DEFINE_COMMAND
(
  INC,
  0x72,
  {
    RCX++;
  }
)

DEFINE_COMMAND
(
  INC,
  0x73,
  {
    RDX++;
  }
)

DEFINE_COMMAND
(
  INC,
  0x74,
  {
    RBP++;
  }
)

DEFINE_COMMAND
(
  INC,
  0x75,
  {
    RSP++;
  }
)

DEFINE_COMMAND
(
  DEC,
  0x76,
  {
    RAX--;
  }
)


DEFINE_COMMAND
(
  DEC,
  0x77,
  {
    RBX--;
  }
)


DEFINE_COMMAND
(
  DEC,
  0x78,
  {
    RCX--;
  }
)


DEFINE_COMMAND
(
  DEC,
  0x79,
  {
    RDX--;
  }
)


DEFINE_COMMAND
(
  DEC,
  0x7A,
  {
    RBP--;
  }
)


DEFINE_COMMAND
(
  DEC,
  0x7B,
  {
    RSP--;
  }
)  
