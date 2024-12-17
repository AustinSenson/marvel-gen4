/**
********************************************************************************
*
* @file:    adbms_2950.c
*
* @brief:   This file contains the test cases implementation.
*
* @details:
*
*******************************************************************************
Copyright(c) 2020 Analog Devices, Inc. All Rights Reserved. This software is
proprietary & confidential to Analog Devices, Inc. and its licensors. By using
this software you agree to the terms of the associated Analog Devices License
Agreement.
*******************************************************************************
*/
/*! \addtogroup Main
*  @{
*/

/*! \addtogroup Test_Cases
*  @{
*/
#include "adbms2950.h"
#include "adi_2950.h"
#include "serialPrintResult.h"
// #include "application.h"
#ifdef MBED
extern Serial pc;
#endif /* MBED */
/*******************************************************************************
* @brief Setup Variables
* The following variables can be modified to configure the software.
*******************************************************************************/
/*!< ********************************GLOBAL VARIABLES****************************/
/*!< ADC Command Configurations */
VCH   VOLTAGE_MEASUREMENT     = SM_V1;
extern RD    REDUNDANT_MEASUREMENT;
extern ACH   AUX_CH_TO_CONVERT;
extern CONT  CONTINUOUS_MEASUREMENT;
OW    OW_WIRE_DETECTION       = OW_OFF;
extern ERR   INJECT_ERR_SPI_READ;
extern uint32_t pladc_count;
/*!< ****************************************************************************/

/**
*******************************************************************************
* @brief Set configuration register A. Refer to the data sheet
*        Set configuration register B. Refer to the data sheet
*******************************************************************************
*/
bmsStatus_t adi2950_init_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;

    /* Init config B */
    ic[cic].tx_cfgb.vs2 = VSM_SGND;
  }
  status = adBmsWakeupIc(tIC);
  status = adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_2950, A);
  status = adBmsWriteData2950(tIC, &ic[0], WRCFGB, Config_2950, B);

  return status;
}

/**
*******************************************************************************
* @brief Write and Read Configuration Register A/B
*******************************************************************************
*/
bmsStatus_t adi2950_write_read_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.refon = PWR_UP;
    /* Init config B */
    ic[cic].tx_cfgb.vs2 = VSM_SGND;
  }
  status = adBmsWakeupIc(tIC);
  CHECK_STATUS(status);
  status = adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_2950, A);
  CHECK_STATUS(status);
  status = adBmsWriteData2950(tIC, &ic[0], WRCFGB, Config_2950, B);
  // CHECK_STATUS(status);
  // status = adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_2950, A);
  // CHECK_STATUS(status);
  // status = adBmsReadData2950(tIC, &ic[0], RDCFGB, Config_2950, B);

  return status;
}

/**
*******************************************************************************
* @brief Read Configuration Register A/B
*******************************************************************************
*/
bmsStatus_t adi2950_read_config(uint8_t tIC, cell_asic *ic)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsWakeupIc(tIC);	CHECK_STATUS(status);
	CHECK_STATUS(status);
  status = adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
	CHECK_STATUS(status);
  status = adBmsReadData2950(tIC, &ic[0], RDCFGB, Config_6832, B);

  return status;
}

/**
*******************************************************************************
* @brief Start Adi1 Single Measurement
*******************************************************************************
*/
bmsStatus_t adi2950_start_adi1_single_measurment(uint8_t tIC)
{
  bmsStatus_t status = BMS_ERROR;

  status = soft_reset(tIC);
  CHECK_STATUS(status);
  status = adBmsWakeupIc(tIC);
  CHECK_STATUS(status);
  status = adBms2950_Adi1(REDUNDANT_MEASUREMENT, CONTINUOUS_MEASUREMENT, OW_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);

  return status;
}

/**
*******************************************************************************
* @brief Start Adi1 Continuous Measurement
*******************************************************************************
*/
bmsStatus_t adi2950_start_adi1_continuous_measurment(uint8_t tIC)
{
  bmsStatus_t status = BMS_ERROR;

  status = soft_reset(tIC);
	CHECK_STATUS(status);
  status = adBmsWakeupIc(tIC);
  CHECK_STATUS(status);
  status = adBms2950_Adi1(REDUNDANT_MEASUREMENT, CONTINUOUS, OW_WIRE_DETECTION);
  delayMilliSeconds(1); 

  return status;
}

