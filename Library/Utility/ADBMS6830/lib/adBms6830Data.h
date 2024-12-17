/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    adBms6830Data.h
* @brief:   Generic bms data structure header file
* @version: $Revision$
* @date:    $Date$
* Developed by: ADIBMS Software team, Bangalore, India
*****************************************************************************/

/*! @addtogroup BMS DRIVER
*  @{
*

/*! @addtogroup BMS_DATA BMS DATA
*  @{
*
*/

#ifndef __ADBMSDATA_H
#define __ADBMSDATA_H

#include "common.h"

#define CELL 18                 /* Bms ic number of cell              */
#define AUX  10                 /* Bms ic number of Aux               */
#define RAUX 10                 /* Bms ic number of RAux              */
#define PWMA 12                 /* Bms ic number of PWMA              */
#define PWMB 4                  /* Bms ic number of PWMB              */
#define COMM 3                  /* GPIO communication comm reg        */
#define RSID 6                  /* Bms ic number of SID byte          */
#define TX_DATA 6               /* Bms tx data byte                   */
#define RX_DATA 8               /* Bms rx data byte                   */
#define RDCVALL_SIZE    34      /* RDCVALL data byte size             */
#define RDSALL_SIZE     34      /* RDSALL data byte size              */
#define RDACALL_SIZE    34      /* RDACALL data byte size             */
#define RDFCALL_SIZE    34      /* RDFCALL data byte size             */
#define RDCSALL_SIZE    66      /* RDCSALL data byte size             */
#define RDASALL_SIZE    70      /* RDASALL data byte size             */
#define RDACSALL_SIZE   66      /* RDACSALL data byte size            */

#define VR_SIZE 12              /*!< Bms ic number of Voltage Resister  */
#define VRX_SIZE 6              /*!< Bms ic number of Voltage Resister X */
#define RVR_SIZE 6              /*!< Bms ic number of Redundant Voltage Resister  */
#define COMM 3                  /*!< communication comm reg byte size   */
#define RSID 6                  /*!< Bms ic number of SID byte          */
#define TX_DATA 6               /*!< Bms tx data byte                   */
#define RX_DATA 8               /*!< Bms rx data byte                   */
#define RDALLA_SIZE    34       /*!< RDALLA data byte size              */
#define RDALLB_SIZE    34       /*!< RDALLB data byte size              */
#define RDCALL_SIZE    34       /*!< RDCALL data byte size              */
// #define RDASALL_SIZE   16       /*!< RDASALL data byte size             */
#define ALLVR_SIZE     24       /*!< ALL Voltage Reg. byte size         */
#define ALLREDVR_SIZE  12       /*!< ALL Redundant Voltage Reg. byte size*/


/* For ADBMS6830 config register structure */
typedef struct
{
  uint8_t       refon        :1;
  uint8_t       vs1          :2;
  uint8_t       vs10         :1;
  uint8_t       cth          :3;
  uint8_t       soak         :3;
  uint8_t       vs6          :1;
  uint8_t       gpio         :4;
  uint8_t       gpo_2950     :6;
  uint8_t       snap_st      :1;
  uint8_t       comm_bk      :1;
  uint8_t       vs5          :1;
  uint8_t       vs4          :1;
  uint8_t       vs3          :1;
  uint8_t       vs7          :1;
  uint8_t       flag_d       :8;
  uint8_t       soakon       :1;
  uint8_t       owrng        :1;
  uint8_t       owa          :3;
  uint16_t      gpo          :10;
  uint8_t       snap         :1;
  uint8_t       mute_st      :1;
  uint8_t       fc           :3;
  uint8_t       dcc          :2;
}cfa_;

/* For ADBMS6830 config register structure */
typedef struct
{
  uint16_t 	vuv     :16;
  uint16_t 	vov     :16;
  uint8_t 	dtmen   :1;
  uint8_t 	dtrng   :1;
  uint8_t 	dcto    :6;
  uint16_t 	dcc     :16;      // how will the balancing command behave when the cell measurement channels are greater than 16 like in ADBMS6832?
/*ADBMS2950*/
  uint16_t 	oc1th   :7;
  uint16_t 	oc2th   :7;
  uint8_t 	vs2     :2;
  uint8_t 	dg1th   :3;
  uint8_t 	dg2th   :3;
  uint8_t 	vs9     :1;
  uint8_t 	vs8     :1;
}cfb_;

