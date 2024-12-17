/**
 * @file threadxMain.c
 * @brief 
 * @version 0.1
 * @date 2022-03-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Standard C includes -------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "math.h"

/* Private includes ----------------------------------------------------------*/
#include "onBoardMemory.h"
#include "bmsErrorCodes.h"
#include "customDelays.h"
#include "initializeOperations.h"
#include "monitorOperations.h"
#include "controlOperations.h"
#include "estimateParameters.h"
#include "conversion.h"
#include "usart.h"
#include "dataProcessing.h"

#include "onBoardCANv2b.h"
#include "bmsCANMatrix.h"
#include "onBoardIOs.h"

#include "bmsConfiguration.h"
#include "gpioControls.h"
#include "onBoardUARTs.h"
#include "canControls.h"
#include "flashControls.h"
#include "onBoardFlash.h"
#include "wwdg.h"
#include "iwdg.h"
#include "timerControls.h"
#include "functionalTests.h"
#include "monitorOperations.h"
#include "lookupTable.h"
#include "firmwareUpgradeApp.h"
#include "batteryCooling.h"
#include "HighLevelStatemachine_types.h"
#include "marvelStateMachine.h"


#include "stm32l4xx_it.h"
/* Self include --------------------------------------------------------------*/
#include "threadxMain.h"

#include "adBms_Application.h"
#include "adi_2950.h"



/* Type definiton ------------------------------------------------------------*/

/************************************************************************* Threadx declarations *********************************************************************/

TX_BYTE_POOL    mainBytePool;

// MARVEL threads
TX_THREAD       errorStateHandler;
TX_THREAD       collectBatteryData;
TX_THREAD       protection;
TX_THREAD       contactorControl;
TX_THREAD       ccEKF;
TX_THREAD       coulombCounting;
TX_THREAD       chargingAlgorithm;
TX_THREAD       canTransmission;
TX_THREAD       canTxHandler;
TX_THREAD       canRxHandler;
TX_THREAD       firmware_upgrade_thread;
TX_THREAD       iwdgResetTaskHandler;

// MARVEL queues
TX_QUEUE        canTxQueue;
TX_QUEUE        canRxQueue;

//MARVEL mutex
TX_MUTEX        ccOutputMutex;
TX_MUTEX        afeMutex;

// MARVEL threads timer
TX_TIMER        canTimer;

// MARVEL threadsx flags
TX_EVENT_FLAGS_GROUP systemEventFlag;

/************************************************************************* Threadx declarations end *********************************************************************/

ULONG actualFlag;
/* External variables definiton ----------------------------------------------*/
uint8_t shutdownCount = 0;  //shutdown counter debounce

uint8_t errorCode = 0;
char debugData[256] = {'\0'};

//MARVEL DATA
uint8_t MARVEL_DATA[8] = {0};
uint8_t HARDWARE_DATA[8] = {0};

//vehicle state from Matlab
HighLevelMode state;

volatile uint16_t timerCounter=0;
extern uint64_t __IO bmsDefaultConfig[25];
extern uint64_t Systemfrequency;
extern uint8_t activeAPP;

uint8_t balanceActiveFlag = 0;
volatile uint32_t taskBreakpointCycleCount = 0;

//charging 
volatile float time = 0;
float k = 1.03;
int32_t targetCellVoltage_mV = 0;
uint8_t SlowChargingMaxCurrent_A = 45;
uint8_t FastChargingMaxCurrent = 180;
uint8_t findCase[2][2] = { {1 , 2}, {3, 4} };
bool powerCycleDetected = false;
float requestedCurrent_A = 0;

//balancing
uint16_t voltageReadCounter = 0;
uint32_t cellsToBalance[NUMBER_OF_CMU] = {0};

//testing
uint8_t bmsInTestingFlag = 0;
uint8_t fullChargeLatch = 0;
uint8_t latchCheckFlag = 0;
uint8_t pseudoLatchFlag = 0;
uint8_t eepromMagic = 0;
bool contactorSequenceCompleted = false;
uint8_t contactorCommandFromMerlin ;
uint8_t enableMerlyn;
uint8_t debounceTimeContactorSequence = 0;

uint8_t errorCodes1Dataframe[8] = {0};
uint8_t errorCodes2Dataframe[8] = {0};

uint8_t marvelState = 0;


extern uint64_t ccWriteBufferFLASH[7];
extern uint64_t ccReadBufferFLASH[6];

extern uint8_t socDataWriteBuff[EEPROM_SECTION_WIDTH];
extern uint8_t socDataReadBuff[EEPROM_SECTION_WIDTH];

extern uint8_t criticalFlagsWriteBuff[EEPROM_SECTION_WIDTH];
extern uint8_t criticalFlagsReadBuff[EEPROM_SECTION_WIDTH];

extern uint8_t commonDataWriteBuff[EEPROM_SECTION_WIDTH];
extern uint8_t commonDataReadBuff[EEPROM_SECTION_WIDTH];

extern uint8_t merlynDataWriteBuff[EEPROM_SECTION_WIDTH];
extern uint8_t merlynDataReadBuff[EEPROM_SECTION_WIDTH];

extern uint8_t marvelConfigVersion[3];

uint8_t eepromEraseFlag = 0;
uint8_t resetDetectedEEPROM = 0;
uint16_t shortCircuitCounter = 0;
uint8_t chargerTypeFromStark = 0;

uint8_t marvelActiveErrors = 0;

extern uint8_t externalShortCircuitDetectionFlag;
extern bool overrideContactorLogic;
extern bool externalToggle;

//soc
uint32_t eepromSOCCounter = 0;
bool startEEPROMWriteFlag;
bool triggerDetection;
bool SoCFromLookupTableFlag = 0;
bool bypassEEPROMFetchRead = false;
bool bypassEEPROMFetchWrite = false;
bool externalLatchDetected = false;

bmsStatus_t statusSoC = BMS_ERROR;

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

dataPipelineInput_t   dpInputData     = {0};
dataPipelineOutput_t  dpOutputData    = {0};

cellBalancingInput_t    cbInputData     = {0};
cellBalancingOutput_t   cbOutputData    = {0};

protectionInput_t       pInputData      = {0};
protectionOutput_t      pOutputData     = {0};

contactorInput_t        cInputData      = {0};
contactorOutput_t       cOutputData     = {0};

DynamicLimitsTable_t    dynamicLimitsTable = {0};
vcuData_t               vcuData            = {0};

#ifdef EKF
ccEKFInput_t        ccEKFInputData     = {0};
ccEKFOutput_t       ccEKFOutputData    = {0};
#else
coulombCountingInput_t  ccInputData     = {0};
coulombCountingOutput_t ccOutputData    = {0};
#endif

stateMachineInput_t        smInputData     = {0};
stateMachineOutput_t       smOutputData    = {0};

chargingOutputData_t       chargingOutputData = {0};

thresholdData_t     thresholdData = {0};
coulombCountingOutput_t ccOutputDataEEPROM = {0};
coulombCountingOutput_t ccOutputDataFlash = {0};

contactorState contState;
merlynTestRequest testRequest;


#ifdef GF100
    //GanFeng cells 
    const int32_t continuousChargingTableData[110] = { 
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        24,     24,     24,     24,     24,     24,     24,     24,     24,     24,     20,
        60,     60,     60,     60,     60,     60,     60,     60,     60,     60,     40,
        100,    100,    100,    100,    100,    100,    100,    100,    100,    100,    40,
        200,    200,    200,    200,    200,    200,    200,    200,    200,    150,    60,
        200,    200,    200,    200,    200,    200,    200,    200,    200,    160,    160,
        200,    200,    200,    200,    200,    200,    200,    200,    200,    160,    160,
        160,    160,    160,    160,    160,    160,    160,    160,    160,    160,    60,
        60,     60,     60,     60,     60,     60,     60,     60,     60,     60,     40,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0};

    const int32_t continuousDischargingTableData[165] =  { 
        40,        60,         80,         100,        100,        100,        100,        100,        100,        100,        100,
        60,        100,        120,        160,        160,        160,        160,        160,        160,        160,        160,
        60,        100,        120,        160,        160,        160,        160,        160,        160,        160,        160,
        60,        100,        120,        160,        160,        160,        160,        160,        160,        160,        160,
        60,        100,        120,        160,        160,        160,        160,        160,        160,        160,        160,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        100,       120,        150,        200,        200,        200,        200,        200,        200,        200,        200,
        60,        100,        120,        160,        160,        160,        160,        160,        160,        160,        160,
        20,        30,         40,         60,         60,         60,         60,         60,         60,         60,         60,
        0,         0,          0,          0,          0,          0,          0,          0,          0,          0,          0};


    /*Instantaneous Dynamic current Limits Table*/
        const int32_t instantaneousDischargingTableData[165] = {
        40,     60,     80,     200,    200,    200,    200,    200,    200,    200,    200,
        60,     100,    120,    240,    240,    240,    240,    240,    240,    240,    240,
        60,     100,    200,    300,    300,    300,    300,    300,    300,    300,    300,
        60,     100,    200,    300,    300,    300,    300,    300,    300,    300,    300,
        60,     100,    200,    300,    300,    300,    300,    300,    300,    300,    300,
        100,    200,    240,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    240,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        200,    200,    300,    300,    300,    300,    300,    300,    300,    300,    300,
        20,     60,     120,    240,    240,    240,    240,    240,    240,    240,    240,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0};


    const int32_t instantaneousChargingTableData[110] = {
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        80,     80,     80,     80,     80,     80,     80,     80,     80,     80,     20,
        160,    160,    160,    160,    160,    160,    160,    160,    160,    160,    40,
        240,    240,    240,    240,    240,    240,    240,    240,    240,    240,    40,
        300,    300,    300,    300,    300,    300,    300,    300,    300,    300,    60,
        300,    300,    300,    300,    300,    300,    300,    300,    300,    300,    160,
        300,    300,    300,    300,    300,    300,    300,    300,    300,    300,    160,
        300,    300,    300,    300,    300,    300,    300,    300,    300,    300,    60,
        240,    240,    240,    240,    240,    240,    240,    240,    240,    240,    40,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0 };

    
    const int32_t arcReactorChargingTable[88] = {
        33,    33,     33,    33,    33,    25,    25,    18,    18,    10,    0,
        180,   180,    180,   126,   126,   90,    90,    90,    90,    90,    0,
        180,   180,    180,   180,   180,   180,   180,   180,   180,   90,    20,
        180,   180,    180,   180,   180,   170,   150,   130,   108,   108,   23,
        135,   135,    135,   135,   135,   120,   120,   120,   100,   23,    23,
        85,    85,     85,    85,    85,    70,    70,    40,    30,    30,    20,
        50,    50,     50,    50,    50,    50,    36,    36,    36,    17,    17,
        0,     0,      0,     0,     0,     0,     0,     0,     0,     0,     0 };


    const int32_t socData[11] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    const int32_t chargingTemperatureData[10] =  { 0, 5, 10, 15, 20, 25, 45, 50, 55, 60 };
    const int32_t chargingTemperatureDataArcReactor[8] =  { 0, 10, 25, 35, 40, 45, 50, 60 };
    const int32_t dischargingTemperatureData[15] =  { -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60 };

    const int32_t chargingMaxElementsArcReactor[2] = { 8, 11 };    //[0] - temp, [1] - soc
    const int32_t chargingMaxElements[2] = { 10, 11 };    //[0] - temp, [1] - soc
    const int32_t dischargingMaxElements[2] = { 15, 11 }; //[0] - temp, [1] - soc

#else
    //GP100-2P
    const int32_t continuousChargingTableData[132] = { 
        0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
        0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
        20,		20,		20,		20,		20,		20,		20,		20,		20,		20,		10,
        40,		40,		40,		40,		40,		40,		40,		40,		40,		40,		15,
        40,		40,		40,		40,		40,		40,		40,		40,		40,		40,		30,
        80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		60,
        140,	140,	140,	140,	140,	140,	140,	140,	140,	100,	60,
        180,	180,	180,	180,	180,	180,	180,	180,	180,	100,	60,
        180,	180,	180,	180,	180,	180,	180,	180,	180,	100,	60,
        120,	120,	120,	120,	120,	120,	120,	120,	120,	100,	40,
        80,		80,		80,		80,		80,		80,		80,		80,		80,		80,		20,
        0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0 };

    const int32_t continuousDischargingTableData[165] =  { 
        20,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
        40,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
        40,		40,		100,		100,		100,		100,		100,		100,		100,		100,		100,
        40,		100,	100,		100,		100,		100,		100,		100,		100,		100,		100,
        40,		100,	120,		120,		120,		120,		120,		120,		120,		120,		120,
        40,		100,	120,		120,		120,		120,		120,		120,		120,		120,		120,
        60,		100,	160,		160,		160,		160,		160,		160,		160,		160,		160,
        90,		120,	160,		200,		200,		200,		200,		200,		200,		160,		160,
        150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
        150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
        150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
        150,	150,	200,		200,		200,		200,		200,		200,		200,		200,		200,
        40,		60,		120,		160,		160,		160,		160,		160,		160,		160,		160,
        20,		40,		60,			120,		120,		120,		120,		120,		120,		120,		120,
        20,		20,		20,			20,			20,			40,			40,			40,			40,			40,			40 };

        /*Instantaneous Dynamic current Limits Table*/

    const int32_t instantaneousDischargingTableData[165] = {
        20,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
        40,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
        40,         40,         100,         100,         100,         100,         100,         100,         100,         100,         100,
        40,         100,        100,         100,         100,         100,         100,         100,         100,         100,         100,
        40,         100,        120,         120,         120,         120,         120,         120,         120,         120,         120,
        40,         100,        120,         120,         120,         200,         200,         200,         200,         200,         200,
        60,         100,        200,         200,         200,         200,         200,         200,         200,         200,         200,
        90,         160,        300,         300,         300,         300,         300,         300,         300,         200,         200,
        100,        300,        300,         400,         400,         400,         400,         400,         400,         300,         200,
        100,        300,        300,         400,         400,         400,         400,         400,         400,         300,         200,
        100,        160,        300,         400,         400,         400,         400,         400,         400,         300,         200,
        140,        160,        300,         400,         400,         400,         400,         400,         400,         300,         200,
        80,         160,        200,         200,         200,         200,         200,         200,         200,         200,         160,
        40,         120,        120,         120,         120,         120,         120,         120,         120,         120,         120,
        20,         60,         60,          60,          60,          60,          60,          60,          60,          60,          60 };


    const int32_t instantaneousChargingTableData[132] = {
        0,        0,         0,         0,         0,         0,         0,         0,         0,         0,         0,
        0,        30,        30,        30,        30,        30,        30,        30,        30,        20,        10,
        20,       30,        30,        30,        30,        30,        30,        30,        30,        20,        10,
        40,       60,        60,        60,        60,        60,        60,        60,        60,        60,        20,
        40,       80,        80,        80,        80,        80,        80,        80,        80,        60,        40,
        80,       120,       120,       120,       120,       120,       120,       120,       120,       100,       60,
        140,      160,       200,       240,       240,       240,       240,       240,       240,       200,       100,
        180,      180,       200,       300,       300,       300,       300,       300,       300,       200,       100,
        180,      180,       200,       300,       300,       300,       300,       300,       300,       200,       100,
        120,      160,       200,       300,       300,       300,       300,       300,       300,       200,       60,
        80,       100,       160,       200,       200,       200,       200,       200,       200,       160,       60,
        0,        40,        60,        100,       100,       100,       100,       100,       100,       100,       40 };


    const int32_t socData[11] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    const int32_t chargingTemperatureData[12] =  { -0, 5, 10, 10, 15, 20, 25, 30, 40, 45, 50, 55 };
    const int32_t dischargingTemperatureData[15] =  { -20, -15, -10, 0, 5, 10, 15, 20, 25, 30, 40, 45, 50, 55, 60 };

    const int32_t chargingMaxElements[2] = { 12, 11 };    //[0] - temp, [1] - soc
    const int32_t dischargingMaxElements[2] = { 15, 11 }; //[0] - temp, [1] - soc

#endif

// SoC-OCV lookup table discharging   0 deg, 25deg, 45 deg, 55 deg respectively
const uint32_t socTableDischarging[LOOKUPTABLE_TEMPS][LOOKUPTABLE_NUMBERS_SIZE] = 
{
    {3127, 3129, 3131, 3135, 3159, 3173, 3180, 3183, 3186, 3188, 3189, 3191, 3193, 3195, 3200, 3205, 3211, 3216, 3219, 3223, 3227, 3231, 3234, 3237, 3241, 3244, 3247, 3249, 3251, 3252, 3254, 3256, 3258, 3259, 3262, 3263, 3265, 3266, 3268, 3271, 3271, 3271, 3271, 3272, 3273, 3273, 3273, 3273, 3273, 3273, 3274, 3274, 3275, 3275, 3275, 3275, 3275, 3275, 3275, 3276, 3276, 3276, 3276, 3277, 3277, 3278, 3278, 3279, 3280, 3281, 3282, 3285, 3290, 3299, 3308, 3308, 3308, 3312, 3314, 3315, 3317, 3318, 3318, 3319, 3319, 3320, 3320, 3320, 3320, 3320, 3320, 3320, 3320, 3320, 3322, 3324, 3326, 3326, 3328, 3330, 3336},

    {3177, 3186, 3190, 3192, 3193, 3195, 3195, 3197, 3198, 3201, 3207, 3212, 3217, 3221, 3225, 3230, 3233, 3237, 3240, 3244, 3246, 3248, 3250, 3253, 3255, 3257, 3260, 3262, 3265, 3267, 3270, 3272, 3276, 3278, 3280, 3280, 3281, 3281, 3281, 3283, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3284, 3285, 3285, 3285, 3286, 3286, 3286, 3286, 3286, 3287, 3288, 3289, 3292, 3294, 3299, 3308, 3317, 3320, 3322, 3322, 3323, 3324, 3324, 3324, 3324, 3324, 3324, 3324, 3325, 3325, 3325, 3326, 3326, 3326, 3326, 3327, 3327, 3328, 3328, 3328, 3329, 3330, 3332, 3334, 3339},
    
    {3150, 3173, 3186, 3191, 3193, 3195, 3196, 3197, 3198, 3200, 3201, 3206, 3212, 3217, 3221, 3224, 3228, 3233, 3237, 3240, 3243, 3245, 3247, 3250, 3252, 3254, 3256, 3259, 3262, 3264, 3266, 3269, 3273, 3277, 3281, 3285, 3286, 3286, 3287, 3287, 3288, 3288, 3288, 3288, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3289, 3290, 3290, 3290, 3290, 3290, 3291, 3291, 3291, 3292, 3292, 3293, 3293, 3294, 3295, 3298, 3302, 3309, 3318, 3323, 3324, 3324, 3325, 3325, 3325, 3326, 3326, 3326, 3326, 3327, 3327, 3327, 3327, 3327, 3328, 3328, 3328, 3329, 3329, 3329, 3330, 3330, 3331, 3332, 3334, 3338},
    
    {3143, 3177, 3203, 3216, 3219, 3221, 3223, 3225, 3227, 3230, 3233, 3237, 3242, 3247, 3253, 3258, 3263, 3268, 3273, 3276, 3280, 3283, 3287, 3291, 3295, 3298, 3299, 3301, 3302, 3304, 3306, 3308, 3310, 3311, 3311, 3312, 3312, 3312, 3313, 3313, 3313, 3313, 3314, 3314, 3314, 3314, 3315, 3315, 3315, 3315, 3315, 3315, 3315, 3316, 3316, 3316, 3316, 3317, 3317, 3318, 3318, 3318, 3319, 3320, 3320, 3320, 3321, 3322, 3324, 3325, 3328, 3331, 3335, 3340, 3343, 3345, 3345, 3345, 3345, 3345, 3345, 3345, 3345, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3347, 3347, 3347, 3347, 3348, 3348, 3348, 3348}
};

const uint32_t socTableCharging[LOOKUPTABLE_TEMPS][LOOKUPTABLE_NUMBERS_SIZE] = 
{
    {3176, 3217, 3228, 3232, 3234, 3236, 3238, 3241, 3244, 3248, 3252, 3256, 3262, 3266, 3272, 3277, 3282, 3286, 3290, 3294, 3297, 3300, 3304, 3307, 3311, 3313, 3314, 3315, 3315, 3315, 3315, 3316, 3316, 3316, 3317, 3317, 3317, 3317, 3317, 3317, 3318, 3318, 3318, 3318, 3319, 3319, 3319, 3319, 3320, 3320, 3320, 3320, 3320, 3321, 3321, 3322, 3322, 3323, 3323, 3324, 3325, 3326, 3327, 3329, 3331, 3333, 3336, 3339, 3343, 3347, 3350, 3352, 3353, 3354, 3354, 3354, 3355, 3355, 3355, 3355, 3355, 3355, 3355, 3355, 3355, 3356, 3356, 3356, 3356, 3356, 3356, 3356, 3357, 3357, 3358, 3358, 3359, 3359, 3359, 3360, 3361},

    {3172, 3200, 3213, 3217, 3219, 3221, 3223, 3226, 3228, 3231, 3236, 3240, 3246, 3252, 3257, 3263, 3268, 3273, 3277, 3281, 3284, 3288, 3292, 3296, 3299, 3302, 3304, 3306, 3307, 3309, 3309, 3310, 3311, 3311, 3311, 3311, 3312, 3312, 3312, 3312, 3312, 3313, 3313, 3313, 3314, 3314, 3314, 3315, 3315, 3315, 3315, 3315, 3315, 3315, 3315, 3315, 3316, 3317, 3317, 3317, 3318, 3318, 3319, 3320, 3321, 3322, 3323, 3324, 3326, 3329, 3333, 3338, 3343, 3347, 3348, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3349, 3350, 3350},
    
    {3143, 3177, 3203, 3216, 3219, 3221, 3223, 3225, 3227, 3230, 3233, 3237, 3242, 3247, 3253, 3258, 3263, 3268, 3273, 3276, 3280, 3283, 3287, 3291, 3295, 3298, 3299, 3301, 3302, 3304, 3306, 3308, 3310, 3311, 3311, 3312, 3312, 3312, 3313, 3313, 3313, 3313, 3314, 3314, 3314, 3314, 3315, 3315, 3315, 3315, 3315, 3315, 3315, 3316, 3316, 3316, 3316, 3317, 3317, 3318, 3318, 3318, 3319, 3320, 3320, 3320, 3321, 3322, 3324, 3325, 3328, 3331, 3335, 3340, 3343, 3345, 3345, 3345, 3345, 3345, 3345, 3345, 3345, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3346, 3347, 3347, 3347, 3347, 3348, 3348, 3348, 3348},
    
    {3147, 3181, 3206, 3218, 3221, 3223, 3225, 3227, 3228, 3231, 3234, 3237, 3242, 3247, 3252, 3257, 3263, 3268, 3272, 3276, 3280, 3283, 3287, 3291, 3294, 3297, 3298, 3300, 3302, 3303, 3305, 3307, 3310, 3311, 3312, 3313, 3313, 3314, 3314, 3314, 3314, 3315, 3315, 3315, 3315, 3315, 3315, 3316, 3316, 3316, 3317, 3317, 3317, 3317, 3317, 3318, 3318, 3318, 3318, 3318, 3320, 3320, 3320, 3320, 3321, 3321, 3322, 3323, 3324, 3326, 3328, 3332, 3337, 3341, 3344, 3344, 3344, 3344, 3344, 3344, 3344, 3344, 3344, 3344, 3344, 3345, 3345, 3345, 3345, 3345, 3345, 3345, 3346, 3346, 3346, 3347, 3347, 3348, 3348, 3348, 3348}
};


const uint32_t lookupTableTemps[LOOKUPTABLE_TEMPS] = {0, 25, 45, 55};

uint8_t temperatureLimits[2][12] = 
{
    {
        OTC_ERROR_TEMPERATURE_GROUP_1, OTC_WARNING_TEMPERATURE_GROUP_1, OTC_RECOVERY_TEMPERATURE_GROUP_1, \
        OTD_ERROR_TEMPERATURE_GROUP_1, OTD_WARNING_TEMPERATURE_GROUP_1, OTD_RECOVERY_TEMPERATURE_GROUP_1, \
        UTC_ERROR_TEMPERATURE_GROUP_1, UTC_WARNING_TEMPERATURE_GROUP_1, UTC_RECOVERY_TEMPERATURE_GROUP_1, \
        UTD_ERROR_TEMPERATURE_GROUP_1, UTD_WARNING_TEMPERATURE_GROUP_1, UTD_RECOVERY_TEMPERATURE_GROUP_1  \
    },

    {
        OTC_ERROR_TEMPERATURE_GROUP_2, OTC_WARNING_TEMPERATURE_GROUP_2, OTC_RECOVERY_TEMPERATURE_GROUP_2, \
        OTD_ERROR_TEMPERATURE_GROUP_2, OTD_WARNING_TEMPERATURE_GROUP_2, OTD_RECOVERY_TEMPERATURE_GROUP_2, \
        UTC_ERROR_TEMPERATURE_GROUP_2, UTC_WARNING_TEMPERATURE_GROUP_2, UTC_RECOVERY_TEMPERATURE_GROUP_2, \
        UTD_ERROR_TEMPERATURE_GROUP_2, UTD_WARNING_TEMPERATURE_GROUP_2, UTD_RECOVERY_TEMPERATURE_GROUP_2  \
    },
};

/* Private function prototypes -----------------------------------------------*/

/* Private custom code -------------------------------------------------------*/

/* Function definition -------------------------------------------------------*/
/**
 * @brief ThreadX Kernel Entry - Initialize
 * 
 * @param first_unused_memory 
 */