/**
*******************************************************************************
* @brief Start Adi2 Single Measurement
*******************************************************************************
*/
void adi2950_start_adi2_single_measurment(uint8_t tIC)
{
  soft_reset(tIC);
  adBmsWakeupIc(tIC);
  pladc_count = adBmsPollAdc(PLADC);
  adBms2950_Adi2(CONTINUOUS_MEASUREMENT, OW_WIRE_DETECTION);
  // printMsg("Adi2 conversion completed");
  printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Start Adi2 Continuous Measurement
*******************************************************************************
*/
void adi2950_start_adi2_continuous_measurment(uint8_t tIC)
{
  soft_reset(tIC);
  adBmsWakeupIc(tIC);
  adBms2950_Adi2(CONTINUOUS, OW_WIRE_DETECTION);
  delayMilliSeconds(8);  /* As per data sheet current register update rate(1ms) min  & average current register update rate (8ms) max*/
}

/**
*******************************************************************************
* @brief Start Adi1 Redundant Single Measurement
*******************************************************************************
*/
void adi2950_start_adi1_redundant_single_measurment(uint8_t tIC)
{
  soft_reset(tIC);
  adBmsWakeupIc(tIC);
  adBms2950_Adi1(RD_ON, CONTINUOUS_MEASUREMENT, OW_WIRE_DETECTION);
  pladc_count = adBmsPollAdc(PLADC);
  // printMsg("Adi1 redundant conversion completed");
  printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Start Adi1 Redundant Continuous Measurement
*******************************************************************************
*/
void adi2950_start_adi1_redundant_continuous_measurment(uint8_t tIC)
{
  soft_reset(tIC);
  adBmsWakeupIc(tIC);
  adBms2950_Adi1(RD_ON, CONTINUOUS, OW_WIRE_DETECTION);
  delayMilliSeconds(8);  /* As per data sheet current register update rate(1ms) min  & average current register update rate (8ms) max*/
  // printMsg("Adi1 redundant conversion completed");
}

/**
*******************************************************************************
* @brief Read Current, Battery & Current and Battery Registers
*******************************************************************************
*/
bmsStatus_t adi2950_read_cr_vbat_ivbat_registers(uint8_t tIC, cell_asic *ic)
{
  	bmsStatus_t status = BMS_ERROR;

    status = adBmsWakeupIc(tIC);
    status = adBmsReadData2950(tIC, &ic[0], RDI, Cr, NONE);        /* Current Register Group */
    status = adBmsReadData2950(tIC, &ic[0], RDVBAT, Vbat, NONE);   /* Battery Voltage Group*/
    status = adBmsReadData2950(tIC, &ic[0], RDIVBAT, Ivbat, NONE); /* Current and Battery Voltage Group */

    return status;
}

/**
*******************************************************************************
* @brief Read Overcurrent ADC Register
*******************************************************************************
*/
void adi2950_read_ocr_register(uint8_t tIC, cell_asic *ic)
{
    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      /* Init config A */
      ic[cic].tx_cfga.refon = PWR_UP;
    }
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDOCR, Oc, NONE);
    // printOc(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Read (Average Current), (Average Batter)  & (Average Battery Current and Voltage) Registers
*******************************************************************************
*/
void adi2950_read_avgcr_avgvbat_avgivbat_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDIAV, AvgCr, NONE);
    adBmsReadData2950(tIC, &ic[0], RDVBAV, AvgVbat, NONE);
    adBmsReadData2950(tIC, &ic[0], RDIVBAV, AvgIvbat, NONE);
    // printAvgCr(tIC, &ic[0]);
    // printAvgVbat(tIC, &ic[0]);
    // printAvgIVbat(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Read All Current & Battery Voltage Registers
