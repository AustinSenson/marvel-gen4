/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    mcuWrapper.c
* @brief:   BMS SPI driver functions
* @version: $Revision$
* @date:    $Date$
* Developed by: ADIBMS Software team, Bangalore, India
*****************************************************************************/
/*! \addtogroup MCU DRIVER
*  @{
*/

/*! @addtogroup Mcu Driver
*  @{
*/
#include "common.h"
#include "mcuWrapper.h"
#include "onBoardIOs.h"
#define WAKEUP_DELAY 4                          /* BMS ic wakeup delay  */

/* Function definition -------------------------------------------------------*/
/**
 * @brief wakes up adbms6815 IC
 * 
 * @param t_ic total number of adbms6815 ICs
 * @retval BMS status
 */
bmsStatus_t adBmsWakeupIc(uint8_t t_ic)
{
  bmsStatus_t status = BMS_ERROR;

  for (uint8_t ic = 0; ic < t_ic; ic++)
  {
    status = adBmsCsLow(cmuSpi);
    delayMilliSeconds(1);
    status = adBmsCsHigh(cmuSpi);  
    delayMilliSeconds(1); 
  }

  return status;
}

bmsStatus_t adBmsCsLow(uint8_t spiID)
{
  bmsStatus_t status = BMS_ERROR;

  switch(spiID)
  {
    case cmuSpi:
    case ccmSpi:
      status = resetSPISlaveSelect(spiID);
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  
  return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief adBms Cs High
 * 
 * @param spiID spi ID
 * @retval BMS status
 */
bmsStatus_t adBmsCsHigh(uint8_t spiID)
{
  bmsStatus_t status = BMS_ERROR;
  switch(spiID)
  {
    case cmuSpi:
    case ccmSpi:
      status = setSPISlaveSelect(spiID);
      break;
    default:
      status = BMS_ERROR;
      break;
  }
  return status;
}