/* Cell Voltage Data structure */
typedef struct
{
  int16_t c_codes[CELL]; /* Cell Voltage Codes */
} cv_;

typedef struct
{
  int16_t ac_codes[CELL]; /* Average Cell Voltage Codes */
} acv_;

/* S Voltage Data structure */
typedef struct
{
  int16_t sc_codes[CELL]; /* S Voltage Codes */
} scv_;

/* Filtered Cell Voltage Data structure */
typedef struct
{
  int16_t fc_codes[CELL]; /* filtered Cell Voltage Codes */
} fcv_;

/* Aux Voltage Data Structure*/
typedef struct
{
  int16_t a_codes[AUX]; /* Aux Voltage Codes */
} ax_;

/* Redundant Aux Voltage Data Structure*/
typedef struct
{
  int16_t ra_codes[RAUX]; /* Aux Voltage Codes */
} rax_;

/* Status A register Data structure*/
typedef struct
{
  uint16_t 	vref2;
  uint16_t 	itmp;
  // uint16_t 	vref3;
  uint8_t oc1min;
  uint8_t oc1max;
  uint8_t oc2min;
  uint8_t oc2max;
  uint16_t vref1p25;
  uint16_t vreg2;
} sta_;

/* Status B register Data structure*/
typedef struct
{
  uint16_t vd;
  uint16_t va;
  uint16_t vr4k;
  uint8_t oc1min;
  uint8_t oc1max;
  uint8_t oc2min;
  uint8_t oc2max;
} stb_;

/* Status C register Data structure*/
typedef struct
{
  uint16_t      cs_flt;
  uint8_t       va_ov   :1;
  uint8_t       va_uv   :1;
  uint8_t       vd_ov   :1;
  uint8_t       vd_uv   :1;
  uint8_t       otp1_ed :1;
  uint8_t       otp1_med:1;
  uint8_t       otp2_ed :1;
  uint8_t       otp2_med:1;
  uint8_t       vde     :1;
  uint8_t       vdel    :1;
  uint8_t       comp    :1;
  uint8_t       spiflt  :1;
  uint8_t       sleep   :1;
  uint8_t       thsd    :1;
  uint8_t       tmodchk :1;
  uint8_t       oscchk  :1;

  uint8_t oc1a           :1;
  uint8_t oc1a_inv       :1;
  uint8_t oc2a           :1;
  uint8_t oc2a_inv       :1;
  uint8_t cts            :2;
  uint16_t ct            :11;
  uint8_t insync         :1;

} stc_;

/* ClrFlag register Data structure*/
typedef struct
{
  uint16_t      cl_csflt;
  uint8_t       cl_smed   :1;
  uint8_t       cl_sed    :1;
  uint8_t       cl_cmed   :1;
  uint8_t       cl_ced    :1;
  uint8_t       cl_vduv   :1;
  uint8_t       cl_vdov   :1;
  uint8_t       cl_vauv   :1;
  uint8_t       cl_vaov   :1;
  uint8_t       cl_oscchk :1;
  uint8_t       cl_tmode  :1;
  uint8_t       cl_thsd   :1;
  uint8_t       cl_sleep  :1;
  uint8_t       cl_spiflt :1;
  uint8_t       cl_vdel   :1;
  uint8_t       cl_vde    :1;

/*ADBMS2950*/
  uint8_t  cl_oc1a      :1;
  uint8_t  cl_oc1m      :1;
  uint8_t  cl_oc2a      :1;
  uint8_t  cl_oc2m      :1;

  uint8_t  cl_opt2_med  :1;
  uint8_t  cl_opt2_ed   :1;
  uint8_t  cl_opt1_med  :1;
  uint8_t  cl_opt1_ed   :1;
} clrflag_;


