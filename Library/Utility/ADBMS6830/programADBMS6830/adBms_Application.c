/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    adbms_Application.c
* @brief:   adbms application test cases
* @version: $Revision$
* @date:    $Date$
* Developed by: ADIBMS Software team, Bangalore, India
*****************************************************************************/
/*! \addtogroup APPLICATION
*  @{
*/

/*! @addtogroup Application
*  @{
*/
#include "common.h"
#include "adBms_Application.h"
#include "adBms6830CmdList.h"
#include "adBms6830GenericType.h"
#include "serialPrintResult.h"
#include "mcuWrapper.h"
// #include "adbms2950.h"
#ifdef MBED
extern Serial pc;
#endif
/**
*******************************************************************************
* @brief Setup Variables
* The following variables can be modified to configure the software.
*******************************************************************************
*/

#define TOTAL_IC 2
#define AUX_ALL  0
cell_asic IC[TOTAL_IC];

/* ADC Command Configurations */
RD      REDUNDANT_MEASUREMENT           = RD_ON;
CH      AUX_CH_TO_CONVERT               = AUX_ALL;
CONT    CONTINUOUS_MEASUREMENT          = SINGLE;
OW_C_S  CELL_OPEN_WIRE_DETECTION        = OW_OFF_ALL_CH;
OW_AUX  AUX_OPEN_WIRE_DETECTION         = AUX_OW_OFF;
PUP     OPEN_WIRE_CURRENT_SOURCE        = PUP_DOWN;
DCP     DISCHARGE_PERMITTED             = DCP_OFF;
RSTF    RESET_FILTER                    = RSTF_OFF;
ERR     INJECT_ERR_SPI_READ             = WITHOUT_ERR;

/* Set Under Voltage and Over Voltage Thresholds */
const float OV_THRESHOLD = 6.0;                 /* Volt */
const float UV_THRESHOLD = 0.0;                 /* Volt */
const int OWC_Threshold = 2000;                 /* Cell Open wire threshold(mili volt) */
const int OWA_Threshold = 50000;                /* Aux Open wire threshold(mili volt) */
const uint32_t LOOP_MEASUREMENT_COUNT = 1;      /* Loop measurment count */
const uint16_t MEASUREMENT_LOOP_TIME  = 10;     /* milliseconds(mS)*/
uint32_t loop_count = 0;
uint32_t pladc_count;

/*Loop Measurement Setup These Variables are ENABLED or DISABLED Remember ALL CAPS*/
LOOP_MEASURMENT MEASURE_CELL            = ENABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_AVG_CELL        = ENABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_F_CELL          = ENABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_S_VOLTAGE       = ENABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_AUX             = DISABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_RAUX            = DISABLED;        /*   This is ENABLED or DISABLED       */
LOOP_MEASURMENT MEASURE_STAT            = DISABLED;        /*   This is ENABLED or DISABLED       */

uint8_t WRCFGA[2]        = { 0x00, 0x01 };
uint8_t WRCFGB[2]        = { 0x00, 0x24 };
uint8_t RDCFGA[2]        = { 0x00, 0x02 };
uint8_t RDCFGB[2]        = { 0x00, 0x26 };

/*!< Read cell voltage result registers commands */
uint8_t RDI[2]         = { 0x00, 0x04 };
uint8_t RDVBAT[2]      = { 0x00, 0x06 };
uint8_t RDIVBAT[2]     = { 0x00, 0x08 };
uint8_t RDVA[2]        = { 0x00, 0x0A };
uint8_t RDVB[2]        = { 0x00, 0x09 };
uint8_t RDOCR[2]       = { 0x00, 0x0B };
uint8_t RDALLA[2]      = { 0x00, 0x0C };

/*!< Read Result Registers Commands C */
uint8_t RDRVA[2]       = { 0x00, 0x07 };
uint8_t RDRVB[2]       = { 0x00, 0x0D };
uint8_t RDVC[2]        = { 0x00, 0x03 };
uint8_t RDVD[2]        = { 0x00, 0x05 };
uint8_t RDCALL[2]      = { 0x00, 0x10 };

/*!< Read Result Registers Commands B */
uint8_t RDIAV[2]       = { 0x00, 0x44 };
uint8_t RDVBAV[2]      = { 0x00, 0x46 };
uint8_t RDIVBAV[2]     = { 0x00, 0x48 };
uint8_t RDALLB[2]      = { 0x00, 0x4C };