*******************************************************************************
*/
void adi2950_all_current_battery_voltage_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDI, Cr, NONE);
    adBmsReadData2950(tIC, &ic[0], RDVBAT, Vbat, NONE);
    adBmsReadData2950(tIC, &ic[0], RDIVBAT, Ivbat, NONE);

    adBmsReadData2950(tIC, &ic[0], RDOCR, Oc, NONE);

    adBmsReadData2950(tIC, &ic[0], RDIAV, AvgCr, NONE);
    adBmsReadData2950(tIC, &ic[0], RDVBAV, AvgVbat, NONE);
    adBmsReadData2950(tIC, &ic[0], RDIVBAV, AvgIvbat, NONE);
    adBmsReadData2950(tIC, &ic[0], RDIVBAV, AvgIvbat, NONE);

    // printCr(tIC, &ic[0]);
    // printVbat(tIC, &ic[0]);
    // printIvbat(tIC, &ic[0]);
    // printOc(tIC, &ic[0]);
    // printAvgCr(tIC, &ic[0]);
    // printAvgVbat(tIC, &ic[0]);
    // printAvgIVbat(tIC, &ic[0]);
    // printAvgIVbat(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Start Adv All Channel Measurement
*******************************************************************************
*/
void adi2950_start_adv_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms2950_Adv(OW_WIRE_DETECTION, RR_VCH0_VCH8);
  pladc_count = adBmsPollAdc(PLADC);
  // printMsg("Adv conversion completed");
  printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Read Vr registers
*******************************************************************************
*/
void adi2950_read_vr_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDVA, Vr, A);
    adBmsReadData2950(tIC, &ic[0], RDVB, Vr, B);
    adBmsReadData2950(tIC, &ic[0], RDVC, Vr, C);
    adBmsReadData2950(tIC, &ic[0], RDVD, Vr, D);
    // printVoltage(tIC, &ic[0], Vr);
}

/**
*******************************************************************************
* @brief Read Rvr registers
*******************************************************************************
*/
void adi2950_read_rvr_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDRVA, Rvr, A);
    adBmsReadData2950(tIC, &ic[0], RDRVB, Rvr, B);
    // printVoltage(tIC, &ic[0], Rvr);
}

void adi2950_read_vr_rvr_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDVA, Vr, A);
    adBmsReadData2950(tIC, &ic[0], RDVB, Vr, B);
    adBmsReadData2950(tIC, &ic[0], RDVC, Vr, C);
    adBmsReadData2950(tIC, &ic[0], RDVD, Vr, D);
    adBmsReadData2950(tIC, &ic[0], RDRVA, Rvr, A);
    adBmsReadData2950(tIC, &ic[0], RDRVB, Rvr, B);
    // printVoltage(tIC, &ic[0], Vr);
    // printVoltage(tIC, &ic[0], Rvr);
}

/**
*******************************************************************************
* @brief Read Vrx registers
*******************************************************************************
*/
void adi2950_read_vrx_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDAUXC, Vrx, X);
    adBmsReadData2950(tIC, &ic[0], RDAUXD, Vrx, Y);
    // printVoltage(tIC, &ic[0], Vrx);
}

/**
*******************************************************************************
* @brief Start Adaux Measurement
*******************************************************************************
*/
void adi2950_start_adaux_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms2950_Adaux(AUX_CH_TO_CONVERT);
  pladc_count = adBmsPollAdc(PLADC);
  // printMsg("Adaux conversion completed");
  printPollAdcConvTime(pladc_count);
}

/**
*******************************************************************************
* @brief Read Adaux status A measurment
*******************************************************************************
*/
void adi2950_read_adaux_measurment(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDSTATA, Status_2950, A); /*!< Status_2950 A */
    printStatus(tIC, &ic[0], Status_2950, A);
}

/**
*******************************************************************************
* @brief Read Status_2950 C Register
*******************************************************************************
*/
void adi2950_read_status_c_register(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDSTATC, Status_2950, C); /*!< Status_2950 C */
    printStatus(tIC, &ic[0], Status_2950, C);
}

/**
*******************************************************************************
* @brief Read All Status_2950 Registers
*******************************************************************************
*/
void adi2950_read_all_status_registers(uint8_t tIC, cell_asic *ic)
{
    adBmsWakeupIc(tIC);
    adBmsReadData2950(tIC, &ic[0], RDSTATA, Status_2950, A); /*!< Status_2950 A */
    adBmsReadData2950(tIC, &ic[0], RDSTATB, Status_2950, B); /*!< Status_2950 B */
    adBmsReadData2950(tIC, &ic[0], RDSTATC, Status_2950, C); /*!< Status_2950 C */
    adBmsReadData2950(tIC, &ic[0], RDSTATD, Status_2950, D); /*!< Status_2950 D */
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Status_2950 E_ADBMS */
    printStatus(tIC, &ic[0], Status_2950, ALL_GRP);
}