/*!< ADBMS2950 Current Register Data structure */
typedef struct
{
  uint32_t i1;
  uint32_t i2;
} crg_;


/*!< ADBMS2950 Battery Voltage Register Data structure */
typedef struct
{
  uint16_t vbat1;
  uint16_t vbat2;
} vbat_;

/*!< ADBMS2950 Current and Battery Voltage Register Data structure */
typedef struct
{
  uint32_t i1;
  uint16_t vbat1;
} i_vbat_;

/*!< ADBMS2950 Overcurrent ADC Register Data structure */
typedef struct
{
  uint8_t oc1r;
  uint8_t oc2r;
} oc_;

/*!< ADBMS2950 Average Current Register Data structure */
typedef struct
{
  uint32_t i1avg;
  uint32_t i2avg;
} iavg_;

/*!< ADBMS2950 Average Batter Voltage Register Data structure */
typedef struct
{
  uint32_t vb1avg;
  uint32_t vb2avg;
} vbavg_;

/*!< ADBMS2950 Average Batter Current and Voltage Register Data structure */
typedef struct
{
  uint32_t i1avg;
  uint32_t vb1avg;
} i_vbavg_;

/*!< ADBMS2950 Voltage Register Data structure */
typedef struct
{
  uint16_t v_codes[VR_SIZE];
} vr_;

/*!< ADBMS2950 Voltage Register X Data structure */
typedef struct
{
  uint16_t vx_codes[VRX_SIZE];
} vrx_;

/*!< ADBMS2950 Redundant Voltage Register Data structure */
typedef struct
{
  uint16_t redv_codes[RVR_SIZE];
} rvr_;

/*!< ADBMS2950 Status A Register Data structure */
typedef struct
{
  uint16_t vref1p25;
  uint16_t itmp;
  uint16_t vreg2;
} stata_;

/*!< ADBMS2950 Status B Register Data structure */
typedef struct
{
  uint8_t oc1min;
  uint8_t oc1max;
  uint8_t oc2min;
  uint8_t oc2max;
}statb_;

/*!< ADBMS2950 Status C Register Data structure */
typedef struct
{
  uint8_t oc1a           :1;
  uint8_t oc1a_inv       :1;
  uint8_t oc2a           :1;
  uint8_t oc2a_inv       :1;

  uint8_t cts           :2;
  uint16_t ct           :11;

  uint8_t va_ov         :1;
  uint8_t va_uv         :1;
  uint8_t vd_ov         :1;
  uint8_t vd_uv         :1;
  uint8_t otp1_ed       :1;
  uint8_t otp1_med      :1;
  uint8_t otp2_ed       :1;
  uint8_t otp2_med      :1;

  uint8_t vde           :1;
  uint8_t vdel          :1;
  uint8_t insync        :1;
  uint8_t spiflt        :1;
  uint8_t sleep         :1;
  uint8_t thsd          :1;
  uint8_t tmodchk       :1;
  uint8_t oscchk        :1;
} statc_;

/*!< ADBMS2950 Status D Register Data structure */
typedef struct
{
  uint8_t oc_cntr;
} statd_;

/*!< ADBMS2950 Status E Register Data structure */
typedef struct
{
  uint8_t gpio  :4;
  uint8_t gpo_2950   :6;
  uint8_t rev   :4;
} state_;

/* Status D register Data structure*/
typedef struct
{
  uint8_t c_ov[CELL];
  uint8_t c_uv[CELL];
  uint8_t ct            :6;
  uint8_t cts           :2;
  uint8_t oc_cntr;
} std_;

/* Status E register Data structure*/
typedef struct
{
  uint16_t gpi          :10;
  uint8_t rev           :4;
  uint8_t gpio          :4;
  uint8_t gpo_2950   :6;
} ste_;

/* Pwm register Data structure*/
typedef struct
{
  uint8_t pwma[PWMA];
} pwma_;

/*PWMB Register Structure */
typedef struct
{
  uint8_t pwmb[PWMB];
} pwmb_;