/* Read cell voltage result registers commands */
uint8_t RDCVA[2]         = { 0x00, 0x04 };
uint8_t RDCVB[2]         = { 0x00, 0x06 };
uint8_t RDCVC[2]         = { 0x00, 0x08 };
uint8_t RDCVD[2]         = { 0x00, 0x0A };
uint8_t RDCVE[2]         = { 0x00, 0x09 };
uint8_t RDCVF[2]         = { 0x00, 0x0B };
uint8_t RDCVALL[2]       = { 0x00, 0x0C };

/* Read average cell voltage result registers commands commands */
uint8_t RDACA[2]         = { 0x00, 0x44 };
uint8_t RDACB[2]         = { 0x00, 0x46 };
uint8_t RDACC[2]         = { 0x00, 0x48 };
uint8_t RDACD[2]         = { 0x00, 0x4A };
uint8_t RDACE[2]         = { 0x00, 0x49 };
uint8_t RDACF[2]         = { 0x00, 0x4B };
uint8_t RDACALL[2]       = { 0x00, 0x4C };

/* Read s voltage result registers commands */
uint8_t RDSVA[2]         = { 0x00, 0x03 };
uint8_t RDSVB[2]         = { 0x00, 0x05 };
uint8_t RDSVC[2]         = { 0x00, 0x07 };
uint8_t RDSVD[2]         = { 0x00, 0x0D };
uint8_t RDSVE[2]         = { 0x00, 0x0E };
uint8_t RDSVF[2]         = { 0x00, 0x0F };
uint8_t RDSALL[2]        = { 0x00, 0x10 };

/* Read c and s results */
uint8_t RDCSALL[2]       = { 0x00, 0x11 };
uint8_t RDACSALL[2]      = { 0x00, 0x51 };

/* Read all AUX and all Status Registers */
uint8_t RDASALL[2]       = { 0x00, 0x35 };

/* Read filtered cell voltage result registers*/
uint8_t RDFCA[2]         = { 0x00, 0x12 };
uint8_t RDFCB[2]         = { 0x00, 0x13 };
uint8_t RDFCC[2]         = { 0x00, 0x14 };
uint8_t RDFCD[2]         = { 0x00, 0x15 };
uint8_t RDFCE[2]         = { 0x00, 0x16 };
uint8_t RDFCF[2]         = { 0x00, 0x17 };
uint8_t RDFCALL[2]       = { 0x00, 0x18 };

/* Read aux results */
uint8_t RDAUXA[2]        = { 0x00, 0x19 };
uint8_t RDAUXB[2]        = { 0x00, 0x1A };
uint8_t RDAUXC[2]        = { 0x00, 0x1B };
uint8_t RDAUXD[2]        = { 0x00, 0x1F };
uint8_t RDAUXE[2]        = { 0x00, 0x36 };

/* Read redundant aux results */
uint8_t RDRAXA[2]        = { 0x00, 0x1C };
uint8_t RDRAXB[2]        = { 0x00, 0x1D };
uint8_t RDRAXC[2]        = { 0x00, 0x1E };
uint8_t RDRAXD[2]        = { 0x00, 0x25 };

/* Read status registers */
uint8_t RDSTATA[2]       = { 0x00, 0x30 };
uint8_t RDSTATB[2]       = { 0x00, 0x31 };
uint8_t RDSTATC[2]       = { 0x00, 0x32 };
uint8_t RDSTATCERR[2]    = { 0x00, 0x72 };              /* ERR */
uint8_t RDSTATD[2]       = { 0x00, 0x33 };
uint8_t RDSTATE[2]       = { 0x00, 0x34 };

/* Pwm registers commands */
uint8_t WRPWM1[2]        = { 0x00, 0x20 };
uint8_t RDPWM1[2]        = { 0x00, 0x22 };

uint8_t WRPWM2[2]        = { 0x00, 0x21 };
uint8_t RDPWM2[2]        = { 0x00, 0x23 };

/* Clear commands */
uint8_t CLRCELL[2]       = { 0x07, 0x11 };
uint8_t CLRAUX [2]       = { 0x07, 0x12 };
uint8_t CLRSPIN[2]       = { 0x07, 0x16 };
uint8_t CLRFLAG[2]       = { 0x07, 0x17 };
uint8_t CLRSTAT [2]      = { 0x07, 0x13 };
uint8_t CLRFC[2]         = { 0x07, 0x14 };
uint8_t CLOVUV[2]        = { 0x07, 0x15 };