/**
*******************************************************************************
* @brief Read Device SID
*******************************************************************************
*/
void adi2950_read_device_sid(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  adBmsReadData2950(tIC, &ic[0], RDSID, SID, NONE);
  printDeviceSID(tIC, &ic[0], SID);
}

/**
*******************************************************************************
* @brief Soft Reset
*******************************************************************************
*/
void adi2950_soft_reset(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(SRST);
  // printMsg("Soft Reset Done");
}

/**
*******************************************************************************
* @brief Reset command counter
*******************************************************************************
*/
void adi2950_reset_cmd_count(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(RSTCC);
  // printMsg("Command Counter Reset Done");
}

/**
*******************************************************************************
* @brief Snapshot
*******************************************************************************
*/
void adi2950_snap(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(SNAP);
  // printMsg("Snap Done");
}

/**
*******************************************************************************
* @brief Release Snapshot
*******************************************************************************
*/
void adi2950_unsnap(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(UNSNAP);
  // printMsg("Unsnap Done");
}

/**
*******************************************************************************
* @brief Set and reset the gpo_2950 pins(to drive output on gpo_2950 pins)
*******************************************************************************
*/
void adi2950_set_reset_gpo_pins(uint8_t tIC, cell_asic *ic)
{
  int option = 0;
  int gpo_2950 = 0;
  adBmsWakeupIc(tIC);
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    ic[cic].tx_cfga.refon = PWR_UP;
    if(option == 1)
    {
      ic[cic].tx_cfga.gpo_2950 = adBms2950ConfigA_Gpo((GPO)gpo_2950, GPO_SET);    /* Gpos pins drive to high */
    }
    else if(option == 2)
    {
      ic[cic].tx_cfga.gpo_2950 = adBms2950ConfigA_Gpo((GPO)gpo_2950, GPO_CLR);    /* Gpo pins drive to low*/
    }
  }
  adBmsWakeupIc(tIC);
  adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
  adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
  adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read status E_ADBMS for gpo_2950 pin status*/
  // printWriteConfig(tIC, &ic[0], Config_6832, A);
  // printReadConfig(tIC, &ic[0], Config_6832, A);
  // printStatus(tIC, &ic[0], Status_2950, E_ADBMS);
}

/**
*******************************************************************************
* @brief GPIO SPI Write to Slave.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_spi_write_to_slave(uint8_t tIC, cell_asic *ic)
{
    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      ic[cic].tx_cfga.refon = PWR_UP;
      ic[cic].tx_cfga.gpio = 0xF;            /*!< All gpios pull down disable */

      ic[cic].tx_comm.icomm[0] = 0x8;        /*!< Generate a CSBM Low signal */
      ic[cic].tx_comm.fcomm[0] = 0x0;        /*!< Holds CSBM low */
      ic[cic].tx_comm.icomm[1] = 0x8;        /*!< Generate a CSBM Low signal */
      ic[cic].tx_comm.fcomm[1] = 0x9;        /*!< CSBM high */
      ic[cic].tx_comm.icomm[2] = 0xF;        /*!< No transmit */
      ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< CSBM high */

      ic[cic].tx_comm.data[0] = 0x55;        /*!< data1, write the data on COMM register (D0,D1,D2) for sending the data on SPI bus*/
      ic[cic].tx_comm.data[1] = 0xAA;        /*!< data2 */
      ic[cic].tx_comm.data[2] = 0xFF;        /*!< data3 */
    }
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read Status_2950 E_ADBMS for gpio status*/
    adBmsWriteData2950(tIC, &ic[0], WRCOMM, Comm_2950, NONE);
    adBms2950_Stcomm();
    adBmsReadData2950(tIC, &ic[0], RDCOMM, Comm_2950, NONE);
    // printMsg("GPIO SPI Write to Slave Completed");
    printWriteConfig(tIC, &ic[0], Config_6832, A);
    printReadConfig(tIC, &ic[0], Config_6832, A);
    printStatus(tIC, &ic[0], Status_2950, E_ADBMS);
    printWriteCommData(tIC, &ic[0], Comm_2950);
    printReadCommData(tIC, &ic[0], Comm_2950);
}