/* COMM register Data structure*/
typedef struct
{
  uint8_t fcomm[COMM];
  uint8_t icomm[COMM];
  uint8_t data[COMM];
} com_;

/*SID Register Structure */
typedef struct
{
  uint8_t sid[RSID];
} sid_;

/* Transmit byte and recived byte data structure */
typedef struct
{
  uint8_t tx_data[TX_DATA];
  uint8_t rx_data[RX_DATA];
} ic_register_;

/* Command counter and pec error data Structure */
typedef struct
{
  uint8_t cmd_cntr;
  uint8_t cfgr_pec;
  uint8_t cell_pec;
  uint8_t acell_pec;
  uint8_t scell_pec;
  uint8_t fcell_pec;
  uint8_t aux_pec;
  uint8_t raux_pec;
  uint8_t stat_pec;
  uint8_t comm_pec;
  uint8_t pwm_pec;
  uint8_t sid_pec;

  uint8_t cr_pec;
  uint8_t vbat_pec;
  uint8_t ivbat_pec;
  uint8_t oc_pec;
  uint8_t avgcr_pec;
  uint8_t avgvbat_pec;
  uint8_t avgivbat_pec;
  uint8_t vr_pec;
  uint8_t vrx_pec;
  uint8_t rvr_pec;

} cmdcnt_pec_;

/* Diagnostic test result data structure */
typedef struct
{
  uint8_t osc_mismatch;
  uint8_t supply_error;
  uint8_t supply_ovuv;
  uint8_t thsd;
  uint8_t fuse_ed;
  uint8_t fuse_med;
  uint8_t tmodchk;
  uint8_t cell_ow[CELL];
  uint8_t cellred_ow[CELL];
  uint8_t aux_ow[(AUX-2)];
} diag_test_;

/* Aux open wire data structure */
typedef struct
{
  int cell_ow_even[CELL];
  int cell_ow_odd[CELL];
} cell_ow_;

/* Aux open wire data structure */
typedef struct
{
  int aux_pup_up[(AUX-2)];
  int aux_pup_down[(AUX-2)];
} aux_ow_;

/* BMS ic main structure */
typedef struct
{
  cfa_ tx_cfga;
  cfa_ rx_cfga;
  cfb_ tx_cfgb;
  cfb_ rx_cfgb;
  clrflag_ clflag;
  cv_  cell;
  acv_ acell;
  scv_ scell;
  fcv_ fcell;
  ax_  aux;
  rax_ raux;
  sta_ stata;
  stb_ statb;
  stc_ statc;
  std_ statd;
  ste_ state;
  com_ comm;
  pwma_ PwmA;
  pwmb_ PwmB;
  sid_ sid;
  ic_register_ configa;
  ic_register_ configb;
  ic_register_ clrflag;
  ic_register_ stat;
  ic_register_ com;
  ic_register_ pwma;
  ic_register_ pwmb;
  ic_register_ sta;
  ic_register_ stb;
  ic_register_ stc;
  ic_register_ std;
  ic_register_ ste;
  ic_register_ rsid;
  ic_register_ reg;

  cmdcnt_pec_ cccrc;
  aux_ow_ gpio;
  cell_ow_ owcell;
  diag_test_ diag_result;

/*ADBMS2950*/
  crg_ i;
  iavg_ iavg;
  vbat_ vbat;
  vbavg_ vbavg;
  i_vbat_ ivbat;
  i_vbavg_ i_vbavg;
  vr_  vr;
  vrx_ vrx;
  rvr_ rvr;
  oc_ oc;

  com_ tx_comm;
  com_ rx_comm;
} cell_asic;

/*!
*  \enum Loop Measurement ENABLED or DISABLED.
*/
typedef enum { DISABLED = 0X0, ENABLED = 0X1} LOOP_MEASURMENT;