/* Poll adc command */
uint8_t PLADC[2]         = { 0x07, 0x18 };
uint8_t PLAUT[2]         = { 0x07, 0x19 };
uint8_t PLCADC[2]        = { 0x07, 0x1C };
uint8_t PLSADC[2]        = { 0x07, 0x1D };
uint8_t PLAUX1[2]        = { 0x07, 0x1E };
uint8_t PLAUX2[2]        = { 0x07, 0x1F };

/* Diagn command */
uint8_t DIAGN[2]         = {0x07 , 0x15};

/* GPIOs Comm commands */
uint8_t WRCOMM[2]        = { 0x07, 0x21 };
uint8_t RDCOMM[2]        = { 0x07, 0x22 };
// uint8_t STCOMM[13]       = { 0x07, 0x23, 0xB9, 0xE4 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

/* Mute and Unmute commands */
uint8_t MUTE[2] 	     = { 0x00, 0x28 };
uint8_t UNMUTE[2]        = { 0x00, 0x29 };

uint8_t RSTCC[2]         = { 0x00, 0x2E };
uint8_t SNAP[2]          = { 0x00, 0x2D };
uint8_t UNSNAP[2]        = { 0x00, 0x2F };
uint8_t SRST[2]          = { 0x00, 0x27 };

/* Read SID command */
uint8_t RDSID[2]         = { 0x00, 0x2C };





void adbms_main()
{
  // printMenu();
  adBms6830_init_config(TOTAL_IC, &IC[0]);
  while(1)
  {
    int user_command;
#ifdef MBED
    pc.scanf("%d", &user_command);
    pc.printf("Enter cmd:%d\n", user_command);
#else
    scanf("%d", &user_command);
    printf("Enter cmd:%d\n", user_command);
#endif
    run_command(user_command);
  }
}