void tx_application_define(void *first_unused_memory)
{
    CHAR *pointer;
    tx_byte_pool_create(&mainBytePool, "mainBytePool", first_unused_memory, sizeBytePool_mainBytePool);
    
    //collect battery data task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_collectBatteryData, TX_NO_WAIT);

    tx_thread_create(&collectBatteryData, "BatteryData", entry_collectBatteryData, 0, pointer, 
                    sizeStack_collectBatteryData, priority_P0, priority_P0, TX_NO_TIME_SLICE, TX_AUTO_START);

    // protection task
    // tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_protection, TX_NO_WAIT);

    // tx_thread_create(&protection, "Protection", entry_protection, 0, pointer, 
    //                  sizeStack_protection, priority_P2, priority_P2, TX_NO_TIME_SLICE, TX_AUTO_START);

    // control main contactors task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_contactorControl, TX_NO_WAIT);

    tx_thread_create(&contactorControl, "contactorControl", entry_contactorControl, 0, pointer, 
    sizeStack_contactorControl, priority_P1, priority_P1, TX_NO_TIME_SLICE, TX_AUTO_START);

    // firmware upgrade task
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_firmwareUpgradeHandler, TX_NO_WAIT);

    tx_thread_create(&firmware_upgrade_thread, "firmware upgrade task", entry_firmwareUpgrade, 0, pointer, 
    sizeStack_firmwareUpgradeHandler, priority_P0, priority_P0, TX_NO_TIME_SLICE, TX_AUTO_START);
    
    // task coulomb counting
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_coulombCounting, TX_NO_WAIT);

    tx_thread_create(&coulombCounting, "coulombCounting", entry_coulombCounting, 0, pointer, 
    sizeStack_coulombCounting, priority_P3, priority_P3, TX_NO_TIME_SLICE, TX_AUTO_START);

    // task charging algorithm
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_chargingAlgorithm, TX_NO_WAIT);

    tx_thread_create(&chargingAlgorithm, "chargingAlgorithm", entry_chargingAlgorithm, 0, pointer, 
    sizeStack_chargingAlgorithm, priority_P3, priority_P3, TX_NO_TIME_SLICE, TX_AUTO_START);

    // task independent watchDog
	tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_iwdgHandler, TX_NO_WAIT);

	tx_thread_create(&iwdgResetTaskHandler, "IWDG Task", iwdgResetEntry, 0, pointer, 
    sizeStack_iwdgHandler, priority_P3, priority_P3, TX_NO_TIME_SLICE, TX_AUTO_START);

    // CAN Tx Queue Task 
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_canTxHandler, TX_NO_WAIT);

    tx_thread_create(&canTxHandler, "canTxHandler", entry_canTxHandler, 0, pointer, 
    sizeStack_canTxHandler, priority_P4, priority_P4, TX_NO_TIME_SLICE, TX_AUTO_START);

    // CAN Rx Queue Task 
    tx_byte_allocate(&mainBytePool, (void*)&pointer, sizeStack_canRxHandler, TX_NO_WAIT);

    tx_thread_create(&canRxHandler, "canRxHandler", entry_canRxHandler, 0, pointer, 
    sizeStack_canRxHandler, priority_P4, priority_P4, TX_NO_TIME_SLICE, TX_AUTO_START);

    /***************************************** Queue Init **********************************************************************************/
    
    // CAN Tx Queue
    tx_byte_allocate(&mainBytePool, (void*)&pointer, CAN_TX_QUEUE_LENGTH * CAN_TX_QUEUE_MESSAGE_SIZE, TX_NO_WAIT);

    tx_queue_create(&canTxQueue, "canTxQueue", CAN_TX_QUEUE_MESSAGE_SIZE, pointer, CAN_TX_QUEUE_LENGTH * CAN_TX_QUEUE_MESSAGE_SIZE);

    // CAN Rx Queue
    tx_byte_allocate(&mainBytePool, (void*)&pointer, CAN_RX_QUEUE_LENGTH * CAN_RX_QUEUE_MESSAGE_SIZE, TX_NO_WAIT);

    tx_queue_create(&canRxQueue, "canRxQueue", CAN_RX_QUEUE_MESSAGE_SIZE, pointer, CAN_RX_QUEUE_LENGTH * CAN_RX_QUEUE_MESSAGE_SIZE);

    // system event flag
    tx_event_flags_create(&systemEventFlag, "system event flag");

    // EEPROM buffer mutex
    tx_mutex_create(&ccOutputMutex, "EEPROM mutex", TX_INHERIT);

    // AFE data collection mutex
    tx_mutex_create(&afeMutex, "AFE mutex", TX_INHERIT);

    // threadx timer
    tx_timer_create(&canTimer, "CAN timer", threadxTimerCallback, 0, TIMER_TICKS, TIMER_TICKS, TX_AUTO_ACTIVATE);
}

VOID threadxTimerCallback(ULONG input) 
{      
    #ifdef DEBUG_CAN
    sendCoulombCountingDataOnCAN(&ccInputData, &ccOutputData);
    sendContactorDataOnCAN(&thresholdData, &cInputData, &cOutputData);
    sendProtectionDataOnCAN(&thresholdData, &pInputData, &pOutputData, &ccOutputData);
    sendBMSDataOnCAN(&mdata, &dpOutputData, &dpInputData);
    // sendCellBalancingDataOnCAN(&thresholdData, &cbInputData, &cbOutputData);
    #endif

    sendDataOnCAN(&mdata, &pOutputData, &ccOutputData);
    sendChargingDataOnCAN(65.0, requestedCurrent_A);       //TODO charging voltage should be a function of max voltage and cells in series

    sendCANDataOnQueue(MARVEL_OFF_READY,&shutdownCount, sizeof(shutdownCount));

    //publishing MARVEL data on CAN 
    MARVEL_DATA[0] = MARVEL_DEVICE_ID;     
    MARVEL_DATA[1] = SW_MAJOR_VERSION;
    MARVEL_DATA[2] = SW_MINOR_VERSION;
    MARVEL_DATA[3] = SW_PATCH_VERSION;
    MARVEL_DATA[4] = HW_MAJOR_VERSION;
    MARVEL_DATA[5] = marvelConfigVersion[CONFIG_MAJOR];
    MARVEL_DATA[6] = marvelConfigVersion[CONFIG_MINOR];
    MARVEL_DATA[7] = activeAPP;
    sendCANDataOnQueue(CAN_DEBUG_ID_033, MARVEL_DATA, sizeof(MARVEL_DATA));

    // //publishing on 7A1
    HARDWARE_DATA[0] = MARVEL_DEVICE_ID;
    HARDWARE_DATA[1] = HW_MAJOR_VERSION;
    HARDWARE_DATA[2] = HW_MINOR_VERSION;
    HARDWARE_DATA[7] = externalShortCircuitDetectionFlag;
    sendCANDataOnQueue(CAN_DEBUG_ID_042, HARDWARE_DATA, sizeof(HARDWARE_DATA));
}

/**
 * @brief iwdg Reset data thread
 * 
 * @param threadInput 
 */
void iwdgResetEntry(ULONG threadInput)
{
    while(1)
    {
        #ifdef WWDOG
            HAL_WWDG_Refresh(&hwwdg);
        #else  
            HAL_IWDG_Refresh(&hiwdg);
        #endif
        
        delayMilliSeconds(LOOP_DELAY_IWDG_RESET);
    }
}

/**
 * @brief can transmit data thread
 * 
 * @param threadInput 
 */
void entry_canTxHandler(ULONG threadInput)
{
    bmsStatus_t     status          = BMS_ERROR;
    canQueueData_t  canTxQueueData  = {0};
    ULONG queue_count = 0;
    
	while(1)
	{
        if((QUEUE_RECEIVE(&canTxQueue, &canTxQueueData, TX_WAIT_FOREVER)) == TX_SUCCESS)
        {
            #ifdef CAN_CHARGER_EXTENDED_ID
                if(canTxQueueData.identifier == CAN_TX_ID_CHARGER)
                {
                    RETRY(RETRY_CAN_COMMUNICATION, status, sendBatteryDataOnCAN(EXTENDED_CAN_ID, canTxQueueData.identifier, canTxQueueData.data));
                }
                else
                {
                    RETRY(RETRY_CAN_COMMUNICATION, status, sendBatteryDataOnCAN(STANDARD_CAN_ID, canTxQueueData.identifier, canTxQueueData.data));
                }
            #else
                RETRY(RETRY_CAN_COMMUNICATION, status, sendBatteryDataOnCAN(STANDARD_CAN_ID, canTxQueueData.identifier, canTxQueueData.data));
            #endif


            // if CAN Tx fails then send the same data to the queue again
            if(status != BMS_OK)
            {
                QUEUE_SEND(&canTxQueue, &canTxQueueData, NO_WAIT);
            }
        }
        tx_queue_info_get(&canTxQueue, NULL, &queue_count, NULL, NULL, NULL, NULL);
	}
}

/**
 * @brief can receive data thread
 * 
 * @param threadInput 
 */
void entry_canRxHandler(ULONG threadInput)
{
    canQueueData_t canRxQueueData = {0};
    uint8_t errorData[8] = {0};

	while(1)
	{
		if((QUEUE_RECEIVE(&canRxQueue, &canRxQueueData, WAIT_FOREVER)) == TX_SUCCESS)
		{
			// message received in queue, now Process the message
            switch(canRxQueueData.identifier)
            {
                case FLASH_CONFIG_CAN_RX_ID:
                    writeDataToFlash(FLASH_CONFIG_SECTION, FLASH_CONFIG_ADDRESS, defaultFlashConfigData, FLASH_CONFIG_TOTAL_LENGTH, canRxQueueData.data);
                    break;

                case VEHICLE_STATE_CAN_RX_ID:
                    chargerTypeFromStark = canRxQueueData.data[6];
                    if(chargerTypeFromStark & 0x01)                //checking charger connection
                    {
                        if((chargerTypeFromStark >> 4) == 0)       //slow charger connected
                        {
                            vcuData.FastCharge           = 0;
                            vcuData.SlowCharge           = 1;
                            vcuData.isChargerConnected   = 1;                        
                        }
                        else if((chargerTypeFromStark >> 4) == 1 || (chargerTypeFromStark >> 4) == 2)  //fast charger/Euler thor connected
                        {
                            vcuData.FastCharge           = 1;
                            vcuData.SlowCharge           = 0;
                            vcuData.isChargerConnected   = 1;                        
                        }
                    }
                    else                                            //charger disconnected                    
                    {
                        vcuData.FastCharge           = 0;
                        vcuData.SlowCharge           = 0;
                        vcuData.isChargerConnected   = 0;  
                    }       
                    break;
                
                // case STARK_VEHICLE_STATE:
                //     canRxQueueData.data[3] == 1 ? (vcuData.isChargerConnected = 1):(vcuData.isChargerConnected = 0);
                //     (canRxQueueData.data[1] & (1 << 5)) ? (vcuData.SlowCharge = 1):(vcuData.SlowCharge = 0);
                //     (canRxQueueData.data[1] & (1 << 6)) ? (vcuData.FastCharge = 1):(vcuData.FastCharge = 0); 

                //     break;


                case EEPROM_RESET_CAN_RX_ID:
                    //Byte 0 - EEPROM RESET
                    if(canRxQueueData.data[0] == 1 && eepromEraseFlag == 0)
                    {
                        eepromEraseFlag++;
                        resetDetectedEEPROM++;

                        // acquiring mutex to access critical resource
                        tx_mutex_get(&ccOutputMutex, TX_WAIT_FOREVER);

                        memset((void*)&socDataWriteBuff, 0, sizeof(socDataWriteBuff));
                        memset((void*)&criticalFlagsWriteBuff, 0, sizeof(criticalFlagsWriteBuff));
                        memset((void*)&merlynDataWriteBuff, 0, sizeof(merlynDataWriteBuff));

                        writeEEPROMDataToBuffer(&ccOutputData, commonDataWriteBuff, ccWriteBufferFLASH, COMMON_DATA);

                    // should set a reset flag in CRITICAL_FLAGS section 

                        RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, socDataWriteBuff, (sizeof(socDataWriteBuff)), SOC_SECTION)); 
                        RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, criticalFlagsWriteBuff, (sizeof(criticalFlagsWriteBuff)), CRITICAL_FLAGS)); 
                        RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, commonDataWriteBuff, (sizeof(commonDataWriteBuff)), COMMON_DATA)); 
                        RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, merlynDataWriteBuff, (sizeof(merlynDataWriteBuff)), MERLYN_HANDLER_SECTION)); 
                        
                        // releasing mutex
                        tx_mutex_put(&ccOutputMutex);


#ifdef STORE_SOC_IN_FLASH
                        ccOutputFlash[0] = 0;
                        ccOutputFlash[1] = 0;
                        ccOutputFlash[2] = 0; 
                        ccOutputFlash[3] = 0;
                        ccOutputFlash[4] = 0;
    
                        flashWrite(SOC_DATA_ADDRESS, ccOutputFlash, sizeof(ccOutputFlash));
#endif
                        //clear the cc input and cc Output
                        memset((void*)&ccInputData, 0, sizeof(coulombCountingInput_t));
                        memset((void*)&ccOutputData, 0, sizeof(coulombCountingOutput_t));

                        systemReset();
                    }
                    
                    //Byte 1 - SHORT CIRCUIT 
                    if(canRxQueueData.data[1] == 1) //Reset the SC Pin
                    {
                        resetShortCircuit(shortCircuit);
                        externalShortCircuitDetectionFlag = 0;
                    }

                    //Byte 2 - BMS IN Testing
                    if(canRxQueueData.data[2] == 1) //Reset the SC Pin
                    {
                        bmsInTestingFlag = 1;
                        switchPartitionAndReset(); 
                    }
                    else
                    {
                        bmsInTestingFlag = 0;
                    }

                    //Byte 3 - BMS system reset
                    // if(canRxQueueData.data[3] == 1)
                    // {
                    //     systemReset();
                    // }

                    //Byte 4 - external balancing
                    if(canRxQueueData.data[4] == 1)
                    {
                        vcuData.BalanceEn_atRest = 1;
                    }

    /*********************************************** Merlyn Requirements ****************************************************/

                    if(canRxQueueData.data[6] == 2)
                    {
                        // change state to EEPROM test
                        testRequest = ENABLE_MERLYN;
                    }
                    if(canRxQueueData.data[6] == 0)
                    {
                        // change state to EEPROM test
                        testRequest = DISABLE_MERLYN;
                    }
                    // Byte 6 - EEPROM test
                    if(canRxQueueData.data[6] == 1)
                    {
                        // change state to EEPROM test
                        testRequest = EEPROM_TEST;
                    }

                    // Byte 7 - Contactor test
                    if(canRxQueueData.data[7] == 1)
                    {
                        // change state to contactor test
                        if(canRxQueueData.data[5] == 1){
                            testRequest = CONTACTOR_TEST_ON;
                        }
                        else if(canRxQueueData.data[5] == 0){
                            testRequest = CONTACTOR_TEST_OFF;
                        }
                    }
                    else if(canRxQueueData.data[7] == 2)
                    {
                        // change state to publish unique IDs
                        testRequest = UNIQUE_IDS;
                    }

                    if(BMS_OK != handleMerlynRequest(testRequest, &mdata))
                    {
                        // notify Merlyn about the failed test
                        errorData[7] = 0xFF;
                        sendCAN(communicationCAN, S, CAN_RX_ID_003, errorData, 8);     
                    }

                    break;

                default:
                    break;
            }
        }
	}
}

/**
 * @brief state machine thread
 * 
 * @param threadInput 
 */
void entry_chargingAlgorithm(ULONG threadInput)
{
    // bmsMainData_t mdata = {0};
    ChargingAlgorithm_initialize();
    chargingInit(&thresholdData, &ChargingAlgorithm_U);

    while(1)
    {
        chargingInput(&DataPipeline_U, &DataPipeline_Y, &ChargingAlgorithm_U);
        ChargingAlgorithm_step();
        chargingOutput(&chargingOutputData, &ChargingAlgorithm_Y);

        // uint8_t canData[8] = {0};
        // sendCAN(communicationCAN, STANDARD_CAN_ID, 0x423, canData, 8);

        delayMilliSeconds(LOOP_DELAY_STATE_MACHINE);
    }
}

/**
 * @brief protection thread
 * 
 * @param threadInput 
 */
void entry_protection(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    
    pipelineInit();
    readFromPipeline(&mdata);

    Protection_initialize();
    protectionInit(&thresholdData, &dpInputData, &dpOutputData, &pInputData, &Protection_U);

    while(1)
    {
        readFromPipeline(&mdata);
        protectionInput(&mdata, &dpOutputData, &pInputData, &Protection_U);
        Protection_step();
        protectionOutput(&mdata, &pOutputData, &Protection_Y);

        delayMilliSeconds(LOOP_DELAY_PROTECTION);
    }
}

/**
 * @brief upgrade firmware thread
 * 
 * @param threadInput 
 */

void entry_firmwareUpgrade(ULONG thread_input)
{
    while(1)
    {
        upgradeFirmware();
        delayMilliSeconds(LOOP_DELAY_FIRMWARE_UPGRADE);
    }	
}


/**
 * @brief contactor ocntrol thread
 * 
 * @param threadInput 
 */
void entry_contactorControl(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    pipelineInit();
    readFromPipeline(&mdata);
    Contactors_initialize();
    contactorInit(&thresholdData, &cInputData, &Contactors_U);
    
    HighLevelStatemachine_initialize();
    stateMachineInit(&thresholdData, &dpInputData, &HighLevelStatemachine_U);

    Protection_initialize();
    protectionInit(&thresholdData, &dpInputData, &dpOutputData, &pInputData, &Protection_U);

    cOutputData.negativeContactorState  = 0;
    cOutputData.positiveContactorState  = 0;
    cOutputData.preChargeContactorState = 0;

    while(1)
    {
        readFromPipeline(&mdata);

        // protection block processing
        protectionInput(&mdata, &dpOutputData, &pInputData, &Protection_U);
        Protection_step();
        protectionOutput(&mdata, &pOutputData, &Protection_Y); 

        contactorInput(&mdata, &dpOutputData, &pOutputData, &cInputData, &cOutputData, &Contactors_U, &smOutputData);
        Contactors_step();
        contactorOutput(&mdata, &cOutputData, &Contactors_Y);

        // BMS state
        stateMachineInput(&dpOutputData, &pOutputData, &cOutputData, &HighLevelStatemachine_U);
        HighLevelStatemachine_step();
        stateMachineOutput(&smOutputData, &HighLevelStatemachine_Y, &mdata);

        // marvelState = mdata.bmsState;
        publishErrorCodes(&cOutputData, &pOutputData);

        // writeToPipeline(&mdata);
        delayMilliSeconds(LOOP_DELAY_CONTACTOR_CONTROL);
    }
}


#ifdef EKF
/**
 * @brief ccEKF thread 
 * 
 * @param threadInput 
 */

void entry_ccEKF(ULONG threadInput)
{
    bmsMainData_t mdata = {0};
    // bmsStatus_t status = BMS_ERROR;

    // pipelineInit();
    delayMilliSeconds(1000); // delay before init
    EKF_initialize();
    coulombCountingEKFInit(&ccEKFInputData, &EKF_U);
    
    while(1)
    {
        readFromPipeline(&mdata);
        coulombCountingEKFInput(&mdata, &dpOutputData, &pOutputData, &ccEKFInputData, &EKF_U);
        EKF_step();
        coulombCountingEKFOutput(&mdata, &ccEKFOutputData, &EKF_Y);
        
        delayMilliSeconds(100);
    }
}

#else
/**
 * @brief coulomb counting thread 
 * 
 * @param threadInput 
 */

void entry_coulombCounting(ULONG threadInput)
{
    bmsMainData_t mdata     = {0};
    SOCEstimation_initialize();
    coulombCountingInit(&ccInputData, &SOCEstimation_U, &ccOutputData);

    while(1)
    {
        fetchSoCFromLookupTable(&dpOutputData);
        readFromPipeline(&mdata);

        if(tx_event_flags_get(&systemEventFlag, DATA_COLLECTION_SUCCESSFUL, TX_OR, &actualFlag, TX_WAIT_FOREVER) == TX_SUCCESS)
        {
            coulombCountingInput(&mdata, &dpOutputData, &chargingOutputData, &pOutputData, &ccInputData, &SOCEstimation_U);
            SOCEstimation_step();
            coulombCountingOutput(&mdata, &ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash, &SOCEstimation_Y);

            //Keep the function locked until ccOutput SoC == EEPROM SoC {indicating completion of first cycle}

            if(!bypassEEPROMFetchRead && !bypassEEPROMFetchWrite)
            {
                writeSoCToFlash(&ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash);
            }

            // uint8_t canData[8] = {0};
            // sendCAN(communicationCAN, STANDARD_CAN_ID, 0x422, canData, 8);
        }
        delayMilliSeconds(LOOP_COULOMB_COUNTING);
    }
}
#endif

/**
 * @brief collect battery data thread
 * 
 * @param threadInput 
 */
void entry_collectBatteryData(ULONG threadInput)
{
    bmsStatus_t status = BMS_ERROR;
    
    // initializing interfaces
    status = initializeMarvelFunctionalities();

    // handling EEPROM data collection and data corruption scenarios 
    status = handleEEPROMDataCollection();
    
    while(1)
    {
        // reading from ADBMS6832
        RETRY(RETRY_CMU_READ, status, readCellVoltages(&mdata));

        RETRY(RETRY_CMU_READ, status, readCellTemperatures(&mdata));
        
        // RETRY(RETRY_CMU_READ, status, readPackStatus(&mdata));

        // Read Temp from MCU
        // RETRY(RETRY_CMU_READ, status, readCellTemperaturesValues(&mdata)); 

        // // reading from ADBMS2950
        RETRY(RETRY_CCM_READ, status, readPackVoltageCurrent(&mdata));       

        // DCC discharge test function. Turning on all 16 mosfets
        // cellsToBalance[0] = 0xFFFF;     // setting all 16 bits

        // adBms6830_set_dcc_discharge(cellsToBalance, mdata.cmuData.cmuCount, mdata.cmuData.cmuCellData);

        // clearing all the bits and turning off all the MOSFETS
        // adBms6830_clear_dcc_discharge(mdata.cmuData.cmuCount, mdata.cmuData.cmuCellData);

        tx_event_flags_set(&systemEventFlag, DATA_COLLECTION_SUCCESSFUL, TX_OR);

        mdata.mcuResetCounter++;

        dataPipelineInput(&mdata, &dpInputData, &ccOutputData, &DataPipeline_U);
  
        DataPipeline_step();
        dataPipelineOutput(&mdata, &dpOutputData, &DataPipeline_Y);

        calculateDynamicCurrentLimits(&mdata, &dpInputData, &dpOutputData, &dynamicLimitsTable, &DataPipeline_Y);

        cellBalancingInput(&mdata, &dpOutputData, &cbInputData, &CellBalancing_U);
        CellBalancing_step();
        cellBalancingOutput(&mdata, &cbOutputData, &CellBalancing_Y);

        SoCManipulation(&mdata, &SOCEstimation_Y);

        // shutdown_sequence
        shuntdownRoutine();



        delayMilliSeconds(LOOP_DELAY_100MS);
    }
}

