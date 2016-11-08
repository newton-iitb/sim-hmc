#ifndef _HMC_REGISTER_H_
#define _HMC_REGISTER_H_

#include <cstdint>
#include <iostream>
#include "config.h"
#include "hmc_macros.h"

class hmc_cube;

#define    HMC_REG_EDR_IDX    0x000000 /* - 0x3 */
#define    HMC_REG_ERR_IDX    0x000004
#define    HMC_REG_GC_IDX     0x000005
#define    HMC_REG_LC_IDX     0x000006 /* - 0x9 */
#define    HMC_REG_LRLL_IDX   0x00000A /* - 0xD */
#define    HMC_REG_LR_IDX     0x00000E /* - 0x11 */
#define    HMC_REG_IBTC_IDX   0x000012 /* - 0x15 */
#define    HMC_REG_AC_IDX     0x000016
#define    HMC_REG_VCR_IDX    0x000017
#define    HMC_REG_FEAT_IDX   0x000018
#define    HMC_REG_RVID_IDX   0x000019

typedef enum{
  HMC_RW,				/*! HMC-SIM: HMC_REG_DEF_T: READ+WRITE REGISTER */
  HMC_RO,				/*! HMC-SIM: HMC_REG_DEF_T: READ-ONLY REGISTER */
  HMC_RWS				/*! HMC-SIM: HMC_REG_DEF_T: CLEAR ON WRITE REGISTER */
} hmc_reg_def_t;

enum _hmc_regslots_e {
/* table 32: External Request Register */
  HMC_REG_ERR__REGISTER_REQUEST_CMD,
  HMC_REG_ERR__TYPE,
  HMC_REG_ERR__TARGET_LOCATION,
  HMC_REG_ERR__SIZE,
  HMC_REG_ERR__EXTERNAL_REQUEST_STATUS,
  HMC_REG_ERR__START,

/* table 33: Global Configuration */
  HMC_REG_GC__STOP_ON_FATAL_ERROR,
  HMC_REG_GC__CLEAR_ERROR,
  HMC_REG_GC__WARM_RESET,

/* table 34: Link Configuration */
  HMC_REG_LC__LINK_MODE,
  HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE,
  HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION,
  HMC_REG_LC__LINK_CRC_DETECTION,
  HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION,
  HMC_REG_LC__PACKET_INPUT_ENABLE,
  HMC_REG_LC__PACKET_OUTPUT_ENABLE,
  HMC_REG_LC__INHIBIT_LINK_DOWN_MODE,
  HMC_REG_LC__LINK_DESCRAMBLE_ENABLE,
  HMC_REG_LC__LINK_SCRAMBLE_ENABLE,
  HMC_REG_LC__ERROR_RESPONSE_PACKET,

/* table 35: Link Run Length Limit */
  HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT,

/* table 36: Link Retry */
  HMC_REG_LR__RETRY_STATUS,
  HMC_REG_LR__RETRY_LIMIT,
  HMC_REG_LR__RETRY_TIMEOUT_PERIOD,
  HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER,
  HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER,

/* table 37: Input Buffer Tocken Count */
  HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX,

/* table 38: Address Configuration */
  HMC_REG_AC__ADDRESS_MAPPING_MODE,
  HMC_REG_AC__USER_DEFINED_VAULT,
  HMC_REG_AC__USER_DEFINED_BANK,

/* table 39: Vault Control Register */
  HMC_REG_VCR__DRAM_INITIALIZATION_MODE,
  HMC_REG_VCR__HARD_SBE_DETECT,
  HMC_REG_VCR__DEMAND_SCRUBBING,
  HMC_REG_VCR__PATROL_SCRUBBING,
  HMC_REG_VCR__PACKET_CRC_DETECTION,
  HMC_REG_VCR__COMMAND_ADDRESS_RETRY_COUNT,
  HMC_REG_VCR__DATA_ECC_CORRECTION,
  HMC_REG_VCR__HARD_SBE_REPAIR,
  HMC_REG_VCR__ENABLE_SBE_REPORT,

/* table 40: Features */
  HMC_REG_FEAT__CUBE_SIZE,
  HMC_REG_FEAT__NUMBER_OF_VAULTS,
  HMC_REG_FEAT__NUMBER_OF_BANKS_PER_VAULT,
  HMC_REG_FEAT__PHY,

/* table 41: Revisions and Vendor ID */
  HMC_REG_RVID__VENDOR_ID,
  HMC_REG_RVID__PRODUCT_REVISION,
  HMC_REG_RVID__PROTOCOL_REVISION,
  HMC_REG_RVID__PHY_REVISION
};
typedef enum _hmc_regslots_e hmc_regslots_e;