void run_command(int cmd)
{
  switch(cmd)
  {

  case 1:
    adBms6830_write_read_config(TOTAL_IC, &IC[0]);             // Step 1
    break;

  case 2:
    adBms6830_read_config(TOTAL_IC, &IC[0]);
    break;

  case 3:
    adBms6830_start_adc_cell_voltage_measurment(TOTAL_IC);     // Step 2
    break;

  case 4:
    adBms6830_read_cell_voltages(TOTAL_IC, &IC[0]);            // Step 3   
    break;

  case 5:
    adBms6830_start_adc_s_voltage_measurment(TOTAL_IC);
    break;

  case 6:
    adBms6830_read_s_voltages(TOTAL_IC, &IC[0]);
    break;

  case 7:
    adBms6830_start_avgcell_voltage_measurment(TOTAL_IC);
    break;

  case 8:
    adBms6830_read_avgcell_voltages(TOTAL_IC, &IC[0]);
    break;

  case 9:
    adBms6830_start_fcell_voltage_measurment(TOTAL_IC);
    break;

  case 10:
    adBms6830_read_fcell_voltages(TOTAL_IC, &IC[0]);
    break;

  case 11:
    adBms6830_start_aux_voltage_measurment(TOTAL_IC, &IC[0]);
    break;

  case 12:
    adBms6830_read_aux_voltages(TOTAL_IC, &IC[0]);
    break;

  case 13:
    adBms6830_start_raux_voltage_measurment(TOTAL_IC, &IC[0]);
    break;

  case 14:
    adBms6830_read_raux_voltages(TOTAL_IC, &IC[0]);
    break;

  case 15:
    adBms6830_read_status_registers(TOTAL_IC, &IC[0]);
    break;

  case 16:
    loop_count = 0;
    adBmsWakeupIc(TOTAL_IC);
    adBmsWriteData6832(TOTAL_IC, &IC[0], WRCFGA, Config_6832, A);
    adBmsWriteData6832(TOTAL_IC, &IC[0], WRCFGB, Config_6832, B);
    adBmsWakeupIc(TOTAL_IC);
    adBms6830_Adcv(REDUNDANT_MEASUREMENT, CONTINUOUS, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
    delayMilliSeconds(1); // ADCs are updated at their conversion rate is 1ms
    adBms6830_Adcv(RD_ON, CONTINUOUS, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
    delayMilliSeconds(1); // ADCs are updated at their conversion rate is 1ms
    adBms6830_Adsv(CONTINUOUS, DISCHARGE_PERMITTED, CELL_OPEN_WIRE_DETECTION);
    delayMilliSeconds(8); // ADCs are updated at their conversion rate is 8ms
    while(loop_count < LOOP_MEASUREMENT_COUNT)
    {
      measurement_loop();
      delayMilliSeconds(MEASUREMENT_LOOP_TIME);
      loop_count = loop_count + 1;
    }
    // printMenu();
    break;

  case 17:
    adBms6830_clear_cell_measurement(TOTAL_IC);
    break;

  case 18:
    adBms6830_clear_aux_measurement(TOTAL_IC);
    break;

  case 19:
    adBms6830_clear_spin_measurement(TOTAL_IC);
    break;

  case 20:
    adBms6830_clear_fcell_measurement(TOTAL_IC);
    break;

  case 21:
    adBms6830_write_config(TOTAL_IC, &IC[0]);
    break;

  case 0:
    // printMenu();
    break;

  default:
#ifdef MBED
    pc.printf("Incorrect Option\n\n");
#else
    printf("Incorrect Option\n\n");
#endif
    break;
  }
}

/**
*******************************************************************************
* @brief Set configuration register A. Refer to the data sheet
*        Set configuration register B. Refer to the data sheet
*******************************************************************************
*/
bmsStatus_t adBms6830_init_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsCsHigh(cmuSpi);


  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    ic[cic].tx_cfga.cth = CVT_5_1mV;
//    ic[cic].cfga.flag_d = ConfigA_Flag(FLAG_D0, FLAG_SET) | ConfigA_Flag(FLAG_D1, FLAG_SET);
  //  ic[cic].cfga.gpo = ConfigA_Gpo(GPO2, GPO_SET) | ConfigA_Gpo(GPO10, GPO_SET);
   ic[cic].tx_cfga.gpo = 0x01;
   ic[cic].tx_cfga.gpo = 0X3FF; /* All GPIO pull down off */
   ic[cic].tx_cfga.soakon = SOAKON_SET;
   ic[cic].tx_cfga.fc = IIR_FPA4;

    /* Init config B */
//    ic[cic].cfgb.dtmen = DTMEN_ON;
    ic[cic].tx_cfgb.vov = SetOverVoltageThreshold(OV_THRESHOLD);
    ic[cic].tx_cfgb.vuv = SetUnderVoltageThreshold(UV_THRESHOLD);
//    ic[cic].cfgb.dcc = ConfigB_DccBit(DCC16, DCC_BIT_SET);
//    SetConfigB_DischargeTimeOutValue(tIC, &ic[cic], RANG_0_TO_63_MIN, TIME_1MIN_OR_0_26HR);
  }
  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);

  return status;
}

/**
*******************************************************************************
* @brief Write and Read Configuration Register A/B
*******************************************************************************
*/
bmsStatus_t adBms6830_write_read_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsWakeupIc(tIC);
	CHECK_STATUS(status);
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
	CHECK_STATUS(status);
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);
	CHECK_STATUS(status);
  status = adBmsReadData6832(tIC, &ic[0], RDCFGA, Config_6832, A);
	CHECK_STATUS(status);
  status = adBmsReadData6832(tIC, &ic[0], RDCFGB, Config_6832, B);

  return status;
}

/**
*******************************************************************************
* @brief Write Configuration Register A/B
*******************************************************************************
*/
bmsStatus_t adBms6830_write_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;
  adBmsWakeupIc(tIC);
  adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);

  return status;
}

/**
*******************************************************************************
* @brief Read Configuration Register A/B
*******************************************************************************
*/
void adBms6830_read_config(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData6832(tIC, &ic[0], RDCFGA, Config_6832, A);
  adBmsReadData6832(tIC, &ic[0], RDCFGB, Config_6832, B);
  printReadConfig(tIC, &ic[0], Config_6832, ALL_GRP);
}

/**
*******************************************************************************
* @brief Start ADC Cell Voltage Measurement
*******************************************************************************
*/
bmsStatus_t adBms6830_start_adc_cell_voltage_measurment(uint8_t tIC)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;

  status = adBms6830_Adcv(REDUNDANT_MEASUREMENT, CONTINUOUS_MEASUREMENT, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);

  return status;
}