float calculateChargingCurrent(dataPipelineOutput_t *dpOutput, coulombCountingOutput_t *ccOutput, chargingOutputData_t *chargerOutput, thresholdData_t *tData)
{
    float requestedCurrent_A        = 0;
    float oldRequestedCurrent_A     = 0;
    float newReqCurrent             = 0;
    // float arcReactorDCLI            = 0;
    float dcli_A                    = 0;

    bool ccLogicActivated  = false;
    bool cvLogicActivated  = false;

    uint16_t minCellVoltageForLatch   = 3380;
    uint16_t minRequestedCurrentFast  = 18000;
    uint16_t minRequestedCurrentSlow  = 15000;
    uint16_t minLatchingCurrent        = 5000;
    uint16_t maxAllowedImblance = 0;

    static uint8_t debounceCount  = 0;
    static bool fastChargingLatch = false;
    static bool cvModeLatch       = false;

    // arcReactorDCLI      = dpOutput->arcReactorDCLI_A;
    dcli_A              = dpOutput->DCL_ChargingCurrent_A;

    // calculating requested current
    if(chargerOutput->ChargingState == slowCharge)
    {
        oldRequestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageSlowCharging - dpOutput->voltageSense.voltageMax) * k;
        oldRequestedCurrent_A = (oldRequestedCurrent_A > SlowChargingMaxCurrent_A) ? SlowChargingMaxCurrent_A : oldRequestedCurrent_A;
    }
    else if(chargerOutput->ChargingState == fastCharge)
    {
        oldRequestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageFastCharging - dpOutput->voltageSense.voltageMax) * k;
        oldRequestedCurrent_A = (oldRequestedCurrent_A > FastChargingMaxCurrent) ? FastChargingMaxCurrent : oldRequestedCurrent_A;
    }
    else
    {
       oldRequestedCurrent_A = 0;
       cvModeLatch = false;
    }

    ccLogicActivated = (dpOutput->voltageSense.voltageMax < 3380) ? true : false;
    cvLogicActivated = (dpOutput->voltageSense.voltageMax < 3380) ? false : true;

    maxAllowedImblance  = findMaxAllowedImbalance(ccOutput->SOC_pct, chargerOutput);

    if(ccLogicActivated && !cvModeLatch)
    {
        newReqCurrent       = oldRequestedCurrent_A / ((float)dpOutput->voltageSense.voltageDelta / (float)maxAllowedImblance);
        requestedCurrent_A  = newReqCurrent < oldRequestedCurrent_A ? newReqCurrent : oldRequestedCurrent_A;
    }
    else if(cvLogicActivated || cvModeLatch)
    {
        if(chargerOutput->ChargingState == slowCharge)
        {
            requestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageSlowCharging - dpOutput->voltageSense.voltageMin) * REQUESTED_CURRENT_SCALING;
            requestedCurrent_A = (requestedCurrent_A > SlowChargingMaxCurrent_A) ? SlowChargingMaxCurrent_A : requestedCurrent_A;
        }
        else if(chargerOutput->ChargingState == fastCharge)
        {
            if((dpOutput->voltageSense.voltageMin < 3400) && (!fastChargingLatch))          // charged to almost 85% SoC
            {
                requestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageFastCharging - dpOutput->voltageSense.voltageMin) * REQUESTED_CURRENT_SCALING_FASTCHARGING;
            }
            else    
            {
                requestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageSlowCharging - dpOutput->voltageSense.voltageMin);
                fastChargingLatch = true;
            }
            requestedCurrent_A = (requestedCurrent_A > FastChargingMaxCurrent) ? FastChargingMaxCurrent : requestedCurrent_A;
        }
        cvModeLatch = true;
    }
    else
    {
        requestedCurrent_A = 0;
    }


    // adding 1.2 K factor latching logic for fast charging
    if(chargerOutput->ChargingState == fastCharge)
    {
        requestedCurrent_A = ((int32_T)tData->ChargeAlgorithm.targetVoltageFastCharging - dpOutput->voltageSense.voltageMax) * 1.2;
        requestedCurrent_A = (requestedCurrent_A > FastChargingMaxCurrent) ? FastChargingMaxCurrent : requestedCurrent_A;
    }


    // requestedCurrent_A  = requestedCurrent_A < arcReactorDCLI ? requestedCurrent_A : arcReactorDCLI;         // handling arc reactor DCLI condition here
    requestedCurrent_A  = requestedCurrent_A < dcli_A ? requestedCurrent_A : dcli_A;                         // handling DCLI condition here

    // handle imbalance condition
    vcuData.BalanceEn_atRest = (dpOutput->voltageSense.voltageDelta > maxAllowedImblance) ? true : false;

    // protection and latch condition handling
    if(dpOutput->voltageSense.voltageDelta >= 200 && cvModeLatch)
    {
        requestedCurrent_A = 0;
    }

    // give latch true flag when the min cell has been charged to max capacity
    if((((dpOutput->voltageSense.voltageMin >= minCellVoltageForLatch) && (dpOutput->current < minRequestedCurrentSlow) && (chargerOutput->ChargingState == slowCharge)) || ((dpOutput->voltageSense.voltageMin >= minCellVoltageForLatch) && (dpOutput->current < minRequestedCurrentFast) && (chargerOutput->ChargingState == fastCharge))) && (dpOutput->voltageSense.voltageDelta < 200) && !fullChargeLatch && (dpOutput->current >= minLatchingCurrent))
    {
        // should also add a debounce time of 100mSec here to prevent false latching
        debounceCount++;
        if(debounceCount >= 20)
        {
            fullChargeLatch = true;
            latchCheckFlag  = true;
            pseudoLatchFlag = true;

            // calibrating SoC parameters to 100%
            ccOutput->initialCapacity        = 180000;
            ccOutput->totalCapacityRemaining = 180000;
            ccOutput->SOC_pct                = 100;
            // ccOutput->SOC_cpct               = 10000;

            // should remove this when integrating charging logic in model

            bypassEEPROMFetchRead  = true;
            bypassEEPROMFetchWrite = true; 
            externalLatchDetected  = true;
            SOCEstimation_step();
            coulombCountingOutput(&mdata, &ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash, &SOCEstimation_Y);

            writeSoCToFlash(&ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash);
            
        }
        
    }

    if((dpOutput->voltageSense.voltageMin <= tData->UV_ErrorVoltage && chargerOutput->ChargingState != noCharge))
    {
        requestedCurrent_A = PSEUDO_LATCH_CURRENT;
    }

    if((fullChargeLatch) || (dpOutput->voltageSense.voltageMax > tData->ChargeAlgorithm.targetVoltageFastCharging))
    {
        requestedCurrent_A = 0;
    }
    requestedCurrent_A = (requestedCurrent_A < 0) ? 0 : requestedCurrent_A; 

    return requestedCurrent_A;
}

bmsStatus_t calculateDynamicCurrentLimits(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, dataPipelineOutput_t *dpOutput, DynamicLimitsTable_t *ldata, ExtY_DataPipeline_T *dpOutputMBD)
{
    bmsStatus_t status                 = BMS_ERROR;

    if( (NULL == mdata)        ||\
        (NULL == dpInput)      ||\
        (NULL == dpOutput)     ||\
        (NULL == ldata)    )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    int32_t cellTemperature            = 0;
    int32_t contChargingLimit_A        = 0;
    int32_t contDischargingLimit_A     = 0;
    int32_t contChargingArcReactor_A   = 0;
    int32_t instantaneousChargingLimit_A    = 0;
    int32_t instantaneousDischargingLimit_A = 0; 
    int32_t eFuseChargingLimit_A2s     = 0;
    int32_t eFuseDischargingLimit_A2s  = 0;
    static uint8_t eFuseAllowedTime    = 10;

    if(dpInput->TemperatureSelectionforDynamicC) //1 is Avg temperature
    {
        for(uint8_t cellIndex = 0; cellIndex < 6; cellIndex++)
        {
            cellTemperature += dpInput->temperature1_C[cellIndex];
        }

        //taking avg of 6 temperatures
        cellTemperature /= 6;
    }
    else    //0 is for max temperature
    {
        cellTemperature = dpOutput->temperatureSense[0].temperatureMax;
    }
        
    //To Do: If SoC or Temperature is not changed then return and don't do the calculations

    // dpInput->soc    = 50;
    // cellTemperature = 24;

    //ContinuousCharging
    contChargingLimit_A = 0;
    lookup2d(&(ldata->ContinuousChargingTable), dpInput->soc, cellTemperature, &contChargingLimit_A);

    //ContinuousDischarging
    contDischargingLimit_A = 0;
    lookup2d(&(ldata->ContinuousDischargingTable), dpInput->soc, cellTemperature, &contDischargingLimit_A);

    //InstantaneousDischarging
    instantaneousDischargingLimit_A = 0;
    lookup2d(&(ldata->instantaneousDischargingTableData), dpInput->soc, cellTemperature, &instantaneousDischargingLimit_A);
    
    //InstantaneousCharging
    instantaneousChargingLimit_A = 0;
    lookup2d(&(ldata->instantaneousChargingTableData), dpInput->soc, cellTemperature, &instantaneousChargingLimit_A);

    //Arc Reactor Lookup table
    contChargingArcReactor_A = 0;
    lookup2d(&(ldata->ContChargingArcReactorTable), dpInput->soc, cellTemperature, &contChargingArcReactor_A);

    // manipulated values
    // contChargingLimit_A    = 104;
    // contDischargingLimit_A = 105;

    // instantaneousChargingLimit_A    = 150;
    // instantaneousDischargingLimit_A = 150;

    eFuseChargingLimit_A2s    = pow((instantaneousChargingLimit_A - contChargingLimit_A), 2) * eFuseAllowedTime;
    eFuseDischargingLimit_A2s = pow((instantaneousDischargingLimit_A - contDischargingLimit_A), 2) * eFuseAllowedTime;
    
    //copying dynamic current limits to dataPipeline Output and dataPipeline Output MBD
    if(dpInput->capacityRemaining <= 0)
    {
        dpOutput->DCL_DisChargingCurrent_A      = 80;       // this will prevent the battery from discharging till UV condition is met
        dpOutput->i2t_DisChargingCurrent_A2sec  = 0;
    }
    else
    {
        dpOutput->DCL_DisChargingCurrent_A  = contDischargingLimit_A;
        dpOutput->i2t_DisChargingCurrent_A2sec = eFuseDischargingLimit_A2s;
    }

    // dpOutput->DCL_DisChargingCurrent_A = 80;

    //copying eFuse limits to dataPipeline Output and dataPipeline Output MBD
    
    dpOutput->DCL_ChargingCurrent_A        = contChargingLimit_A;
    dpOutput->i2t_ChargingCurrent_A2sec    = eFuseChargingLimit_A2s;

    dpOutput->arcReactorDCLI_A      = contChargingArcReactor_A;

    dpOutputMBD->DataPipelineb.DCL_ChargingCurrent_A     = dpOutput->DCL_ChargingCurrent_A;
    dpOutputMBD->DataPipelineb.DCL_DisChargingCurrent_A  = dpOutput->DCL_DisChargingCurrent_A;

    dpOutputMBD->DataPipelineb.i2t_ChargingCurrent_A2sec    = dpOutput->i2t_ChargingCurrent_A2sec;
    dpOutputMBD->DataPipelineb.i2t_DisChargingCurrent_A2sec = dpOutput->i2t_DisChargingCurrent_A2sec;

    //copying to main data
    //To Do: copy it using separate function
    mdata->dynamicLimitIn_A              = dpOutput->DCL_ChargingCurrent_A;
    mdata->dynamicLimitOut_A             = dpOutput->DCL_DisChargingCurrent_A;

    mdata->instaneousLimitIn_A           = instantaneousChargingLimit_A;
    mdata->instaneousLimitOut_A          = instantaneousDischargingLimit_A;

    status = BMS_OK;
	return status;
}



#ifdef EKF
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, protectionInput_t *pInput, contactorInput_t *cInput, ccEKFInput_t *ccEKFInput)
#else
bmsStatus_t thresholdInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, cellBalancingInput_t *cbInput, protectionInput_t *pInput, contactorInput_t *cInput, coulombCountingInput_t *ccInput)
#endif
{
    bmsStatus_t status = BMS_ERROR;
#ifdef EKF
	if( (NULL == tData)        ||\
        (NULL == dpInput)      ||\
        (NULL == cbInput)      ||\
        (NULL == pInput)       ||\
        (NULL == cInput)       ||\
        (NULL == ccEKFInput))
#else
	if( (NULL == tData)        ||\
        (NULL == dpInput)      ||\
        (NULL == cbInput)      ||\
        (NULL == pInput)       ||\
        (NULL == cInput)       ||\
        (NULL == ccInput))
#endif
	{
		status = BMS_THRESHOLD_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    tData->MaxOperatingTemp_C           = 60;
    tData->MaxAllowedTempChange_C       = 2;
    tData->MaxAllowedTempGradientGroup1_C = 10;
    tData->MaxAllowedTempGradientGroup2_C = 50;
    tData->Protections_AverageTimeforProtection_msec = 2000;

    //VCU
    tData->NormalModeTimeout_msec       = 100;
    tData->PowerSaverModeTimeout_msec   = 100;
    tData->FaultsModeTimeout_msec       = 100;
    tData->ServiceModeTimeout_msec      = 100;

    // tData->dynamicInstTimeout_msec      = 200;
    tData->i2tTimeout_msec              = 1000;
    tData->dynamicContTimeout_msec      = 1000;
    tData->dynamicCurrentThresholdsGain = 1;
    tData->TempGroupTimeout             = 100;
    tData->PermanentFailCounts_eFuse    = 4;
    

    //protections threshold init
    tData->OCC_ErrorCurrent 			= CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_ERROR_CURRENT_A_INDEX]);
    tData->OCC_WarningCurrent 		    = CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_WARNING_CURRENT_A_INDEX]);
    tData->OCD_ErrorCurrent 			= CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCD_ERROR_CURRENT_A_INDEX]);
    tData->OCD_WarningCurrent 		    = CONVERT_AMPERE_TO_MILLI_AMPERE(defaultFlashConfigData[OCC_WARNING_CURRENT_A_INDEX]);

    tData->OCC_ErrorTimeout 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OCC_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX] ;
    tData->OCD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OCD_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX] ;

    tData->RecoveryProtections_OCC_Warning_mA       = 1000;
    tData->RecoveryProtections_OCC_Error_mA         = 1000;
    tData->RecoveryProtections_OCD_Warning_mA       = 1000;
    tData->RecoveryProtections_OCD_Error_mA         = 1000;
    tData->CurrentChangeTimeout_msec                = 1000;
    tData->eFuseMeltingTimeout_msec                 = 300;
    tData->overCurrentProtectionTimeout             = 100;
    tData->BMS_ActivatingToActive_Timeout_msec      = 100;
    tData->BMS_ActivatingToError_Timeout_msec       = 300;
    tData->BMS_ActiveToError_Timeout_msec           = 1000;
    tData->BMS_ActiveToReady_Timeout_msec           = 100;
    tData->BMS_ReadyToActivating_Timeout_msec       = 100;
    tData->BMS_ErrorToActivating_Timeout_msec       = 100;

    //temperature external
    for(int i = 0; i < 2; i++)
    {
        tData->temperatureData[i].OTC_ErrorTemperature 		    = temperatureLimits[i][OTC_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTC_WarningTemperature 		= temperatureLimits[i][OTC_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTC_RecoveryTemperature 	    = temperatureLimits[i][OTC_RECOVERY_TEMPERATURE_INDEX];
        
        tData->temperatureData[i].OTD_ErrorTemperature 		    = temperatureLimits[i][OTD_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTD_WarningTemperature 		= temperatureLimits[i][OTD_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].OTD_RecoveryTemperature 	    = temperatureLimits[i][OTD_RECOVERY_TEMPERATURE_INDEX];
        
        tData->temperatureData[i].UTC_ErrorTemperature	 	    = temperatureLimits[i][UTC_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTC_WarningTemperature	 	= temperatureLimits[i][UTC_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTC_RecoveryTemperature	 	= temperatureLimits[i][UTC_RECOVERY_TEMPERATURE_INDEX];

        tData->temperatureData[i].UTD_ErrorTemperature	 	    = temperatureLimits[i][UTD_ERROR_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTD_WarningTemperature	 	= temperatureLimits[i][UTD_WARNING_TEMPERATURE_INDEX];
        tData->temperatureData[i].UTD_RecoveryTemperature	 	= temperatureLimits[i][UTD_RECOVERY_TEMPERATURE_INDEX];

        /*------ Temperature Groups 1 to 5 Timeouts ------*/
        tData->temperatureData[i].OTC_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_RecoveryTimeout 	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTC_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

        tData->temperatureData[i].OTD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_WarningTimeout  	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_RecoveryTimeout 	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].OTD_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];


        tData->temperatureData[i].UTC_ErrorTimeout  	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_WarningTimeout	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_RecoveryTimeout  	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTC_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

        tData->temperatureData[i].UTD_ErrorTimeout	            = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_WarningTimeout	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
        tData->temperatureData[i].UTD_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    }
    
    //ov uv
    tData->slowCharging_OV_ErrorVoltage	 		    = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->slowCharging_OV_WarningVoltage	 		= defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX] - 20;
    tData->slowCharging_OV_RecoveryVoltage	 		= defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX] - 50;

    tData->fastCharging_OV_ErrorVoltage             = 3650;
    tData->fastCharging_OV_WarningVoltage           = 3650 - 20;          
    tData->fastCharging_OV_RecoveryVoltage          = 3650 - 50;


    tData->OV_ErrorTimeout	 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    tData->OV_WarningTimeout            = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
    tData->OV_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    tData->OV_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];
    
    // tData->UV_ErrorVoltage	 		    = defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX];
    // tData->UV_WarningVoltage	 		= defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX] + 20;
    // tData->UV_RecoveryVoltage	 		= defaultFlashConfigData[CELL_MIN_VOLTAGE_THRESHOLD_mV_INDEX] + 50;
    tData->UV_ErrorVoltage	 		    = 3100;
    tData->UV_WarningVoltage	 		= 3120;
    tData->UV_RecoveryVoltage	 		= 3150;
    tData->UV_ErrorTimeout	 	        = defaultFlashConfigData[ERROR_TIMEOUT_ms_INDEX];
    // tData->UV_WarningTimeout 	        = defaultFlashConfigData[WARNING_TIMEOUT_ms_INDEX];
    tData->UV_WarningTimeout 	        = 1000;
    tData->UV_RecoveryTimeout	        = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX]; 
    tData->UV_NoErrorTimeout_msec       = defaultFlashConfigData[RECOVERY_TIMEOUT_ms_INDEX];

    tData->HighImbalanceError_mV        = 250;
    tData->MinOpearatingVolt_mV         = 2900;

    tData->ShortCircuitCurrent_mA       = CONVERT_AMPERE_TO_MILLI_AMPERE(500);
    tData->SuddenVoltageDrop_mV_by_time = 1000;

    tData->ThermalRunawayWarningTimeout_msec    = 1000;
    tData->ThermalRunawayErrorTimeout_msec      = 1000;
    tData->TemperatureGradientTimeout_msec      = 1000;
    tData->HighImbalanceFlagTimeout_msec        = 1000;
    tData->ShortCircuitDetect_msec              = 500;
    tData->SuddenVoltageDropTimeout_msec        = 500;
    tData->AdditionalProtectionEn               = 0;     //0 - Disable, 1 - Enable
    tData->HighImbalanceFlagEn                  = 0;     //0 - Disable, 1 - Enable

    // tData->overCurrentWarningToErrorTimeout_msec            = 60000 / PROTECTION_TASK_SCALING;
    tData->overCurrentWarningToErrorTimeout_msec            = 60000;
    tData->overCurrentRecoveryCurrentProtectionTimeout_msec = 300;

    //cell balancing threshold init
    tData->balancingMinThreshold                  = 3100;
    tData->balancingStateRestPeroid               = defaultFlashConfigData[BALANCING_MAX_OFF_TIME_ms_INDEX];          //off time
    tData->balancingMaxAllowedImbalance           = defaultFlashConfigData[CELL_IMBALANCE_THRESHOLD_mV_INDEX] ;
    tData->balancing_SlowCh_OVThr_mV              = defaultFlashConfigData[CELL_MAX_VOLTAGE_THRESHOLD_mV_INDEX];
    tData->balancing_FastCh_OVThr_mV              = 3610;      
    tData->balancing_DefaultBalancingTimeOn_msec  = defaultFlashConfigData[BALANCING_MAX_ON_TIME_ms_INDEX];
    tData->balancingCurrentDerateActivationTemp_C = 80;  //derating will start at 80 degree C
    tData->balancingCurrentDerateCheckTime_msec   = 100;

    //contactor threshold init
    tData->contactorCutoffTime			          = 100;
    tData->preChargeContactorCurrent			  = 250;
    // tData->preChargeTimeout_msec                  = 1000 * 1.25;
    tData->preChargeTimeout_msec                  = defaultFlashConfigData[PRECHARGE_TIMEOUT];
    // tData->preChargeRetryTimeout_msec             = 2500;       //TODO should debug and see the need for scaling later | actual time 2.5 secs | factor of 1.3
    tData->preChargeRetryTimeout_msec             = defaultFlashConfigData[PRECHARGE_RETRY_TIMEOUT];       //TODO should debug and see the need for scaling later | actual time 2.5 secs | factor of 1.3
    tData->PreChargeVoltageDelta_mV               = 20000;
    tData->preChargeRetryLimit                    = 1;
    tData->ContWeldCheckCurrent_mA                = CONVERT_AMPERE_TO_MILLI_AMPERE(500);
    tData->ContactorWarningCounterLimit           = 232;
    tData->ContactorWeldTimeout_msec              = 200;
    tData->OpenToPreChargeStateTime_msec          = 100;
    tData->Min_PreChargeTime_msec                 = 3000;
    tData->Max_PreChargeTime_msec                 = 5000;
    tData->ContOpenCurrent_mA                     = 100000;
    tData->FlagDebounceTime_msec                  = 300;
    tData->Min_ClosedToOpeningTime_msec           = 10;
    tData->Max_ClosedToOpeningTime_msec           = 10;
    tData->PreChargeRecoveryTimeout_msec          = 100;

    //VCU
    tData->BMS_Timeout_msec                     = 1000;
    tData->ContactorsCheck_Timeout_msec         = 1000;
    tData->VCU_Timeout_msec                     = 1000;
    tData->MosfetsPreChargeCurrentLimit_mA      = 1000;
    tData->ExternalShortCheckTime_msec          = 1000;
    tData->ExternalShortLimit                   = 5;
    tData->ContWeldCheckLimit                   = 200;
    tData->BMS_stateMachineTimeout              = 100;

    //charging
    tData->ChargeAlgorithm.SOC_forCVCharge_cpct         = 7500;
    tData->ChargeAlgorithm.BatteryRevivalVoltage_mV     = 3100;
    tData->ChargeAlgorithm.NoCharge_K                   = 0;
    tData->ChargeAlgorithm.FastChargersCurrentLimit_A   = 180;
    tData->ChargeAlgorithm.SlowChargersCurrentLimit_A   = 45;
    tData->ChargeAlgorithm.slowCharging_K               = 1000;
    tData->ChargeAlgorithm.fastCharging_K               = 1000;
    tData->ChargeAlgorithm.CVTransitionVoltage_mV       = 3380;
    tData->ChargeAlgorithm.TransitionTimeout_msec       = 500;
    tData->ChargeAlgorithm.targetVoltageSlowCharging    = 3420;
    tData->ChargeAlgorithm.targetVoltageFastCharging    = 3550;

    //contactor data init
    cInput->communicationFlag 		              = 0;           // 0
    cInput->permanentFailFlag		              = 0;           // 0
    cInput->contactorErrorFlag		              = 0;           // 0

    dpInput->thresholdDataBus.chargeDetectionThreshold_mA        = 500;
    dpInput->isDynamicCurrentEn                                  = 1;
    dpInput->nCells_series                                       = defaultFlashConfigData[CELL_IN_SERIES_INDEX];
    dpInput->nCells_parallel                                     = 2;
    dpInput->TemperatureSelectionforDynamicC                     = 0;    //0 - Max Temp as input, 1 - Avg Temp as input 
    dpInput->nCelltemperatureGroups                              = 1;    //group 1 temperatures are used for dynamic limits

    //cell balancing
    cbInput->BalancingTempGroup      = 2;
    cbInput->NumberOfMosfets         = 12;
    cbInput->StLineEq_varA           = (defaultFlashConfigData[BALANCING_DERATING_START_TEMP_C_INDEX] - defaultFlashConfigData[BALANCING_DERATING_END_TEMP_C_INDEX])/(float)(defaultFlashConfigData[BALANCING_MAX_ON_TIME_ms_INDEX] - defaultFlashConfigData[BALANCING_MIN_ON_TIME_ms_INDEX]);
    cbInput->StLineEq_varB           = defaultFlashConfigData[BALANCING_DERATING_END_TEMP_C_INDEX] - (cbInput->StLineEq_varA * defaultFlashConfigData[BALANCING_MIN_ON_TIME_ms_INDEX]);

    status = BMS_OK;
    return status;
}

bmsStatus_t dataPipelineInit(DynamicLimitsTable_t *ldata, dataPipelineInput_t *dpInput, ExtU_DataPipeline_T *dpInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpInput)    ||\
        (NULL == dpInputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    dpInputMBD->Thresholds.ChargeDetectionThreshold_mA      =  dpInput->thresholdDataBus.chargeDetectionThreshold_mA;
    dpInputMBD->isDynamicCurrentEn                          =  dpInput->isDynamicCurrentEn;
    dpInputMBD->nCells_series                               =  dpInput->nCells_series;
    dpInputMBD->nCells_parallel                             =  dpInput->nCells_parallel;
    dpInputMBD->TempSelectForDCL             =  dpInput->TemperatureSelectionforDynamicC;
    dpInputMBD->nCellTempGrps                      =  dpInput->nCelltemperatureGroups;
    
    // Create a Table2d structure for ContinuousCharging
    ldata->ContinuousChargingTable.numberOfColumns = (chargingMaxElements[1]);                               //number of elements in SoC table 
    ldata->ContinuousChargingTable.numberOfRows    = (chargingMaxElements[0]);                               //number of elements in temperature table
    ldata->ContinuousChargingTable.columnsData     = (const int32_t *) socData;                              //SoC data table
    ldata->ContinuousChargingTable.rowsData        = (const int32_t *) chargingTemperatureData;              //Temperaure data table  
    ldata->ContinuousChargingTable.tableData       = (const int32_t *) continuousChargingTableData;          //tableData

    // Create a Table2d structure for ContinuousDischarging  
    ldata->ContinuousDischargingTable.numberOfColumns = (dischargingMaxElements[1]);                         //number of elements in SoC table 
    ldata->ContinuousDischargingTable.numberOfRows    = (dischargingMaxElements[0]);                         //number of elements in temperature table
    ldata->ContinuousDischargingTable.columnsData     = (const int32_t *) socData;                           //SoC data table
    ldata->ContinuousDischargingTable.rowsData        = (const int32_t *) dischargingTemperatureData;        //Temperaure data table  
    ldata->ContinuousDischargingTable.tableData       = (const int32_t *) continuousDischargingTableData;    //tableData

    // Create a Table2d structure for InstantaneousCharging 
    ldata->instantaneousChargingTableData.numberOfColumns     = (chargingMaxElements[1]);                        //number of elements in SoC table 
    ldata->instantaneousChargingTableData.numberOfRows        = (chargingMaxElements[0]);                        //number of elements in temperature table
    ldata->instantaneousChargingTableData.columnsData         = (const int32_t *) socData;                          //SoC data table
    ldata->instantaneousChargingTableData.rowsData            = (const int32_t *) chargingTemperatureData;          //Temperaure data table 
    ldata->instantaneousChargingTableData.tableData           = (const int32_t *) instantaneousChargingTableData;   //tableData

    // Create a Table2d structure for InstantaneousCharging 
    ldata->instantaneousDischargingTableData.numberOfColumns     = (dischargingMaxElements[1]);                           //number of elements in SoC table 
    ldata->instantaneousDischargingTableData.numberOfRows        = (dischargingMaxElements[0]);                           //number of elements in temperature table
    ldata->instantaneousDischargingTableData.columnsData         = (const int32_t *) socData;                             //SoC data table
    ldata->instantaneousDischargingTableData.rowsData            = (const int32_t *) dischargingTemperatureData;          //Temperaure data table 
    ldata->instantaneousDischargingTableData.tableData           = (const int32_t *) instantaneousDischargingTableData;   //tableData

    // Create a Table2d structure for ArcReactor SoP table
    ldata->ContChargingArcReactorTable.numberOfColumns           = (chargingMaxElementsArcReactor[1]);
    ldata->ContChargingArcReactorTable.numberOfRows              = (chargingMaxElementsArcReactor[0]);
    ldata->ContChargingArcReactorTable.columnsData               = (const int32_t *) socData;
    ldata->ContChargingArcReactorTable.rowsData                  = (const int32_t *) chargingTemperatureDataArcReactor;
    ldata->ContChargingArcReactorTable.tableData                 = (const int32_t *) arcReactorChargingTable;

    // Create a Table2d structure for eFuseCharging  
    // ldata->eFuseChargingTable.numberOfColumns = (chargingMaxElements[1]);                         //number of elements in SoC table 
    // ldata->eFuseChargingTable.numberOfRows    = (chargingMaxElements[0]);                         //number of elements in temperature table
    // ldata->eFuseChargingTable.columnsData     = (const int32_t *) socData;                        //SoC data table
    // ldata->eFuseChargingTable.rowsData        = (const int32_t *) chargingTemperatureData;        //Temperaure data table  
    // ldata->eFuseChargingTable.tableData       = (const int32_t *) eFuseChargingTableData;         //tableData

    // // Create a Table2d structure for eFuseDischarging
    // ldata->eFuseDishargingTable.numberOfColumns = (dischargingMaxElements[1]);                    //number of elements in SoC table
    // ldata->eFuseDishargingTable.numberOfRows    = (dischargingMaxElements[0]);                    //number of elements in temperature table
    // ldata->eFuseDishargingTable.columnsData     = (const int32_t *) socData;                      //SoC data table
    // ldata->eFuseDishargingTable.rowsData        = (const int32_t *) dischargingTemperatureData;   //Temperaure data table 
    // ldata->eFuseDishargingTable.tableData       = (const int32_t *) eFuseDischargingTableData;    //tableData

    status = BMS_OK;
	return status;
}

