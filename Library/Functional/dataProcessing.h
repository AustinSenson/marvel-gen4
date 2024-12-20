/**

/* Header Guard --------------------------------------------------------------*/
#ifndef DATAPIPELINE_H_
#define DATAPIPELINE_H_

/* Standard C includes -------------------------------------------------------*/
#include "inttypes.h"
/* Private includes ----------------------------------------------------------*/
#include "bmsConfiguration.h"
#include "onBoardCCM2949.h"
#include "onBoardIOs.h"
#include "adBms6830Data.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
typedef struct tempData{
    float tempValue[NUM_CHANNELS];
    // float   temperatures[NUM_CHANNELS];
}tempData_t;

typedef struct ccmData
{
    double packCurrent;

    double packVoltage;
    double packPower;

    double timeBase;
    double packCapacity;
    double remainingCapacity;
    double lastStoredCapacity;
    double standByCapacity;
    double packEnergy;
// #ifdef LTC2949
    float  ccmInternalTemp;
    double packFastCurrent;
    double packFilteredCurrent;
    uint8_t isCCMReset;
    uint32_t timeBaseResetCounter;
// #endif

    //status registers
    uint8_t statusReg;
    uint8_t opctrlReg;
    uint8_t factrlReg;
    uint8_t adcconfReg;
    uint8_t extfaultsReg;
    uint8_t faultsReg;
    uint8_t tbReg;
    uint8_t occ1Reg;
    uint8_t occ2Reg;

}ccmData_t;

typedef struct
{
    float refGPIOVoltage[NUMBER_OF_CMU];
    float refPowerVoltage[NUMBER_OF_CMU];
    float sumOfCells[NUMBER_OF_CMU];
    float cellVoltages[NUMBER_OF_CMU][MAX_CELL_PER_SLAVE];

    double packSumOfCells;

    float minCellVoltage;
    float maxCellVoltage;
    float meanCellVoltage;

    float packImbalance;

}cmuVoltageData_t;

typedef struct
{
    float externalNTC[NUMBER_OF_CMU][EXTERNAL_NTC_PER_SLAVE];
    float onBoardNTC[NUMBER_OF_CMU][INTERNAL_NTC_PER_SLAVE];
    float cmuInternalTemp[NUMBER_OF_CMU];

    float minCellTemperature;
    float maxCellTemperature;
    float meanCellTemperature;
}cmuTemperatureData_t;

typedef struct
{
    uint8_t overVoltage[NUMBER_OF_CMU];
    uint8_t underVoltage[NUMBER_OF_CMU];
}cmuFaultsData_t;  

typedef struct 
{
    uint8_t cmuCount;
    cell_asic cmuCellData[NUMBER_OF_CMU];

    cmuVoltageData_t cmuVolatges;
    cmuTemperatureData_t cmuTemperatures;
    cmuFaultsData_t cmuFaults;
} cmuData_t;

typedef struct 
{
    int preChargeTimer;
    double preChargeCurrent;
    uint8_t retryPreChargeLoad;
    uint8_t retryPreChargeCharge;
}preCharge_t;

typedef struct 
{
    float requestedChargeCurrent;
    float requestedChargeVoltage;
}chargeData_t;

typedef struct 
{
    uint8_t chargingCutoffCounter;
    uint8_t emergencyCutoffFlag;
}faultData_t;

typedef struct 
{
    float maxChargeCurrent;
    float maxRegenCurrent;
    float maxLoadCurrent;
}protectionData_t;

typedef struct 
{
    uint16_t estimatedSoC;
}socData_t;

typedef enum
{
    BMS_INIT = 0,    // default
    BMS_READY,       // 12 v && cont off
    BMS_ACTIVATING,  // precharge active
    BMS_ACTIVE,      // 12v and cont on     
    BMS_ERROR1        // cont cmd of 1 && cont off 
}BmsState;

typedef struct
{
    ccmData_t           ccmData;
    cmuData_t           cmuData;
    tempData_t          tempData;
    preCharge_t         preCharge;
    chargeData_t        chargeData;
    faultData_t         faultData;
    socData_t           socData;
    protectionData_t    protectionData;
    uint32_t            mcuResetCounter;
    uint32_t            dynamicLimitIn_A;
    uint32_t            dynamicLimitOut_A;
    uint32_t            instaneousLimitIn_A;
    uint32_t            instaneousLimitOut_A;
    BmsState            bmsState;
    }bmsMainData_t;

bmsStatus_t pipelineInit(void);
bmsStatus_t readFromPipeline(bmsMainData_t *mdata);

bmsStatus_t writeToPipeline(bmsMainData_t *mdata);
bmsStatus_t writeCCMDataToPipeline(ccmData_t *cdata);
bmsStatus_t writeCMUDataToPipeline(cmuData_t *cdata);
bmsStatus_t writePreChargeDataToPipeline(preCharge_t *pdata);
bmsStatus_t writeChargeDataToPipeline(chargeData_t *cdata);
bmsStatus_t writeFaultDataToPipeline(faultData_t *fdata);
bmsStatus_t writeSocDataToPipeline(socData_t *sdata);
bmsStatus_t writeProtectionDataToPipeline(protectionData_t *pdata);
//bms protection, bms fault, 
#endif
/* End of File ---------------------------------------------------------------*/