/**
*******************************************************************************
* @brief GPIO SPI Read from Slave.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_spi_read_from_slave(uint8_t tIC, cell_asic *ic)
{
    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      ic[cic].tx_cfga.refon = PWR_UP;
      ic[cic].tx_cfga.gpio = 0xF;            /*!< All gpios pull down disable */

      ic[cic].tx_comm.icomm[0] = 0x8;        /*!< Generate a CSBM Low signal */
      ic[cic].tx_comm.fcomm[0] = 0x0;        /*!< Holds CSBM low */
      ic[cic].tx_comm.icomm[1] = 0x8;        /*!< Generate a CSBM Low signal */
      ic[cic].tx_comm.fcomm[1] = 0x9;        /*!< CSBM high */
      ic[cic].tx_comm.icomm[2] = 0xF;        /*!< No transmit */
      ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< CSBM high */

      ic[cic].tx_comm.data[0] = 0x55;        /*!< data1, write the data on COMM register (D0,D1,D2) for sending the data on SPI bus*/
      ic[cic].tx_comm.data[1] = 0xAA;        /*!< data2 */
      ic[cic].tx_comm.data[2] = 0xFF;        /*!< data3 */
    }
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read Status_2950 E_ADBMS for gpio status*/
    adBmsWriteData2950(tIC, &ic[0], WRCOMM, Comm_2950, NONE);
    adBms2950_Stcomm();
    adBmsReadData2950(tIC, &ic[0], RDCOMM, Comm_2950, NONE);
    // printMsg("GPIO SPI Read from Slave Completed");
    printWriteConfig(tIC, &ic[0], Config_6832, A);
    printReadConfig(tIC, &ic[0], Config_6832, A);
    printStatus(tIC, &ic[0], Status_2950, E_ADBMS);
    printWriteCommData(tIC, &ic[0], Comm_2950);
    printReadCommData(tIC, &ic[0], Comm_2950);
}

/**
*******************************************************************************
* @brief GPIO I2C Write on the GPIO Port.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_i2c_write_to_slave(uint8_t tIC, cell_asic *ic)
{
    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      ic[cic].tx_cfga.refon = PWR_UP;
      ic[cic].tx_cfga.gpio = 0xF;            /*! All gpios pull down disable*/

      ic[cic].tx_comm.icomm[0] = 0x6;        /*!< Generate a START Signal on I2C Port Followed by Data Transmission */
      ic[cic].tx_comm.fcomm[0] = 0x8;        /*!< Master Generated an NACK Signal*/
      ic[cic].tx_comm.icomm[1] = 0x0;        /*!<! Blank, SDA Held Low Between Bytes*/
      ic[cic].tx_comm.fcomm[1] = 0x8;        /*!< Master Generated an NACK Signal*/
      ic[cic].tx_comm.icomm[2] = 0x0;        /*!< Blank, SDA Held Low Between Bytes*/
      ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< Master Generated a NACK Signal, Master Generated a STOP Signal*/

      ic[cic].tx_comm.data[0] = 0xA0;        /*!< data1, write the data on COMM register (D0,D1,D2) for sending the data on I2C bus*/
      ic[cic].tx_comm.data[1] = 0x00;        /*!< data2*/
      ic[cic].tx_comm.data[2] = 0x25;        /*!< data3*/
    }
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read Status_2950 E_ADBMS for gpio status*/
    adBmsWriteData2950(tIC, &ic[0], WRCOMM, Comm_2950, NONE);
    adBms2950_Stcomm();
    adBmsReadData2950(tIC, &ic[0], RDCOMM, Comm_2950, NONE);
    // printMsg("GPIO I2C Write data to Slave completed");
    printWriteConfig(tIC, &ic[0], Config_6832, A);
    printReadConfig(tIC, &ic[0], Config_6832, A);
    printStatus(tIC, &ic[0], Status_2950, E_ADBMS);
    printWriteCommData(tIC, &ic[0], Comm_2950);
    printReadCommData(tIC, &ic[0], Comm_2950);
}