#ifdef EKF
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, ccEKFOutput_t *ccEKFOutput, ExtU_DataPipeline_T *dpInputMBD)
#else
bmsStatus_t dataPipelineInput(bmsMainData_t *mdata, dataPipelineInput_t *dpInput, coulombCountingOutput_t *ccOutput, ExtU_DataPipeline_T *dpInputMBD)
#endif
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t index      = 0;

	if( (NULL == mdata)        ||\
        (NULL == dpInput)      ||\
        (NULL == dpInputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    #ifdef FUNCTIONALTESTS
        //choose the test to be performed from the enum
        functionalTests(overCurrentDischargeTest, mdata);
    #endif
                                            
    dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(mdata->ccmData.packCurrent);        //milli Ampere
    
    if(dpInput->current > 0 && dpInput->current < 500)                                      // to handle C blinking error on ignition
    {
        mdata->ccmData.packCurrent = 0;
    }

    // dpInput->current = CONVERT_AMPERE_TO_MILLI_AMPERE(-0.5);        //milli Ampere

#ifdef EKF
    dpInput->soc     = ccEKFOutput->socCC;
#else
    dpInput->soc                = ccOutput->SOC_pct;
    dpInput->capacityRemaining  = ccOutput->totalCapacityRemaining;
#endif

    //cell voltages 
	// for(uint8_t cmuIndex = 0; cmuIndex < defaultFlashConfigData[NUMBER_OF_CMU_INDEX]; cmuIndex++)
	// {
	// }

    //To Do: Efficient way to select number of cells in series
    
    for(uint8_t cellIndex = 0; cellIndex < 18; cellIndex++)
    {
        dpInput->voltage[index++] = mdata->cmuData.cmuVolatges.cellVoltages[0][cellIndex];
    }

    dpInput->voltage[8] = mdata->cmuData.cmuVolatges.cellVoltages[0][5];
    dpInput->voltage[18] = mdata->cmuData.cmuVolatges.cellVoltages[0][5];
    dpInput->voltage[19] = mdata->cmuData.cmuVolatges.cellVoltages[0][5];

    //temperature groups

    /* GROUP1 AFE SENSED TEMPERATURES */

    /* MICROCONTROLLER SENSED TEMPERATURES */ 
    for(uint8_t tempIndex = 0; tempIndex < 7; tempIndex++)
    {
        if(tempIndex < 4){
            dpInput->temperature1_C[tempIndex] = mdata->tempData.tempValue[tempIndex];
        }
        else{
            dpInput->temperature1_C[tempIndex] = mdata->cmuData.cmuTemperatures.externalNTC[0][tempIndex - 4];
        }
    }

    /* MICROCONTROLLER SENSED TEMPERATURES */                                               //TODO should add microcontroller temperature ADCs parsing code
    // dpInput->temperature1_C[4] = mdata->cmuData.cmuTemperatures.externalNTC[0][1];
    // dpInput->temperature1_C[5] = mdata->cmuData.cmuTemperatures.externalNTC[0][1];
    // dpInput->temperature1_C[6] = mdata->cmuData.cmuTemperatures.externalNTC[0][1];

    // conencted to shunt
    dpInput->shuntTemp = mdata->cmuData.cmuTemperatures.externalNTC[0][3];

    for(uint8_t tempIndex = 0; tempIndex < 6; tempIndex++)
    {
        dpInput->temperature2_C[tempIndex] = mdata->cmuData.cmuTemperatures.onBoardNTC[0][tempIndex];
    }
	
    // copy
    dpInput->temperature2_C[6] = mdata->cmuData.cmuTemperatures.onBoardNTC[0][2];

    //VCU
    dpInput->SlowCharge             = vcuData.SlowCharge;
    dpInput->FastCharge             = vcuData.FastCharge;
    dpInput->isChargerConnected     = vcuData.isChargerConnected || dpOutputData.isChargeEn;
    dpInput->TransmissionFail       = vcuData.TransmissionFail;
    dpInput->VCUPermanentFail       = vcuData.VCUPermanentFail;
    dpInput->ServiceMode            = vcuData.ServiceMode;
    dpInput->BalanceEn_atRest       = vcuData.BalanceEn_atRest;
    dpInput->EmergencyDisconnectEn  = vcuData.EmergencyDisconnectEn;
    dpInput->SlowChargersCurrentLimit_A = 45;  
    dpInput->FastChargersCurrentLimit_A = 180;  
    dpInput->LoopTimer_SOC_msec         = 80;

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    dpInputMBD->CurrentSense_mA              = dpInput->current;
    dpInputMBD->SOC                          = dpInput->soc;

    //VCU
    dpInputMBD->SlowCharge                   = dpInput->SlowCharge;
    dpInputMBD->FastCharge                   = dpInput->FastCharge;
    dpInputMBD->isChargerConnected           = dpInput->isChargerConnected;
    dpInputMBD->TransmissionFail             = dpInput->TransmissionFail;
    dpInputMBD->VCUPermanentFail             = dpInput->VCUPermanentFail;
    dpInputMBD->ServiceMode                  = dpInput->ServiceMode;
    dpInputMBD->BalanceEn_atRest             = dpInput->BalanceEn_atRest;
    dpInputMBD->EmergencyDisconnectEn        = dpInput->EmergencyDisconnectEn;
    dpInputMBD->LoopTimer_SOC_msec           = dpInput->LoopTimer_SOC_msec;
    dpInputMBD->FastChargersCurrentLimit_A   = dpInput->FastChargersCurrentLimit_A;
    dpInputMBD->SlowChargersCurrentLimit_A   = dpInput->SlowChargersCurrentLimit_A;

    for(uint8_t i = 0; i < 20; i++)
    {
        dpInputMBD->VoltageSense_mV[i]          = dpInput->voltage[i];
    }
    
    for(uint8_t i = 0; i < 7; i++)
    {
        dpInputMBD->Temperatures1_C[i]       = dpInput->temperature1_C[i];
        dpInputMBD->Temperatures2_C[i]       = dpInput->temperature2_C[i];
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t dataPipelineOutput(bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, ExtY_DataPipeline_T *dpOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == dpOutputMBD) )
	{
		status = BMS_DATA_PIPELINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/

    dpOutput->current                   = dpOutputMBD->DataPipelineb.Current_mA;
    dpOutput->isChargeEn                = dpOutputMBD->DataPipelineb.isChargeEn; 
    
    dpOutput->temperatureSense[0].temperatureMax    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].T_max;
    dpOutput->temperatureSense[0].temperatureMin    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].T_min;
    dpOutput->temperatureSense[0].temperatureDelta  = dpOutputMBD->DataPipelineb.TemperatureSenseBus[0].TemperatureDelta_C;

    dpOutput->temperatureSense[1].temperatureMax    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].T_max;
    dpOutput->temperatureSense[1].temperatureMin    = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].T_min;
    dpOutput->temperatureSense[1].temperatureDelta  = dpOutputMBD->DataPipelineb.TemperatureSenseBus[1].TemperatureDelta_C;

    dpOutput->maxTemperature_C          = dpOutputMBD->DataPipelineb.MaxTemperature_C;
    dpOutput->minTemperature_C          = dpOutputMBD->DataPipelineb.MinTemperature_C;
    dpOutput->deltaTemperature_C        = dpOutputMBD->DataPipelineb.DeltaTemperature_C;
    dpOutput->DeltaTemperatureGroup1_C  = dpOutputMBD->DataPipelineb.DeltaTemperatureGroup1_C;
    dpOutput->DeltaTemperatureGroup2_C  = dpOutputMBD->DataPipelineb.DeltaTemperatureGroup2_C;

    dpOutput->isDynamicCurrentEn        = dpOutputMBD->DataPipelineb.isDynamicCurrentEn;
    dpOutput->nCells_series             = dpOutputMBD->DataPipelineb.nCells_series;
    dpOutput->nCells_parallel           = dpOutputMBD->DataPipelineb.nCells_parallel;

    //VCU
    dpOutput->VCU.SlowCharge            = dpOutputMBD->DataPipelineb.VCU.SlowCharge;
    dpOutput->VCU.FastCharge            = dpOutputMBD->DataPipelineb.VCU.FastCharge;
    dpOutput->VCU.isChargerConnected    = dpOutputMBD->DataPipelineb.VCU.isChargerConnected;
    dpOutput->VCU.TransmissionFail      = dpOutputMBD->DataPipelineb.VCU.TransmissionFail;
    dpOutput->VCU.VCUPermanentFail      = dpOutputMBD->DataPipelineb.VCU.VCUPermanentFail;
    dpOutput->VCU.ServiceMode           = dpOutputMBD->DataPipelineb.VCU.ServiceMode;
    dpOutput->VCU.BalanceEn_atRest      = dpOutputMBD->DataPipelineb.VCU.BalanceEn_atRest;
    dpOutput->VCU.EmergencyDisconnectEn = dpOutputMBD->DataPipelineb.VCU.EmergencyDisconnectEn;
    dpOutput->LoopTimer_SOC_msec            = dpOutputMBD->DataPipelineb.LoopTimer_SOC_msec;
    // dpOutput->VCU.ChargersCurrentLimit_A    = dpOutputMBD->DataPipelineb.VCU.ChargersCurrentLimit_A;

    dpOutput->VCU.FastChargersCurrentLimit_A = dpOutputMBD->DataPipelineb.VCU.FastChargersCurrentLimit_A;
    dpOutput->VCU.SlowChargersCurrentLimit_A = dpOutputMBD->DataPipelineb.VCU.SlowChargersCurrentLimit_A;

    for(uint8_t i = 0; i < CELL_IN_SERIES; i++)
    {
        dpOutput->voltageSense.voltage[i] = dpOutputMBD->DataPipelineb.VoltageSenseBus.Voltages_mV[i];
    }

    dpOutput->voltageSense.voltageMin   = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_min;
    dpOutput->voltageSense.voltageMax   = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_max;
    dpOutput->voltageSense.voltageDelta = dpOutputMBD->DataPipelineb.VoltageSenseBus.VoltageDelta_mV;

    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        for(uint8_t i = 0; i < 7; i++)
        {
            dpOutput->temperatureSense[groupIndex].temperature[i] = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].Temperatures_C[i];
        }

        dpOutput->temperatureSense[groupIndex].temperatureMin   = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].T_min;
        dpOutput->temperatureSense[groupIndex].temperatureMax   = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].T_max;
        dpOutput->temperatureSense[groupIndex].temperatureDelta = dpOutputMBD->DataPipelineb.TemperatureSenseBus[groupIndex].TemperatureDelta_C;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingInit(thresholdData_t *tData, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD, dataPipelineInput_t *dpInput)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == cbInput)      ||\
        (NULL == cbInputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //cell balancing threshold
    cbInputMBD->Thresholds.Balancing_Min_Thr_mV               = tData->balancingMinThreshold;
    cbInputMBD->Thresholds.Balancing_StateRestPeriod_msec     = tData->balancingStateRestPeroid;
    cbInputMBD->Thresholds.Balancing_MaxAllowedImb_mV         = tData->balancingMaxAllowedImbalance;
    // cbInputMBD->Thresholds.Balancing_OVProtectThr_mV          = tData->balancingOVProtectionThreshold;

//       int32_T Balancing_SlowCh_OVThr_mV;
//   int32_T Balancing_FastCh_OVThr_mV;
    cbInputMBD->Thresholds.Balancing_SlowCh_OVThr_mV          = tData->balancing_SlowCh_OVThr_mV;
    cbInputMBD->Thresholds.Balancing_FastCh_OVThr_mV          = tData->balancing_FastCh_OVThr_mV;

    cbInputMBD->Thresholds.Balancing_DefaultBalancingTimeOn_msec           = tData->balancing_DefaultBalancingTimeOn_msec;
    cbInputMBD->Thresholds.BalancingCurrentDerateActivationTemp_C          = tData->balancingCurrentDerateActivationTemp_C;
    cbInputMBD->Thresholds.BalancingCurrentDerateCheckTime_msec            = tData->balancingCurrentDerateCheckTime_msec;

    cbInputMBD->BalancingTempGroup                            = cbInput->BalancingTempGroup;
    cbInputMBD->NumberOfMosfets                               = cbInput->NumberOfMosfets;
    cbInputMBD->StLineEq_varA                                 = cbInput->StLineEq_varA;
    cbInputMBD->StLineEq_varB                                 = cbInput->StLineEq_varB;

    cbInputMBD->DataPipeline.nCells_series                    = dpInput->nCells_series;

    for(int i = 0; i < 2; i++)
    {
        cbInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C   = tData->temperatureData[i].OTC_ErrorTemperature;
        cbInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C   = tData->temperatureData[i].OTD_ErrorTemperature;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, cellBalancingInput_t *cbInput, ExtU_CellBalancing_T *cbInputMBD)
{
    bmsStatus_t status = BMS_ERROR;
    static uint8_t balanceEnableDebounce = 0;

	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == cbInput)      ||\
        (NULL == cbInputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    cbInput->dataPipelineBus.current                    = dpOutput->current;
    cbInput->dataPipelineBus.VCU.isChargerConnected     = dpOutput->VCU.isChargerConnected;

    if(dpOutput->VCU.BalanceEn_atRest)
    {
        balanceEnableDebounce++;
        if(balanceEnableDebounce >= 3)
        {
            cbInput->dataPipelineBus.VCU.BalanceEn_atRest       = dpOutput->VCU.BalanceEn_atRest;
        }
    }
    else
    {
        balanceEnableDebounce = 0;
    }

    //Cell Voltages
    for(uint8_t i = 0; i < CELL_IN_SERIES; i++)
    {
        cbInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    }

    cbInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    cbInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    cbInput->dataPipelineBus.voltageSense.voltageDelta = dpOutput->voltageSense.voltageDelta;
    
    //temperatures
    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        // for(uint8_t i = 0; i < 4; i++)
        // {
        //     cbInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i] = dpOutput->temperatureSense[groupIndex].temperature[i];
        // }

        // cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin     = dpOutput->temperatureSense[groupIndex].temperatureMin;
        cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax     = dpOutput->temperatureSense[groupIndex].temperatureMax;
        // cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta   = dpOutput->temperatureSense[groupIndex].temperatureDelta;
    }

    /*************** Copy to MBD structures *********************************************************************************************************/

    cbInputMBD->DataPipeline.Current_mA                 = cbInput->dataPipelineBus.current;
    cbInputMBD->DataPipeline.VCU.isChargerConnected     = cbInput->dataPipelineBus.VCU.isChargerConnected;
    cbInputMBD->DataPipeline.VCU.BalanceEn_atRest       = cbInput->dataPipelineBus.VCU.BalanceEn_atRest;

    //Cell Voltages
    for(uint8_t i = 0; i < CELL_IN_SERIES; i++)
    {
        cbInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = cbInput->dataPipelineBus.voltageSense.voltage[i];
    }

    cbInputMBD->DataPipeline.VoltageSenseBus.mV_min             = cbInput->dataPipelineBus.voltageSense.voltageMin;
    cbInputMBD->DataPipeline.VoltageSenseBus.mV_max             = cbInput->dataPipelineBus.voltageSense.voltageMax;
    cbInputMBD->DataPipeline.VoltageSenseBus.VoltageDelta_mV    = cbInput->dataPipelineBus.voltageSense.voltageDelta;

    for (uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        // for (uint8_t i = 0; i < 4; i++)
        // {
        //     cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].Temperatures_C[i] = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperature[i];
        // }

        // cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_min                  = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin;
        cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_max                  = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax;
        // cbInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].TemperatureDelta_C     = cbInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t cellBalancingOutput(bmsMainData_t *mdata, cellBalancingOutput_t *cbOutput, ExtY_CellBalancing_T *cbOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == cbOutput)     ||\
        (NULL == cbOutputMBD) )
	{
		status = BMS_CELL_BALANCING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    cbOutput->BalanceEn         = cbOutputMBD->CellBalancingOutput.BalanceEn;
    cbOutput->volatgeImbalance  = cbOutputMBD->CellBalancingOutput.MaxImbalance_mV;
    cbOutput->balancingState    = cbOutputMBD->CellBalancingOutput.CurrentBalancingState;
    balanceActiveFlag           = cbOutput->BalanceEn;

    switch(cbOutput->balancingState)
    {
        case 0: //Terminate
        case 1: //Lobby
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));
            // adBms6815_clear_dcc_discharge(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;

        case 2: //Even Balancing
        case 3: //Odd Balancing
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));

            for(uint16_t i = 0; i < CELL_IN_SERIES; i++)
            {
                cbOutput->balancingCommand[i]    = (int)cbOutputMBD->BalanceCmd[i];
                cellsToBalance[(i/18)]           |= (cbOutput->balancingCommand[i] << (i%18));  
            }

            adBms6830_set_dcc_discharge(cellsToBalance, mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
            // adBms6815_set_dcc_discharge(cellsToBalance, mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;

        default:
            memset(cellsToBalance, 0, sizeof(cellsToBalance));
            memset(cbOutput->balancingCommand, 0, sizeof(cbOutput->balancingCommand));

            adBms6830_clear_dcc_discharge(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);

            // adBms6815_clear_dcc_discharge(mdata->cmuData.cmuCount, mdata->cmuData.cmuCellData);
        break;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t protectionInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == pInput)       ||\
        (NULL == pInputMBD) )
	{
		status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    //
    pInputMBD->Thresholds.Protections_MaxOperatingTemp_C                     =   tData->MaxOperatingTemp_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempChange_C                 =   tData->MaxAllowedTempChange_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup1_C         =   tData->MaxAllowedTempGradientGroup1_C;
    pInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup2_C         =   tData->MaxAllowedTempGradientGroup2_C;
    pInputMBD->Thresholds.Protections_AverageTimeforProtection_msec          =   tData->Protections_AverageTimeforProtection_msec;


    //
    pInputMBD->Thresholds.i2tTimeout_msec                              = tData->i2tTimeout_msec;
    pInputMBD->Thresholds.DynContTimeout_msec                          = tData->dynamicContTimeout_msec;
    pInputMBD->Thresholds.DynamicCurrentThresholdsGain                 = tData->dynamicCurrentThresholdsGain;
    pInputMBD->Thresholds.TempGroupTimeout                             = tData->TempGroupTimeout;
    pInputMBD->Thresholds.PermanentFailCounts_eFuse                    = tData->PermanentFailCounts_eFuse;

    //
    pInputMBD->Thresholds.RecoveryProtections_OCC_Warning_mA           = tData->RecoveryProtections_OCC_Warning_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCC_Error_mA             = tData->RecoveryProtections_OCC_Error_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCD_Warning_mA           = tData->RecoveryProtections_OCD_Warning_mA;
    pInputMBD->Thresholds.RecoveryProtections_OCD_Error_mA             = tData->RecoveryProtections_OCD_Error_mA;
    pInputMBD->Thresholds.CurrentChangeTimeout_msec                    = tData->CurrentChangeTimeout_msec;
    pInputMBD->Thresholds.eFuseMeltingTimeout_msec                     = tData->eFuseMeltingTimeout_msec;
    pInputMBD->Thresholds.DCL_CurrentProtectionTimeout_msec            = tData->overCurrentProtectionTimeout;
    pInputMBD->Thresholds.DCL_RecoveryCurrentProtectionTimeout_msec    = tData->overCurrentRecoveryCurrentProtectionTimeout_msec;
    pInputMBD->Thresholds.DCL_WarningToErrorTimeout_msec               = tData->overCurrentWarningToErrorTimeout_msec;

    //Current
    pInputMBD->Thresholds.Protections_OCC_Warning_mA 	        = tData->OCC_WarningCurrent;
    pInputMBD->Thresholds.Protections_OCC_Error_mA 			    = tData->OCC_ErrorCurrent;
    pInputMBD->Thresholds.Protections_OCC_WarningTimeout_msec 	= tData->OCC_WarningTimeout;
    pInputMBD->Thresholds.Protections_OCC_ErrorTimeout_msec	    = tData->OCC_ErrorTimeout;
    
    pInputMBD->Thresholds.Protections_OCD_Warning_mA 		    = tData->OCD_WarningCurrent;
    pInputMBD->Thresholds.Protections_OCD_Error_mA 			    = tData->OCD_ErrorCurrent;
    pInputMBD->Thresholds.Protections_OCD_WarningTimeout_msec   = tData->OCD_WarningTimeout;
    pInputMBD->Thresholds.Protections_OCD_ErrorTimeout_msec     = tData->OCD_ErrorTimeout;
    
    //temperature groups
    for(int i = 0; i < 2; i++)
    {
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Warning_C      = tData->temperatureData[i].OTC_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C        = tData->temperatureData[i].OTC_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_Recovery_C 	= tData->temperatureData[i].OTC_RecoveryTemperature;
        
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Warning_C 		= tData->temperatureData[i].OTD_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C 		= tData->temperatureData[i].OTD_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_Recovery_C 	= tData->temperatureData[i].OTD_RecoveryTemperature;
        
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Warning_C	 	= tData->temperatureData[i].UTC_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Error_C	 	= tData->temperatureData[i].UTC_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_Recovery_C	 	= tData->temperatureData[i].UTC_RecoveryTemperature;

        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Warning_C	 	= tData->temperatureData[i].UTD_WarningTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Error_C	 	= tData->temperatureData[i].UTD_ErrorTemperature;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_Recovery_C	    = tData->temperatureData[i].UTD_RecoveryTemperature;
    
        //timeouts
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_WarningTimeout_msec   = tData->temperatureData[i].OTC_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_ErrorTimeout_msec     = tData->temperatureData[i].OTC_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_RecoveryTimeout_msec  = tData->temperatureData[i].OTC_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTC_NoErrorTimeout_msec   = tData->temperatureData[i].OTC_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_OTD_WarningTimeout_msec   = tData->temperatureData[i].OTD_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_ErrorTimeout_msec	   = tData->temperatureData[i].OTD_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_RecoveryTimeout_msec  = tData->temperatureData[i].OTD_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_OTD_NoErrorTimeout_msec   = tData->temperatureData[i].OTD_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_UTC_WarningTimeout_msec   = tData->temperatureData[i].UTC_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_ErrorTimeout_msec     = tData->temperatureData[i].UTC_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_RecoveryTimeout_msec  = tData->temperatureData[i].UTC_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTC_NoErrorTimeout_msec   = tData->temperatureData[i].UTC_NoErrorTimeout_msec;

        pInputMBD->Thresholds.Temperature[i].Protections_UTD_WarningTimeout_msec   = tData->temperatureData[i].UTD_WarningTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_ErrorTimeout_msec	   = tData->temperatureData[i].UTD_ErrorTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_RecoveryTimeout_msec  = tData->temperatureData[i].UTD_RecoveryTimeout;
        pInputMBD->Thresholds.Temperature[i].Protections_UTD_NoErrorTimeout_msec   = tData->temperatureData[i].UTD_NoErrorTimeout_msec;

    }
    


    pInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    pInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    pInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    pInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;


    pInputMBD->Thresholds.Protections_OV_WarningTimeout_msec    = tData->OV_WarningTimeout;
    pInputMBD->Thresholds.Protections_OV_ErrorTimeout_msec	 	= tData->OV_ErrorTimeout;
    pInputMBD->Thresholds.Protections_OV_RecoveryTimeout_msec	= tData->OV_RecoveryTimeout;
    pInputMBD->Thresholds.Protections_OV_NoErrorTimeout_msec	= tData->OV_NoErrorTimeout_msec;
    
    pInputMBD->Thresholds.Protections_UV_Warning_mV	 		    = tData->UV_WarningVoltage;
    pInputMBD->Thresholds.Protections_UV_Error_mV	 		    = tData->UV_ErrorVoltage;
    pInputMBD->Thresholds.Protections_UV_Recovery_mV	 		= tData->UV_RecoveryVoltage;
    pInputMBD->Thresholds.Protections_UV_WarningTimeout_msec 	= tData->UV_WarningTimeout;
    pInputMBD->Thresholds.Protections_UV_ErrorTimeout_msec	 	= tData->UV_ErrorTimeout;
    pInputMBD->Thresholds.Protections_UV_RecoveryTimeout_msec	= tData->UV_RecoveryTimeout;  
    pInputMBD->Thresholds.Protections_UV_NoErrorTimeout_msec	= tData->UV_NoErrorTimeout_msec;

    pInputMBD->Thresholds.Protections_HighImbalanceError_mV            = tData->HighImbalanceError_mV;
    pInputMBD->Thresholds.Protections_MinOpearatingVolt_mV             = tData->MinOpearatingVolt_mV;
    pInputMBD->Thresholds.Protections_ShortCircuitCurrent_mA           = tData->ShortCircuitCurrent_mA;
    pInputMBD->Thresholds.Protections_SuddenVoltageDrop_mV_by_time     = tData->SuddenVoltageDrop_mV_by_time;
    

    pInputMBD->Thresholds.ThermalRunawayWarningTimeout_msec            = tData->ThermalRunawayWarningTimeout_msec;
    pInputMBD->Thresholds.ThermalRunawayErrorTimeout_msec              = tData->ThermalRunawayErrorTimeout_msec;
    pInputMBD->Thresholds.TemperatureGradientTimeout_msec              = tData->TemperatureGradientTimeout_msec;
    pInputMBD->Thresholds.HighImbalanceFlagTimeout_msec                = tData->HighImbalanceFlagTimeout_msec;
    pInputMBD->Thresholds.ShortCircuitDetect_msec                      = tData->ShortCircuitDetect_msec;
    pInputMBD->Thresholds.SuddenVoltageDropTimeout_msec                = tData->SuddenVoltageDropTimeout_msec;
    pInputMBD->Thresholds.AdditionalProtectionEn                       = tData->AdditionalProtectionEn;
    pInputMBD->Thresholds.HighImbalanceFlagEn                          = tData->HighImbalanceFlagEn;

    pInputMBD->Thresholds.ExternalShortCheckTime_msec                  = tData->ExternalShortCheckTime_msec;
    pInputMBD->Thresholds.ExternalShortLimit                           = tData->ExternalShortLimit;
    pInputMBD->Thresholds.ContWeldCheckLimit                           = tData->ContWeldCheckLimit;

    //contactor
    pInputMBD->Thresholds.ContCutoffTime_msec                = tData->contactorCutoffTime;
    pInputMBD->Thresholds.ContCurrent_mA                     = tData->preChargeContactorCurrent;
    pInputMBD->Thresholds.ContWeldCheckCurrent_mA            = tData->ContWeldCheckCurrent_mA;
    pInputMBD->Thresholds.ContactorWarningCounterLimit       = tData->ContactorWarningCounterLimit;
    pInputMBD->Thresholds.ContactorWeldTimeout_msec          = tData->ContactorWeldTimeout_msec;
    pInputMBD->Thresholds.OpenToPreChargeStateTime_msec      = tData->OpenToPreChargeStateTime_msec;
    pInputMBD->Thresholds.Min_PreChargeTime_msec             = tData->Min_PreChargeTime_msec;
    pInputMBD->Thresholds.Max_PreChargeTime_msec             = tData->Max_PreChargeTime_msec;
    pInputMBD->Thresholds.ContOpenCurrent_mA                 = tData->ContOpenCurrent_mA;
    pInputMBD->Thresholds.FlagDebounceTime_msec              = tData->FlagDebounceTime_msec;
    pInputMBD->Thresholds.Min_ClosedToOpeningTime_msec       = tData->Min_ClosedToOpeningTime_msec;
    pInputMBD->Thresholds.Max_ClosedToOpeningTime_msec       = tData->Max_ClosedToOpeningTime_msec;

    //dcli enable
    pInputMBD->DataPipeline.isDynamicCurrentEn               = dpInput->isDynamicCurrentEn;

    status = BMS_OK;
	return status;
}


