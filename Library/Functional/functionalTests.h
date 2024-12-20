/**
 * @file controlOperations.h
 * @author Arpit Gupta (arpit@euler)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef FUNCTIONALTESTS_H_
#define FUNCTIONALTESTS_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "dataProcessing.h"
#include "main.h"
#include "string.h"
#include "lookupTable.h"
#include "tmwtypes.h"
#include "HighLevelStatemachine_types.h"
#include "structures.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

enum
{

    //dynamic conditions
    overVoltageTest = 0,
    underVoltageTest,
    overCurrentChargeTest,
    overCurrentDischargeTest,
    overTemperatureChargeTest,
    overTemperatureDischargeTest,
    underTemperatureChargeTest,
    underTemperatureDischargeTest,
    retryTest,

    //advanced protection features
    thermalRunaway,
    suddenVoltageDrop,
    temperatureGradient,
    highImbalanceFlag,
    shortCircuitDetect,

    //Static conditions
    overVoltageTestStatic,
    underVoltageTestStatic,
    overCurrentChargeTestStatic,
    overCurrentDischargeTestStatic,
    overTemperatureChargeTestStatic,
    overTemperatureDischargeTestStatic,
    underTemperatureChargeTestStatic,
    underTemperatureDischargeTestStatic,

    socLatching,    
    highImbalanceError,
    chargerEnable,

    voltageBus,
    fullChargeLatchSim,

};


/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern bmsMainData_t mdata;
extern dataPipelineInput_t dpInput;
/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
// bmsStatus_t overVoltageProtection(bmsMainData_t *mdata);
// bmsStatus_t underVoltageProtection(bmsMainData_t *mdata);
// bmsStatus_t overCurrentCharge(bmsMainData_t *mdata);
// bmsStatus_t overCurrentDischarge(bmsMainData_t *mdata);
// bmsStatus_t overTemperatureCharge(bmsMainData_t *mdata);
// bmsStatus_t overTemperatureDischarge(bmsMainData_t *mdata);
// bmsStatus_t underTemperatureCharge(bmsMainData_t *mdata);
// bmsStatus_t underTemperatureDischarge(bmsMainData_t *mdata);

void functionalTests(int testCase, bmsMainData_t *mdata);
void functionalTestsInputParam(int testCase, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput);
void chargerSim(dataPipelineInput_t  *dpInput);
void voltageSim(dataPipelineOutput_t *dpOutput);
void latchingCondition(dataPipelineInput_t  *dpInput);


#endif
/* End of File ---------------------------------------------------------------*/