/**
*******************************************************************************
* @brief GPIO I2C Read from the GPIO Ports(using eeprom 24AA01)
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_i2c_read_from_slave(uint8_t tIC, cell_asic *ic)
{
    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      ic[cic].tx_cfga.refon = PWR_UP;
      ic[cic].tx_cfga.gpio = 0xF;            /*!< All gpios pull down disable*/

      ic[cic].tx_comm.icomm[0] = 0x6;        /*!< Generate a START Signal on I2C Port Followed by Data Transmission*/
      ic[cic].tx_comm.fcomm[0] = 0x8;        /*!< Master Generated an NACK Signal*/
      ic[cic].tx_comm.icomm[1] = 0x0;        /*!< Blank, SDA Held Low Between Bytes*/
      ic[cic].tx_comm.fcomm[1] = 0x8;        /*!< Master Generated an NACK Signal*/
      ic[cic].tx_comm.icomm[2] = 0x6;        /*!< Blank, SDA Held Low Between Bytes*/
      ic[cic].tx_comm.fcomm[2] = 0x8;        /*!< Master Generated a NACK Signal, Master Generated a STOP Signal*/

      ic[cic].tx_comm.data[0] = 0xA0;        /*!<data1*/
      ic[cic].tx_comm.data[1] = 0x00;        /*!<data2*/
      ic[cic].tx_comm.data[2] = 0xA1;        /*!<data3*/
    }
    /***************************** Write to slave devcie *****************/
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read Status_2950 E_ADBMS for gpio status*/
    adBmsWriteData2950(tIC, &ic[0], WRCOMM, Comm_2950, NONE);
    adBms2950_Stcomm();
    printWriteConfig(tIC, &ic[0], Config_6832, A);
    printReadConfig(tIC, &ic[0], Config_6832, A);
    printWriteCommData(tIC, &ic[0], Comm_2950);
    printStatus(tIC, &ic[0], Status_2950, E_ADBMS);

    for(uint8_t cic = 0; cic < tIC; cic++)
    {
      ic[cic].tx_cfga.refon = PWR_UP;
      ic[cic].tx_cfga.gpio = 0xF;            /*!< All gpios pull down disable */

      ic[cic].tx_comm.icomm[0] = 0x0;        /*!< SDA held low */
      ic[cic].tx_comm.fcomm[0] = 0x9;        /*!<Slave NACk+ master STOP */
      ic[cic].tx_comm.icomm[1] = 0x7;        /*!< SDA held high */
      ic[cic].tx_comm.fcomm[1] = 0x9;        /*!< Slave NACk+ master STOP */
      ic[cic].tx_comm.icomm[2] = 0x7;        /*!<SDA held high */
      ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< Slave NACk+ master STOP */

      ic[cic].tx_comm.data[0] = 0xFF;        /*!<data1 */
      ic[cic].tx_comm.data[1] = 0xFF;        /*!<data2 */
      ic[cic].tx_comm.data[2] = 0xFF;        /*!<data3 */
    }
    /***************************** Read from slave devcie *****************/
    adBmsWakeupIc(tIC);
    adBmsWriteData2950(tIC, &ic[0], WRCFGA, Config_6832, A);
    adBmsReadData2950(tIC, &ic[0], RDSTATE, Status_2950, E_ADBMS); /*!< Read Status_2950 E_ADBMS for gpio status*/
    adBms2950_Stcomm();
    adBmsReadData2950(tIC, &ic[0], RDCOMM, Comm_2950, NONE);
    // printMsg("GPIO I2C Read data from Slave completed");
    printReadCommData(tIC, &ic[0], Comm_2950);
    printStatus(tIC, &ic[0], Status_2950, E_ADBMS);
}

/**
*******************************************************************************
* @brief Soft Reset
*******************************************************************************
*/
bmsStatus_t soft_reset(uint8_t tIC)
{
  bmsStatus_t status = BMS_ERROR;

  status = adBmsWakeupIc(tIC);
  status = spiSendCmd(SRST);
  delayMilliSeconds(8); /*  After SRST cmd 8ms delay required*/

  return status;
}

/** @}*/
/** @}*/