/**************************************** CMDEnums *************************************************/
/*!-
*  \enum GPIO CHANNEL
* CH: GPIO Channels.
*/
/* Channel selection */
typedef enum
{
  // AUX_ALL = 0,
  GPIO1 = 0,
  GPIO2,
  GPIO3,
  GPIO4,
  GPIO5,
  GPIO6,
  GPIO7,
  GPIO8,
  GPIO9,
  GPIO10,
  V_POSTIVE_2_NAGATIVE,
  V_NAGATIVE,
  VR4K,
  VREF3
}CH;

/*!
*  \enum RD
* RD: Read Device.
*/
typedef enum { RD_OFF = 0X0, RD_ON = 0X1} RD;

/*!
*  \enum CONT
* CONT: Continuous or single measurement.
*/
/* Continuous or single measurement */
typedef enum { SINGLE = 0X0, CONTINUOUS = 0X1} CONT;

/*!
*  \enum OW_C_S
* OW_C_S: Open wire c/s.
*/
/* Open wire c/s adcs */
typedef enum { OW_OFF_ALL_CH = 0X0, OW_ON_EVEN_CH, OW_ON_ODD_CH, OW_ON_ALL_CH} OW_C_S;

/*!
*  \enum OW_AUX
* OW_AUX: Open wire Aux.
*/
/* Open wire AUX */
typedef enum { AUX_OW_OFF = 0X0, AUX_OW_ON = 0X1} OW_AUX;

/*!
*  \enum OPEN WIRE
* OW: OPEN WIRE.
*/
typedef enum { OW_OFF = 0X0, OW_SRC_P_SNK_N, OW_SRC_N_SNK_P, OW_OFF_} OW;


/*!
*  \enum PUP
* PUP: Pull Down current during aux conversion.
*/
/* Pull Down current during aux conversion (if OW = 1) */
typedef enum { PUP_DOWN = 0X0, PUP_UP = 0X1 } PUP;

/*!
*  \enum DCP
* DCP: Discharge permitted.
*/
/* Discharge permitted */
typedef enum { DCP_OFF = 0X0, DCP_ON = 0X1} DCP;

/*!
*  \enum RSTF
* RSTF: Reset Filter.
*/
/* Reset filter */
typedef enum  { RSTF_OFF = 0x0, RSTF_ON = 0x1 } RSTF;

/*!
*  \enum ERR
* ERR: Inject error is spi read out.
*/
/* Inject error is spi read out */
typedef enum  { WITHOUT_ERR = 0x0, WITH_ERR = 0x1 } ERR;


/*!
*  \enum SOAK
* SOAK: Enables soak on V- ADCs
*/
typedef enum
{
  SOAK_DISABLE = 0,
  SOAK_100us,
  SOAK_500us,
  SOAK_1ms,
  SOAK_2ms,
  SOAK_10ms,
  SOAK_20ms,
  SOAK_150ms
}SOAK;


/**************************************** Mem bits *************************************************/
/* Configuration Register A */

/*!
*  \enum REFON
* REFON: Refernece remains power up/down.
*/
/* Refernece remains power up/down */
typedef enum  { PWR_DOWN = 0x0, PWR_UP = 0x1 } REFON;

/*!
*  \enum CTH
* CTH: Comparison voltages threshold C vs S.
*/
/* Comparison voltages threshold C vs S*/
typedef enum
{
  CVT_5_1mV = 0,        /* 5.1mV                */
  CVT_8_1mV,            /* 8.1mV (Default)      */
  CVT_10_05mV,          /* 10.05mV              */
  CVT_15mV,             /* 15mV                 */
  CVT_22_5mV,           /* 22.5mV               */
  CVT_45mV,             /* 45mV                 */
  CVT_75mV,             /* 75mV                 */
  CVT_135mV,            /* 135mV                */
}CTH;

/*!
*  \enum FLAG_D
* FLAG_D: Fault flags.
*/
/* Fault flags */
typedef enum
{
  FLAG_D0 = 0,          /* Force oscillator counter fast */
  FLAG_D1,              /* Force oscillator counter slow */
  FLAG_D2,              /* Force Supply Error detection  */
  FLAG_D3,              /* FLAG_D[3]: 1--> Select Supply OV and delta detection, 0 --> Selects UV */
  FLAG_D4,              /* Set THSD */
  FLAG_D5,              /* Force Fuse ED */
  FLAG_D6,              /* Force Fuse MED */
  FLAG_D7,              /* Force TMODCHK  */
} FLAG_D;