bmsStatus_t protectionInput(bmsMainData_t *mdata, dataPipelineOutput_t  *dpOutput, protectionInput_t *pInput, ExtU_Protection_T *pInputMBD)
{
    bmsStatus_t status = BMS_ERROR;
    static uint32_t currentTick = 0;
    static uint32_t lastTick = 0;
    static uint32_t duration = 0;
    
	if( (NULL == mdata)        ||\
        (NULL == dpOutput)     ||\
        (NULL == pInput)       ||\
        (NULL == pInputMBD) )
	{
		status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    currentTick = HAL_GetTick();
    duration = (lastTick == 0) ? 0 : (currentTick - lastTick);
    lastTick = currentTick;

    pInput->dataPipelineBus.current                      = dpOutput->current;
    pInput->dataPipelineBus.isChargeEn                   = dpOutput->isChargeEn;

    pInput->dataPipelineBus.maxTemperature_C             = dpOutput->maxTemperature_C;
    pInput->dataPipelineBus.deltaTemperature_C           = dpOutput->deltaTemperature_C;
    pInput->dataPipelineBus.DeltaTemperatureGroup1_C     = dpOutput->DeltaTemperatureGroup1_C;
    pInput->dataPipelineBus.DeltaTemperatureGroup2_C     = dpOutput->DeltaTemperatureGroup2_C;

    pInput->dataPipelineBus.DCL_ChargingCurrent_A        = dpOutput->DCL_ChargingCurrent_A;
    pInput->dataPipelineBus.DCL_DisChargingCurrent_A     = dpOutput->DCL_DisChargingCurrent_A;

    pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec    = dpOutput->i2t_ChargingCurrent_A2sec;
    pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec = dpOutput->i2t_DisChargingCurrent_A2sec;
    
    //Cell Voltages
    // for(uint8_t i = 0; i < 20; i++)
    // {
    //     pInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    // }

    pInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    pInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    pInput->dataPipelineBus.voltageSense.voltageDelta = dpOutput->voltageSense.voltageDelta;

    pInput->dataPipelineBus.VCU.FastCharge            = dpOutput->VCU.FastCharge;
    pInput->dataPipelineBus.VCU.isChargerConnected    = (dpOutput->VCU.isChargerConnected || dpOutput->isChargeEn);
    pInput->dataPipelineBus.VCU.ServiceMode           = dpOutput->VCU.ServiceMode;
    pInput->dataPipelineBus.VCU.SlowCharge            = dpOutput->VCU.SlowCharge;
    pInput->dataPipelineBus.VCU.TransmissionFail      = dpOutput->VCU.TransmissionFail;
    pInput->dataPipelineBus.VCU.VCUPermanentFail      = dpOutput->VCU.VCUPermanentFail;
    // pInput->dataPipelineBus.LoopTimer_SOC_msec        = dpOutput->LoopTimer_SOC_msec;
    pInput->dataPipelineBus.LoopTimer_SOC_msec        = duration;
    
    //temperatures
    for(uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {

        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin     = dpOutput->temperatureSense[groupIndex].temperatureMin;
        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax     = dpOutput->temperatureSense[groupIndex].temperatureMax;
        pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta   = dpOutput->temperatureSense[groupIndex].temperatureDelta;
    }

    /*************** Copy to MBD structures *********************************************************************************************************/

    pInputMBD->DataPipeline.Current_mA                   = pInput->dataPipelineBus.current;
    pInputMBD->DataPipeline.isChargeEn                   = pInput->dataPipelineBus.isChargeEn;

    pInputMBD->DataPipeline.MaxTemperature_C             = pInput->dataPipelineBus.maxTemperature_C;
    pInputMBD->DataPipeline.DeltaTemperature_C           = pInput->dataPipelineBus.deltaTemperature_C;
    pInputMBD->DataPipeline.DeltaTemperatureGroup1_C     = pInput->dataPipelineBus.DeltaTemperatureGroup1_C;
    pInputMBD->DataPipeline.DeltaTemperatureGroup2_C     = pInput->dataPipelineBus.DeltaTemperatureGroup2_C;

    pInputMBD->DataPipeline.DCL_ChargingCurrent_A        = pInput->dataPipelineBus.DCL_ChargingCurrent_A;
    pInputMBD->DataPipeline.DCL_DisChargingCurrent_A     = pInput->dataPipelineBus.DCL_DisChargingCurrent_A;

    pInputMBD->DataPipeline.i2t_ChargingCurrent_A2sec    = pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec;
    pInputMBD->DataPipeline.i2t_DisChargingCurrent_A2sec = pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec;

    // for (uint8_t i = 0; i < 20; i++)
    // {
    //     pInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = pInput->dataPipelineBus.voltageSense.voltage[i];
    // }

    pInputMBD->DataPipeline.VoltageSenseBus.mV_min             = pInput->dataPipelineBus.voltageSense.voltageMin;
    pInputMBD->DataPipeline.VoltageSenseBus.mV_max             = pInput->dataPipelineBus.voltageSense.voltageMax;
    pInputMBD->DataPipeline.VoltageSenseBus.VoltageDelta_mV    = pInput->dataPipelineBus.voltageSense.voltageDelta;
    
    //copying VCU parameters
    pInputMBD->DataPipeline.VCU.FastCharge                     = pInput->dataPipelineBus.VCU.FastCharge;
    pInputMBD->DataPipeline.VCU.isChargerConnected             = pInput->dataPipelineBus.VCU.isChargerConnected;
    pInputMBD->DataPipeline.VCU.ServiceMode                    = pInput->dataPipelineBus.VCU.ServiceMode;
    pInputMBD->DataPipeline.VCU.SlowCharge                     = pInput->dataPipelineBus.VCU.SlowCharge;
    pInputMBD->DataPipeline.VCU.TransmissionFail               = pInput->dataPipelineBus.VCU.TransmissionFail;
    pInputMBD->DataPipeline.VCU.VCUPermanentFail               = pInput->dataPipelineBus.VCU.VCUPermanentFail;

    pInputMBD->DataPipeline.LoopTimer_SOC_msec                 = pInput->dataPipelineBus.LoopTimer_SOC_msec;
    pInputMBD->Protection_LoopTime_msec                        = pInput->dataPipelineBus.LoopTimer_SOC_msec; 

    for (uint8_t groupIndex = 0; groupIndex < 2; groupIndex++)
    {
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_min                  = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMin;
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].T_max                  = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureMax;
        pInputMBD->DataPipeline.TemperatureSenseBus[groupIndex].TemperatureDelta_C     = pInput->dataPipelineBus.temperatureSense[groupIndex].temperatureDelta;
    }

    status = BMS_OK;
	return status;
}


bmsStatus_t protectionOutput(bmsMainData_t *mdata, protectionOutput_t *pOutput, ExtY_Protection_T *pOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)        ||\
        (NULL == pOutput)      ||\
        (NULL == pOutputMBD) )
	{
		status = BMS_PROTECTION_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
  
    pOutput->protectionOutputData.ThermalRunaway			= pOutputMBD->ProtectionState_Out_j.ThermalRunaway;
    pOutput->protectionOutputData.TemperatureGradient		= pOutputMBD->ProtectionState_Out_j.TemperatureGradient;
    pOutput->protectionOutputData.HighImbalanceFlag	        = pOutputMBD->ProtectionState_Out_j.HighImbalanceFlag;
    pOutput->protectionOutputData.ShortCircuit		        = pOutputMBD->ProtectionState_Out_j.ShortCircuitDetect;
    pOutput->protectionOutputData.SuddenVoltageDrop	        = pOutputMBD->ProtectionState_Out_j.SuddenVoltageDrop;
    pOutput->protectionOutputData.OCC						= pOutputMBD->ProtectionState_Out_j.OCC;
    pOutput->protectionOutputData.OCD						= pOutputMBD->ProtectionState_Out_j.OCD;
    pOutput->protectionOutputData.OV						= pOutputMBD->ProtectionState_Out_j.OV;
    pOutput->protectionOutputData.UV						= pOutputMBD->ProtectionState_Out_j.UV;

    pOutput->protectionOutputData.TempOverallState				= pOutputMBD->ProtectionState_Out_j.TempOverallState;
    pOutput->protectionOutputData.TempState1					= pOutputMBD->ProtectionState_Out_j.TempState1;
    pOutput->protectionOutputData.TempState2					= pOutputMBD->ProtectionState_Out_j.TempState2;

    pOutput->protectionOutputData.FlagGroup1				    = pOutputMBD->ProtectionState_Out_j.FlagGroup1;
    pOutput->protectionOutputData.FlagGroup2				    = pOutputMBD->ProtectionState_Out_j.FlagGroup2;

    pOutput->protectionOutputData.eFuseChargeFlag        		= pOutputMBD->ProtectionState_Out_j.eFuseChargeFlag;
    pOutput->protectionOutputData.eFuseDischargeFlag            = pOutputMBD->ProtectionState_Out_j.eFuseDischargeFlag;
    pOutput->protectionOutputData.overCurrentChargeFlag         = pOutputMBD->ProtectionState_Out_j.DCLI_CurrentFlag;
    pOutput->protectionOutputData.overCurrentDischargeFlag      = pOutputMBD->ProtectionState_Out_j.DCLO_CurrentFlag;

    pOutput->protectionOutputData.i2t_Calculated_A2sec        	= pOutputMBD->ProtectionState_Out_j.i2t_Calculated_A2sec;
    pOutput->protectionOutputData.ErrorDetect        			= pOutputMBD->ProtectionState_Out_j.ErrorDetect;


    if(pOutput->protectionOutputData.UV == 2 || (pOutput->protectionOutputData.TempOverallState == 2 || pOutput->protectionOutputData.TempOverallState == 4))
    {
        tx_event_flags_set(&systemEventFlag, ERROR_DISPLAY_FLAG, TX_OR); 
    }
    
    status = BMS_OK;
	return status;
}


bmsStatus_t contactorInit(thresholdData_t *tData, contactorInput_t *cInput, ExtU_Contactors_T *cInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)       ||\
        (NULL == cInput)      ||\
        (NULL == cInputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    powerCycleDetected = true;

    cInputMBD->Thresholds.Protections_MaxOperatingTemp_C                     =   tData->MaxOperatingTemp_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempChange_C                 =   tData->MaxAllowedTempChange_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup1_C         =   tData->MaxAllowedTempGradientGroup1_C;
    cInputMBD->Thresholds.Protections_MaxAllowedTempGradientGroup2_C         =   tData->MaxAllowedTempGradientGroup2_C;

    // cInputMBD->Thresholds.DynInstTimeout_msec                          = tData->dynamicInstTimeout_msec;
    cInputMBD->Thresholds.DynContTimeout_msec                          = tData->dynamicContTimeout_msec;
    cInputMBD->Thresholds.DynamicCurrentThresholdsGain                 = tData->dynamicCurrentThresholdsGain;
    cInputMBD->Thresholds.TempGroupTimeout                             = tData->TempGroupTimeout;
    
    cInputMBD->Thresholds.RecoveryProtections_OCC_Warning_mA           = tData->RecoveryProtections_OCC_Warning_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCC_Error_mA             = tData->RecoveryProtections_OCC_Error_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCD_Warning_mA           = tData->RecoveryProtections_OCD_Warning_mA;
    cInputMBD->Thresholds.RecoveryProtections_OCD_Error_mA             = tData->RecoveryProtections_OCD_Error_mA;
    cInputMBD->Thresholds.CurrentChangeTimeout_msec                    = tData->CurrentChangeTimeout_msec;

    //threshold
    cInputMBD->Thresholds.Protections_OCC_Warning_mA 	        = tData->OCC_WarningCurrent;
    cInputMBD->Thresholds.Protections_OCC_Error_mA 			    = tData->OCC_ErrorCurrent;
    cInputMBD->Thresholds.Protections_OCC_WarningTimeout_msec 	= tData->OCC_WarningTimeout;
    cInputMBD->Thresholds.Protections_OCC_ErrorTimeout_msec	    = tData->OCC_ErrorTimeout;
    
    cInputMBD->Thresholds.Protections_OCD_Warning_mA 		    = tData->OCD_WarningCurrent;
    cInputMBD->Thresholds.Protections_OCD_Error_mA 			    = tData->OCD_ErrorCurrent;
    cInputMBD->Thresholds.Protections_OCD_WarningTimeout_msec   = tData->OCD_WarningTimeout;
    cInputMBD->Thresholds.Protections_OCD_ErrorTimeout_msec     = tData->OCD_ErrorTimeout;

    cInputMBD->Thresholds.PreChargeRecoveryTimeout_msec = tData->PreChargeRecoveryTimeout_msec;
    
    //temperature groups
    for(int i = 0; i < 2; i++)
    {
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Warning_C      = tData->temperatureData[i].OTC_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Error_C        = tData->temperatureData[i].OTC_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_Recovery_C 	= tData->temperatureData[i].OTC_RecoveryTemperature;
        
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Warning_C 		= tData->temperatureData[i].OTD_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Error_C 		= tData->temperatureData[i].OTD_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_Recovery_C 	= tData->temperatureData[i].OTD_RecoveryTemperature;
        
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Warning_C	 	= tData->temperatureData[i].UTC_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Error_C	 	= tData->temperatureData[i].UTC_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_Recovery_C	 	= tData->temperatureData[i].UTC_RecoveryTemperature;

        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Warning_C	 	= tData->temperatureData[i].UTD_WarningTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Error_C	 	= tData->temperatureData[i].UTD_ErrorTemperature;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_Recovery_C	    = tData->temperatureData[i].UTD_RecoveryTemperature;
        
        //timeouts
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_WarningTimeout_msec   = tData->temperatureData[i].OTC_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_ErrorTimeout_msec     = tData->temperatureData[i].OTC_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTC_RecoveryTimeout_msec  = tData->temperatureData[i].OTC_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_OTD_WarningTimeout_msec   = tData->temperatureData[i].OTD_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_ErrorTimeout_msec	   = tData->temperatureData[i].OTD_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_OTD_RecoveryTimeout_msec  = tData->temperatureData[i].OTD_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_UTC_WarningTimeout_msec   = tData->temperatureData[i].UTC_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_ErrorTimeout_msec     = tData->temperatureData[i].UTC_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTC_RecoveryTimeout_msec  = tData->temperatureData[i].UTC_RecoveryTimeout;

        cInputMBD->Thresholds.Temperature[i].Protections_UTD_WarningTimeout_msec   = tData->temperatureData[i].UTD_WarningTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_ErrorTimeout_msec	   = tData->temperatureData[i].UTD_ErrorTimeout;
        cInputMBD->Thresholds.Temperature[i].Protections_UTD_RecoveryTimeout_msec  = tData->temperatureData[i].UTD_RecoveryTimeout;
    }
    
    cInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    cInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    cInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    cInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;

    cInputMBD->Thresholds.Protections_OV_WarningTimeout_msec    = tData->OV_WarningTimeout;
    cInputMBD->Thresholds.Protections_OV_ErrorTimeout_msec	 	= tData->OV_ErrorTimeout;
    cInputMBD->Thresholds.Protections_OV_RecoveryTimeout_msec	= tData->OV_RecoveryTimeout;
    
    cInputMBD->Thresholds.Protections_UV_Warning_mV	 		    = tData->UV_WarningVoltage;
    cInputMBD->Thresholds.Protections_UV_Error_mV	 		    = tData->UV_ErrorVoltage;
    cInputMBD->Thresholds.Protections_UV_Recovery_mV	 		= tData->UV_RecoveryVoltage;
    cInputMBD->Thresholds.Protections_UV_WarningTimeout_msec 	= tData->UV_WarningTimeout;
    cInputMBD->Thresholds.Protections_UV_ErrorTimeout_msec	 	= tData->UV_ErrorTimeout;
    cInputMBD->Thresholds.Protections_UV_RecoveryTimeout_msec	= tData->UV_RecoveryTimeout;  
    
    //
    cInputMBD->Thresholds.Protections_HighImbalanceError_mV            = tData->HighImbalanceError_mV;
    cInputMBD->Thresholds.Protections_MinOpearatingVolt_mV             = tData->MinOpearatingVolt_mV;
    cInputMBD->Thresholds.Protections_ShortCircuitCurrent_mA           = tData->ShortCircuitCurrent_mA;
    cInputMBD->Thresholds.Protections_SuddenVoltageDrop_mV_by_time     = tData->SuddenVoltageDrop_mV_by_time;
    
    cInputMBD->Thresholds.ExternalShortCheckTime_msec                  = tData->ExternalShortCheckTime_msec;
    cInputMBD->Thresholds.ExternalShortLimit                           = tData->ExternalShortLimit;
    cInputMBD->Thresholds.ContWeldCheckLimit                           = tData->ContWeldCheckLimit;

    cInputMBD->Thresholds.AdditionalProtectionEn                       = tData->AdditionalProtectionEn;
    cInputMBD->Thresholds.HighImbalanceFlagEn                          = tData->HighImbalanceFlagEn;

    //contactor
    cInputMBD->Thresholds.ContCutoffTime_msec                = tData->contactorCutoffTime;
    cInputMBD->Thresholds.ContCurrent_mA                     = tData->preChargeContactorCurrent;
    cInputMBD->Thresholds.ContWeldCheckCurrent_mA            = tData->ContWeldCheckCurrent_mA;
    cInputMBD->Thresholds.ContactorWarningCounterLimit       = tData->ContactorWarningCounterLimit;
    cInputMBD->Thresholds.ContactorWeldTimeout_msec          = tData->ContactorWeldTimeout_msec;
    cInputMBD->Thresholds.OpenToPreChargeStateTime_msec      = tData->OpenToPreChargeStateTime_msec;
    cInputMBD->Thresholds.Min_PreChargeTime_msec             = tData->Min_PreChargeTime_msec;
    cInputMBD->Thresholds.Max_PreChargeTime_msec             = tData->Max_PreChargeTime_msec;
    cInputMBD->Thresholds.ContOpenCurrent_mA                 = tData->ContOpenCurrent_mA;
    cInputMBD->Thresholds.FlagDebounceTime_msec              = tData->FlagDebounceTime_msec;
    cInputMBD->Thresholds.Min_ClosedToOpeningTime_msec       = tData->Min_ClosedToOpeningTime_msec;
    cInputMBD->Thresholds.Max_ClosedToOpeningTime_msec       = tData->Max_ClosedToOpeningTime_msec;

    cInputMBD->Thresholds.PreChargeVoltageDelta_mV           = tData->PreChargeVoltageDelta_mV;
    cInputMBD->Thresholds.PreChargeEndMaxCurrent_mA          = tData->preChargeContactorCurrent;
    cInputMBD->Thresholds.PreChargeTimeout_msec              = tData->preChargeTimeout_msec;
    cInputMBD->Thresholds.PreChargeRetryTimeout_msec         = tData->preChargeRetryTimeout_msec;
    cInputMBD->Thresholds.PreChargeRetryLimit                = tData->preChargeRetryLimit;


    contState = INITIALIZING;

    status = BMS_OK;
	return status;
}


