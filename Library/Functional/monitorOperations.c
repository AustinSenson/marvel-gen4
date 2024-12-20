/**
 * @file monitorOperations.c
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Private includes ----------------------------------------------------------*/
#include "bmsCANMatrix.h"
#include "bmsErrorCodes.h"
#include "customDelays.h"
#include "initializeOperations.h"

#include "onBoardIOs.h"
#include "onBoardUARTs.h"
#include "onBoardCCM2949.h"
#include "onBoardCANv2b.h"
#include "canControls.h"
#include "onBoardMemory.h"
#include "conversion.h"
#include "adBms_Application.h"
#include "adi_2950.h"
/* Self include --------------------------------------------------------------*/
#include "monitorOperations.h"
#include "dataProcessing.h"
#include "threadxMain.h"
#include "onBoardFlash.h"
/* Type definiton ------------------------------------------------------------*/

#define TIMEBASE_FREQUENCY_1MHZ 	0x07
#define TIMEBASE_FREQUENCY_4MHZ 	0xF2
#define TIMEBASE_FREQUENCY 			TIMEBASE_FREQUENCY_4MHZ


#define multiplierVolt				0.0001
/* External variables definiton ----------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t switchRequest = 0;
float estimatedSoH = 100.0;				//HARDCODED

extern uint8_t fullChargeLatch;
extern uint16_t adbms6830CRCErrorCount;
extern uint16_t ltc2949CRCErrorCount;
extern cmuVoltageData_t cmuVoltages;
extern uint8_t isBalancingCommandZero;
extern uint16_t cellsToBalance[2];
extern uint8_t balanceActiveFlag;
extern thresholdData_t thresholdData; 
extern contactorOutput_t cOutputData;

extern uint8_t marvelState;

extern uint8_t adcReady; //DMA flag
extern uint16_t adcData[NUM_CHANNELS]; //ADC Data Buffer 
#define VREF 3.3  //MCU TEMP ref voltage
/* Private function prototypes -----------------------------------------------*/
// static uint32_t checkOverVoltage(stb_ statusB, uint8_t cmuIndex);
// static uint32_t checkUnderVoltage(stb_ statusB, uint8_t cmuIndex);
/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell voltages
 * @param 
 * @retval BMS status
 */
bmsStatus_t readCellVoltages(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;
	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	memset((void*)&mdata->cmuData.cmuVolatges,0,sizeof(cmuVoltageData_t));


    status = adBms6830_write_read_config(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);       
	CHECK_STATUS(status);     
    status = adBms6830_start_adc_cell_voltage_measurment(mdata->cmuData.cmuCount);   
	CHECK_STATUS(status); 

	// starting S adc and avg cell conversions
	// adBms6830_start_adc_s_voltage_measurment(mdata->cmuData.cmuCount);
	// adBms6830_start_avgcell_voltage_measurment(mdata->cmuData.cmuCount); 
	
	status = adBms6830_read_cell_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);	
	CHECK_STATUS(status);

	// reading S ADCs and the average voltage values and teh fault registers for debug purpose
	// adBms6830_read_s_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	// adBms6830_read_avgcell_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	adBms6830_read_status_registers(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);

	status = checkPackVoltages(mdata);
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell temperatures
 * @param 
 * @retval BMS status
 */