typedef enum  { FLAG_CLR = 0x0, FLAG_SET = 0x1 } CFGA_FLAG;

/*!
*  \enum CL FLAG
* CL FLAG: Fault clear bit set or clear enum
*/
typedef enum  { CL_FLAG_CLR = 0x0, CL_FLAG_SET = 0x1 } CL_FLAG;

// /*!
// *  \enum CL FLAG
// * FLAG: Fault Clear.
// */
// typedef enum  { CL_FLAG_CLR = 0x0, CL_FLAG_SET = 0x1 } FLAG;

/*!
*  \enum SOAKON
* SOAKON: Enables or disable soak time for all commands.
*/
/* Enables or disable soak time for all commands */
typedef enum  { SOAKON_CLR = 0x0, SOAKON_SET = 0x1 } SOAKON;


/* Open wire sokon time owa */
typedef enum  {OWA0 = 0x0, OWA1, OWA2, OWA3, OWA4, OWA5, OWA6, OWA7} OWA;

/*!
*  \enum OWRNG
* OWRNG: Set soak time range Long/Short.
*/
/* Set soak time range Long/Short */
typedef enum  { SHORT_ADBMS = 0x0, LONG_ADBMS = 0x1 } OWRNG;


/*!
*  \enum OW_TIME
* OW_TIME:Open Wire Soak times
*          For Aux commands. If OWRNG=0, Soak time = 2^(6 +OWA[2:0]) Clocks (32 us 4.1 ms)
*          For Aux commands. If OWRNG=1, Soak time = 2^(13+OWA[2:0]) Clocks (41 ms 524 ms)
*/
typedef enum  { TIME_32US_TO_4_1MS = 0x0, TIME_41MS_TO_524MS = 0x1 } OW_TIME;

/*!
*  \enum GPO
* GPO: GPO Pins.
*/
/* GPO Pins */
typedef enum
{
  GPO1 = 0,
  GPO2,
  GPO3,
  GPO4,
  GPO5,
  GPO6,
  GPO7,
  GPO8,
  GPO9,
  GPO10,
} GPO;

// /*!
// *  \enum GPIO
// * GPIO: GPIO Pins.
// */
// typedef enum
// {
//   GPIO1 = 0,
//   GPIO2,
//   GPIO3,
//   GPIO4,
// } GPIO;

/*!
*  \enum Reference Voltage for VSx (x=3 to 10) measurement
* VSx: Reference Voltage for VSx measurement.
*/
typedef enum  { VSMV_SGND = 0x0, VSMV_VREF2P5 = 0x1 } VSB;

/*!
*  \enum Reference Voltage for VSx[1:0] Measurement
* VS[1:0]: Reference Voltage for VSx[1:0] Measurement.
*/
typedef enum
{
  VSM_SGND = 0,
  VSM_VREF2P5,
  VSM_V3,
  VSM_V4
} VS;


/*!
*  \enum GPIO
* GPIO: GPIO Pin Control.
*/
/* GPO Pin Control */
typedef enum  { GPO_CLR = 0x0, GPO_SET = 0x1 } CFGA_GPO;

/*!
*  \enum IIR_FPA
* IIR_FPA: IIR Filter Parameter.
*/
/* IIR Filter Parameter */
typedef enum
{
  IIR_FPA_OFF = 0,              /* Filter Disabled          */
  IIR_FPA2,                     /* 110   Hz -3dB Frequency  */
  IIR_FPA4,                     /* 45    Hz -3dB Frequency  */
  IIR_FPA8,                     /* 21    Hz -3dB Frequency  */
  IIR_FPA16,                    /* 10    Hz -3dB Frequency  */
  IIR_FPA32,                    /* 5     Hz -3dB Frequency  */
  IIR_FPA128,                   /* 1.25  Hz -3dB Frequency  */
  IIR_FPA256,                   /* 0.625 Hz -3dB Frequency  */
}IIR_FPA;