bmsStatus_t contactorInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorInput_t *cInput, contactorOutput_t *cOutput, ExtU_Contactors_T *cInputMBD, stateMachineOutput_t *smOutput)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == dpOutput)    ||\
        (NULL == pOutput)     ||\
        (NULL == cInput)      ||\
        (NULL == cOutput)     ||\
        (NULL == cInputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    static uint32_t currentTick = 0;
    static uint32_t lastTick = 0;
    static uint32_t duration = 0;


    currentTick = HAL_GetTick();
    duration = (lastTick == 0) ? 0 : (currentTick - lastTick);
    lastTick = currentTick;

    // precharge dynamic looptime
    cInput->PreCharge_LoopTime_msec = duration;

    // BMS state



    //protections output
    cInput->protectionOutputData.ThermalRunaway			    = pOutput->protectionOutputData.ThermalRunaway;
    cInput->protectionOutputData.TemperatureGradient		= pOutput->protectionOutputData.TemperatureGradient;
    cInput->protectionOutputData.HighImbalanceFlag	        = pOutput->protectionOutputData.HighImbalanceFlag;
    cInput->protectionOutputData.ShortCircuit		        = pOutput->protectionOutputData.ShortCircuit;
    cInput->protectionOutputData.SuddenVoltageDrop	        = pOutput->protectionOutputData.SuddenVoltageDrop;    
    cInput->protectionOutputData.OCC						= pOutput->protectionOutputData.OCC;
    cInput->protectionOutputData.OCD						= pOutput->protectionOutputData.OCD;
    cInput->protectionOutputData.OV						    = pOutput->protectionOutputData.OV;
    cInput->protectionOutputData.UV						    = pOutput->protectionOutputData.UV;
    cInput->protectionOutputData.TempOverallState           = pOutput->protectionOutputData.TempOverallState;
    cInput->PermanentFailRecoveryTrigger		            = 0;
    cInput->protectionOutputData.eFuseChargeFlag            = pOutput->protectionOutputData.eFuseChargeFlag;
    cInput->protectionOutputData.eFuseDischargeFlag         = pOutput->protectionOutputData.eFuseDischargeFlag;

    cInput->protectionOutputData.overCurrentChargeFlag      = pOutput->protectionOutputData.overCurrentChargeFlag;
    cInput->protectionOutputData.overCurrentDischargeFlag   = pOutput->protectionOutputData.overCurrentDischargeFlag;

    cInput->protectionOutputData.ErrorDetect                = pOutput->protectionOutputData.ErrorDetect;

    //contactor
    if(enableMerlyn)
    {
        cInput->contactorCommand                            = contactorCommandFromMerlin;
    }
    else
    {
        cInput->contactorCommand                            = isLoadRequested() || isChargeRequested();
        if(!cInput->contactorCommand)
        {
            contactorSequenceCompleted = false;
            debounceTimeContactorSequence = 0;
        }
    }
        // triggerDetection = false start checking after cmd goes high the first time. OR start checking after a debouncing time

    switch(contState)
    {
        case INITIALIZING:

            if((cInput->contactorCommand) || (HAL_GetTick() > 1000))
            {
               contState = CONTACTOR_OVERRIDE_CHECK;
            }
            externalToggle = false;
            break;

        case CONTACTOR_OVERRIDE_CHECK:

            if(fullChargeLatch)
            {
                contState = CONTACTOR_OVERRIDE;
            }
            else if(cInput->contactorCommand)
            {
                contState = CONTACTOR_ON;
            }
            else
            {
                contState = CONTACTOR_OFF;
            }
            break;

        case CONTACTOR_OVERRIDE:
            
            externalToggle = false;
            contState = WAIT_FOR_TOGGLE;
            break;

        case WAIT_FOR_TOGGLE:

            if((!cInput->contactorCommand) || (powerCycleDetected))
            {
                externalToggle = true;
            }
            break;

        case CONTACTOR_ON:
        case CONTACTOR_OFF:   

            externalToggle = true;
            break;
    }

    // cInput->contactorCommand                            = 1;
    cInput->positiveContactorFeedback                   = cOutput->positiveContactorState;
    cInput->preChargeContactorFeedback                  = cOutput->preChargeContactorState;    
    cInput->negativeContactorFeedback                   = cOutput->negativeContactorState; 

    cInput->dataPipelineBus.VCU.EmergencyDisconnectEn   = dpOutput->VCU.EmergencyDisconnectEn;
    cInput->TerminalVoltage_mV					        = mdata->ccmData.packVoltage * 1000;
    cInput->shortCircuitFlag					        = externalShortCircuitDetectionFlag;

    //data pipeline
    cInput->dataPipelineBus.current                     = dpOutput->current;
    for(uint8_t i = 0; i < CELL_IN_SERIES; i++)
    {
        cInput->dataPipelineBus.voltageSense.voltage[i] = dpOutput->voltageSense.voltage[i];
    }

    /*************** Copy to MBD structures *********************************************************************************************************/
    

    //protections
    cInputMBD->ProtectionOutput.ThermalRunaway			    = cInput->protectionOutputData.ThermalRunaway;
    cInputMBD->ProtectionOutput.TemperatureGradient		    = cInput->protectionOutputData.TemperatureGradient;
    cInputMBD->ProtectionOutput.HighImbalanceFlag			= cInput->protectionOutputData.HighImbalanceFlag;
    cInputMBD->ProtectionOutput.OCC						    = cInput->protectionOutputData.OCC;
    cInputMBD->ProtectionOutput.OCD						    = cInput->protectionOutputData.OCD;
    cInputMBD->ProtectionOutput.OV						    = cInput->protectionOutputData.OV;
    cInputMBD->ProtectionOutput.UV						    = cInput->protectionOutputData.UV;
    cInputMBD->ProtectionOutput.TempOverallState			  = cInput->protectionOutputData.TempOverallState;
    cInputMBD->ProtectionOutput.ShortCircuitDetect            = cInput->protectionOutputData.ShortCircuit;

    cInputMBD->ProtectionOutput.eFuseChargeFlag               = cInput->protectionOutputData.eFuseChargeFlag;
    cInputMBD->ProtectionOutput.eFuseDischargeFlag            = cInput->protectionOutputData.eFuseDischargeFlag;
    cInputMBD->ProtectionOutput.ErrorDetect                   = cInput->protectionOutputData.ErrorDetect;

    cInputMBD->ProtectionOutput.DCLI_CurrentFlag              = cInput->protectionOutputData.overCurrentChargeFlag;
    cInputMBD->ProtectionOutput.DCLO_CurrentFlag              = cInput->protectionOutputData.overCurrentDischargeFlag;   

    cInputMBD->PermanentFailRecoveryTrigger                     = cInput->PermanentFailRecoveryTrigger;
    cInputMBD->PermanentFail_Flag                               = cInput->permanentFailFlag;
    cInputMBD->ContactorError_Flag                              = cInput->contactorErrorFlag;

    //contactor
    cInputMBD->ContactorCommand			   = cInput->contactorCommand;
    cInputMBD->PosContactors_FB            = cInput->positiveContactorFeedback; 
    cInputMBD->PreChargeContactors_FB      = cInput->preChargeContactorFeedback;    
    cInputMBD->NegContactors_FB            = cInput->negativeContactorFeedback;

    cInputMBD->DataPipeline.VCU.EmergencyDisconnectEn             = cInput->dataPipelineBus.VCU.EmergencyDisconnectEn;

    //data pipeline
    cInputMBD->DataPipeline.Current_mA             = cInput->dataPipelineBus.current;

    cInputMBD->DataPipeline.PreChargeLogic         = cInput->dataPipelineBus.preChargeLogic;
    #ifdef TERMINAL_SENSE_CONNECTED
        cInputMBD->TerminalVoltage_mV                  = cInput->TerminalVoltage_mV;
    #else
        cInputMBD->TerminalVoltage_mV                  = 58000;
    #endif

    for (uint8_t i = 0; i < CELL_IN_SERIES; i++)
    {
        cInputMBD->DataPipeline.VoltageSenseBus.Voltages_mV[i] = cInput->dataPipelineBus.voltageSense.voltage[i];
    }

    // looptime
    cInputMBD->PreCharge_LoopTime_msec = cInput->PreCharge_LoopTime_msec;
    cInputMBD->BMS_State               = smOutput->bmsState;


    status = BMS_OK;
	return status;
}


bmsStatus_t contactorOutput(bmsMainData_t *mdata, contactorOutput_t *cOutput, ExtY_Contactors_T *cOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == cOutput)     ||\
        (NULL == cOutputMBD) )
	{
		status = BMS_CONTACTOR_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /******************************************************* Copy from MBD structures *****************************************************************************************/

    cOutput->negativeContactorState     = cOutputMBD->NegContactors;
    cOutput->preChargeContactorState    = cOutputMBD->PreChargeContactors;
    cOutput->positiveContactorState     = cOutputMBD->PosContactors;
    cOutput->contactorWeldWarning       = cOutputMBD->ContactorsWeldWarning;
    cOutput->permanentFaiil             = cOutputMBD->PermanentFaiil;
    cOutput->contactorsState            = cOutputMBD->ContactorsState;
    // cOutput->Elapsed_TimeToOpen_msec    = cOutputMBD->Elapsed_TimeToOpen_msec;
    
    if(externalShortCircuitDetectionFlag == 1)
    {
        //Switch off contactor
        TIM3 -> CCR1 = 0;
        TIM3 -> CCR2 = 0;
        TIM3 -> CCR3 = 0;
        TIM3 -> CCR4 = 0;
    }
    else 
    {
        if((cOutput->negativeContactorState) && (externalToggle))
        {
            turnContactorON(chargeNegative);
        }
        else
        {
            turnContactorOFF(chargeNegative);
            // contactorSequenceCompleted = false;
        }

        if((cOutput->preChargeContactorState) && (externalToggle))
        {
            turnContactorON(prechargePositive);
        }
        else
        {
            turnContactorOFF(prechargePositive);
        }

        if ((cOutput->positiveContactorState) && (externalToggle))
        {
            turnContactorON(commonPositive);

            debounceTimeContactorSequence++;
            if(debounceTimeContactorSequence >= 3)
            {
                contactorSequenceCompleted = true;
            }
        }
        else
        {
            turnContactorOFF(commonPositive);
        }
    }

    // setting flag for BMS state sanity

    // should also check all the protection conditions here
    // cOutput->contactorsState != 0 ? errorDisplayFlag : 0; //add duration here for the flag to be set
    powerCycleDetected = false;
    
    status = BMS_OK;
	return status;
}

bmsStatus_t coulombCountingInit(coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD, coulombCountingOutput_t *ccOutputData)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == ccInput)   ||\
        (NULL == ccInputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    ccInput->cc_Inputs.EquivalentCycleCapacity_mAh  = EQUIVALENT_CYCLE_CAPACITY;  //TO DO: multiply by SoH, for degrading capacity
    ccInput->cc_Inputs.InstalledCapacity_mAh        = INSTALLED_CAPACITY;
    ccInput->cc_Inputs.SensorOffset_mA              = SENSOR_OFFSET_mA;
    ccInput->cc_Inputs.SensorAccuracy_pct           = SENSOR_ACCURACY;
    ccInput->cc_Inputs.BleedingCurrent_mA           = 0;                           //BLEEDING_CURRENT_mA;
    ccInput->cc_Inputs.CCTimeout_msec               = CC_TIMEOUT_mS;
    ccInput->cc_Inputs.DebouncingTimeout_msec       = DEBOUNCING_TIMEOUT_mS;


    ccInput->cc_Inputs.SlowChargingMaxCurrentLimit_mA           = 15000;
    ccInput->cc_Inputs.SlowChargingMinCurrentLimit_mA           = 5000;
    ccInput->cc_Inputs.SlowChargingMaxAllowedImb_mV             = 35;    //CELL_IMBALANCE_THRESHOLD_mV;
    ccInput->cc_Inputs.SlowChargingMinVoltageLimit_mV           = 3385;
    ccInput->cc_Inputs.SlowChargingMaxVoltageLimit_mV           = 3410;

    ccInput->cc_Inputs.FastChargingMaxCurrentLimit_mA           = 18000;
    ccInput->cc_Inputs.FastChargingMinCurrentLimit_mA           = 5000;
    ccInput->cc_Inputs.FastChargingMaxAllowedImb_mV             = 170;    //CELL_IMBALANCE_THRESHOLD_mV;
    ccInput->cc_Inputs.FastChargingMinVoltageLimit_mV           = 3380;
    ccInput->cc_Inputs.FastChargingMaxVoltageLimit_mV           = 3550;

    ccInput->currentBased                                       = 0;     // 1 - currentBased, 0 - capacityBased


    ccInput->cc_Inputs.CapacityDeltaCheck_mAh       = CAPACITY_DELTA_CHECK_mAh;
    ccInput->cc_Inputs.InitialCapacity_Guess        = 0;     // 0 - flash, 1 - lookup table
    ccInput->cc_Inputs.SOH_Vs_Capacity_Gain         = 0;
    ccInput->cc_Inputs.SOH_Vs_Capacity_Offset       = 0;
    ccInput->cc_Inputs.LoopTimeSelector             = 0;     //0 - Timer DWT, 1 - Integrated time
    ccInput->cc_Inputs.SOHCalibrationOn             = 0;     //0 - SoH calibration OFF, 1 - SoH calibration ON
    ccInput->cc_Inputs.SOHCalibrationTimeout_msec   = 500;

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    ccInputMBD->CC_Inputs.EquivalentCycleCapacity_mAh   = ccInput->cc_Inputs.EquivalentCycleCapacity_mAh;
    ccInputMBD->CC_Inputs.InstalledCapacity_mAh         = ccInput->cc_Inputs.InstalledCapacity_mAh;
    ccInputMBD->CC_Inputs.SensorOffset_mA               = ccInput->cc_Inputs.SensorOffset_mA;
    ccInputMBD->CC_Inputs.SensorAccuracy_pct            = ccInput->cc_Inputs.SensorAccuracy_pct;
    ccInputMBD->CC_Inputs.BleedingCurrent_mA            = ccInput->cc_Inputs.BleedingCurrent_mA;
    ccInputMBD->CC_Inputs.CCTimeout_msec                = ccInput->cc_Inputs.CCTimeout_msec;
    
    ccInputMBD->CC_Inputs.DebouncingTimeout_msec        = ccInput->cc_Inputs.DebouncingTimeout_msec;

    //Recalibration thresholds for fast charging
    ccInputMBD->CC_Inputs.FastCharge.MaxAllowedImb_mV      = ccInput->cc_Inputs.FastChargingMaxAllowedImb_mV;
    ccInputMBD->CC_Inputs.FastCharge.MaxCurrentLimit_mA    = ccInput->cc_Inputs.FastChargingMaxCurrentLimit_mA;
    ccInputMBD->CC_Inputs.FastCharge.MaxVoltageLimit_mV    = ccInput->cc_Inputs.FastChargingMaxVoltageLimit_mV;
    ccInputMBD->CC_Inputs.FastCharge.MinCurrentLimit_mA    = ccInput->cc_Inputs.FastChargingMinCurrentLimit_mA;

    //Recalibration thresholds for slow charging
    ccInputMBD->CC_Inputs.SlowCharge.MaxAllowedImb_mV      = ccInput->cc_Inputs.SlowChargingMaxAllowedImb_mV;
    ccInputMBD->CC_Inputs.SlowCharge.MaxCurrentLimit_mA    = ccInput->cc_Inputs.SlowChargingMaxCurrentLimit_mA;
    ccInputMBD->CC_Inputs.SlowCharge.MaxVoltageLimit_mV    = ccInput->cc_Inputs.SlowChargingMaxVoltageLimit_mV;
    ccInputMBD->CC_Inputs.SlowCharge.MinCurrentLimit_mA    = ccInput->cc_Inputs.SlowChargingMinCurrentLimit_mA;

    ccInputMBD->CC_Inputs.CapacityDeltaCheck_mAh        = ccInput->cc_Inputs.CapacityDeltaCheck_mAh;
    ccInputMBD->CC_Inputs.InitialCapacity_Guess         = ccInput->cc_Inputs.InitialCapacity_Guess;
    ccInputMBD->CC_Inputs.SOHCalibrationOn              = ccInput->cc_Inputs.SOHCalibrationOn;
    ccInputMBD->CC_Inputs.SOH_Vs_Capacity_Gain          = ccInput->cc_Inputs.SOH_Vs_Capacity_Gain;
    ccInputMBD->CC_Inputs.SOH_Vs_Capacity_Offset        = ccInput->cc_Inputs.SOH_Vs_Capacity_Offset;
    ccInputMBD->CC_Inputs.LoopTimeSelector              = ccInput->cc_Inputs.LoopTimeSelector;
    
    status = BMS_OK;
	return status;
}

bmsStatus_t coulombCountingInput(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, chargingOutputData_t *chargerOutput, protectionOutput_t *pOutput, coulombCountingInput_t *ccInput, ExtU_SOCEstimation_T *ccInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == mdata)       ||\
        (NULL == dpOutput)    ||\
        (NULL == pOutput)     ||\
        (NULL == ccInput)     ||\
        (NULL == ccInputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    taskBreakpointCycleCount = DWT->CYCCNT;
    time = ((float)(taskBreakpointCycleCount)/Systemfrequency);
    DWT->CYCCNT = 0;

    ccInput->current_mA                                = dpOutput->current;
    ccInput->voltage_mV                                = dpOutput->voltageSense.voltageMax;
    ccInput->dataPipelineBus.VCU.isChargerConnected    = dpOutput->VCU.isChargerConnected;
    ccInput->fastChargeEnabled                         = dpOutput->VCU.FastCharge;
    ccInput->CellBalancingOutput.volatgeImbalance      = dpOutput->voltageSense.voltageDelta;
    ccInput->dataPipelineBus.voltageSense.voltageMin   = dpOutput->voltageSense.voltageMin;
    ccInput->dataPipelineBus.voltageSense.voltageMax   = dpOutput->voltageSense.voltageMax;
    ccInput->LoopTimer_SOC_msec                        = time * 1000;

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    ccInputMBD->Current_mA                                 = ccInput->current_mA;
    ccInputMBD->Voltage_mV                                 = ccInput->voltage_mV;
    ccInputMBD->DataPipeline.VCU.isChargerConnected        = ccInput->dataPipelineBus.VCU.isChargerConnected;
    ccInputMBD->CellBalancingOutput.MaxImbalance_mV        = ccInput->CellBalancingOutput.volatgeImbalance;
    ccInputMBD->DataPipeline.VoltageSenseBus.mV_min        = ccInput->dataPipelineBus.voltageSense.voltageMin;
    ccInputMBD->DataPipeline.VoltageSenseBus.mV_max        = ccInput->dataPipelineBus.voltageSense.voltageMax;
    ccInputMBD->DataPipeline.LoopTimer_SOC_msec            = ccInput->LoopTimer_SOC_msec;
    ccInputMBD->DataPipeline.VCU.FastCharge                = ccInput->fastChargeEnabled;

    status = BMS_OK;
	return status;  
}


bmsStatus_t coulombCountingOutput(bmsMainData_t *mdata, coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFLASH, ExtY_SOCEstimation_T *ccOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

    if( (NULL == mdata)       ||\
        (NULL == ccOutput)    ||\
        (NULL == ccOutputMBD) )
	{
		status = BMS_COULOMB_COUNTING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/

    ccOutput->initialCapacity                = ccOutputMBD->CC_Outputs.Initial_Capacity_mAh;
    ccOutput->totalCapacityRemaining         = ccOutputMBD->CC_Outputs.Total_CapacityRemains_mAh;

    if(ccOutput->totalCapacityRemaining <= 0)
    {
        ccOutput->totalCapacityRemaining = 0;
    }

    if(ccOutput->totalCapacityRemaining > 180000)
    {
        ccOutput->totalCapacityRemaining = 180000;
    }

    #ifdef SOC_IN_FLASH
    ccOutput->totalCapacityDischarge         = ccOutputMBD->CC_Outputs.Total_Discharge_mAh        + ccOutputFLASH->totalCapacityDischarge;    //add the previous stored value in eeprom
    ccOutput->TotalCapacityExchange_mAh      = ccOutputMBD->CC_Outputs.TotalCapacityExchange_mAh  + ccOutputFLASH->TotalCapacityExchange_mAh; 

    #else
    ccOutput->totalCapacityDischarge         = ccOutputMBD->CC_Outputs.Total_Discharge_mAh        + ccOutputEEPROM->totalCapacityDischarge;    //add the previous stored value in eeprom
    ccOutput->TotalCapacityExchange_mAh      = ccOutputMBD->CC_Outputs.TotalCapacityExchange_mAh  + ccOutputEEPROM->TotalCapacityExchange_mAh;

    #endif

    ccOutput->SOC_cpct                       = ccOutputMBD->CC_Outputs.SOC_cpct;
    ccOutput->SOC_pct                        = (ccOutputMBD->CC_Outputs.SOC_cpct)/100;
    ccOutput->coulombCountingState           = ccOutputMBD->CC_Outputs.CC_State;
    ccOutput->MaxUsableCapacity_mAh          = ccOutputMBD->CC_Outputs.MaxUsableCapacity_mAh;
    ccOutput->SOH_pct                        = (ccOutputMBD->CC_Outputs.SOH_cpct)/100;
    // ccOutput->CycleCount                     = ccOutputMBD->CC_Outputs.CycleCount;

    ccOutput->CycleCount                     = ccOutput->TotalCapacityExchange_mAh/EQUIVALENT_CYCLE_CAPACITY;

    ccOutput->SOC_FromFlash                  = ccOutputMBD->SOC_FromFlash;
    ccOutput->packEnergy                     = (ccOutput->totalCapacityRemaining/10)*PACK_NOMINAL_VOLTAGE;

    mdata->ccmData.packCapacity = (ccOutput->totalCapacityRemaining)/10;

    #ifdef EEPROM_TEST
        ccOutput->initialCapacity           += 100;
        ccOutput->totalCapacityRemaining    += 100;
        ccOutput->totalCapacityDischarge    += 200;
        ccOutput->TotalCapacityExchange_mAh += 23;
        ccOutput->SOC_pct                   += 1;
    #endif

    // if(ccOutput->coulombCountingState == Recalibrate)
    // {
    //     fullChargeLatch = 1;  //for how long should the latch true flag stay?
    //     latchCheckFlag = 1;
    //     pseudoLatchFlag = 1;
    //     contState = CONTACTOR_OVERRIDE;
    // }

    status = BMS_OK;
	return status;
}

void writeSoCToFlash(coulombCountingOutput_t *ccOutput, coulombCountingOutput_t *ccOutputEEPROM, coulombCountingOutput_t *ccOutputFlash)
{
    // static uint8_t writeToFlashFlag = 0;
    static uint8_t continueEvaluation = 1;

    // Check the flag before evaluating the condition
    if (continueEvaluation) 
    {
        startEEPROMWriteFlag = ((ccOutput->totalCapacityRemaining == ccOutputEEPROM->totalCapacityRemaining) || ccOutput->totalCapacityRemaining != 0) ? TRUE : FALSE;

        // Set the flag to stop further evaluation if startEEPROMWriteFlag is TRUE
        if (startEEPROMWriteFlag) 
        {
            tx_event_flags_set(&systemEventFlag, SANITY_CHECK_FLAG, TX_OR);
            continueEvaluation = 0;
        }
    }
    
    // should we add temporary premtion change here?
    if(startEEPROMWriteFlag)
    {
        // delayInMilliSeconds(100);

        if(pseudoLatchFlag)
        {
            tx_event_flags_set(&systemEventFlag, ACTIVATE_PSEUDO_LATCH, TX_OR);
        }

        if((abs(abs(ccOutput->totalCapacityRemaining) - abs(ccOutputEEPROM->totalCapacityRemaining)) >= 1000) || latchCheckFlag)
        {
                // if((ccOutput->totalCapacityRemaining <= 179000) && (ccOutput->SOC_pct > 99))       // should revisit this
                if((ccOutput->totalCapacityRemaining <= 179000) && fullChargeLatch)      // should revisit this
                {
                    fullChargeLatch = 0;
                    pseudoLatchFlag = 1;

                    // writeEEPROMDataToBuffer(ccOutput, criticalFlagsWriteBuff, ccWriteBufferFLASH, CRITICAL_FLAGS);
                    // RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, criticalFlagsWriteBuff, (sizeof(criticalFlagsWriteBuff)), CRITICAL_FLAGS)); 
                }           
                else if((ccOutput->SOC_pct < 90) && (pseudoLatchFlag))    
                {
                    //clear pseudo latch flag here
                    fullChargeLatch = 0;
                    pseudoLatchFlag = 0;
                    tx_event_flags_set(&systemEventFlag, ~ACTIVATE_PSEUDO_LATCH, TX_AND);

                    // writeEEPROMDataToBuffer(ccOutput, criticalFlagsWriteBuff, ccWriteBufferFLASH, CRITICAL_FLAGS);
                    // RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, criticalFlagsWriteBuff, (sizeof(criticalFlagsWriteBuff)), CRITICAL_FLAGS)); 
                }

                ccOutputEEPROM->totalCapacityRemaining = ccOutput->totalCapacityRemaining;
                ccOutputEEPROM->SOC_pct                = ccOutput->SOC_pct;

                // acquiring mutex to access critical resource
                tx_mutex_get(&ccOutputMutex, TX_WAIT_FOREVER);

                // writing SoC data to memory
                writeEEPROMDataToBuffer(ccOutput, socDataWriteBuff, ccWriteBufferFLASH, SOC_SECTION);
                RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, socDataWriteBuff, (sizeof(socDataWriteBuff)), SOC_SECTION)); 

                statusSoC != 0 ? tx_event_flags_set(&systemEventFlag, EEPROM_WRITE_FAIL, TX_OR) : 0;


                // writing critical flags to memory
                writeEEPROMDataToBuffer(ccOutput, criticalFlagsWriteBuff, ccWriteBufferFLASH, CRITICAL_FLAGS);
                RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, criticalFlagsWriteBuff, (sizeof(criticalFlagsWriteBuff)), CRITICAL_FLAGS)); 
                
                statusSoC != 0 ? tx_event_flags_set(&systemEventFlag, EEPROM_WRITE_FAIL, TX_OR) : 0;

                // releasing mutex
                tx_mutex_put(&ccOutputMutex);

                // eeprom write failure error handling here

                // if((statusSoC) && (abs(ccOutput->totalCapacityRemaining - ccOutputFlash->totalCapacityRemaining) >= 5000))
                // {
                //     // add flash as backup here
                //     ccOutputFlash->totalCapacityRemaining = ccOutput->totalCapacityRemaining;    

                //     ccWriteBufferFLASH[0] = ccOutput->initialCapacity;
                //     ccWriteBufferFLASH[1] = ccOutput->totalCapacityRemaining;
                //     ccWriteBufferFLASH[2] = ccOutput->totalCapacityDischarge; 
                //     ccWriteBufferFLASH[3] = ccOutput->TotalCapacityExchange_mAh;
                //     ccWriteBufferFLASH[4] = ccOutput->SOC_pct;
                //     ccWriteBufferFLASH[5] = READ_FROM_FLASH;

                //     statusSoC = flashWrite(SOC_DATA_ADDRESS, ccWriteBufferFLASH, 6);
                //     writeToFlashFlag  = 1;
                // }
                // else if((!statusSoC) && (writeToFlashFlag))
                // {
                //     ccWriteBufferFLASH[5] = 0;
                //     statusSoC = flashWrite(SOC_DATA_ADDRESS, ccWriteBufferFLASH, 6);
                //     writeToFlashFlag = 0;
                // }

                #ifdef SOC_IN_FLASH
                    ccOutputFlash->SOC_pct = ccOutput->SOC_pct;
                    statusSoC = flashWrite(SOC_DATA_ADDRESS, ccWriteBufferFLASH, 5);
                #endif


            }
            if(fullChargeLatch && latchCheckFlag)
            {
                latchCheckFlag = 0;
                // systemReset();
            }    
        
            latchCheckFlag  = 0;
    }
    }

