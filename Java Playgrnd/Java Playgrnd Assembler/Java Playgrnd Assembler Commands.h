
DEFINE_COMMAND
(
  PUSHN, 
  0x10,
  {
    if ( 1 == sscanf(input_buffer + input_pos, "%x%n", &input_number, &input_length) )
      {
        input_pos += input_length;

        *(int64_t*)(output_buffer + output_pos) = input_number;

        output_pos += sizeof(int64_t);
      }
    else
      onError(PUSHN_ERROR);
  }
)



DEFINE_COMMAND /* 0x11 - 0x17 */
(
  PUSHR,
  0x11,
  {
    if ( 1 == sscanf(input_buffer + input_pos, "%s%n", input_string, &input_length) )
      {
        input_pos += input_length;

        if ( !(register_code = findRegister(input_string)) )
          onError(UNKNOWN_REGISTER);
        else 
          *(unsigned char*)(output_buffer + output_pos - 1 ) = register_code;
      }
    else 
      onError(PUSHR_ERROR);
  }
)



DEFINE_COMMAND
(
  OFFSET,
  0x18,
  {
    if ( 1 == sscanf(input_buffer + input_pos, "%s%n", input_string, &input_length) )
      {
        input_pos += input_length;
        
        *(unsigned char*)(output_buffer + output_pos - 1) = 0x2a; // MOVN

        addLabelRequest(&list, input_string, output_pos);

        output_pos += sizeof(int64_t);
      }
    else
      onError(OFFSET_ERROR);
  }
)


DEFINE_COMMAND /* 0x19 - 0x1f */
(
  POP,
  0x19,
  { 
    if ( 1 == sscanf(input_buffer + input_pos, "%s%n", input_string, &input_length) )
      {
        input_pos += input_length;

        if ( register_code = findRegister(input_string)  )
          *(unsigned char*)(output_buffer + output_pos - 1) = register_code + 8;
        else 
          onError(UNKNOWN_REGISTER);
      }
    else 
      onError(POP_ERROR);
  }
)



DEFINE_COMMAND
(
  ADD,
  0x20,
  {
  }
)

DEFINE_COMMAND
(
  SUB,
  0x21,
  {}
)

DEFINE_COMMAND
(
  MUL,
  0x22,
  {}
)

DEFINE_COMMAND
(
  DIV,
  0x23,
  {}
)

DEFINE_COMMAND /*0x24 - 0x29 */
(
  MOVR,
  0x24,
  {
    getBinaryCommand(MOVR, 0x24 - 0x11)
  }
)


DEFINE_COMMAND /* 0x2a - 0x2f */
(
  MOVN,
  0x2a,
  {
    if ( 2 == sscanf(input_buffer + input_pos, "%s %x%n", input_string, &input_number, &input_length) )
      {
        input_pos += input_length;

        if ( register_code = findRegister(input_string) )
          *(unsigned char*)(output_buffer + output_pos - 1) = register_code + 0x19;
        else
          onError(UNKNOWN_REGISTER);

        *(int64_t*)(output_buffer + output_pos) = input_number;

        output_pos += sizeof(int64_t);  
      }
    else 
      onError(MOVN_ERROR);
  }
)



DEFINE_COMMAND /* 0x30 - 0x35 */
(
  MOVB,
  0x30,
  {
    getBinaryCommand(MOVB, 0x30 - AX)
  }
)


DEFINE_COMMAND /* 0x36 - 0x3b */
(
  MOVW,
  0x36,
  {
    getBinaryCommand(MOVW, 0x36 - AX)
  }
)


DEFINE_COMMAND /* 0x3c - 0x41 */
(
  MOVD,
  0x3c,
  {
    getBinaryCommand(MOVD, 0x3c -  AX)
  }
)


DEFINE_COMMAND /* 0x42 - 0x47 */
(
  MOVQ,
  0x42,
  {
    getBinaryCommand(MOVQ, 0x42 - AX)
  }
)


DEFINE_COMMAND  /* 0x48 - 0x4d */
(
  WRTB,
  0x48,
  {
    getBinaryCommand(WRTB, 0x48 - AX)
  }
)


DEFINE_COMMAND  /* 0x4e - 0x53 */
(
  WRTW,
  0x4e,
  {
    getBinaryCommand(WRTW, 0x4e - AX)
  }
)


DEFINE_COMMAND  /* 0x54 - 0x59 */
(
  WRTD,
  0x54,
  {
    getBinaryCommand(WRTD, 0x54 - AX)
  }
)


DEFINE_COMMAND  /* 0x5a - 0x5f */
(
  WRTQ,
  0x5a,
  {
    getBinaryCommand(WRTQ, 0x5a - AX)
  }
)

DEFINE_COMMAND
(
  IN,
  0x60,
  {}
)

DEFINE_COMMAND
(
  OUT,
  0x61,
  {}
)

DEFINE_COMMAND
(
  RET,
  0x62,
  {}
)

DEFINE_COMMAND
(
  END,
  0x63,
  {}
)

DEFINE_COMMAND
(
  CALL,
  0x64,
  {
    getLabelCommand(CALL)
  }
)

DEFINE_COMMAND
(
  JMP,
  0x65,
  {
    getLabelCommand(JMP)
  }
)

DEFINE_COMMAND
(
  JE,
  0x66,
  {
    getLabelCommand(JE)
  }
)

DEFINE_COMMAND
(
  JNE,
  0x67,
  {
    getLabelCommand(JNE)
  }
)

DEFINE_COMMAND
(
  JA,
  0x68,
  {
    getLabelCommand(JA)
  }
)

DEFINE_COMMAND
(
  JAE,
  0x69,
  {
    getLabelCommand(JAE)
  }
)

DEFINE_COMMAND
(
  JB,
  0x6a,
  {
    getLabelCommand(JB)
  }
)

DEFINE_COMMAND
(
  JBE,
  0x6b,
  {
    getLabelCommand(JBE)
  }
)

DEFINE_COMMAND
(
  DB,
  0x6c,
  {
    getDataCommand(DB, unsigned char)
  }
)

DEFINE_COMMAND
(
  DW,
  0x6d,
  {
    getDataCommand(DW, short)
  }
)

DEFINE_COMMAND
(
  DD,
  0x6e,
  {
    getDataCommand(DD, int)
  }
)

DEFINE_COMMAND
(
  DQ,
  0x6f,
  {
    getDataCommand(DQ, int64_t)
  }
)