/**
*******************************************************************************
* @brief Read Cell Voltages
*******************************************************************************
*/
bmsStatus_t adBms6830_read_cell_voltages(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
   status = adBmsReadData6832(tIC, &ic[0], RDCVA, Cell, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCVB, Cell, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCVC, Cell, C);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCVD, Cell, D);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCVE, Cell, E_ADBMS);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCVF, Cell, F);

  return status;
}

/**
*******************************************************************************
* @brief Start ADC S-Voltage Measurement
*******************************************************************************
*/
void adBms6830_start_adc_s_voltage_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms6830_Adsv(CONTINUOUS_MEASUREMENT, DISCHARGE_PERMITTED, CELL_OPEN_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);
}

/**
*******************************************************************************
* @brief Read S-Voltages
*******************************************************************************
*/
void adBms6830_read_s_voltages(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData6832(tIC, &ic[0], RDSVA, S_volt, A);
  adBmsReadData6832(tIC, &ic[0], RDSVB, S_volt, B);
  adBmsReadData6832(tIC, &ic[0], RDSVC, S_volt, C);
  adBmsReadData6832(tIC, &ic[0], RDSVD, S_volt, D);
  adBmsReadData6832(tIC, &ic[0], RDSVE, S_volt, E_ADBMS);
  adBmsReadData6832(tIC, &ic[0], RDSVF, S_volt, F);
  // printVoltages(tIC, &ic[0], S_volt);
}

/**
*******************************************************************************
* @brief Start Avarage Cell Voltage Measurement
*******************************************************************************
*/
void adBms6830_start_avgcell_voltage_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms6830_Adcv(RD_ON, CONTINUOUS_MEASUREMENT, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);
}

/**
*******************************************************************************
* @brief Read Avarage Cell Voltages
*******************************************************************************
*/
void adBms6830_read_avgcell_voltages(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData6832(tIC, &ic[0], RDACA, AvgCell, A);
  adBmsReadData6832(tIC, &ic[0], RDACB, AvgCell, B);
  adBmsReadData6832(tIC, &ic[0], RDACC, AvgCell, C);
  adBmsReadData6832(tIC, &ic[0], RDACD, AvgCell, D);
  adBmsReadData6832(tIC, &ic[0], RDACE, AvgCell, E_ADBMS);
  adBmsReadData6832(tIC, &ic[0], RDACF, AvgCell, F);
  // printVoltages(tIC, &ic[0], AvgCell);
}

/**
*******************************************************************************
* @brief Start Filtered Cell Voltages Measurement
*******************************************************************************
*/
void adBms6830_start_fcell_voltage_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms6830_Adcv(REDUNDANT_MEASUREMENT, CONTINUOUS_MEASUREMENT, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);
// #ifdef MBED
//   pc.printf("F Cell voltage conversion completed\n");
// #else
//   printf("F Cell voltage conversion completed\n");
// #endif
//   printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Read Filtered Cell Voltages
*******************************************************************************
*/
void adBms6830_read_fcell_voltages(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData6832(tIC, &ic[0], RDFCA, F_volt, A);
  adBmsReadData6832(tIC, &ic[0], RDFCB, F_volt, B);
  adBmsReadData6832(tIC, &ic[0], RDFCC, F_volt, C);
  adBmsReadData6832(tIC, &ic[0], RDFCD, F_volt, D);
  adBmsReadData6832(tIC, &ic[0], RDFCE, F_volt, E_ADBMS);
  adBmsReadData6832(tIC, &ic[0], RDFCF, F_volt, F);
  printVoltages(tIC, &ic[0], F_volt);
}