bmsStatus_t readCellTemperaturesValues(bmsMainData_t *mdata){
	bmsStatus_t status = BMS_OK;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// memset((void*)&mdata->tempData.tempValue,0,sizeof(tempData_t));

	if (adcReady) {
        for (uint8_t NTCINDEX = 0; NTCINDEX < NUM_CHANNELS; NTCINDEX++) {
            float Vout 							= ADCVOLMULTIPLIER * adcData[NTCINDEX];
            float resistorRatio 				= (Vout / (VREF - Vout));
            mdata->tempData.tempValue[NTCINDEX] = round(1.0 / (((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15);    //round temp
			resistorRatio = 0.0;
        }

        adcReady = 0;  // Reset flag

    }
	

	return status;
}

bmsStatus_t readCellTemperatures(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}
	
	memset((void*)&mdata->cmuData.cmuTemperatures,0,sizeof(cmuTemperatureData_t));

	status = adBms6830_start_aux_voltage_measurment(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
	status = adBms6830_read_aux_voltages(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	CHECK_STATUS(status);
	status = checkPackTemperatures(mdata);

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read cell status
 * @param 
 * @retval BMS status
 */
bmsStatus_t readPackStatus(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_OK;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	// mdata->cmuData.cmuVolatges.packSumOfCells = 0;

	// status = adBms6815_read_status(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
	// CHECK_STATUS(status);

	// //Pack voltages
	// for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	// {
	// 	mdata->cmuData.cmuVolatges.sumOfCells[cmuIndex] = mdata->cmuData.cmuCellData[cmuIndex].stata.sc * multiplierVolt * 20;
	// 	mdata->cmuData.cmuVolatges.packSumOfCells += mdata->cmuData.cmuVolatges.sumOfCells[cmuIndex];
	// }

	// //CMU internal Temperature
	// for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	// {
	// 	mdata->cmuData.cmuTemperatures.cmuInternalTemp[cmuIndex] = (mdata->cmuData.cmuCellData[cmuIndex].stata.itmp * multiplierVolt/0.0075) - 273.15;
	// }

	// uint32_t faultsData[2] = {0};
	// //CMU Status B values/ under voltage and over voltage
	// for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	// {

	// 	faultsData[cmuIndex] = checkOverVoltage(mdata->cmuData.cmuCellData[cmuIndex].statb, cmuIndex);
	// 	if(faultsData[cmuIndex])
	// 	{
	// 		mdata->cmuData.cmuFaults.overVoltage[cmuIndex] = 1;
	// 	}

	// 	faultsData[cmuIndex] = checkUnderVoltage(mdata->cmuData.cmuCellData[cmuIndex].statb, cmuIndex);
	// 	if(faultsData[cmuIndex])
	// 	{
	// 		mdata->cmuData.cmuFaults.underVoltage[cmuIndex] = 1;
	// 	}
	// }

	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check pack voltages
 * @param 
 * @retval BMS status
 */
bmsStatus_t checkPackVoltages(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	static float busbarCompensation = 0;
	static float busbarOffset = 0;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	for(uint8_t cmuIndex = 0; cmuIndex < NUMBER_OF_CMU; cmuIndex++) 
	{
		for(uint8_t cellIndex = 0; cellIndex < MAX_CELL_PER_SLAVE; cellIndex++) 
		{
			if(cmuIndex == 2 && cellIndex == 6) 
			{
				// to compensate for the resting potential of the busbar. should be 0 when no current is flowing
				if((mdata->ccmData.packCurrent > -0.5) && (mdata->ccmData.packCurrent < 0.5))
				{
					busbarOffset = CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]);
				}

				// Store busbarCompensation value
				busbarCompensation = CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]) - busbarOffset;
			}
			else if(cmuIndex == 2 && cellIndex == 7) 
			{
				// Subtract busbarCompensation data from cmuCellData[2][7] 
				if(mdata->ccmData.packCurrent > 0)
				{
					mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex - 1] =
					CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]) - busbarCompensation;
				}
				else if(mdata->ccmData.packCurrent > 0)
				{
					mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex - 1] =
					CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]) + busbarCompensation;
				}
				else
				{
					mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex - 1] =
					CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]);
				}
			} 
			else if(cmuIndex == 2 && cellIndex > 7)
			{
				mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex - 1] =
				CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]);
			} 
			else 
			{
				// Default behavior for other indices
				mdata->cmuData.cmuVolatges.cellVoltages[cmuIndex][cellIndex] =
				CONVERT_CMU_VOLTAGE_TO_MILLI_VOLT(mdata->cmuData.cmuCellData[cmuIndex].cell.c_codes[cellIndex]);
			}
		}
	}

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief check pack temperature
 * @param 
 * @retval BMS status
 */