/*!
*  \enum COMM_BK
* COMM_BK: Communication Break.
*/
/* Communication Break */
typedef enum  { COMM_BK_OFF = 0x0, COMM_BK_ON = 0x1 } COMM_BK;

/*!
*  \enum SNAPSHOT
* SNAPSHOT: Snapshot.
*/
/* Snapshot */
typedef enum  { SNAP_OFF = 0x0, SNAP_ON = 0x1 } SNAPSHOT;

/* Configuration Register B */

/*!
*  \enum DTMEN
* DTMEN: Enable Dis-charge Timer Monitor.
*/
/* Enable Dis-charge Timer Monitor */
typedef enum  { DTMEN_OFF = 0x0, DTMEN_ON = 0x1 } DTMEN;

/*!
*  \enum DTRNG
* DTRNG: Discharge Timer Range Setting.
*/
/* Discharge Timer Range Setting */
typedef enum  { RANG_0_TO_63_MIN = 0x0, RANG_0_TO_16_8_HR = 0x1 } DTRNG;

/*!
*  \enum DCTO
* DCTO: DCTO timeout values.
*/
typedef enum
{
  DCTO_TIMEOUT = 0,
  TIME_1MIN_OR_0_26HR,
  TIME_2MIN_OR_0_53HR,
  TIME_3MIN_OR_0_8HR,
  TIME_4MIN_OR_1_06HR,
  TIME_5MIN_OR_1_33HR,
  TIME_6MIN_OR_1_6HR,
  /* If required more time out value add here */
} DCTO;

/*!
*  \enum PWM
* PWM: PWM Duty cycle.
*/
typedef enum
{
  PWM_0_0_PCT = 0,      /* 0.0%  (default) */
  PWM_6_6_PCT,          /* 6.6%            */
  PWM_13_2_PCT,         /* 13.2%           */
  PWM_19_8_PCT,         /* 19.8%           */
  PWM_26_4_PCT,         /* 26.4%           */
  PWM_33_0_PCT,         /* 33.0%           */
  PWM_39_6_PCT,         /* 39.6%           */
  PWM_46_2_PCT,         /* 46.2%           */
  PWM_52_8_PCT,         /* 52.8%           */
  PWM_59_4_PCT,         /* 59.4%           */
  PWM_66_0_PCT,         /* 66.0%           */
  PWM_72_6_PCT,         /* 72.6%           */
  PWM_79_2_PCT,         /* 79.2%           */
  PWM_85_8_PCT,         /* 85.8%           */
  PWM_92_4_PCT,         /* 92.4%           */
  PWM_100_0_PCT,        /* ~100.0%         */
} PWM_DUTY;

/*!
*  \enum DCC
* DCC: DCC bits.
*/
/* DCC bits */
typedef enum
{
  DCC1 = 0,
  DCC2,
  DCC3,
  DCC4,
  DCC5,
  DCC6,
  DCC7,
  DCC8,
  DCC9,
  DCC10,
  DCC11,
  DCC12,
  DCC13,
  DCC14,
  DCC15,
  DCC16,
} DCC;

/*!
*  \enum Single & Round-Robin Measurement.
* VCH: Single & Round-Robin Measurement Cmd bytes.
*/
typedef enum
{
  SM_V1 = 0,
  SM_V2,
  SM_V3,
  SM_V4,
  SM_V5,
  SM_V6,
  SM_V7_V9,
  SM_V8_V10,
  SM_VREF2,
  RR_VCH0_VCH8,
  RR_VCH0_VCH7,
  RR_VCH0_VCH5,
  RR_VCH0_VCH3,
  RR_VCH4_VCH8,
  RR_VCH4_VCH7,
  RR_VCH6_VCH8
}VCH;