typedef struct _hmcsim_reg_decode_fields_t hmcsim_reg_decode_fields_t;
struct _hmcsim_reg_decode_fields_t {
  hmc_regslots_e name;
  unsigned start_bit;
  unsigned size;
  hmc_reg_def_t type;
  unsigned reset_value;
};

class hmc_register {
private:
  hmcsim_reg_decode_fields_t hmcsim_decode_fields[47] = {
  /* table 32: External Request Register */
    { HMC_REG_ERR__REGISTER_REQUEST_CMD, 0, 8, HMC_RW, 0x00 },
    { HMC_REG_ERR__TYPE, 8, 8, HMC_RW, 0x00 },
    { HMC_REG_ERR__TARGET_LOCATION, 16, 6, HMC_RW, 0x00 },
    { HMC_REG_ERR__SIZE, 22, 4, HMC_RW, 0x00 },
    { HMC_REG_ERR__EXTERNAL_REQUEST_STATUS, 26, 5, HMC_RO, 0x00 },
    { HMC_REG_ERR__START, 31, 1, HMC_RW, 0x00 },

  /* table 33: Global Configuration */
    { HMC_REG_GC__STOP_ON_FATAL_ERROR, 4, 1, HMC_RW, 0x00 },
    { HMC_REG_GC__CLEAR_ERROR, 5, 1, HMC_RWS, 0x00 },
    { HMC_REG_GC__WARM_RESET, 6, 1, HMC_RWS, 0x00 },

  /* table 34: Link Configuration */
    { HMC_REG_LC__LINK_MODE, 0, 2, HMC_RW, 0x1 },
    { HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE, 2, 1, HMC_RW, 0x0 },
    { HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION, 3, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__LINK_CRC_DETECTION, 4, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION, 5, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__PACKET_INPUT_ENABLE, 6, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__PACKET_OUTPUT_ENABLE, 7, 1, HMC_RW, 0x0 },
    { HMC_REG_LC__INHIBIT_LINK_DOWN_MODE, 8, 1, HMC_RW, 0x0 },
    { HMC_REG_LC__LINK_DESCRAMBLE_ENABLE, 9, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__LINK_SCRAMBLE_ENABLE, 10, 1, HMC_RW, 0x1 },
    { HMC_REG_LC__ERROR_RESPONSE_PACKET, 11, 1, HMC_RW, 0x1 },

  /* table 35: Link Run Length Limit */
    { HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT, 16, 8, HMC_RW, 0x0 },

  /* table 36: Link Retry */
    { HMC_REG_LR__RETRY_STATUS, 0, 1, HMC_RW, 0x0 },
    { HMC_REG_LR__RETRY_LIMIT, 1, 3, HMC_RW, 0x3 },
    { HMC_REG_LR__RETRY_TIMEOUT_PERIOD, 4, 3, HMC_RW, 0x5 },
    { HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER, 8, 6, HMC_RW, 0x8 },
    { HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER, 16, 6, HMC_RW, 0x10 },

  /* table 37: Input Buffer Tocken Count */
    { HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX, 0, 8, HMC_RW, 0x64 },

  /* table 38: Address Configuration */
    { HMC_REG_AC__ADDRESS_MAPPING_MODE, 0, 4, HMC_RW, 0x2 },
    { HMC_REG_AC__USER_DEFINED_VAULT, 4, 5, HMC_RW, 0x0 },
    { HMC_REG_AC__USER_DEFINED_BANK, 9, 5, HMC_RW, 0x0 },

  /* table 39: Vault Control Register */
    { HMC_REG_VCR__DRAM_INITIALIZATION_MODE, 0, 2, HMC_RW, 0x0 },
    { HMC_REG_VCR__HARD_SBE_DETECT, 2, 1, HMC_RW, 0x0 },
    { HMC_REG_VCR__DEMAND_SCRUBBING, 3, 1, HMC_RW, 0x1 },
    { HMC_REG_VCR__PATROL_SCRUBBING, 4, 1, HMC_RW, 0x1 },
    { HMC_REG_VCR__PACKET_CRC_DETECTION, 5, 1, HMC_RW, 0x1 },
    { HMC_REG_VCR__COMMAND_ADDRESS_RETRY_COUNT, 6, 3, HMC_RW, 0x1 },
    { HMC_REG_VCR__DATA_ECC_CORRECTION, 9, 1, HMC_RW, 0x1 },
    { HMC_REG_VCR__HARD_SBE_REPAIR, 11, 1, HMC_RW, 0x1 },
    { HMC_REG_VCR__ENABLE_SBE_REPORT, 12, 1, HMC_RW, 0x1 },

  /* table 40: Features */
    { HMC_REG_FEAT__CUBE_SIZE, 0, 4, HMC_RO, 0x0 },
    { HMC_REG_FEAT__NUMBER_OF_VAULTS, 4, 4, HMC_RO, 0x0 },
    { HMC_REG_FEAT__NUMBER_OF_BANKS_PER_VAULT, 8, 4, HMC_RO, 0x0 },
    { HMC_REG_FEAT__PHY, 12, 4, HMC_RO, HMC_PHY_SPEED },

  /* table 41: Revisions and Vendor ID */
    { HMC_REG_RVID__VENDOR_ID, 0, 8, HMC_RO, HMC_VENDOR_ID },
    { HMC_REG_RVID__PRODUCT_REVISION, 8, 8, HMC_RO, HMC_PRODUCT_REVISION },
    { HMC_REG_RVID__PROTOCOL_REVISION, 16, 8, HMC_RO, HMC_PROTOCOL_REVISION },
    { HMC_REG_RVID__PHY_REVISION, 24, 8, HMC_RO, HMC_PHY_REVISION },
  };

