#ifndef _HMC_SIM_T_H_
#define _HMC_SIM_T_H_

typedef enum{
  RD_RS     = 0x38,	/*! HMC-SIM: HMC_RESPONSE_T: READ RESPONSE */
  WR_RS     = 0x39,	/*! HMC-SIM: HMC_RESPONSE_T: WRITE RESPONSE */
  MD_RD_RS  = 0x3A,	/*! HMC-SIM: HMC_RESPONSE_T: MODE READ RESPONSE */
  MD_WR_RS  = 0x3B,	/*! HMC-SIM: HMC_RESPONSE_T: MODE WRITE RESPONSE */
  RSP_ERROR = 0x3E,	/*! HMC-SIM: HMC_RESPONSE_T: ERROR RESPONSE */
  RSP_NONE  = 0x00,	/*! HMC-SIM: HMC_RESPONSE_T: NO RESPONSE COMMAND */ // not really defined, but let's take it
  RSP_CMC				/*! HMC-SIM: HMC_RESPONSE_T: CUSTOM CMC RESPONSE */
}hmc_response_t;

typedef enum{
  WR16      = 0x08,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE WRITE REQUEST */
  WR32      = 0x09,	/*! HMC-SIM: HMC_RQST_T: 32-BYTE WRITE REQUEST */
  WR48      = 0x0A,	/*! HMC-SIM: HMC_RQST_T: 48-BYTE WRITE REQUEST */
  WR64      = 0x0B,	/*! HMC-SIM: HMC_RQST_T: 64-BYTE WRITE REQUEST */
  WR80      = 0x0C,	/*! HMC-SIM: HMC_RQST_T: 80-BYTE WRITE REQUEST */
  WR96      = 0x0D,	/*! HMC-SIM: HMC_RQST_T: 96-BYTE WRITE REQUEST */
  WR112     = 0x0E,	/*! HMC-SIM: HMC_RQST_T: 112-BYTE WRITE REQUEST */
  WR128     = 0x0F,	/*! HMC-SIM: HMC_RQST_T: 128-BYTE WRITE REQUEST */
  MD_WR     = 0x10,	/*! HMC-SIM: HMC_RQST_T: MODE WRITE REQUEST */
  BWR       = 0x11,	/*! HMC-SIM: HMC_RQST_T: BIT WRITE REQUEST */
  TWOADD8   = 0x12,	/*! HMC-SIM: HMC_RQST_T: DUAL 8-byte ADD IMMEDIATE */
  ADD16     = 0x13,	/*! HMC-SIM: HMC_RQST_T: SINGLE 16-byte ADD IMMEDIATE */
  P_WR16    = 0x18,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE POSTED WRITE REQUEST */
  P_WR32    = 0x19,	/*! HMC-SIM: HMC_RQST_T: 32-BYTE POSTED WRITE REQUEST */
  P_WR48    = 0x1A,	/*! HMC-SIM: HMC_RQST_T: 48-BYTE POSTED WRITE REQUEST */
  P_WR64    = 0x1B,	/*! HMC-SIM: HMC_RQST_T: 64-BYTE POSTED WRITE REQUEST */
  P_WR80    = 0x1C,	/*! HMC-SIM: HMC_RQST_T: 80-BYTE POSTED WRITE REQUEST */
  P_WR96    = 0x1D,	/*! HMC-SIM: HMC_RQST_T: 96-BYTE POSTED WRITE REQUEST */
  P_WR112   = 0x1E,	/*! HMC-SIM: HMC_RQST_T: 112-BYTE POSTED WRITE REQUEST */
  P_WR128   = 0x1F,	/*! HMC-SIM: HMC_RQST_T: 128-BYTE POSTED WRITE REQUEST */
  P_BWR     = 0x21,	/*! HMC-SIM: HMC_RQST_T: POSTED BIT WRITE REQUEST */
  P_2ADD8   = 0x22,	/*! HMC-SIM: HMC_RQST_T: POSTED DUAL 8-BYTE ADD IMMEDIATE */
  P_ADD16   = 0x23,	/*! HMC-SIM: HMC_RQST_T: POSTED SINGLE 16-BYTE ADD IMMEDIATE */
  RD16      = 0x30,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE READ REQUEST */
  RD32      = 0x31,	/*! HMC-SIM: HMC_RQST_T: 32-BYTE READ REQUEST */
  RD48      = 0x32,	/*! HMC-SIM: HMC_RQST_T: 48-BYTE READ REQUEST */
  RD64      = 0x33,	/*! HMC-SIM: HMC_RQST_T: 64-BYTE READ REQUEST */
  RD80      = 0x34,	/*! HMC-SIM: HMC_RQST_T: 80-BYTE READ REQUEST */
  RD96      = 0x35,	/*! HMC-SIM: HMC_RQST_T: 96-BYTE READ REQUEST */
  RD112     = 0x36,	/*! HMC-SIM: HMC_RQST_T: 112-BYTE READ REQUEST */
  RD128     = 0x37,	/*! HMC-SIM: HMC_RQST_T: 128-BYTE READ REQUEST */
  RD256     = 0x77,	/*! HMC-SIM: HMC_RQST_T: 256-BYTE READ REQUEST */
  MD_RD     = 0x28,	/*! HMC-SIM: HMC_RQST_T: MODE READ REQUEST */
  FLOW_NULL = 0x00,	/*! HMC-SIM: HMC_RQST_T: NULL FLOW CONTROL */
  PRET      = 0x01,	/*! HMC-SIM: HMC_RQST_T: RETRY POINTER RETURN FLOW CONTROL */
  TRET      = 0x02,	/*! HMC-SIM: HMC_RQST_T: TOKEN RETURN FLOW CONTROL */
  IRTRY     = 0x03,	/*! HMC-SIM: HMC_RQST_T: INIT RETRY FLOW CONTROL */

  /* -- version 2.0 Command Additions */
  WR256     = 0x4F,	/*! HMC-SIM: HMC_RQST_T: 256-BYTE WRITE REQUEST */
  P_WR256   = 0x5F,	/*! HMC-SIM: HMC_RQST_T: 256-BYTE POSTED WRITE REQUEST */
  TWOADDS8R = 0x52,	/*! HMC-SIM: HMC_RQST_T: */
  ADDS16R   = 0x53,	/*! HMC-SIM: HMC_RQST_T: */
  INC8      = 0x50,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC INCREMENT */
  P_INC8    = 0x54,	/*! HMC-SIM: HMC_RQST_T: POSTED 8-BYTE ATOMIC INCREMENT */
  XOR16     = 0x40,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC XOR */
  OR16      = 0x41,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC OR */
  NOR16     = 0x42,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC NOR */
  AND16     = 0x43,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC AND */
  NAND16    = 0x44,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC NAND */
  CASGT8    = 0x60,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF GT */
  CASGT16   = 0x62,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF GT */
  CASLT8    = 0x61,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF LT */
  CASLT16   = 0x63,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF LT */
  CASEQ8    = 0x64,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF EQ */
  CASZERO16 = 0x65,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF ZERO */
  EQ8       = 0x69,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC EQUAL */
  EQ16      = 0x68,	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC EQUAL */
  BWR8R     = 0x51,	/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC BIT WRITE WITH RETURN */
  SWAP16    = 0x6A	/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC SWAP */
} hmc_rqst_t;

#endif /* #ifndef _HMC_SIM_T_H_ */