/*!
*  \enum Selection for Aux Inputs.
* ACH: Selection for Aux Inputs Cmd bytes.
*/
typedef enum
{
  ALL = 0,
  ITMP_TEMP,
  VREF1P25,
  VREG,
  CMD_IGNORED
}ACH;


/*!
*  \enum DCC_BIT
* DCC_BIT: Discharge cell set and claer.
*/
/* Discharge cell set and claer  */
typedef enum  { DCC_BIT_CLR = 0x0, DCC_BIT_SET = 0x1 } DCC_BIT;

/* General Enums */
typedef enum { ALL_GRP = 0x0, A,B, C, D, E_ADBMS, F, X, Y, Z, NONE} GRP;
typedef enum { Cell = 0x0, Aux, RAux, Status_6832, Pwm, AvgCell, S_volt, F_volt, Config_6832, Comm_6832, Sid, Clrflag_6832, Rdcvall, Rdacall, Rdsall, Rdcsall, Rdacsall, Rdfcall, Rdasall_6832} TYPE_ADBMS6832;
typedef enum { Vr = 0x0, Vrx, Rvr, Wrcfg, Config_2950, Cr, Vbat, Ivbat, Oc, AvgCr, AvgVbat, AvgIvbat, Status_2950, Wrcomm, Comm_2950, SID, Time, Clrflag_2950, Rdalla, Rdallb, Rdcall, Rdasall_2950} TYPE_ADBMS2950;

typedef enum { PASS , FAIL } RESULT ;
typedef enum { OSC_MISMATCH = 0x0, SUPPLY_ERROR, THSD, FUSE_ED, FUSE_MED, TMODCHK} DIAGNOSTIC_TYPE;
#endif /* __BMS_DATA_H */

/** @}*/
/** @}*/


// void adBms2950_Adi1(RD rd, CONT cont, OW ow);
void adBms2950_Adi2(CONT cont, OW ow);
void adBms2950_Adv(OW ow, VCH vch);
void adBms2950_Adaux(ACH ach);
void adBms2950_Stcomm(void);
uint8_t adBms2950ConfigA_Flag(FLAG_D flag_d, CFGA_FLAG flag);
uint8_t adBms2950ConfigA_Gpio(GPO gpio, CFGA_GPO set_clr);
uint8_t adBms2950ConfigA_Gpo(GPO gpo_2950, CFGA_GPO set_clr);
void adBms2950ParseConfiga(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseConfigb(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseConfig(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data);
void adBms2950ParseCurrentRegData(uint8_t tIC, cell_asic *ic, uint8_t *i_data);
void adBms2950ParseVbatRegData(uint8_t tIC, cell_asic *ic, uint8_t *vbat_data);
void adBms2950ParseIVbatRegData(uint8_t tIC, cell_asic *ic, uint8_t *ivbat_data);
void adBms2950ParseOcRegData(uint8_t tIC, cell_asic *ic, uint8_t *oc_data);
void adBms2950ParseAvgCurrentRegData(uint8_t tIC, cell_asic *ic, uint8_t *iavg_data);
void adBms2950ParseAvgVbatRegData(uint8_t tIC, cell_asic *ic, uint8_t *avgvbat_data);
void adBms2950ParseAvgIVbatRegData(uint8_t tIC, cell_asic *ic, uint8_t *avgivbat_data);
void adBms2950VrParseData(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *vr_data);
void adBms2950VrxParseData(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *vrx_data);
void adBms2950RedVrParseData(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *redvr_data);
void adBms2950ParseStatusA(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseStatusB(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseStatusC(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseStatusD(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseStatusE(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseStatus(uint8_t tIC, cell_asic *ic, GRP grp, uint8_t *data);
void adBms2950ParseComm(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950ParseSID(uint8_t tIC, cell_asic *ic, uint8_t *data);
void adBms2950CreateConfiga(uint8_t tIC, cell_asic *ic);
void adBms2950CreateConfigb(uint8_t tIC, cell_asic *ic);
void adBms2950CreateClrflagData(uint8_t tIC, cell_asic *ic);
void adBms2950CreateComm(uint8_t tIC, cell_asic *ic);