  struct {
    unsigned reg;
    unsigned idx;
    hmc_regslots_e name;
  } hmcsim_decode[119] = {
  /* table 32: External Request Register */
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__REGISTER_REQUEST_CMD },
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__TYPE },
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__TARGET_LOCATION },
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__SIZE },
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__EXTERNAL_REQUEST_STATUS },
    { HMC_REG_EDR(0), HMC_REG_EDR_IDX, HMC_REG_ERR__START },

    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__REGISTER_REQUEST_CMD },
    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__TYPE },
    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__TARGET_LOCATION },
    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__SIZE },
    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__EXTERNAL_REQUEST_STATUS },
    { HMC_REG_EDR(1), HMC_REG_EDR_IDX + 1, HMC_REG_ERR__START },

    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__REGISTER_REQUEST_CMD },
    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__TYPE },
    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__TARGET_LOCATION },
    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__SIZE },
    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__EXTERNAL_REQUEST_STATUS },
    { HMC_REG_EDR(2), HMC_REG_EDR_IDX + 2, HMC_REG_ERR__START },

    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__REGISTER_REQUEST_CMD },
    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__TYPE },
    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__TARGET_LOCATION },
    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__SIZE },
    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__EXTERNAL_REQUEST_STATUS },
    { HMC_REG_EDR(3), HMC_REG_EDR_IDX + 3, HMC_REG_ERR__START },

  /* table 33: Global Configuration */
    { HMC_REG_GC, HMC_REG_GC_IDX, HMC_REG_GC__STOP_ON_FATAL_ERROR },
    { HMC_REG_GC, HMC_REG_GC_IDX, HMC_REG_GC__CLEAR_ERROR },
    { HMC_REG_GC, HMC_REG_GC_IDX, HMC_REG_GC__WARM_RESET },

  /* table 34: Link Configuration */
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_MODE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_CRC_DETECTION },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__PACKET_INPUT_ENABLE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__PACKET_OUTPUT_ENABLE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__INHIBIT_LINK_DOWN_MODE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_DESCRAMBLE_ENABLE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__LINK_SCRAMBLE_ENABLE },
    { HMC_REG_LC(0), HMC_REG_LC_IDX, HMC_REG_LC__ERROR_RESPONSE_PACKET },

    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_MODE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_CRC_DETECTION },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__PACKET_INPUT_ENABLE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__PACKET_OUTPUT_ENABLE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__INHIBIT_LINK_DOWN_MODE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_DESCRAMBLE_ENABLE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__LINK_SCRAMBLE_ENABLE },
    { HMC_REG_LC(1), HMC_REG_LC_IDX + 1, HMC_REG_LC__ERROR_RESPONSE_PACKET },

    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_MODE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_CRC_DETECTION },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__PACKET_INPUT_ENABLE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__PACKET_OUTPUT_ENABLE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__INHIBIT_LINK_DOWN_MODE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_DESCRAMBLE_ENABLE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__LINK_SCRAMBLE_ENABLE },
    { HMC_REG_LC(2), HMC_REG_LC_IDX + 2, HMC_REG_LC__ERROR_RESPONSE_PACKET },

    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_MODE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_RESPONSE_OPEN_LOOP_MODE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_PACKET_SEQUENCE_DETECTION },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_CRC_DETECTION },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_DUPLICATE_LENGTH_DETECTION },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__PACKET_INPUT_ENABLE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__PACKET_OUTPUT_ENABLE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__INHIBIT_LINK_DOWN_MODE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_DESCRAMBLE_ENABLE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__LINK_SCRAMBLE_ENABLE },
    { HMC_REG_LC(3), HMC_REG_LC_IDX + 3, HMC_REG_LC__ERROR_RESPONSE_PACKET },

  /* table 35: Link Run Length Limit */
    { HMC_REG_LRLL(0), HMC_REG_LRLL_IDX, HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT },
    { HMC_REG_LRLL(1), HMC_REG_LRLL_IDX + 1, HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT },
    { HMC_REG_LRLL(2), HMC_REG_LRLL_IDX + 2, HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT },
    { HMC_REG_LRLL(3), HMC_REG_LRLL_IDX + 3, HMC_REG_LRLL__TRANSMIT_RUN_LENGTH_LIMIT },

  /* table 36: Link Retry */
    { HMC_REG_LR(0), HMC_REG_LR_IDX, HMC_REG_LR__RETRY_STATUS },
    { HMC_REG_LR(0), HMC_REG_LR_IDX, HMC_REG_LR__RETRY_LIMIT },
    { HMC_REG_LR(0), HMC_REG_LR_IDX, HMC_REG_LR__RETRY_TIMEOUT_PERIOD },
    { HMC_REG_LR(0), HMC_REG_LR_IDX, HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER },
    { HMC_REG_LR(0), HMC_REG_LR_IDX, HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER },

    { HMC_REG_LR(1), HMC_REG_LR_IDX + 1, HMC_REG_LR__RETRY_STATUS },
    { HMC_REG_LR(1), HMC_REG_LR_IDX + 1, HMC_REG_LR__RETRY_LIMIT },
    { HMC_REG_LR(1), HMC_REG_LR_IDX + 1, HMC_REG_LR__RETRY_TIMEOUT_PERIOD },
    { HMC_REG_LR(1), HMC_REG_LR_IDX + 1, HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER },
    { HMC_REG_LR(1), HMC_REG_LR_IDX + 1, HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER },

    { HMC_REG_LR(2), HMC_REG_LR_IDX + 2, HMC_REG_LR__RETRY_STATUS },
    { HMC_REG_LR(2), HMC_REG_LR_IDX + 2, HMC_REG_LR__RETRY_LIMIT },
    { HMC_REG_LR(2), HMC_REG_LR_IDX + 2, HMC_REG_LR__RETRY_TIMEOUT_PERIOD },
    { HMC_REG_LR(2), HMC_REG_LR_IDX + 2, HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER },
    { HMC_REG_LR(2), HMC_REG_LR_IDX + 2, HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER },

    { HMC_REG_LR(3), HMC_REG_LR_IDX + 3, HMC_REG_LR__RETRY_STATUS },
    { HMC_REG_LR(3), HMC_REG_LR_IDX + 3, HMC_REG_LR__RETRY_LIMIT },
    { HMC_REG_LR(3), HMC_REG_LR_IDX + 3, HMC_REG_LR__RETRY_TIMEOUT_PERIOD },
    { HMC_REG_LR(3), HMC_REG_LR_IDX + 3, HMC_REG_LR__INIT_RETRY_PACKET_TRANSMIT_NUMBER },
    { HMC_REG_LR(3), HMC_REG_LR_IDX + 3, HMC_REG_LR__INIT_RETRY_PACKET_RECEIVE_NUMBER },

  /* table 37: Input Buffer Tocken Count */
    { HMC_REG_IBTC(0), HMC_REG_IBTC_IDX, HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX },
    { HMC_REG_IBTC(1), HMC_REG_IBTC_IDX + 1, HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX },
    { HMC_REG_IBTC(2), HMC_REG_IBTC_IDX + 2, HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX },
    { HMC_REG_IBTC(3), HMC_REG_IBTC_IDX + 3, HMC_REG_IBTC__LINK_INPUT_BUFFER_MAX },

  /* table 38: Address Configuration */
    { HMC_REG_AC, HMC_REG_AC_IDX, HMC_REG_AC__ADDRESS_MAPPING_MODE },
    { HMC_REG_AC, HMC_REG_AC_IDX, HMC_REG_AC__USER_DEFINED_VAULT },
    { HMC_REG_AC, HMC_REG_AC_IDX, HMC_REG_AC__USER_DEFINED_BANK },

  /* table 39: Vault Control Register */
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__DRAM_INITIALIZATION_MODE },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__HARD_SBE_DETECT },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__DEMAND_SCRUBBING },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__PATROL_SCRUBBING },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__PACKET_CRC_DETECTION },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__COMMAND_ADDRESS_RETRY_COUNT },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__DATA_ECC_CORRECTION },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__HARD_SBE_REPAIR },
    { HMC_REG_VCR, HMC_REG_VCR_IDX, HMC_REG_VCR__ENABLE_SBE_REPORT },

  /* table 40: Features */
    { HMC_REG_FEAT, HMC_REG_FEAT_IDX, HMC_REG_FEAT__CUBE_SIZE },
    { HMC_REG_FEAT, HMC_REG_FEAT_IDX, HMC_REG_FEAT__NUMBER_OF_VAULTS },
    { HMC_REG_FEAT, HMC_REG_FEAT_IDX, HMC_REG_FEAT__NUMBER_OF_BANKS_PER_VAULT },
    { HMC_REG_FEAT, HMC_REG_FEAT_IDX, HMC_REG_FEAT__PHY },

  /* table 41: Revisions and Vendor ID */
    { HMC_REG_RVID, HMC_REG_RVID_IDX, HMC_REG_RVID__VENDOR_ID },
    { HMC_REG_RVID, HMC_REG_RVID_IDX, HMC_REG_RVID__PRODUCT_REVISION },
    { HMC_REG_RVID, HMC_REG_RVID_IDX, HMC_REG_RVID__PROTOCOL_REVISION },
    { HMC_REG_RVID, HMC_REG_RVID_IDX, HMC_REG_RVID__PHY_REVISION }
  };

  hmc_cube *cube;
  uint32_t regs[ HMC_NUM_REGS ];              /*! HMC-SIM: HMC_DEV_T: DEVICE CONFIGURATION REGISTERS */


  int hmcsim_get_decode_idx(unsigned reg);
  bool hmcsim_get_decode_field(hmc_regslots_e name, hmcsim_reg_decode_fields_t **field);
  int hmcsim_reg_value_set_internal(unsigned reg_addr, hmc_regslots_e slot,
                                    uint64_t value, bool force_write);