bmsStatus_t checkPackTemperatures(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;

	if((NULL == mdata))
	{
		status = BMS_CMU_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	for(uint8_t cmuIndex = ADBMS6832_START_INDEX; cmuIndex < NUMBER_OF_CMU; cmuIndex++)
	{
		mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] = CONVERT_CMU_TEMP_TO_VOLT(mdata->cmuData.cmuCellData[cmuIndex].stata.vref2);
		// mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] = 3;

		// first 4 aux readings dedicated for external NTCs			
		for(uint8_t auxIndex = 0; auxIndex < GPIO5; auxIndex++)
		{
			float rawAux = CONVERT_CMU_TEMP_TO_VOLT(mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[auxIndex]);
			float resistorRatio = rawAux/(mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] - (rawAux));
			mdata->cmuData.cmuTemperatures.externalNTC[cmuIndex][auxIndex] = 1.0/(((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15;	// should configure the beta equation parameters if necessary
			resistorRatio = 0.0;
		}

		// proceeding 6 aux readings dedicated for internal NTCs
		for(uint8_t auxIndex = GPIO5; auxIndex <= GPIO10; auxIndex++)
		{
			float rawAux = CONVERT_CMU_TEMP_TO_VOLT(mdata->cmuData.cmuCellData[cmuIndex].aux.a_codes[auxIndex]);
			float resistorRatio = rawAux/(mdata->cmuData.cmuVolatges.refGPIOVoltage[cmuIndex] - (rawAux));
			mdata->cmuData.cmuTemperatures.onBoardNTC[cmuIndex][auxIndex - 4] = 1.0/(((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15;
			resistorRatio = 0.0;
		}

		// additional 1 aux sensing GPIOs are available if need arises	
	}

	//ADC DMA temp Read from MCU pins
	if (adcReady) {        

		// memset((void*)&mdata->tempData.tempValue,0,sizeof(tempData_t));

        for (uint8_t NTCINDEX = 0; NTCINDEX < NUM_CHANNELS; NTCINDEX++) {
            float Vout 							= ADCVOLMULTIPLIER * adcData[NTCINDEX];
            float resistorRatio 				= (Vout / (VREF - Vout));
            mdata->tempData.tempValue[NTCINDEX] = 1.0 / (((log(resistorRatio)) * 0.000251) + 0.003354) - 273.15;   
			resistorRatio = 0.0;
        }

        adcReady = 0;  // Reset flag		
    } 
	
	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read status and fault registers of LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t readStatusFaultReg(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	uint8_t tempStatusReg = 0;
    uint8_t tempOpctrlReg = 0;
    uint8_t tempFactrlReg = 0;
    uint8_t tempAdcconfReg = 0;
    uint8_t tempExtfaultsReg = 0;
    uint8_t tempFaultsReg = 0;
    uint8_t tempTbReg = 0;
    uint8_t tempOcc1Reg = 0;
    uint8_t tempOcc2Reg = 0;

	if((NULL == mdata))
	{
		status = BMS_CCM_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	mdata->ccmData.statusReg 		= tempStatusReg;
	mdata->ccmData.opctrlReg 		= tempOpctrlReg;
	mdata->ccmData.factrlReg 		= tempFactrlReg;
	mdata->ccmData.adcconfReg 		= tempAdcconfReg;
	mdata->ccmData.extfaultsReg 	= tempExtfaultsReg;
	mdata->ccmData.faultsReg 		= tempFaultsReg;

	mdata->ccmData.tbReg 		= tempTbReg;
	mdata->ccmData.occ1Reg 		= tempOcc1Reg;
	mdata->ccmData.occ2Reg 		= tempOcc2Reg;

	LTC2949_READ(LTC2949_REG_STATUS, 1, (uint8_t*)&tempStatusReg);
	LTC2949_READ(LTC2949_REG_OPCTRL, 1, (uint8_t*)&tempOpctrlReg);
	LTC2949_READ(LTC2949_REG_FACTRL, 1, (uint8_t*)&tempFactrlReg);
	LTC2949_READ(LTC2949_REG_ADCCONF, 1, (uint8_t*)&tempAdcconfReg);
	LTC2949_READ(LTC2949_REG_EXTFAULTS, 1, (uint8_t*)&tempExtfaultsReg);
	LTC2949_READ(LTC2949_REG_FAULTS, 1, (uint8_t*)&tempFaultsReg);
	LTC2949_READ(LTC2949_REG_TBCTRL, 1, (uint8_t*)&tempTbReg);
	LTC2949_READ(LTC2949_REG_OCC1CTRL, 1, (uint8_t*)&tempOcc1Reg);
	LTC2949_READ(LTC2949_REG_OCC2CTRL, 1, (uint8_t*)&tempOcc2Reg);

	mdata->ccmData.statusReg 		= tempStatusReg;
	mdata->ccmData.opctrlReg 		= tempOpctrlReg;
	mdata->ccmData.factrlReg 		= tempFactrlReg;
	mdata->ccmData.adcconfReg 		= tempAdcconfReg;
	mdata->ccmData.extfaultsReg 	= tempExtfaultsReg;
	mdata->ccmData.faultsReg 		= tempFaultsReg;

	mdata->ccmData.tbReg 		= tempTbReg;
	mdata->ccmData.occ1Reg 		= tempOcc1Reg;
	mdata->ccmData.occ2Reg 		= tempOcc2Reg;

	status = BMS_OK;
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief read pack voltage and current from LTC2949
 * @param 
 * @retval BMS status
 */

// double current2950 = 0;
// uint16_t rawADC = 0;
// int32_t scaledVoltage = 0;
// double packVoltage2950 = 0;
// uint16_t temp = 0;

bmsStatus_t readPackVoltageCurrent(bmsMainData_t *mdata)
{
	bmsStatus_t status = BMS_ERROR;
	// uint8_t *dataPointer = NULL;


	status = adi2950_write_read_config(ADBMS2950_DAISYCHAIN_INDEX, mdata->cmuData.cmuCellData);     
	CHECK_STATUS(status);
	status = adi2950_start_adi1_continuous_measurment(ADBMS2950_DAISYCHAIN_INDEX); 
	CHECK_STATUS(status);
	status = adi2950_read_cr_vbat_ivbat_registers(ADBMS2950_DAISYCHAIN_INDEX, mdata->cmuData.cmuCellData);

	//TODO transfer the voltage and current parsing logics to a different function

	// Sign-extend the 24-bit value to 32-bit signed integer
	if (mdata->cmuData.cmuCellData[0].ivbat.i1 & 0x800000) 
	{  // 0x800000 = 1 << 23
		mdata->cmuData.cmuCellData[0].ivbat.i1 |= 0xFF000000;  // Set the upper 8 bits to 1.
	}

	mdata->ccmData.packCurrent = (int32_t)mdata->cmuData.cmuCellData[0].ivbat.i1 * 0.000001f;
	mdata->ccmData.packCurrent = mdata->ccmData.packCurrent/CONVERT_MICRO_OHM_TO_OHM(SHUNT_RESISTOR_uOhm);
 	// PACK VOLTAGE SENSING
	mdata->ccmData.packVoltage = (int16_t)(mdata->cmuData.cmuCellData[0].ivbat.vbat1) * 0.0001f;

	// multiplying with the voltage divider multiplier
	mdata->ccmData.packVoltage = mdata->ccmData.packVoltage * 101;	// voltage divider circuit 3MOhm & 30 kOhm | (30k/3.03 MOhm)

	return status;
}



/* Function definition -------------------------------------------------------*/
/**
 * @brief set over current configuration for LTC2949
 * @param 
 * @retval BMS status
 */
bmsStatus_t setOverCurrentConfiguration(int8_t threshold, uint8_t deglitchTime, uint8_t polarity)
{
	bmsStatus_t status = BMS_ERROR;
	status = LTC2949_OCCConfig(threshold, deglitchTime, polarity, threshold, deglitchTime, polarity);
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief initialize cell control module
 * @param 
 * @retval BMS status
 */
bmsStatus_t initializeCCM(void)
{
    bmsStatus_t status = BMS_ERROR;

	status = adi2950_init_config(ADBMS2950_DAISYCHAIN_INDEX, mdata.cmuData.cmuCellData);

#ifdef ENABLE_LTC2949_SHORT_CIRCUIT
	CHECK_STATUS(status);
	status = setOverCurrentConfiguration(OCC_DAC_THRESHOLD_52_mV, OCC_DEGLITCH_TIME_20_us, OCC_POLARITY_BOTH_POLARITY);
#endif    
	return status;
}

/* Function definition -------------------------------------------------------*/
/**
 * @brief send data on CAN
 * @param 
 * @retval BMS status
 */
bmsStatus_t sendDataOnCAN(bmsMainData_t *mdata, protectionOutput_t *pOutput, coulombCountingOutput_t *ccOutput, dataPipelineOutput_t *dpOutput, protectionInput_t *pInput)
{
	bmsStatus_t status = BMS_ERROR;
	uint8_t canDataFrame[8] = {'\0'};

	if((NULL == mdata))
	{
		status = BMS_CAN_FUNCTION_PARAMETER_ERROR;
		return status;
	}

	//BigEndian Format
	// for(uint8_t canDLC = 0; canDLC < CAN_packVoltage.messageDLC; canDLC++)
	// 	canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (packVoltage/CAN_packVoltage.messageScaling)  >> (8 * (CAN_packVoltage.messageDLC - canDLC - 1)));
	
	//LittleEndian Format
	//	for(int8_t canDLC = CAN_packCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
	//		canDataFrame[CAN_packCurrent.messageStartByte + canDLC] = ((int) (mdata->ccmData.packCurrent/CAN_packCurrent.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_balancingLimit.messageDLC - 1; canDLC >=0; canDLC --)
		canDataFrame[CAN_balancingLimit.messageStartByte + canDLC] = ((int) (thresholdData.balancingMinThreshold/CAN_balancingLimit.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsPreChargeActive.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsPreChargeActive.messageStartByte + canDLC] = ((int) (cOutputData.preChargeContactorState/CAN_flagsPreChargeActive.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsPreChargeActive.messageID - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsBalancingActive.messageStartByte + canDLC] = ((int) (balanceActiveFlag/CAN_flagsBalancingActive.messageScaling) >> (8 * canDLC));

	for(int8_t canDLC = CAN_packCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCurrent.messageStartByte + canDLC] = ((int) (mdata->ccmData.packCurrent/CAN_packCurrent.messageScaling)  >> (8 * canDLC));
		
    status = sendCANDataOnQueue(CAN_packCurrent.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

	for(int8_t canDLC = CAN_stateOfCharge.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_stateOfCharge.messageStartByte + canDLC] = ((int) (mdata->socData.estimatedSoC/CAN_stateOfCharge.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_stateOfHealth.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_stateOfHealth.messageStartByte + canDLC] = ((int) (estimatedSoH/CAN_stateOfHealth.messageScaling)  >> (8 * canDLC));
		
	for(int8_t canDLC = CAN_bmsState.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_bmsState.messageStartByte + canDLC] = ((int) (marvelState/CAN_bmsState.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_flagsFullCharge.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_flagsFullCharge.messageStartByte + canDLC] = ((int) (fullChargeLatch/CAN_flagsFullCharge.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_packVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.packSumOfCells/CAN_packVoltage.messageScaling)  >> (8 * canDLC)); //((int) (mdata->ccmData.packVoltage/CAN_packVoltage.messageScaling)  >> (8 * canDLC));

    status = sendCANDataOnQueue(CAN_packVoltage.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_MCUTEMP1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_MCUTEMP1.messageStartByte + canDLC] = ((int) (floor(mdata->tempData.tempValue[0] )/CAN_MCUTEMP1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_MCUTEMP2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_MCUTEMP2.messageStartByte + canDLC] = ((int) (floor(mdata->tempData.tempValue[1] )/CAN_MCUTEMP1.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_MCUTEMP3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_MCUTEMP3.messageStartByte + canDLC] = ((int) (floor(mdata->tempData.tempValue[2] )/CAN_MCUTEMP1.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_MCUTEMP4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_MCUTEMP4.messageStartByte + canDLC] = ((int) (floor(mdata->tempData.tempValue[3] )/CAN_MCUTEMP1.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu1Temp1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp1.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][0])/CAN_cmu1Temp1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Temp2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Temp2.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][1])/CAN_cmu1Temp2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_packCapacity.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_packCapacity.messageStartByte + canDLC] = ((int) ((ccOutput->totalCapacityRemaining/100)/CAN_packCapacity.messageScaling)  >> (8 * canDLC));
		
    status = sendCANDataOnQueue(CAN_packCapacity.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);


		// for(int8_t canDLC = CAN_cmu1Temp3.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_cmu1Temp3.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][2])/CAN_cmu1Temp3.messageScaling)  >> (8 * canDLC));
		
		// for(int8_t canDLC = CAN_cmu1Temp4.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_cmu1Temp4.messageStartByte + canDLC] = ((int) (floor(mdata->cmuData.cmuTemperatures.externalNTC[1][3])/CAN_cmu1Temp4.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cmu1Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][0]/CAN_cmu1Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][1]/CAN_cmu1Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][2]/CAN_cmu1Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][3]/CAN_cmu1Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu1Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);


		for(int8_t canDLC = CAN_cmu1Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][4]/CAN_cmu1Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][5]/CAN_cmu1Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][6]/CAN_cmu1Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][7]/CAN_cmu1Cell8.messageScaling)  >> (8 * canDLC));
	
	status = sendCANDataOnQueue(CAN_cmu1Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);


		for(int8_t canDLC = CAN_cmu1Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][8]/CAN_cmu1Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][9]/CAN_cmu1Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][10]/CAN_cmu1Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][11]/CAN_cmu1Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu1Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);


		for(int8_t canDLC = CAN_cmu1Cell13.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell13.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][12]/CAN_cmu1Cell13.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell14.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell14.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][13]/CAN_cmu1Cell14.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell15.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell15.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][14]/CAN_cmu1Cell15.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu1Cell16.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu1Cell16.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[1][15]/CAN_cmu1Cell16.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu1Cell13.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);


		for(int8_t canDLC = CAN_cmu2Cell1.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell1.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][0]/CAN_cmu2Cell1.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell2.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell2.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][1]/CAN_cmu2Cell2.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell3.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell3.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][2]/CAN_cmu2Cell3.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell4.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell4.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][3]/CAN_cmu2Cell4.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell4.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);

		for(int8_t canDLC = CAN_cmu2Cell5.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell5.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][4]/CAN_cmu2Cell5.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell6.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell6.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][5]/CAN_cmu2Cell6.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell7.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell7.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][6]/CAN_cmu2Cell7.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell8.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell8.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][7]/CAN_cmu2Cell8.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell8.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);

		for(int8_t canDLC = CAN_cmu2Cell9.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell9.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][8]/CAN_cmu2Cell9.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell10.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell10.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][9]/CAN_cmu2Cell10.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell11.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell11.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][10]/CAN_cmu2Cell11.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell12.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell12.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][11]/CAN_cmu2Cell12.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell12.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);

		for(int8_t canDLC = CAN_cmu2Cell13.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell13.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][12]/CAN_cmu2Cell13.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell14.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell14.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][13]/CAN_cmu2Cell14.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell15.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell15.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][14]/CAN_cmu2Cell15.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_cmu2Cell16.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cmu2Cell16.messageStartByte + canDLC] = ((int) (mdata->cmuData.cmuVolatges.cellVoltages[2][15]/CAN_cmu2Cell16.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_cmu2Cell16.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);
	

	for(int8_t canDLC = CAN_pIn_voltageMax.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_pIn_voltageMax.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMax/CAN_pIn_voltageMax.messageScaling)  >> (8 * canDLC));
			
	for(int8_t canDLC = CAN_pIn_voltageMin.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_pIn_voltageMin.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMin/CAN_pIn_voltageMin.messageScaling)  >> (8 * canDLC));
			
	for(int8_t canDLC = CAN_pIn_voltageDelta.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_pIn_voltageDelta.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageDelta/CAN_pIn_voltageDelta.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_pIn_voltageDelta.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
	//Dynamic Currents Instantaneous
	// for(int8_t canDLC = CAN_InstantaneousLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
	// 	canDataFrame[CAN_InstantaneousLimitIN.messageStartByte + canDLC] = ((int) (mdata->instaneousLimitIn_A/CAN_InstantaneousLimitIN.messageScaling) >> (8 * canDLC));

	// for(int8_t canDLC = CAN_InstantaneousLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
	// 	canDataFrame[CAN_InstantaneousLimitOUT.messageStartByte + canDLC] = ((int) (mdata->instaneousLimitOut_A/CAN_InstantaneousLimitOUT.messageScaling) >> (8 * canDLC));

	// canDataFrame[5] = pOutput->protectionOutputData.i2t_Calculated_A2sec;

	// for(uint8_t canDLC = 0; canDLC < CAN_packVoltage.messageDLC; canDLC++)
	// 		canDataFrame[CAN_packVoltage.messageStartByte + canDLC] = ((int) (mdata->ccmData.packVoltage/CAN_packVoltage.messageScaling)  >> (8 * (CAN_packVoltage.messageDLC - canDLC - 1)));

	// status = sendCANDataOnQueue(CAN_TX_ID_015, canDataFrame, sizeof(canDataFrame));
	// CHECK_STATUS(status);


	// canDataFrame[0] = pOutput->protectionOutputData.overCurrentChargeFlag;
	// canDataFrame[1] = pOutput->protectionOutputData.overCurrentDischargeFlag;

	// status = sendCANDataOnQueue(CAN_DEBUG_ID_029, canDataFrame, sizeof(canDataFrame));
	// CHECK_STATUS(status);
	
	for(int8_t canDLC = CAN_packCapacityDesign.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCapacityDesign.messageStartByte + canDLC] = ((int) (defaultFlashConfigData[PACK_MAX_CAPACITY_Ah_INDEX]/CAN_packCapacityDesign.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_packCapacityFull.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_packCapacityFull.messageStartByte + canDLC] = ((int) (defaultFlashConfigData[PACK_MAX_CAPACITY_Ah_INDEX]/CAN_packCapacityFull.messageScaling)  >> (8 * canDLC));
	
	for(int8_t canDLC = CAN_balancingBitMask_CMU1.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_balancingBitMask_CMU1.messageStartByte + canDLC] = ((int) (cellsToBalance[0]/CAN_balancingBitMask_CMU1.messageScaling)  >> (8 * canDLC));

	for(int8_t canDLC = CAN_balancingBitMask_CMU2.messageDLC - 1; canDLC >= 0; canDLC--)
		canDataFrame[CAN_balancingBitMask_CMU2.messageStartByte + canDLC] = ((int) (cellsToBalance[1]/CAN_balancingBitMask_CMU2.messageScaling)  >> (8 * canDLC));

	status = sendCANDataOnQueue(CAN_packCapacityDesign.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);


// #ifdef LTC2949
		for(int8_t canDLC = CAN_timeBaseResetCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_timeBaseResetCounter.messageStartByte + canDLC] = ((int)(mdata->ccmData.timeBaseResetCounter/CAN_timeBaseResetCounter.messageScaling)  >> (8 * canDLC));
	
		for(int8_t canDLC = CAN_mcuResetCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_mcuResetCounter.messageStartByte + canDLC] = ((int)(mdata->mcuResetCounter/CAN_mcuResetCounter.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_2949CRCCounter.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_2949CRCCounter.messageStartByte + canDLC] = ((int)(ltc2949CRCErrorCount/CAN_2949CRCCounter.messageScaling)  >> (8 * canDLC));

		// for(int8_t canDLC = CAN_6815CRCCounter.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_6815CRCCounter.messageStartByte + canDLC] = ((int) (adbms6830CRCErrorCount/CAN_6815CRCCounter.messageScaling)  >> (8 * canDLC));
	status = sendCANDataOnQueue(CAN_6815CRCCounter.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);

// #endif

    //dynamic current data on CAN
        for(int8_t canDLC = CAN_dynamicLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
            canDataFrame[CAN_dynamicLimitIN.messageStartByte + canDLC] = ((int)(mdata->dynamicLimitIn_A/CAN_dynamicLimitIN.messageScaling) >> (8 * canDLC));

        for(int8_t canDLC = CAN_dynamicLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
            canDataFrame[CAN_dynamicLimitOUT.messageStartByte + canDLC] = ((int)(mdata->dynamicLimitOut_A/CAN_dynamicLimitOUT.messageScaling) >> (8 * canDLC));

	canDataFrame[4] = SW_MINOR_VERSION;
	canDataFrame[5] = SW_PATCH_VERSION;
	canDataFrame[6] = MARVEL_CONFIG;

    status = sendCANDataOnQueue(CAN_dynamicLimitOUT.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
 

#ifdef CAN_MATRIX_V_0_3
		for(int8_t canDLC = CAN_maxAllowedCellVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCellVoltage.messageStartByte + canDLC] = ((int)(MAX_ALLOWED_CELL_VOLTAGE/CAN_maxAllowedCellVoltage.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedPackVoltage.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedPackVoltage.messageStartByte + canDLC] = ((int)(MAX_ALLOWED_PACK_VOLTAGE/CAN_maxAllowedPackVoltage.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedCellTemp.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCellTemp.messageStartByte + canDLC] = ((int)(MAX_ALLOWED_CELL_TEMP/CAN_maxAllowedCellTemp.messageScaling)  >> (8 * canDLC));
		
		for(int8_t canDLC = CAN_maxAllowedCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_maxAllowedCurrent.messageStartByte + canDLC] = ((int)(MAX_ALLOWED_CURRENT/CAN_maxAllowedCurrent.messageScaling)  >> (8 * canDLC));
		
	status = sendCANDataOnQueue(CAN_maxAllowedCurrent.messageID, canDataFrame, sizeof(canDataFrame));
	CHECK_STATUS(status);

#endif

	canDataFrame[0] = MARVEL_FW_VER;
	canDataFrame[1] = MARVEL_HW_VER;

	canDataFrame[2] = MARVEL_CONFIG;

	status = sendCANDataOnQueue(CAN_TX_ID_024, canDataFrame, sizeof(canDataFrame));

	return status;

}


/* End of File ---------------------------------------------------------------*/