//shutdown sequence
void shuntdownRoutine(void)
{
    // shutdown sequence
    if (tx_event_flags_get(&systemEventFlag, POWER_OFF_SIG, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS || shutdownCount == 1) 
    {   
        if (shutdownCount == 0) 
        {
            writeSoCToFlash(&ccOutputData, &ccOutputDataEEPROM, &ccOutputDataFlash);
        
            if(tx_event_flags_get(&systemEventFlag, EEPROM_WRITE_FAIL, TX_OR, &actualFlag, TX_NO_WAIT) != TX_SUCCESS) 
            {
                shutdownCount = 1;
            }
         }
        else 
        {
            shutdownCount = 1;
        }
    }
}


bmsStatus_t stateMachineInit(thresholdData_t *tData, dataPipelineInput_t *dpInput, ExtU_HighLevelStatemachine_T *smInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)    ||\
        (NULL == smInputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    smInputMBD->Thresholds.ChargeDetectionThreshold_mA      = dpInput->thresholdDataBus.chargeDetectionThreshold_mA;
    smInputMBD->Thresholds.NormalModeTimeout_msec           = tData->NormalModeTimeout_msec;
    smInputMBD->Thresholds.PowerSaverModeTimeout_msec       = tData->PowerSaverModeTimeout_msec;
    smInputMBD->Thresholds.FaultsModeTimeout_msec           = tData->FaultsModeTimeout_msec;
    smInputMBD->Thresholds.ServiceModeTimeout_msec          = tData->ServiceModeTimeout_msec;
    // smInputMBD->Thresholds.BMS_State_Timeout_msec           = tData->BMS_stateMachineTimeout;

    smInputMBD->Thresholds.BMS_ActivatingToActive_Timeout_msec   = tData->BMS_ActivatingToActive_Timeout_msec;
    smInputMBD->Thresholds.BMS_ActivatingToError_Timeout_msec    = tData->BMS_ActivatingToError_Timeout_msec;
    smInputMBD->Thresholds.BMS_ActiveToError_Timeout_msec        = tData->BMS_ActiveToError_Timeout_msec;
    smInputMBD->Thresholds.BMS_ActiveToReady_Timeout_msec        = tData->BMS_ActiveToReady_Timeout_msec;
    smInputMBD->Thresholds.BMS_ErrorToActivating_Timeout_msec    = tData->BMS_ErrorToActivating_Timeout_msec;
    smInputMBD->Thresholds.BMS_ReadyToActivating_Timeout_msec    = tData->BMS_ReadyToActivating_Timeout_msec;
    // smInputMBD->Thresholds.BMS_State_Timeout_msec                = tData->BMS_State_Timeout_msec;   

    status = BMS_OK;
	return status;
}

bmsStatus_t stateMachineInput(dataPipelineOutput_t *dpOutput, protectionOutput_t *pOutput, contactorOutput_t *cOutput, ExtU_HighLevelStatemachine_T *smInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpOutput)        ||\
        (NULL == pOutput)         ||\
        (NULL == smInputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy to MBD structures *********************************************************************************************************/
    
    smInputMBD->DataPipeline.Current_mA                 = dpOutput->current;
    smInputMBD->DataPipeline.isChargeEn                 = dpOutput->isChargeEn;

    smInputMBD->DataPipeline.VCU.SlowCharge             = dpOutput->VCU.SlowCharge;
    smInputMBD->DataPipeline.VCU.FastCharge             = dpOutput->VCU.FastCharge;
    smInputMBD->DataPipeline.VCU.isChargerConnected     = dpOutput->VCU.isChargerConnected;
    smInputMBD->DataPipeline.VCU.TransmissionFail       = dpOutput->VCU.TransmissionFail;
    smInputMBD->DataPipeline.VCU.VCUPermanentFail       = dpOutput->VCU.VCUPermanentFail;
    smInputMBD->DataPipeline.VCU.ServiceMode            = dpOutput->VCU.ServiceMode;

    smInputMBD->ContactorsState                         = cOutput->contactorsState;
    smInputMBD->ContactorCommand                        = isLoadRequested() || isChargeRequested();

    smInputMBD->ProtectionOutput.ErrorDetect          = pOutput->protectionOutputData.ErrorDetect;
    
    status = BMS_OK;
	return status;
}

bmsStatus_t stateMachineOutput(stateMachineOutput_t *smOutput, ExtY_HighLevelStatemachine_T *smOutputMBD, bmsMainData_t *mdata)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == smOutput)        ||\
        (NULL == smOutputMBD) )
	{
		status = BMS_STATE_MACHINE_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    smOutput->HighLevelStateMode = smOutputMBD->HighLevelStateMode;
    smOutput->bmsState           = smOutputMBD->BMS_State;

    mdata->bmsState              = smOutput->bmsState;
    marvelState                  = mdata->bmsState;

    // if((!pOutputData.protectionOutputData.ErrorDetect) && (marvelState == 4) && (isLoadRequested() || isChargeRequested()))   // handling corner case
    // {
    //     marvelState = 2;
    // }

    status = BMS_OK;
	return status;
}

bmsStatus_t chargingInit(thresholdData_t *tData, ExtU_ChargingAlgorithm_T *chargingInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == tData)        ||\
        (NULL == chargingInputMBD) )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    chargingInputMBD->Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV    = tData->ChargeAlgorithm.BatteryRevivalVoltage_mV;
    chargingInputMBD->Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV      = tData->ChargeAlgorithm.CVTransitionVoltage_mV;
    chargingInputMBD->Thresholds.ChargeAlgorithm.SOC_forCVCharge_cpct        = tData->ChargeAlgorithm.SOC_forCVCharge_cpct;
    chargingInputMBD->Thresholds.ChargeAlgorithm.TransitionTimeout_msec      = tData->ChargeAlgorithm.TransitionTimeout_msec;

    chargingInputMBD->Thresholds.Balancing_Min_Thr_mV                  = tData->balancingMinThreshold;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Warning_mV      = tData->slowCharging_OV_WarningVoltage;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Error_mV        = tData->slowCharging_OV_ErrorVoltage;
    chargingInputMBD->Thresholds.Protections_SlowCh_OV_Recovery_mV     = tData->slowCharging_OV_RecoveryVoltage;

    chargingInputMBD->Thresholds.Protections_FastCh_OV_Warning_mV      = tData->fastCharging_OV_WarningVoltage;
    chargingInputMBD->Thresholds.Protections_FastCh_OV_Error_mV        = tData->fastCharging_OV_ErrorVoltage;
    chargingInputMBD->Thresholds.Protections_FastCh_OV_Recovery_mV     = tData->fastCharging_OV_RecoveryVoltage;

    chargingInputMBD->Thresholds.targetVoltageSlowCharging             = tData->ChargeAlgorithm.targetVoltageSlowCharging;
    chargingInputMBD->Thresholds.targetVoltagesFastCharging            = tData->ChargeAlgorithm.targetVoltageFastCharging;

    chargingInputMBD->Thresholds.SlowCharge_BatteryRevival_K            = tData->ChargeAlgorithm.slowCharging_K;
    chargingInputMBD->Thresholds.SlowCharge_CC_Balancing_K              = tData->ChargeAlgorithm.slowCharging_K;
    chargingInputMBD->Thresholds.SlowCharge_CC_K                        = tData->ChargeAlgorithm.slowCharging_K;
    chargingInputMBD->Thresholds.SlowCharge_CV_K                        = tData->ChargeAlgorithm.slowCharging_K;
    chargingInputMBD->Thresholds.SlowCharge_NoCharge_K                  = tData->ChargeAlgorithm.NoCharge_K;

    chargingInputMBD->Thresholds.FastCharge_BatteryRevival_K            = tData->ChargeAlgorithm.fastCharging_K;
    chargingInputMBD->Thresholds.FastCharge_CC_Balancing_K              = tData->ChargeAlgorithm.fastCharging_K;
    chargingInputMBD->Thresholds.FastCharge_CC_K                        = tData->ChargeAlgorithm.fastCharging_K;
    chargingInputMBD->Thresholds.FastCharge_CV_K                        = tData->ChargeAlgorithm.fastCharging_K;


    chargingInputMBD->DataPipeline.VCU.SlowChargersCurrentLimit_A       = tData->ChargeAlgorithm.SlowChargersCurrentLimit_A;
    chargingInputMBD->DataPipeline.VCU.FastChargersCurrentLimit_A       = tData->ChargeAlgorithm.FastChargersCurrentLimit_A;

    status = BMS_OK;
	return status;
}


bmsStatus_t chargingInput(ExtU_DataPipeline_T *dpInputMBD, ExtY_DataPipeline_T *dpOutputMBD, ExtU_ChargingAlgorithm_T *chargingInputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == dpInputMBD)        ||\
        (NULL == dpOutputMBD)       ||\
        (NULL == chargingInputMBD) )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    chargingInputMBD->DataPipeline.VCU.FastCharge                   = dpOutputMBD->DataPipelineb.VCU.FastCharge;
    chargingInputMBD->DataPipeline.VCU.SlowCharge                   = dpOutputMBD->DataPipelineb.VCU.SlowCharge;
    chargingInputMBD->DataPipeline.VCU.isChargerConnected           = dpOutputMBD->DataPipelineb.VCU.isChargerConnected;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_min           = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_min;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_max           = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_max;
    chargingInputMBD->DataPipeline.DCL_ChargingCurrent_A            = dpOutputMBD->DataPipelineb.DCL_ChargingCurrent_A;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_min           = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_min;
    chargingInputMBD->DataPipeline.VoltageSenseBus.mV_max           = dpOutputMBD->DataPipelineb.VoltageSenseBus.mV_max;
    

    status = BMS_OK;
	return status;
}


bmsStatus_t chargingOutput(chargingOutputData_t *chargerOutput, ExtY_ChargingAlgorithm_T *chargingOutputMBD)
{
    bmsStatus_t status = BMS_ERROR;

	if( (NULL == chargingOutputMBD)
      )
	{
		status = BMS_CHARGING_FUNCTION_PARAMETER_ERROR;
		return status;
	}

    /*************** Copy from MBD structures *********************************************************************************************************/
    
    chargerOutput->ChargingState           = chargingOutputMBD->ChargingState;
    chargerOutput->ChargingCurrentReq_mA   = chargingOutputMBD->ChargingCurrentReq_mA;
    
    // requestedCurrent_A = 0;
    // requestedCurrent_A = calculateChargingCurrent(&dpOutputData, &ccOutputData, &chargingOutputData);
    requestedCurrent_A = calculateChargingCurrent(&dpOutputData, &ccOutputData, &chargingOutputData, &thresholdData);
    requestedCurrent_A = activatePseudoLatching(&ccOutputData, &ccInputData, requestedCurrent_A);  

    status = BMS_OK;
	return status;
}