public:
  hmc_register(hmc_cube *cube);
  ~hmc_register(void);

  ALWAYS_INLINE int hmcsim_reg_value_reset(unsigned reg, hmc_regslots_e slot, uint64_t value)
  {
    return hmcsim_reg_value_set_internal(reg, slot, value, true);
  }
  ALWAYS_INLINE int hmcsim_reg_value_set(unsigned reg, hmc_regslots_e slot, uint64_t value)
  {
    return hmcsim_reg_value_set_internal(reg, slot, value, false);
  }
  int hmcsim_reg_value_set_full(unsigned reg, uint64_t value);
  int hmcsim_reg_value_get(unsigned reg, hmc_regslots_e slot, uint64_t *value);
  int hmcsim_reg_value_get_full(unsigned reg, uint64_t *value);

  ALWAYS_INLINE int hmcsim_util_get_num_banks_per_vault(void)
  {
    uint64_t n;
    if (this->hmcsim_reg_value_get(HMC_REG_FEAT, HMC_REG_FEAT__NUMBER_OF_BANKS_PER_VAULT, &n))
      return -1;

    return (int)(1 << (n + 3));
  }

  ALWAYS_INLINE int hmcsim_util_get_bsize(void)
  {
    uint64_t ret;
    if (this->hmcsim_reg_value_get(HMC_REG_AC, HMC_REG_AC__ADDRESS_MAPPING_MODE, &ret))
      return -1;

    return this->hmcsim_util_decode_bsize((unsigned)ret);
  }

  int hmcsim_util_decode_bsize(unsigned value);
};

#endif /* #ifndef _HMC_REGISTER_H_ */
