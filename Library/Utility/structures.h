/**
 * @file structures.h
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Header Guard --------------------------------------------------------------*/
#ifndef STRUCTURES_H_
#define STRUCTURES_H_

/* Standard C includes -------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
    SOC_SECTION = 0,
    CRITICAL_FLAGS,
    COMMON_DATA,
    MERLYN_HANDLER_SECTION
}eepromSection_t;

// enum vehicleState
// {
//     fastCharge = 0, 
//     slowCharge, 
//     isChargerConnected,
//     ignition, 
//     idle,
//     permanentFail, 
//     serviceMode,
//     transmissionFail,
//     VCUPermanentFail,
//     balanceEn_atRest
// };
// Define a struct to hold the CAN message information
typedef struct canQueueData
{
    uint8_t data[8];
    uint32_t identifier;
} canQueueData_t;

typedef struct {
  uint32_T SOC_forCVCharge_cpct;
  uint32_T BatteryRevivalVoltage_mV;
  uint32_T NoCharge_K;
//   uint32_T BatteryRevival_K;
//   uint32_T CC_K;
//   uint32_T CC_Balancing_K;
//   uint32_T CV_K;
  float slowCharging_K;
  float fastCharging_K;
  uint32_T targetVoltageSlowCharging;
  uint32_T targetVoltageFastCharging;
  uint32_T CVTransitionVoltage_mV;
  uint32_T TransitionTimeout_msec;
  uint32_T SlowChargersCurrentLimit_A;
  uint32_T FastChargersCurrentLimit_A;
  }ChargeAlgorithm_t;

//temperature
typedef struct temperature
{
    int32_T OTC_WarningTemperature;
    int32_T OTC_ErrorTemperature;
    int32_T OTC_RecoveryTemperature;
    int32_T OTC_NoErrorTimeout_msec;
    int32_T OTC_WarningTimeout;
    int32_T OTC_ErrorTimeout;
    int32_T OTC_RecoveryTimeout;
    
    int32_T OTD_WarningTemperature;
    int32_T OTD_ErrorTemperature;
    int32_T OTD_RecoveryTemperature;
    int32_T OTD_NoErrorTimeout_msec;
    int32_T OTD_WarningTimeout;
    int32_T OTD_ErrorTimeout;
    int32_T OTD_RecoveryTimeout;

    int32_T UTC_WarningTemperature;
    int32_T UTC_ErrorTemperature;
    int32_T UTC_RecoveryTemperature;
    int32_T UTC_NoErrorTimeout_msec;
    int32_T UTC_WarningTimeout;
    int32_T UTC_ErrorTimeout;
    int32_T UTC_RecoveryTimeout;

    int32_T UTD_WarningTemperature;
    int32_T UTD_ErrorTemperature;
    int32_T UTD_RecoveryTemperature;
    int32_T UTD_NoErrorTimeout_msec;
    int32_T UTD_WarningTimeout;
    int32_T UTD_ErrorTimeout;
    int32_T UTD_RecoveryTimeout;
}temperatureData_t;

//threshold
typedef struct thresholdData
{
    //protection
    int32_T MaxOperatingTemp_C;
    int32_T MaxAllowedTempChange_C;
    int32_T MaxAllowedTempGradientGroup1_C;
    int32_T MaxAllowedTempGradientGroup2_C;
    int32_T Protections_AverageTimeforProtection_msec;
 
    int32_T chargeDetectionThreshold_mA;
    int32_T NormalModeTimeout_msec;
    int32_T PowerSaverModeTimeout_msec;
    int32_T FaultsModeTimeout_msec;
    int32_T ServiceModeTimeout_msec;

    int32_T i2tTimeout_msec;
    int32_T dynamicContTimeout_msec;
    int32_T dynamicCurrentThresholdsGain;
    int32_T TempGroupTimeout;
    uint8_T PermanentFailCounts_eFuse;

    //over current protection
    int32_T i2t_CalculationTimeout_msec;
    int32_T CurrentChangeTimeout_msec;
    int32_T eFuseMeltingTimeout_msec;
    int32_T overCurrentProtectionTimeout;
    int32_T overCurrentWarningToErrorTimeout_msec;
    int32_T overCurrentRecoveryCurrentProtectionTimeout_msec;

    // BMS state machine
    int32_T BMS_ReadyToActivating_Timeout_msec;
    int32_T BMS_ActivatingToActive_Timeout_msec;
    int32_T BMS_ActivatingToError_Timeout_msec;
    int32_T BMS_ActiveToError_Timeout_msec;
    int32_T BMS_ActiveToReady_Timeout_msec;
    int32_T BMS_ErrorToActivating_Timeout_msec;

    int32_T OCC_WarningCurrent;
    int32_T OCC_ErrorCurrent;
    int32_T RecoveryProtections_OCC_Warning_mA;
    int32_T RecoveryProtections_OCC_Error_mA;
    int32_T OCC_NoErrorTimeout_msec; //
    int32_T OCC_WarningTimeout;
    int32_T OCC_ErrorTimeout;
    int32_T OCC_RecoveryTimeout_msec; //

    int32_T OCD_WarningCurrent;
    int32_T OCD_ErrorCurrent;
    int32_T RecoveryProtections_OCD_Warning_mA;
    int32_T RecoveryProtections_OCD_Error_mA;
    int32_T OCD_NoErrorTimeout_msec; //
    int32_T OCD_WarningTimeout;
    int32_T OCD_ErrorTimeout;
    int32_T OCD_RecoveryTimeout_msec;

    int32_T PreChargeRecoveryTimeout_msec;

    temperatureData_t temperatureData[2];

    int32_T slowCharging_OV_WarningVoltage;
    int32_T slowCharging_OV_ErrorVoltage;
    int32_T slowCharging_OV_RecoveryVoltage;

    int32_T fastCharging_OV_WarningVoltage;
    int32_T fastCharging_OV_ErrorVoltage;
    int32_T fastCharging_OV_RecoveryVoltage;


    int32_T OV_NoErrorTimeout_msec;
    int32_T OV_WarningTimeout;
    int32_T OV_ErrorTimeout;
    int32_T OV_RecoveryTimeout;

    int32_T HighImbalanceError_mV;

    int32_T UV_WarningVoltage;
    int32_T UV_ErrorVoltage;
    int32_T UV_RecoveryVoltage;
    int32_T UV_NoErrorTimeout_msec;
    int32_T UV_WarningTimeout;
    int32_T UV_ErrorTimeout;
    int32_T UV_RecoveryTimeout;

    int32_T MinOpearatingVolt_mV;
    int32_T ShortCircuitCurrent_mA;
    int32_T SuddenVoltageDrop_mV_by_time;

    uint16_T ThermalRunawayWarningTimeout_msec;
    uint16_T ThermalRunawayErrorTimeout_msec;
    uint16_T TemperatureGradientTimeout_msec;
    uint16_T HighImbalanceFlagTimeout_msec;
    uint16_T ShortCircuitDetect_msec;
    uint16_T SuddenVoltageDropTimeout_msec;

    //balancing
    int32_T balancingMinThreshold;  //voltage at which balancing should start
    int32_T balancingStateRestPeroid;
    int32_T balancingMaxAllowedImbalance;
    // int32_T balancingOVProtectionThreshold;
    int32_T balancing_SlowCh_OVThr_mV;
    int32_T balancing_FastCh_OVThr_mV;

    int32_T balancingPeriod;

    int32_T balancing_DefaultBalancingTimeOn_msec;
    int8_T  balancingCurrentDerateActivationTemp_C;
    int32_T balancingCurrentDerateCheckTime_msec;

    int32_T ExternalShortCheckTime_msec;
    int8_T  ExternalShortLimit;
    int8_T  ContWeldCheckLimit;

    //contactor
    int32_T contactorCutoffTime;
    int32_T preChargeTimeout_msec;
    int32_T preChargeRetryTimeout_msec;
    int32_T preChargeContactorCurrent; //ContCurrent_mA
    int32_T PreChargeVoltageDelta_mV;
    uint8_T preChargeRetryLimit;
    int32_T ContWeldCheckCurrent_mA;
    uint8_T ContactorWarningCounterLimit;
    int32_T ContactorWeldTimeout_msec;
    int32_T OpenToPreChargeStateTime_msec;
    int32_T Min_PreChargeTime_msec;
    int32_T Max_PreChargeTime_msec;
    int32_T ContOpenCurrent_mA;
    int32_T FlagDebounceTime_msec;
    int32_T Min_ClosedToOpeningTime_msec;
    int32_T Max_ClosedToOpeningTime_msec;

    uint8_T  AdditionalProtectionEn;
    uint8_T  HighImbalanceFlagEn;

    int32_T BMS_Timeout_msec;
    int32_T BMS_stateMachineTimeout;
    int32_T ContactorsCheck_Timeout_msec;
    int32_T VCU_Timeout_msec;
    int32_T MosfetsPreChargeCurrentLimit_mA;

    ChargeAlgorithm_t ChargeAlgorithm;
}thresholdData_t;


//temperature sense
typedef struct {
    int32_T temperature[7];
    int32_T temperatureMin;
    int32_T temperatureMax;
    int32_T temperatureDelta;
} temperatureSense_t;

//Voltage Sense
typedef struct {
    int32_T voltage[CELL_IN_SERIES];
    int32_T voltageMin;
    int32_T voltageMax;
    int32_T voltageDelta;
} voltageSense_t;

//temperature state
typedef struct {
    uint8_T TempOverallState;
    uint8_T TempGroup1;
    uint8_T TempGroup2;

    uint8_T ErrorTypeGroup1;
    uint8_T ErrorTypeGroup2;

} temperatureState_t;

typedef struct {
    int8_T SlowCharge;
    int8_T FastCharge;
    int8_T isChargerConnected;
    int8_T TransmissionFail;
    int8_T VCUPermanentFail;
    int8_T ServiceMode;
    int8_T BalanceEn_atRest;
    int8_T EmergencyDisconnectEn;
    uint32_T SlowChargersCurrentLimit_A;
    uint32_T FastChargersCurrentLimit_A;
    } vcuData_t;

typedef struct DynamicLimitsTable {
    Table2d ContinuousChargingTable;
    Table2d ContChargingArcReactorTable;
    Table2d ContinuousDischargingTable;
    Table2d instantaneousChargingTableData;
    Table2d instantaneousDischargingTableData;
    Table2d eFuseChargingTable;
    Table2d eFuseDishargingTable;
}DynamicLimitsTable_t;

//data pipeline
typedef struct dataPipelineInput_t
{
    int16_T temperature1_C[7];
    int16_T temperature2_C[7];
    int32_T voltage[20];
    int32_T current;
    thresholdData_t thresholdDataBus;
    int32_T soc;
    int32_T capacityRemaining;
    uint8_T isDynamicCurrentEn;
    uint8_T nCells_series;
    uint8_T nCells_parallel;
    uint8_T TemperatureSelectionforDynamicC;
    uint8_T nCelltemperatureGroups;
    int8_T SlowCharge;
    int8_T FastCharge;
    int8_T isChargerConnected;
    int8_T TransmissionFail;
    int8_T VCUPermanentFail;
    int8_T ServiceMode;
    int8_T BalanceEn_atRest;
    uint32_T LoopTimer_SOC_msec;
    int8_T EmergencyDisconnectEn;
    uint32_T FastChargersCurrentLimit_A;
    uint32_T SlowChargersCurrentLimit_A;
    int32_T shuntTemp;
}dataPipelineInput_t;

typedef struct dataPipelineOutput
{
    int32_T current;
    //voltage sense bus
    voltageSense_t voltageSense;
    int32_T maxTemperature_C;
    int32_T deltaTemperature_C;
    int32_T minTemperature_C;
    //TemperatureSense
    int32_T DeltaTemperatureGroup1_C;
    int32_T DeltaTemperatureGroup2_C;
    temperatureSense_t temperatureSense[2];
    uint8_T isChargeEn;
    uint8_T isDynamicCurrentEn;
    uint8_T nCells_series;
    uint8_T nCells_parallel;

    int32_T DCL_ChargingCurrent_A;
    int32_T DCL_DisChargingCurrent_A;
    int32_T arcReactorDCLI_A;
    int32_T i2t_ChargingCurrent_A2sec;
    int32_T i2t_DisChargingCurrent_A2sec;
    uint32_T LoopTimer_SOC_msec;
    boolean_T preChargeLogic;

    vcuData_t VCU;
}dataPipelineOutput_t;


//cell balancing
typedef struct cellBalancingInput
{
    int32_T NumberOfMosfets;
    int32_T BalancingTempGroup;
    float StLineEq_varA;
    float StLineEq_varB;
    thresholdData_t         thresholdDataBus;
    dataPipelineOutput_t    dataPipelineBus;
}cellBalancingInput_t;

typedef struct cellBalancingOutput
{
    uint8_T balancingCommand[CELL_IN_SERIES];
    int32_T volatgeImbalance;
    uint8_T BalanceEn;
    uint8_T balancingState;
}cellBalancingOutput_t;

//protection
typedef struct protectionOutputData
{
    uint8_T ThermalRunaway;
    uint8_T TemperatureGradient;
    uint8_T HighImbalanceFlag;
    uint8_T ShortCircuit;
    uint8_T SuddenVoltageDrop;
    uint8_T OCC;         //over current during charge
    uint8_T OCD;         //over current during discharge
    uint8_T OV;          //over voltage
    uint8_T UV;          //under voltage
    float   i2t_Calculated_A2sec;
    uint8_T eFuseChargeFlag;
    uint8_T eFuseDischargeFlag;
    uint8_T overCurrentDischargeFlag;
    uint8_T overCurrentChargeFlag;
    uint8_T TempState1;
    uint8_T TempState2;
    uint8_T FlagGroup1;
    uint8_T FlagGroup2;
    uint8_T TempOverallState;
    uint8_T ErrorDetect;
}protectionOutputData_t;

typedef struct protectionInput
{
    thresholdData_t         thresholdDataBus;
    dataPipelineOutput_t    dataPipelineBus;
}protectionInput_t;

typedef struct protectionOutput
{
    protectionOutputData_t protectionOutputData;
}protectionOutput_t;

//contactor
typedef struct contactorInput
{
    //data pipeline
    thresholdData_t         thresholdDataBus;
    uint8_T                 PermanentFailRecoveryTrigger;
    protectionOutputData_t  protectionOutputData;
    dataPipelineOutput_t    dataPipelineBus;

    uint8_T communicationFlag;
    uint8_T contactorCommand;
    uint8_T shortCircuitFlag;
    uint8_T permanentFailFlag;
    uint8_T contactorErrorFlag;
    uint32_T TerminalVoltage_mV;

    uint8_T positiveContactorFeedback;
    uint8_T preChargeContactorFeedback;
    uint8_T negativeContactorFeedback;

    uint16_T PreCharge_LoopTime_msec;
}contactorInput_t;

typedef struct contactorOutput
{
    uint8_T negativeContactorState;
    uint8_T preChargeContactorState;
    uint8_T positiveContactorState;
    uint8_T contactorWeldWarning;
    uint8_T permanentFaiil;
    uint8_T contactorsState;
    uint8_T ExternalShortCheck;
    uint32_T Elapsed_TimeToOpen_msec;
}contactorOutput_t;

#ifdef EKF
//ccEKF
typedef struct ccEKFInput
{
    float current;
    float time_s;
    float vehicleRestTime;
    float vehicleRestCurrent;
    float protectionOutputOVThreshold;
    float protectionOutputUVThreshold;
    float maxPackCapacity;   //Max_CellCapacity_Ah
    float minPackCapacity;
    float avgCellVoltage;
}ccEKFInput_t;

typedef struct ccEKFOutput
{
    float initialCapacity;
    float totalCapacityRemaining;
    float totalCapacityDischarge;
    float socCC;
    float coulombCountingState;

    float capacityEKF;
    float voltageEKF;
    float terminalVoltage;
    float socEKF;
}ccEKFOutput_t;

#else

//coulomb counting

typedef struct coulombCountingInputBus {
    int32_t InstalledCapacity_mAh;
    int32_t EquivalentCycleCapacity_mAh;
    int32_t SensorOffset_mA;
    int8_t SensorAccuracy_pct;
    int32_t BleedingCurrent_mA;
    int32_t CCTimeout_msec;
    int32_t DebouncingTimeout_msec;

    int32_t FastChargingMinVoltageLimit_mV;
    int32_t FastChargingMaxVoltageLimit_mV;
    int32_t FastChargingMaxCurrentLimit_mA;
    int32_t FastChargingMinCurrentLimit_mA;
    int32_t FastChargingMaxAllowedImb_mV;

    int32_t SlowChargingMinVoltageLimit_mV;
    int32_t SlowChargingMaxVoltageLimit_mV;
    int32_t SlowChargingMaxCurrentLimit_mA;
    int32_t SlowChargingMinCurrentLimit_mA;
    int32_t SlowChargingMaxAllowedImb_mV;

    int32_t CapacityDeltaCheck_mAh;
    int8_t InitialCapacity_Guess;
    uint32_T SOH_Vs_Capacity_Gain;
    uint32_T SOH_Vs_Capacity_Offset;
    uint8_t IntegratorLoopTime;
    uint8_t LoopTimeSelector;
    uint8_t SOHCalibrationOn;
    uint16_t SOHCalibrationTimeout_msec;
} coulombCountingInputBus;

typedef struct coulombCountingInput
{
    int32_t current_mA;
    int32_t voltage_mV;
    coulombCountingInputBus cc_Inputs;
    dataPipelineOutput_t dataPipelineBus;
    cellBalancingOutput_t CellBalancingOutput;
    float LoopTimer_SOC_msec;
    uint8_T EEPROM_FirstCycle;
    uint8_t currentBased;
    uint8_t fastChargeEnabled;
}coulombCountingInput_t;

typedef struct coulombCountingOutput
{
    int32_T initialCapacity;
    int32_T totalCapacityRemaining;
    int32_T totalCapacityDischarge;
    uint32_T SOC_pct;
    uint32_T SOC_cpct;
    uint32_T coulombCountingState;
    uint32_T MaxUsableCapacity_mAh;
    uint32_T TotalCapacityExchange_mAh;
    uint32_T SOH_pct;
    uint32_T CycleCount;
    uint32_T SOC_FromFlash;
    uint32_T packEnergy;
    
}coulombCountingOutput_t;

#endif


typedef struct stateMachineInput
{
    uint8_T HighLevelStateMode;
}stateMachineInput_t;

typedef struct stateMachineOutput
{
    uint8_T HighLevelStateMode;
    BMSState bmsState;
}stateMachineOutput_t;

typedef enum {
  noCharge = 0,                        /* Default value */
  slowCharge,
  fastCharge
}chargerState;



typedef struct chargingOutput
{
    int32_T ChargingCurrentReq_mA;       /* '<Root>/ChargingCurrentReq_mA' */
    chargerState ChargingState; 
    // uint8_t ChargingState; 
}chargingOutputData_t;



typedef enum
{
    CHARGING_STATE = 0,
    BALANCING_STATE
}latchingRoutineState;

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Inline functions ----------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


#endif
/* End of File ---------------------------------------------------------------*/