/**
*******************************************************************************
* @brief Start AUX, VMV, V+ Voltages Measurement
*******************************************************************************
*/
bmsStatus_t adBms6830_start_aux_voltage_measurment(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_OK;

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    // skipping the first index
    if(cic == ADBMS2950_DAISYCHAIN_INDEX)
    {
      continue;
    }

    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    ic[cic].tx_cfga.gpo = 0X3FF; /* All GPIO pull down off */
  }
  status = adBmsWakeupIc(tIC);
  if(status != BMS_OK) return status;
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBms6830_Adax(AUX_OPEN_WIRE_DETECTION, OPEN_WIRE_CURRENT_SOURCE, AUX_CH_TO_CONVERT);
  pladc_count = adBmsPollAdc(PLADC);

  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 set dcc discharge
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6830_set_dcc_discharge(uint32_t *cellNo, uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;


  if((cellNo < 0)      ||\
    (tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* 1st to 16th DCC discharge bits in config register B
       17th and 18th bits in config register A  */

    // setting the first 16 DCC discharge bits   
    ic[cic].tx_cfgb.dcc = cellNo[cic];                  

    // setting the last 2 DCC discharge bits
    ic[cic].tx_cfga.dcc = ((cellNo[cic] & 0x20000) << 1) | (cellNo[cic] & 0x10000);      // extracting the 17th and 18th bit values and storing it inside configA DCC register bit fields

  }

  adBmsWakeupIc(tIC);
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCFGB, Config_6832, B);


  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms6815 clear dcc discharge
 * 
 * @param tIC total number of ICs
 * @param ic structure to the IC handle
 * @retval BMS status
 */
bmsStatus_t adBms6830_clear_dcc_discharge(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  if((tIC <= 0)         ||\
    (NULL == ic))
	{
		return BMS_CMU_FUNCTION_PARAMETER_ERROR;
	}

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.dcc = 0;
    ic[cic].tx_cfga.dcc = 0;
  }

  adBmsWakeupIc(tIC);
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCFGA, Config_6832, A);
  if(status != BMS_OK) return status;
  status = adBmsReadData6832(tIC, &ic[0], RDCFGB, Config_6832, B);

  return status;
}

/**
*******************************************************************************
* @brief Read AUX, VMV, V+ Voltages
*******************************************************************************
*/
bmsStatus_t adBms6830_read_aux_voltages(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_OK;

  status = adBmsWakeupIc(tIC);
  status = adBmsReadData6832(tIC, &ic[0], RDAUXA, Aux, A);
  status = adBmsReadData6832(tIC, &ic[0], RDAUXB, Aux, B);
  status = adBmsReadData6832(tIC, &ic[0], RDAUXC, Aux, C);
  status = adBmsReadData6832(tIC, &ic[0], RDAUXD, Aux, D);
  status = adBmsReadData6832(tIC, &ic[0], RDAUXE, Aux, E_ADBMS);

  return status;
}

/**
*******************************************************************************
* @brief Start Redundant GPIO Voltages Measurement
*******************************************************************************
*/
void adBms6830_start_raux_voltage_measurment(uint8_t tIC,  cell_asic *ic)
{
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    ic[cic].tx_cfga.gpo = 0X3FF; /* All GPIO pull down off */
  }
  adBmsWakeupIc(tIC);
  adBmsWriteData6832(tIC, &ic[0], WRCFGA, Status_6832, A);
  adBms6830_Adax2(AUX_CH_TO_CONVERT);
  pladc_count = adBmsPollAdc(PLADC);
#ifdef MBED
  pc.printf("RAux voltage conversion completed\n");
#else
  printf("RAux voltage conversion completed\n");
#endif
  printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Read Redundant GPIO Voltages
*******************************************************************************
*/
void adBms6830_read_raux_voltages(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData6832(tIC, &ic[0], RDRAXA, RAux, A);
  adBmsReadData6832(tIC, &ic[0], RDRAXB, RAux, B);
  adBmsReadData6832(tIC, &ic[0], RDRAXC, RAux, C);
  adBmsReadData6832(tIC, &ic[0], RDRAXD, RAux, D);
  printVoltages(tIC, &ic[0], RAux);
}

