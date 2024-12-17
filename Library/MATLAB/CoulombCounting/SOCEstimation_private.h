/*
 * SOCEstimation_types.h
 *
 * Code generation for model "SOCEstimation".
 *
 * Model version              : 4.1409
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Sat Jul 20 16:10:28 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_SOCEstimation_types_h_
#define RTW_HEADER_SOCEstimation_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_VoltageSenseBus_
#define DEFINED_TYPEDEF_FOR_VoltageSenseBus_

typedef struct {
  int32_T Voltages_mV[20];
  int32_T mV_min;
  int32_T mV_max;
  int32_T VoltageDelta_mV;
} VoltageSenseBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TemperatureSenseBus_
#define DEFINED_TYPEDEF_FOR_TemperatureSenseBus_

typedef struct {
  int16_T Temperatures_C[7];
  int16_T T_min;
  int16_T T_max;
  int16_T TemperatureDelta_C;
} TemperatureSenseBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VCU_
#define DEFINED_TYPEDEF_FOR_VCU_

typedef struct {
  boolean_T SlowCharge;
  boolean_T FastCharge;
  boolean_T isChargerConnected;
  boolean_T TransmissionFail;
  boolean_T VCUPermanentFail;
  boolean_T ServiceMode;
  boolean_T BalanceEn_atRest;
  boolean_T EmergencyDisconnectEn;
  uint32_T SlowChargersCurrentLimit_A;
  uint32_T FastChargersCurrentLimit_A;
} VCU;

#endif

#ifndef DEFINED_TYPEDEF_FOR_DataPipelineBus_
#define DEFINED_TYPEDEF_FOR_DataPipelineBus_

typedef struct {
  int32_T Current_mA;
  VoltageSenseBus VoltageSenseBus;
  int16_T MaxTemperature_C;
  int16_T DeltaTemperature_C;
  int16_T MinTemperature_C;
  int16_T DeltaTemperatureGroup1_C;
  int16_T DeltaTemperatureGroup2_C;
  TemperatureSenseBus TemperatureSenseBus[2];
  uint8_T isChargeEn;
  boolean_T isDynamicCurrentEn;
  uint8_T nCells_series;
  int8_T nCells_parallel;
  int32_T DCL_ChargingCurrent_A;
  int32_T DCL_DisChargingCurrent_A;
  int32_T i2t_ChargingCurrent_A2sec;
  int32_T i2t_DisChargingCurrent_A2sec;
  real32_T LoopTimer_SOC_msec;
  VCU VCU;
  boolean_T PreChargeLogic;
  int32_T TerminalVoltage_mV;
  boolean_T IR_Calculation_En;
} DataPipelineBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ProtectionFlags_
#define DEFINED_TYPEDEF_FOR_ProtectionFlags_

typedef enum {
  NoError = 0,                         /* Default value */
  Warning,
  Error,
  Recovery,
  PermanentFail
} ProtectionFlags;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TempState_
#define DEFINED_TYPEDEF_FOR_TempState_

typedef enum {
  NoErr = 0,                           /* Default value */
  UTC,
  UTD,
  OTC,
  OTD
} TempState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ProtectionState_Out_
#define DEFINED_TYPEDEF_FOR_ProtectionState_Out_

typedef struct {
  ProtectionFlags ThermalRunaway;
  ProtectionFlags TemperatureGradient;
  ProtectionFlags HighImbalanceFlag;
  ProtectionFlags ShortCircuitDetect;
  ProtectionFlags SuddenVoltageDrop;
  ProtectionFlags OV;
  ProtectionFlags UV;
  ProtectionFlags OCC;
  ProtectionFlags OCD;
  real32_T i2t_Calculated_A2sec;
  ProtectionFlags eFuseChargeFlag;
  ProtectionFlags eFuseDischargeFlag;
  TempState TempState1;
  TempState TempState2;
  ProtectionFlags FlagGroup1;
  ProtectionFlags FlagGroup2;
  ProtectionFlags TempOverallState;
  ProtectionFlags ErrorDetect;
  ProtectionFlags DCLI_CurrentFlag;
  ProtectionFlags DCLO_CurrentFlag;
} ProtectionState_Out;

#endif

#ifndef DEFINED_TYPEDEF_FOR_SlowCharge_
#define DEFINED_TYPEDEF_FOR_SlowCharge_

typedef struct {
  int32_T MaxVoltageLimit_mV;
  int32_T MaxCurrentLimit_mA;
  int32_T MinCurrentLimit_mA;
  int32_T MaxAllowedImb_mV;
} SlowCharge;

#endif

#ifndef DEFINED_TYPEDEF_FOR_FastCharge_
#define DEFINED_TYPEDEF_FOR_FastCharge_

typedef struct {
  int32_T MaxVoltageLimit_mV;
  int32_T MaxCurrentLimit_mA;
  int32_T MinCurrentLimit_mA;
  int32_T MaxAllowedImb_mV;
} FastCharge;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CC_InputsBus_
#define DEFINED_TYPEDEF_FOR_CC_InputsBus_

typedef struct {
  int32_T InstalledCapacity_mAh;
  int32_T EquivalentCycleCapacity_mAh;
  int32_T SensorOffset_mA;
  int32_T SensorAccuracy_pct;
  int32_T BleedingCurrent_mA;
  int16_T CCTimeout_msec;
  int16_T DebouncingTimeout_msec;
  int32_T CapacityDeltaCheck_mAh;
  boolean_T InitialCapacity_Guess;
  boolean_T SOHCalibrationOn;
  int16_T SOHCalibrationTimeout_msec;
  int32_T SOH_Vs_Capacity_Gain;
  int32_T SOH_Vs_Capacity_Offset;
  boolean_T LoopTimeSelector;
  int32_T MaxVoltageLimit_mV;
  int32_T MaxCurrentLimit_mA;
  int32_T MinCurrentLimit_mA;
  int32_T MaxAllowedImb_mV;
  SlowCharge SlowCharge;
  FastCharge FastCharge;
} CC_InputsBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_BalancingState_
#define DEFINED_TYPEDEF_FOR_BalancingState_

typedef enum {
  Terminate = 0,                       /* Default value */
  Lobby,
  Even_Balancing,
  Odd_Balancing
} BalancingState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CellBalancingOutputBus_
#define DEFINED_TYPEDEF_FOR_CellBalancingOutputBus_

typedef struct {
  int32_T MaxImbalance_mV;
  boolean_T BalanceEn;
  BalancingState CurrentBalancingState;
} CellBalancingOutputBus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CCState_
#define DEFINED_TYPEDEF_FOR_CCState_

typedef enum {
  CoulombCounting = 0,                 /* Default value */
  Recalibrate,
  Vehicle_at_Rest
} CCState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CC_OutputsBus_
#define DEFINED_TYPEDEF_FOR_CC_OutputsBus_

typedef struct {
  int32_T Initial_Capacity_mAh;
  int32_T Total_CapacityRemains_mAh;
  int32_T Total_Discharge_mAh;
  int32_T SOC_cpct;
  CCState CC_State;
  int32_T MaxUsableCapacity_mAh;
  int32_T TotalCapacityExchange_mAh;
  int32_T SOH_cpct;
  real32_T CycleCount;
} CC_OutputsBus;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_SOCEstimation_T RT_MODEL_SOCEstimation_T;

#endif                                 /* RTW_HEADER_SOCEstimation_types_h_ */