float activatePseudoLatching(coulombCountingOutput_t *ccOutput, coulombCountingInput_t *ccInput, float requestedCurrent)
{
    // this function will get unlocked only when fullChargeLatch flag is present given SoC is between 90 and 100
    // replacing the logic of not requesting current when fullChargeLatch is present with capacity based pseudo latching
    // eeprom write logic should be modified here to account for the worst case scenario
    // will be adding provision for using manipulated profile and capacity based lacthing

    static uint8_t debounceTime = 0;

    if((tx_event_flags_get(&systemEventFlag, ACTIVATE_PSEUDO_LATCH, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS) && (ccOutput->SOC_pct > 90))
    {
        delayInMilliSeconds(50);
        if(ccInput->currentBased)
        {
            //feed in the new current profile as per SoC
        }
        else if((ccOutput->totalCapacityRemaining < ccOutput->MaxUsableCapacity_mAh) && (vcuData.isChargerConnected == 1)) //request current untill pack is charged to 180Ah*SoH 
        {
            // fullChargeLatch == 1 ? (requestedCurrent = 0) : (requestedCurrent = PSEUDO_LATCH_CURRENT);
            fullChargeLatch == 1 ? (requestedCurrent = 0) : (requestedCurrent = requestedCurrent);
        }
        else if(ccOutput->totalCapacityRemaining == ccOutput->MaxUsableCapacity_mAh)
        {
            debounceTime++;
            if(debounceTime > 5)
            {
                //give latchTrue flag to stark
                fullChargeLatch = 1;
                pseudoLatchFlag = 1;
                latchCheckFlag  = 1;
            }
        }
    }
    else
    {
        requestedCurrent = requestedCurrent;  //do not manipulate the current
    }

    return requestedCurrent;
}

float SoCManipulation(bmsMainData_t *mdata, ExtY_SOCEstimation_T *ccOutputMBD)
{
    mdata->socData.estimatedSoC = ccOutputMBD->CC_Outputs.SOC_cpct;   
    return mdata->socData.estimatedSoC;   
}


uint8_t fetchMarvelState(bmsMainData_t *mdata,  contactorOutput_t *cOutput,  contactorInput_t *cInput)
{
    static uint8_t bmsReadyDebounce = 0;
    if(!(cInput->contactorCommand))
    {
        bmsReadyDebounce++;
        if(bmsReadyDebounce >= 3)
        {
            mdata->bmsState = BMS_READY;
        }
    }
    else
    {
        if(cOutput->positiveContactorState == 0)
        {
            mdata->bmsState = BMS_ACTIVATING;
            bmsReadyDebounce = 0;
        }
        else
        {
            mdata->bmsState = BMS_ACTIVE;
        }
        if((cOutput->negativeContactorState == 0 && contactorSequenceCompleted) || (cOutput->contactorsState == 8))
        {
            mdata->bmsState = BMS_ERROR1;
            bmsReadyDebounce = 0;
        } 
    }

    return mdata->bmsState;
}

// uint8_t errorCodes1Dataframe[8] = {0};

uint8_t publishErrorCodes(contactorOutput_t *cOutput, protectionOutput_t *pOutput)
{
    bmsStatus_t status = BMS_ERROR;
    // static uint8_t errorDebounceCounters[2][8] = {0}; // 2 bytes, 8 bits each
    // uint8_t canDataFrame[8] = {'\0'}; 
    memset(errorCodes1Dataframe, 0, sizeof(errorCodes1Dataframe));

    // Check and set error bits with debounce logic
    if((pOutput->protectionOutputData.OV == 2) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 7);       // over voltage error
        // errorDebounceCounters[0][7] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if((pOutput->protectionOutputData.UV == 2) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 6);       // under voltage error
        // errorDebounceCounters[0][6] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(((pOutput->protectionOutputData.eFuseChargeFlag == 2) || (pOutput->protectionOutputData.eFuseChargeFlag == 4)) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 5);       // eFuse trigger charging
        // errorDebounceCounters[0][5] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(((pOutput->protectionOutputData.eFuseDischargeFlag == 2) || (pOutput->protectionOutputData.eFuseDischargeFlag == 4)) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 4);       // eFuse discharging discharge 
        // errorDebounceCounters[0][4] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if((pOutput->protectionOutputData.FlagGroup1 == 2) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 3);       // external NTC over temperature
        // errorDebounceCounters[0][3] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if((pOutput->protectionOutputData.FlagGroup2 == 2) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U << 2);       // PCB NTC over temperature
        // errorDebounceCounters[0][2] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(pOutput->protectionOutputData.HighImbalanceFlag)
    {
        errorCodes1Dataframe[0] |= (1U << 1);       // high imbalance flag
        // errorDebounceCounters[0][1] = DEBOUNCE_TIME_ERROR_CODES;
    }
    // if(pOutput->protectionOutputData.SuddenVoltageDrop)
    if((pOutput->protectionOutputData.overCurrentChargeFlag == 2) && (marvelState == 4))
    {
        errorCodes1Dataframe[0] |= (1U);            // over current discharge error flag
        // errorDebounceCounters[0][0] = DEBOUNCE_TIME_ERROR_CODES;
    }
    
    if((cOutput->contactorsState == 8) && (marvelState == 4))
    {
        errorCodes1Dataframe[1] |= (1U << 4);       // contactor permanent fail
        // errorDebounceCounters[1][4] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(cOutput->contactorsState == 9)
    {
        errorCodes1Dataframe[1] |= (1U << 5);        // preCharge retry fail
        // errorDebounceCounters[1][5] = DEBOUNCE_TIME_ERROR_CODES; 
    }
    if(tx_event_flags_get(&systemEventFlag, EEPROM_READ_FAIL, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
    {
        errorCodes1Dataframe[1] |= (1U << 3);       // eFuse read fail after retry
        // errorDebounceCounters[1][3] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(tx_event_flags_get(&systemEventFlag, EEPROM_WRITE_FAIL, TX_OR, &actualFlag, TX_NO_WAIT) == TX_SUCCESS)
    {
        errorCodes1Dataframe[1] |= (1U << 2);       // eFuse write fail after retry 
        // errorDebounceCounters[1][2] = DEBOUNCE_TIME_ERROR_CODES;
    }

    //changed from temperature gradient
    if(pOutput->protectionOutputData.overCurrentDischargeFlag == 2 && (marvelState == 4))
    {
        errorCodes1Dataframe[1] |= (1U);            // over current discharge flag
        // errorDebounceCounters[1][0] = DEBOUNCE_TIME_ERROR_CODES;
    }

    if((pOutput->protectionOutputData.ShortCircuit) && (marvelState == 4))
    {
        errorCodes2Dataframe[0] |= (1U << 7);       // short circuit error
        // errorDebounceCounters[1][7] = DEBOUNCE_TIME_ERROR_CODES;
    }
    if(((pOutput->protectionOutputData.ThermalRunaway == 2) || (pOutput->protectionOutputData.ThermalRunaway == 4)) && (marvelState == 4))
    {
        errorCodes2Dataframe[0] |= (1U << 6);       // thermal runaway
        // errorDebounceCounters[1][6] = DEBOUNCE_TIME_ERROR_CODES;
    }
    // Update debounce counters and clear bits if needed
    // for(uint8_t byte = 0; byte < 2; byte++)
    // {
    //     for(uint8_t bit = 0; bit < 8; bit++)
    //     {
    //         if(errorDebounceCounters[byte][bit] > 0)
    //         {
    //             errorDebounceCounters[byte][bit]--;
    //         }
    //         else
    //         {
    //             errorCodes1Dataframe[byte] &= ~(1U << bit);
    //         }
    //     }
    // }

    // Publish the error codes over CAN
    // memcpy(canDataFrame, errorCodes1Dataframe, sizeof(errorCodes1Dataframe));
    // status = sendCANDataOnQueue(CAN_TX_ID_013, errorCodes1Dataframe, sizeof(errorCodes1Dataframe));
    // CHECK_STATUS(status);
    return status;
}


float advancedLatchingRoutine(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput)
{
    // bmsStatus_t status = BMS_ERROR;
    float requestedCurrent_A        = 0;
    static bool capacityExchangeCompleted = false;
    static bool imbalanceControlAchieved  = false;
    static bool firstWriteCycleCompleted  = true;
    static latchingRoutineState state = CHARGING_STATE;

    // imbalanceControlAchieved flag should only be set sequentially -> can only be set if capacityExchangeCompleted flag is set

    switch(state)
    {
        // This state ensures battery pack has been charged to its max allowed capacity
        case CHARGING_STATE:
             
            if((ccInput->dataPipelineBus.voltageSense.voltageMin < ccInput->cc_Inputs.FastChargingMinVoltageLimit_mV) && (ccInput->dataPipelineBus.voltageSense.voltageDelta < ccInput->cc_Inputs.FastChargingMaxAllowedImb_mV))
            {
            // charge battery at 15 amps | override MBD charging logic and manually request current
                requestedCurrent_A = PSEUDO_LATCH_CURRENT;
            }
            else if(ccInput->dataPipelineBus.voltageSense.voltageMin > ccInput->cc_Inputs.FastChargingMinVoltageLimit_mV)
            {
                capacityExchangeCompleted = true;
            }
            else
            {
                // changing state here to allow the pack to balance
                // chnage requested current to 0 here
                requestedCurrent_A = 0;
                state = BALANCING_STATE;
            }

            break;
        // This state ensures battery pack imbalance has been controlled
        case BALANCING_STATE:

            // enable external balancing here and wait till the imbalance comes below the threshold
            if(ccInput->dataPipelineBus.voltageSense.voltageMax > ccInput->cc_Inputs.FastChargingMaxAllowedImb_mV)
            {
                // enable external balancing here
                // can add some more actions here that can be taken when the first threshold has been achieved
                vcuData.BalanceEn_atRest = true;
            }
            else if((ccInput->dataPipelineBus.voltageSense.voltageMax > ccInput->cc_Inputs.FastChargingMaxAllowedImb_mV - 30) && !capacityExchangeCompleted)
            {
                // we will wait here for the imbalance to further decrese before we start charging again
            }
            else
            {
                imbalanceControlAchieved = capacityExchangeCompleted ? true : imbalanceControlAchieved;     // exit this subroutine and publist full charge flag

                // we change the state here to resume charging
                state = CHARGING_STATE;
            }

            break;

        default:
            break;
    }

    if(ccOutput->totalCapacityRemaining > 180000)
    {
       ccOutput->totalCapacityRemaining = 180000; 
    }

    // setting latch true flag
    if(imbalanceControlAchieved && capacityExchangeCompleted)
    {
        fullChargeLatch = true;
        vcuData.BalanceEn_atRest = false;
        ccOutput->initialCapacity        = 180000;
        ccOutput->totalCapacityRemaining = 180000;
        ccOutput->SOC_pct                = 100;
    }

    /*
        adding packCapacity saturation to prevent SoC from hanging at 100%
        store this in EEPROM so that after power cycle, pack capacity remaining will start from 180Ah
        not changing initial capacity here to indicate not latched condition

        case 1: fullChargeLatch is not true yet 
                store the saturated pack capacity of 180 Ah to EEPROM

        case 2: calibrate all the parameters as per 100% SoC
                store all the parameters to EEPROM 

        should only write to EEPROM once
    */ 
   if(firstWriteCycleCompleted || fullChargeLatch)
   {
    writeEEPROMDataToBuffer(ccOutput, socDataWriteBuff, ccWriteBufferFLASH, SOC_SECTION);
    RETRY(RETRY_EEPROM, statusSoC, memoryWrite(eeprom, socDataWriteBuff, (sizeof(socDataWriteBuff)), SOC_SECTION)); 
    firstWriteCycleCompleted = false;
   }

    return requestedCurrent_A;
}

uint8_t findMaxAllowedImbalance(uint8_t soc, chargingOutputData_t *chargerOutput)
{
    
    if(chargerOutput->ChargingState == slowCharge)
    {
        if(soc >= 0 && soc < 5) 
        {
            return 50;
        } 
        else if(soc >= 5 && soc < 10) 
        {
            return 25;
        } 
        else if(soc >= 10 && soc < 90) 
        {
            return 15;
        } 
        else if(soc >= 90 && soc < 95) 
        {
            return 25;
        } 
        else if(soc >= 95 && soc < 97) 
        {
            return 30;
        } 
        else if(soc >= 97 && soc <= 100) 
        {
            return 35;
        }
    }
    else if(chargerOutput->ChargingState == fastCharge)
    {
        if(soc >= 0 && soc < 5) 
        {
            return 50;
        } 
        else if(soc >= 5 && soc < 10) 
        {
            return 35;
        } 
        else if(soc >= 10 && soc < 90) 
        {
            return 35;
        } 
        else if(soc >= 90 && soc < 95) 
        {
            return 35;
        } 
        else if(soc >= 95 && soc < 97) 
        {
            return 40;
        } 
        else if(soc >= 97 && soc <= 100) 
        {
            return 180;
        }
    }
    else if(chargerOutput->ChargingState == noCharge)
    {
        if(soc >= 0 && soc < 5) 
        {
            return 60;
        } 
        else if(soc >= 5 && soc < 10) 
        {
            return 25;
        } 
        else if(soc >= 10 && soc < 90) 
        {
            return 15;
        } 
        else if(soc >= 90 && soc < 95) 
        {
            return 25;
        } 
        else if(soc >= 95 && soc < 97) 
        {
            return 30;
        } 
        else if(soc >= 97 && soc <= 100) 
        {
            return 50;
        }
    }       

    return BMS_ERROR;
}


bmsStatus_t publishMarvelUniqueID(void)
{
    bmsStatus_t status = BMS_ERROR;

    const uint32_t *marvelUniqueId1 = (uint32_t *)(0x1FFF7590);  // BASE address
    const uint32_t *marvelUniqueId2 = (uint32_t *)(0x1FFF7594);  // BASE address + 0x04 offset
    const uint32_t *marvelUniqueId3 = (uint32_t *)(0x1FFF7598);  // BASE address + 0x08 offset

    uint8_t canDataFrame[8] = {'\0'};

    // Allocate memory for controllerUniqueId
    uint8_t *deviceUniqueId = (uint8_t *)malloc(12); // 12 bytes for 3 uint32_t values

    if (marvelUniqueId1 != NULL) 
    {
        deviceUniqueId[0]  = (*marvelUniqueId1 >> 0)  & 0xFF;
        deviceUniqueId[1]  = (*marvelUniqueId1 >> 8)  & 0xFF;
        deviceUniqueId[2]  = (*marvelUniqueId1 >> 16) & 0xFF;
        deviceUniqueId[3]  = (*marvelUniqueId1 >> 24) & 0xFF;
        deviceUniqueId[4]  = (*marvelUniqueId2 >> 0)  & 0xFF;
        deviceUniqueId[5]  = (*marvelUniqueId2 >> 8)  & 0xFF;
        deviceUniqueId[6]  = (*marvelUniqueId2 >> 16) & 0xFF;
        deviceUniqueId[7]  = (*marvelUniqueId2 >> 24) & 0xFF;
        deviceUniqueId[8]  = (*marvelUniqueId3 >> 0)  & 0xFF;
        deviceUniqueId[9]  = (*marvelUniqueId3 >> 8)  & 0xFF;
        deviceUniqueId[10] = (*marvelUniqueId3 >> 16) & 0xFF;
        deviceUniqueId[11] = (*marvelUniqueId3 >> 24) & 0xFF;
    }

    // publishing Hardware and Software versions
    canDataFrame[0] = MARVEL_DEVICE_ID;
	canDataFrame[1] = HW_MAJOR_VERSION;
	canDataFrame[2] = HW_MINOR_VERSION;
	canDataFrame[3] = HW_PATCH_VERSION;
	canDataFrame[4] = SW_MAJOR_VERSION;
	canDataFrame[5] = SW_MINOR_VERSION;
	canDataFrame[6] = SW_PATCH_VERSION;
	canDataFrame[7] = deviceUniqueId[0];

    status = sendCANDataOnQueue(CAN_TX_ID_026, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    // publishing Config versions and unique IDs
    canDataFrame[0] = MARVEL_DEVICE_ID;
	canDataFrame[1] = marvelConfigVersion[CONFIG_MAJOR];
	canDataFrame[2] = marvelConfigVersion[CONFIG_MINOR];
	canDataFrame[3] = marvelConfigVersion[CONFIG_PATCH];
	canDataFrame[4] = deviceUniqueId[1];
	canDataFrame[5] = deviceUniqueId[2];
	canDataFrame[6] = deviceUniqueId[3];
	canDataFrame[7] = deviceUniqueId[4];

    status = sendCANDataOnQueue(CAN_TX_ID_027, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    // publishing unique IDs
    canDataFrame[0] = MARVEL_DEVICE_ID;
	canDataFrame[1] = deviceUniqueId[5];
	canDataFrame[2] = deviceUniqueId[6];
	canDataFrame[3] = deviceUniqueId[7];
	canDataFrame[4] = deviceUniqueId[8];
	canDataFrame[5] = deviceUniqueId[9];
	canDataFrame[6] = deviceUniqueId[10];
	canDataFrame[7] = deviceUniqueId[11];

    status = sendCANDataOnQueue(CAN_TX_ID_028, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);
    
    free(deviceUniqueId);
    return status;
}

bmsStatus_t publishMarvelGitSha(void)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};
    uint32_t gitSHA = 0;

    canDataFrame[0] = MARVEL_DEVICE_ID;
	gitSHA = __builtin_bswap32(GIT_SHA);
	memcpy(&canDataFrame[1], &gitSHA, sizeof(gitSHA));

    status = sendCANDataOnQueue(CAN_TX_ID_029, canDataFrame, sizeof(canDataFrame));

    return status;
}

bmsStatus_t fetchSoCFromLookupTable(dataPipelineOutput_t *dpOutput)
{
    bmsStatus_t status = BMS_ERROR;

    uint8_t  interpolatedSoC   = 0;
    uint8_t  maxTemperature    = 0;
    uint16_t averageVoltage    = 0;
    uint32_t sum               = 0;

    if(SoCFromLookupTableFlag)
    {
        if(!dpOutput->voltageSense.voltageMin)
        {
            status = BMS_DATA_PROCESSING_FUNCTION_PARAMETER_ERROR;
            return status;
        }
        else
        {
            // taking average of cell voltages
            for(uint8_t numberOfCells = 0; numberOfCells <= defaultFlashConfigData[CELL_IN_SERIES_INDEX]; numberOfCells++)
            {
                sum += dpOutput->voltageSense.voltage[numberOfCells];
            }

            averageVoltage = sum / defaultFlashConfigData[CELL_IN_SERIES_INDEX];
            maxTemperature = dpOutput->temperatureSense[0].temperatureMax;              // group1 max temperature

            interpolatedSoC = socFromLookupTable(maxTemperature, averageVoltage); 

            // calculated installed capacity accounting for SoH won't get fetched
            ccOutputDataEEPROM.SOC_pct                = interpolatedSoC;
            ccOutputDataEEPROM.totalCapacityRemaining = (interpolatedSoC / 100) * INSTALLED_CAPACITY;

            // clearing the flag
            SoCFromLookupTableFlag = false;
        }
    }
    
    return BMS_OK;
}

#ifdef DEBUG_CAN

bmsStatus_t sendCoulombCountingDataOnCAN(coulombCountingInput_t *ccInput, coulombCountingOutput_t *ccOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    //ID 28
    // for(int8_t canDLC = CAN_ccOut_initialCapacity.messageDLC - 1; canDLC >= 0; canDLC--)
    //     canDataFrame[CAN_ccOut_initialCapacity.messageStartByte + canDLC] = ((int) (ccOutput->initialCapacity/CAN_ccOut_initialCapacity.messageScaling)  >> (8 * canDLC));
            
    // for(int8_t canDLC = CAN_ccOut_totalCapacityRemaining.messageDLC - 1; canDLC >= 0; canDLC--)
    //     canDataFrame[CAN_ccOut_totalCapacityRemaining.messageStartByte + canDLC] = ((int) (ccOutput->totalCapacityRemaining /CAN_ccOut_totalCapacityRemaining.messageScaling)  >> (8 * canDLC));
    
    // status = sendCANDataOnQueue(CAN_ccOut_totalCapacityRemaining.messageID, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);
    
	//ID 2
    for(int8_t canDLC = CAN_ccOut_MaxUsableCapacity_mAh.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_MaxUsableCapacity_mAh.messageStartByte + canDLC] = ((int) (ccOutput->MaxUsableCapacity_mAh /CAN_ccOut_MaxUsableCapacity_mAh.messageScaling)  >> (8 * canDLC));
    
    for(int8_t canDLC = CAN_ccOut_TotalCapacityExchange_mAh.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_TotalCapacityExchange_mAh.messageStartByte + canDLC] = ((int) (ccOutput->TotalCapacityExchange_mAh /CAN_ccOut_TotalCapacityExchange_mAh.messageScaling)  >> (8 * canDLC));
    

    status = sendCANDataOnQueue(CAN_ccOut_TotalCapacityExchange_mAh.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    //ID 3
    // for(int8_t canDLC = CAN_ccOut_totalCapacityDischarge.messageDLC - 1; canDLC >= 0; canDLC--)
    // 	canDataFrame[CAN_ccOut_totalCapacityDischarge.messageStartByte + canDLC] = ((int) (ccOutput->totalCapacityDischarge/CAN_ccOut_totalCapacityDischarge.messageScaling)  >> (8 * canDLC));

    // for(int8_t canDLC = CAN_ccOut_SOCPercentage.messageDLC - 1; canDLC >= 0; canDLC--)
    // 	canDataFrame[CAN_ccOut_SOCPercentage.messageStartByte + canDLC] = ((int) (ccOutput->SOC_pct /CAN_ccOut_SOCPercentage.messageScaling)  >> (8 * canDLC));

    // for(int8_t canDLC = CAN_ccOut_SOH_cpct.messageDLC - 1; canDLC >= 0; canDLC--)
    //         canDataFrame[CAN_ccOut_SOH_cpct.messageStartByte + canDLC] = ((int) (ccOutput->SOH_pct /CAN_ccOut_SOH_cpct.messageScaling )  >> (8 * canDLC));
    
    // for(int8_t canDLC = CAN_ccOut_coulombCountingState.messageDLC - 1; canDLC >= 0; canDLC--)
    //         canDataFrame[CAN_ccOut_coulombCountingState.messageStartByte + canDLC] = ((int) (ccOutput->coulombCountingState /CAN_ccOut_coulombCountingState.messageScaling)  >> (8 * canDLC));
    
    
    // status = sendCANDataOnQueue(CAN_ccOut_coulombCountingState.messageID , canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    return status;
}

bmsStatus_t sendContactorDataOnCAN(thresholdData_t *tData, contactorInput_t *cInput, contactorOutput_t *cOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

        for(int8_t canDLC = CAN_ccOut_PackEnergy.messageDLC - 1; canDLC >= 0; canDLC--)
        canDataFrame[CAN_ccOut_PackEnergy.messageStartByte + canDLC] = ((int) (ccOutputData.packEnergy/CAN_ccOut_PackEnergy.messageScaling)  >> (8 * canDLC));	
		
        for(int8_t canDLC = CAN_cIn_OV.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_OV.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.OV/CAN_cIn_OV.messageScaling)  >> (8 * canDLC));						
	
		for(int8_t canDLC = CAN_cIn_UV.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_UV.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.UV/CAN_cIn_UV.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_cIn_UV.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cIn_ThermalRunaway.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_ThermalRunaway.messageStartByte + canDLC] = ((int) (cInput->protectionOutputData.ThermalRunaway/CAN_cIn_ThermalRunaway.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_communicationFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_communicationFlag.messageStartByte + canDLC] = ((int) (cInput->communicationFlag/CAN_cIn_communicationFlag.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_shortCircuitFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_shortCircuitFlag.messageStartByte + canDLC] = ((int) (externalShortCircuitDetectionFlag/CAN_cIn_shortCircuitFlag.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cIn_permanentFailFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_permanentFailFlag.messageStartByte + canDLC] = ((int) (cInput->permanentFailFlag/CAN_cIn_permanentFailFlag.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_contactorErrorFlag.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorErrorFlag.messageStartByte + canDLC] = ((int) (cInput->contactorErrorFlag/CAN_cIn_contactorErrorFlag.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cIn_preChargeContactorCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_preChargeContactorCurrent.messageStartByte + canDLC] = ((int) (tData->preChargeContactorCurrent/CAN_cIn_preChargeContactorCurrent.messageScaling)  >> (8 * canDLC));
		
        canDataFrame[6]     =     cInput->protectionOutputData.TemperatureGradient;
        canDataFrame[7]     =     cInput->protectionOutputData.HighImbalanceFlag;

    status = sendCANDataOnQueue(CAN_cIn_MaxImbalance.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

		for(int8_t canDLC = CAN_cIn_NegContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_NegContactor_FB.messageStartByte + canDLC] = ((int) (cInput->negativeContactorFeedback/CAN_cIn_NegContactor_FB.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_PosContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_PosContactor_FB.messageStartByte + canDLC] = ((int) (cInput->positiveContactorFeedback/CAN_cIn_PosContactor_FB.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cIn_PreChargeContactor_FB.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_PreChargeContactor_FB.messageStartByte + canDLC] = ((int) (cInput->preChargeContactorFeedback/CAN_cIn_PreChargeContactor_FB.messageScaling)  >> (8 * canDLC));

		for(int8_t canDLC = CAN_cIn_contactorCommand.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorCommand.messageStartByte + canDLC] = ((int) (cInput->contactorCommand/CAN_cIn_contactorCommand.messageScaling)  >> (8 * canDLC));
				
		// for(int8_t canDLC = CAN_cIn_contactorResponseTime_msec.messageDLC - 1; canDLC >= 0; canDLC--)
		// 	canDataFrame[CAN_cIn_contactorResponseTime_msec.messageStartByte + canDLC] = ((int) (tData->contactorResponseTime_msec/CAN_cIn_contactorResponseTime_msec.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cIn_contactorCutoffTime.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cIn_contactorCutoffTime.messageStartByte + canDLC] = ((int) (tData->contactorCutoffTime/CAN_cIn_contactorCutoffTime.messageScaling)  >> (8 * canDLC));
		
    
    status = sendCANDataOnQueue(CAN_cIn_contactorCutoffTime.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    

    	for(int8_t canDLC = CAN_cOut_NegContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_NegContactorState.messageStartByte + canDLC] = ((int) (cOutput->negativeContactorState/CAN_cOut_NegContactorState.messageScaling)  >> (8 * canDLC));
				
		for(int8_t canDLC = CAN_cOut_PosContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_PosContactorState.messageStartByte + canDLC] = ((int) (cOutput->positiveContactorState/CAN_cOut_PosContactorState.messageScaling)  >> (8 * canDLC));
				   
		for(int8_t canDLC = CAN_cOut_PreChargeContactorState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cOut_PreChargeContactorState.messageStartByte + canDLC] = ((int) (cOutput->preChargeContactorState/CAN_cOut_PreChargeContactorState.messageScaling)  >> (8 * canDLC));
		
        canDataFrame[3] = cOutput->contactorWeldWarning;
        canDataFrame[4] = cOutput->permanentFaiil;

        canDataFrame[5] = cInput->protectionOutputData.ShortCircuit;
        canDataFrame[6] = cInput->protectionOutputData.SuddenVoltageDrop;
        canDataFrame[7] = cOutput->contactorsState;

    status = sendCANDataOnQueue(CAN_cOut_PreChargeContactorState.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);  
    return status;
}

bmsStatus_t sendCellBalancingDataOnCAN(thresholdData_t *tData, cellBalancingInput_t *cbInput, cellBalancingOutput_t *cbOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    // 	for(int8_t canDLC = CAN_cbIn_balancingMinThreshold.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_cbIn_balancingMinThreshold.messageStartByte + canDLC] = ((int) (tData->balancingMinThreshold/CAN_cbIn_balancingMinThreshold.messageScaling)  >> (8 * canDLC));
				
    // 	for(int8_t canDLC = CAN_cbIn_balancingMaxAllowedImbalance.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_cbIn_balancingMaxAllowedImbalance.messageStartByte + canDLC] = ((int) (tData->balancingMaxAllowedImbalance/CAN_cbIn_balancingMaxAllowedImbalance.messageScaling)  >> (8 * canDLC));
				   
    // 	// for(int8_t canDLC = CAN_cbIn_balancingOVProtectionThreshold.messageDLC - 1; canDLC >= 0; canDLC--)
	// 	// 	canDataFrame[CAN_cbIn_balancingOVProtectionThreshold.messageStartByte + canDLC] = ((int) (tData->balancingOVProtectionThreshold/CAN_cbIn_balancingOVProtectionThreshold.messageScaling)  >> (8 * canDLC));
        
    //     for(int8_t canDLC = CAN_cbIn_balancingStateRestPeroid.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_cbIn_balancingStateRestPeroid.messageStartByte + canDLC] = ((int) (tData->balancingStateRestPeroid/CAN_cbIn_balancingStateRestPeroid.messageScaling)  >> (8 * canDLC));
	
    // status = sendCANDataOnQueue(CAN_cbIn_balancingStateRestPeroid.messageID, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);   

        for(int8_t canDLC = CAN_cbIn_cellsInSeries.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbIn_cellsInSeries.messageStartByte + canDLC] = ((int) (20/CAN_cbIn_cellsInSeries.messageScaling)  >> (8 * canDLC));
        
        for(int8_t canDLC = CAN_cbOut_balancingState.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_cbOut_balancingState.messageStartByte + canDLC] = ((int) (cbOutput->balancingState/CAN_cbOut_balancingState.messageScaling)  >> (8 * canDLC));
				
        for(int i = 0; i < 6; i++)
            canDataFrame[i+2] = (int) cbOutput->balancingCommand[i];


    status = sendCANDataOnQueue(CAN_cbOut_balancingState.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //     for(int i = 6; i < 14; i++)
    //         canDataFrame[i-6] = (int) cbOutput->balancingCommand[i];

    // status = sendCANDataOnQueue(CAN_DEBUG_ID_010, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

        for(int i = 14; i < CELL_IN_SERIES; i++)
            canDataFrame[i-14] = (int) cbOutput->balancingCommand[i];

    // status = sendCANDataOnQueue(CAN_DEBUG_ID_011, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);
    return status;
}

bmsStatus_t sendBMSDataOnCAN(bmsMainData_t *mdata, dataPipelineOutput_t *dpOutput, dataPipelineInput_t *dpInput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

    //ID 27
    //2949 timebase
    // canDataFrame[0] =  (int)(mdata->ccmData.timeBase/0.1);
    // canDataFrame[1] = ((int)(mdata->ccmData.timeBase/0.1) >> 8);
    // canDataFrame[2] = ((int)(mdata->ccmData.timeBase/0.1) >> 16);
    // canDataFrame[3] = ((int)(mdata->ccmData.timeBase/0.1) >> 24);

    // canDataFrame[4] =  (int)(mdata->ccmData.statusReg);
    // canDataFrame[5] =  (int)(mdata->ccmData.opctrlReg);
    // canDataFrame[6] =  (int)(mdata->ccmData.faultsReg);
    // canDataFrame[7] =  (int)(mdata->ccmData.tbReg);

    // status = sendCANDataOnQueue(CAN_DEBUG_ID_027, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    //p input temperatures 0 to 7
        
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[0].temperature[0]);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[0].temperature[1]);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[0].temperature[2]);
        canDataFrame[3] =  (int) floor(dpOutput->temperatureSense[0].temperature[3]);

        canDataFrame[4] =  (int) floor(dpOutput->temperatureSense[0].temperature[4]);
        canDataFrame[5] =  (int) floor(dpOutput->temperatureSense[0].temperature[5]);
        
        //CMU Temp[3] and CMU Temp[4] data send
        canDataFrame[6] =  (int) floor(dpOutput->temperatureSense[0].temperature[6]);
        canDataFrame[7] =  (int) floor(dpInput->shuntTemp);

    status = sendCANDataOnQueue(CAN_DEBUG_ID_018, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

    //p input temperatures 7 to 13
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[1].temperature[0]);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[1].temperature[1]);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[1].temperature[2]);
        canDataFrame[3] =  (int) floor(dpOutput->temperatureSense[1].temperature[3]);
        canDataFrame[4] =  (int) floor(dpOutput->temperatureSense[1].temperature[4]);
        canDataFrame[5] =  (int) floor(dpOutput->temperatureSense[1].temperature[5]);
        canDataFrame[6] =  (int) floor(dpOutput->temperatureSense[1].temperature[6]);


    status = sendCANDataOnQueue(CAN_DEBUG_ID_019, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //group1 max and min temperatures
        canDataFrame[0] =  (int) floor(dpOutput->temperatureSense[0].temperatureMax);
        canDataFrame[1] =  (int) floor(dpOutput->temperatureSense[0].temperatureMin);
        canDataFrame[2] =  (int) floor(dpOutput->temperatureSense[0].temperatureDelta);
    
    //group2 max and min temperatures
        canDataFrame[3] = (int) floor(dpOutput->temperatureSense[1].temperatureMax);
        canDataFrame[4] = (int) floor(dpOutput->temperatureSense[1].temperatureMin);
        canDataFrame[5] = (int) floor(dpOutput->temperatureSense[1].temperatureDelta);
        canDataFrame[6] = dpOutput->isChargeEn;
    
    status = sendCANDataOnQueue(CAN_DEBUG_ID_020, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    status = sendCANDataOnQueue(CAN_TX_ID_013, errorCodes1Dataframe, sizeof(errorCodes1Dataframe));
    status = sendCANDataOnQueue(CAN_TX_ID_014, errorCodes2Dataframe, sizeof(errorCodes2Dataframe));

    //dynamic current data on CAN

        // canDataFrame[0] =  (uint8_t)mdata.dynamicLimitIn & 0xFF;
        // canDataFrame[1] =  (uint8_t)mdata.dynamicLimitIn>>8 &0xFF;
        // canDataFrame[3] =  (uint8_t)mdata.dynamicLimitOut & 0xFF;
        // canDataFrame[4] =  (uint8_t)mdata.dynamicLimitOut >>8 &0XFF;
   

    // for(int8_t canDLC = CAN_dynamicLimitIN.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_dynamicLimitIN.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitIn/CAN_dynamicLimitIN.messageScaling) >> (8 * canDLC));

    // for(int8_t canDLC = CAN_dynamicLimitOUT.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_dynamicLimitOUT.messageStartByte + canDLC] = ((int) (mdata->dynamicLimitOut/CAN_dynamicLimitOUT.messageScaling) >> (8 * canDLC));

    // for(int8_t canDLC = CAN_FirmwareVersion.messageDLC -1; canDLC >= 0; canDLC--)        
    //     canDataFrame[CAN_FirmwareVersion.messageStartByte + canDLC] = (MARVEL_FW_VER >> (8 * canDLC));

    // canDataFrame[6] = MARVEL_CONFIG;

    // status = sendCANDataOnQueue(CAN_TX_ID_011, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    // //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    return status;
}

bmsStatus_t sendProtectionDataOnCAN(thresholdData_t *tData, protectionInput_t *pInput, protectionOutput_t *pOutput, coulombCountingOutput_t *ccOutput)
{
    bmsStatus_t status = BMS_ERROR;
    uint8_t canDataFrame[8] = {'\0'};

        for(int8_t canDLC = CAN_pIn_voltageMax.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageMax.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMax/CAN_pIn_voltageMax.messageScaling)  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_voltageMin.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageMin.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageMin/CAN_pIn_voltageMin.messageScaling)  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_voltageDelta.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_voltageDelta.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.voltageSense.voltageDelta/CAN_pIn_voltageDelta.messageScaling)  >> (8 * canDLC));
	
    status = sendCANDataOnQueue(CAN_pIn_voltageDelta.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

	// 	for(int8_t canDLC = CAN_pIn_current.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_pIn_current.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.current/CAN_pIn_current.messageScaling)  >> (8 * canDLC));
		
    //     for(int8_t canDLC = CAN_pIn_OCC_WarningCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_pIn_OCC_WarningCurrent.messageStartByte + canDLC] = ((int) (tData->OCC_WarningCurrent/CAN_pIn_OCC_WarningCurrent.messageScaling)  >> (8 * canDLC));
				
    //     for(int8_t canDLC = CAN_pIn_OCC_ErrorCurrent.messageDLC - 1; canDLC >= 0; canDLC--)
	// 		canDataFrame[CAN_pIn_OCC_ErrorCurrent.messageStartByte + canDLC] = ((int) (tData->OCC_ErrorCurrent/CAN_pIn_OCC_ErrorCurrent.messageScaling)  >> (8 * canDLC));

    // status = sendCANDataOnQueue(CAN_pIn_OCC_ErrorCurrent.messageID, canDataFrame, sizeof(canDataFrame));
    // CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

        //ID 0x33D , i^2 fuse values
        for(int8_t canDLC = CAN_pIn_eFuseChargingLimit_A2s.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_eFuseChargingLimit_A2s.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.i2t_ChargingCurrent_A2sec/(CAN_pIn_eFuseChargingLimit_A2s.messageScaling))  >> (8 * canDLC));
				
        for(int8_t canDLC = CAN_pIn_eFuseDischargingLimit_A2s.messageDLC - 1; canDLC >= 0; canDLC--)
			canDataFrame[CAN_pIn_eFuseDischargingLimit_A2s.messageStartByte + canDLC] = ((int) (pInput->dataPipelineBus.i2t_DisChargingCurrent_A2sec/(CAN_pIn_eFuseDischargingLimit_A2s.messageScaling))  >> (8 * canDLC));

        // canDataFrame[4] = contChargingLimit_A & 0xFF ;
        // canDataFrame[5] = (contChargingLimit_A >> 8) & 0xFF;

        // canDataFrame[6] = contDischargingLimit_A & 0xFF;
        // canDataFrame[7] = (contDischargingLimit_A >> 8) & 0xFF;

    status = sendCANDataOnQueue(CAN_pIn_eFuseChargingLimit_A2s.messageID, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY);

        //ID 0x33E, temp states
        canDataFrame[0] =  pOutput->protectionOutputData.TempOverallState;
        canDataFrame[1] =  pOutput->protectionOutputData.TempState1;
        canDataFrame[2] =  pOutput->protectionOutputData.TempState2;
        canDataFrame[3] =  pOutput->protectionOutputData.FlagGroup1;
        canDataFrame[4] =  pOutput->protectionOutputData.FlagGroup2;
        canDataFrame[5] =  pOutput->protectionOutputData.eFuseDischargeFlag; 
        canDataFrame[6] =  pOutput->protectionOutputData.ErrorDetect;
        canDataFrame[7] =  pOutput->protectionOutputData.eFuseChargeFlag;

    status = sendCANDataOnQueue(CAN_DEBUG_ID_031, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

	for(int8_t canDLC = CAN_pIn_calculatedi2tFuseValue.messageDLC -1; canDLC >= 0; canDLC--)        
		canDataFrame[CAN_pIn_calculatedi2tFuseValue.messageStartByte + canDLC] = ((int) (pOutput->protectionOutputData.i2t_Calculated_A2sec/CAN_pIn_calculatedi2tFuseValue.messageScaling) >> (8 * canDLC));

    // canDataFrame[6] = chargerTypeFromStark;
    // canDataFrame[7] = pInput->dataPipelineBus.LoopTimer_SOC_msec;

    for(int8_t canDLC = CAN_ccOut_cycleCount.messageDLC -1; canDLC >= 0; canDLC--)        
		canDataFrame[CAN_ccOut_cycleCount.messageStartByte + canDLC] = ((int) (ccOutput->CycleCount/CAN_ccOut_cycleCount.messageScaling) >> (8 * canDLC));


    status = sendCANDataOnQueue(CAN_DEBUG_ID_032, canDataFrame, sizeof(canDataFrame));
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

#ifdef CELL_VOLTAGES
    //id 22 
    canDataFrame[0] =  (int)(pInput->OCD_WarningCurrent);
    canDataFrame[1] = ((int)(pInput->OCD_WarningCurrent) >> 8);

    canDataFrame[2] =  (int)(pInput->OCD_ErrorCurrent);
    canDataFrame[3] = ((int)(pInput->OCD_ErrorCurrent) >> 8);

    canDataFrame[4] =  (int)(pInput->OTC_WarningTimeout);
    canDataFrame[5] = ((int)(pInput->OTC_WarningTimeout) >> 8);

    canDataFrame[6] =  (int)(pInput->OTC_ErrorTimeout);
    canDataFrame[7] = ((int)(pInput->OTC_ErrorTimeout) >> 8);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_022, canDataFrame);
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 
    
    //id 23 


    canDataFrame[0] =  (int)(pInput->OTC_RecoveryTimeout);
    canDataFrame[1] = ((int)(pInput->OTC_RecoveryTimeout) >> 8);

    canDataFrame[2] =  (int)(pInput->OTC_WarningTemperature);
    canDataFrame[3] =  (int)(pInput->OTC_ErrorTemperature);
    canDataFrame[4] =  (int)(pInput->OTC_RecoveryTemperature);
    
    canDataFrame[5] =  (int)(pInput->OTD_WarningTemperature);
    canDataFrame[6] =  (int)(pInput->OTD_ErrorTemperature);
    canDataFrame[7] =  (int)(pInput->OTD_RecoveryTemperature);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_023, canDataFrame);
    CHECK_STATUS(status);
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    //id 24 

    canDataFrame[0] =  (int)(pInput->UTC_WarningTemperature);
    canDataFrame[1] =  (int)(pInput->UTC_ErrorTemperature);
    canDataFrame[2] =  (int)(pInput->UTC_RecoveryTemperature);

    canDataFrame[3] =  (int)(pInput->UTD_WarningTemperature);
    canDataFrame[4] =  (int)(pInput->UTD_ErrorTemperature);
    canDataFrame[5] =  (int)(pInput->UTD_RecoveryTemperature);

    canDataFrame[6] =  (int)(pInput->OV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[7] = ((int)(pInput->OV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_024, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 


    //id 25 
    canDataFrame[0] =  (int)(pInput->OV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[1] = ((int)(pInput->OV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[2] =  (int)(pInput->OV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[3] = ((int)(pInput->OV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[4] =  (int)(pInput->UV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[5] = ((int)(pInput->UV_RecoveryVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);

    canDataFrame[6] =  (int)(pInput->UV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[7] = ((int)(pInput->UV_ErrorVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);


    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_025, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 

    canDataFrame[0] =  (int)(pInput->UV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING);
    canDataFrame[1] = ((int)(pInput->UV_WarningVoltage/CMU_CELL_VOLTAGE_MESSAGE_SCALING) >> 8);


    sendBatteryDataOnCAN(STANDARD_CAN_ID, CAN_DEBUG_ID_026, canDataFrame);
    CHECK_STATUS(status);
    memset(canDataFrame, 0, sizeof(canDataFrame));
    //delayMilliSeconds(CAN_MESSAGE_DELAY); 
#endif
    return status;
}

#endif

/* End of File ---------------------------------------------------------------*/