/**
*******************************************************************************
* @brief Read Status Reg. A, B, C, D and E.
*******************************************************************************
*/
void adBms6830_read_status_registers(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsWriteData6832(tIC, &ic[0], WRCFGA, Config_6832, A);
  adBmsWriteData6832(tIC, &ic[0], WRCFGB, Config_6832, B);
  adBms6830_Adax(AUX_OPEN_WIRE_DETECTION, OPEN_WIRE_CURRENT_SOURCE, AUX_CH_TO_CONVERT);
  pladc_count = adBmsPollAdc(PLADC);
  adBms6830_Adcv(REDUNDANT_MEASUREMENT, CONTINUOUS_MEASUREMENT, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
  pladc_count = pladc_count + adBmsPollAdc(PLADC);

  adBmsReadData6832(tIC, &ic[0], RDSTATA, Status_6832, A);
  // adBmsReadData6832(tIC, &ic[0], RDSTATB, Status_6832, B);
  // adBmsReadData6832(tIC, &ic[0], RDSTATC, Status_6832, C);
  // adBmsReadData6832(tIC, &ic[0], RDSTATD, Status_6832, D);
  // adBmsReadData6832(tIC, &ic[0], RDSTATE, Status_6832, E_ADBMS);
}

/**
*******************************************************************************
* @brief Loop measurment.
*******************************************************************************
*/
void measurement_loop()
{
  if(MEASURE_CELL == ENABLED)
  {
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVA, Cell, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVB, Cell, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVC, Cell, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVD, Cell, D);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVE, Cell, E_ADBMS);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDCVF, Cell, F);
    printVoltages(TOTAL_IC, &IC[0], Cell);
  }

  if(MEASURE_AVG_CELL == ENABLED)
  {
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACA, AvgCell, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACB, AvgCell, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACC, AvgCell, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACD, AvgCell, D);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACE, AvgCell, E_ADBMS);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDACF, AvgCell, F);
    printVoltages(TOTAL_IC, &IC[0], AvgCell);
  }

  if(MEASURE_F_CELL == ENABLED)
  {
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCA, F_volt, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCB, F_volt, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCC, F_volt, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCD, F_volt, D);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCE, F_volt, E_ADBMS);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDFCF, F_volt, F);
    printVoltages(TOTAL_IC, &IC[0], F_volt);
  }

  if(MEASURE_S_VOLTAGE == ENABLED)
  {
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVA, S_volt, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVB, S_volt, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVC, S_volt, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVD, S_volt, D);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVE, S_volt, E_ADBMS);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSVF, S_volt, F);
    printVoltages(TOTAL_IC, &IC[0], S_volt);
  }

  if(MEASURE_AUX == ENABLED)
  {
    adBms6830_Adax(AUX_OPEN_WIRE_DETECTION, OPEN_WIRE_CURRENT_SOURCE, AUX_CH_TO_CONVERT);
    adBmsPollAdc(PLAUX1);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDAUXA, Aux, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDAUXB, Aux, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDAUXC, Aux, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDAUXD, Aux, D);
    printVoltages(TOTAL_IC, &IC[0], Aux);
  }

  if(MEASURE_RAUX == ENABLED)
  {
    adBmsWakeupIc(TOTAL_IC);
    adBms6830_Adax2(AUX_CH_TO_CONVERT);
    adBmsPollAdc(PLAUX2);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDRAXA, RAux, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDRAXB, RAux, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDRAXC, RAux, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDRAXD, RAux, D);
    printVoltages(TOTAL_IC, &IC[0], RAux);
  }

  if(MEASURE_STAT == ENABLED)
  {
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSTATA, Status_6832, A);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSTATB, Status_6832, B);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSTATC, Status_6832, C);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSTATD, Status_6832, D);
    adBmsReadData6832(TOTAL_IC, &IC[0], RDSTATE, Status_6832, E_ADBMS);
    printStatus(TOTAL_IC, &IC[0], Status_6832, ALL_GRP);
  }
}

/**
*******************************************************************************
* @brief Clear Cell measurement reg.
*******************************************************************************
*/
void adBms6830_clear_cell_measurement(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(CLRCELL);
#ifdef MBED
  pc.printf("Cell Registers Cleared\n\n");
#else
  printf("Cell Registers Cleared\n\n");
#endif
}

/**
*******************************************************************************
* @brief Clear Aux measurement reg.
*******************************************************************************
*/
void adBms6830_clear_aux_measurement(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(CLRAUX);
#ifdef MBED
  pc.printf("Aux Registers Cleared\n\n");
#else
  printf("Aux Registers Cleared\n\n");
#endif
}

/**
*******************************************************************************
* @brief Clear spin measurement reg.
*******************************************************************************
*/
void adBms6830_clear_spin_measurement(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(CLRSPIN);
#ifdef MBED
  pc.printf("Spin Registers Cleared\n\n");
#else
  printf("Spin Registers Cleared\n\n");
#endif
}

/**
*******************************************************************************
* @brief Clear fcell measurement reg.
*******************************************************************************
*/
void adBms6830_clear_fcell_measurement(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(CLRFC);
#ifdef MBED
  pc.printf("Fcell Registers Cleared\n\n");
#else
  printf("Fcell Registers Cleared\n\n");
#endif
}

/** @}*/
/** @}*/