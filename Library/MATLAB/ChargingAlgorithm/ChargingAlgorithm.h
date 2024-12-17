/*
 * ChargingAlgorithm.h
 *
 * Code generation for model "ChargingAlgorithm".
 *
 * Model version              : 4.1409
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Sat Jul 20 16:22:26 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_ChargingAlgorithm_h_
#define RTW_HEADER_ChargingAlgorithm_h_
#ifndef ChargingAlgorithm_COMMON_INCLUDES_
#define ChargingAlgorithm_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* ChargingAlgorithm_COMMON_INCLUDES_ */

#include "ChargingAlgorithm_types.h"
#include "rt_zcfcn.h"
#include <math.h>
#include <string.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Clock;                        /* '<S1>/Clock' */
  real_T V1V2I;                        /* '<S12>/Divide' */
  real_T I1;                           /* '<S3>/Chart' */
  real_T I2;                           /* '<S3>/Chart' */
  uint32_T PseudoChargeCurrent;        /* '<S1>/ChargingAlgorithm' */
  uint32_T IR_Current;                 /* '<S1>/ChargingAlgorithm' */
  real32_T Constant_K;                 /* '<S1>/ChargingAlgorithm' */
  int32_T V1;                          /* '<S3>/Chart' */
  int32_T V2;                          /* '<S3>/Chart' */
  uint8_T i;                           /* '<S1>/ChargingAlgorithm' */
  boolean_T IR_Calculation;            /* '<S1>/ChargingAlgorithm' */
} B_ChargingAlgorithm_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UD_DSTATE;                    /* '<S8>/UD' */
  real_T UD_DSTATE_f;                  /* '<S9>/UD' */
  real_T NextOutput;                   /* '<S4>/White Noise' */
  real32_T BatteryRs[20];              /* '<S3>/Chart1' */
  real32_T Recorded_CycleCount;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1;           /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_d;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_o;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_j;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_c;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_i;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_g;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_jp;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_e;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_io;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2;           /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_g;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_k;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_j;         /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_3;           /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_gb;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_1_oi;        /* '<S1>/ChargingAlgorithm' */
  int32_T durationCounter_2_d;         /* '<S1>/ChargingAlgorithm' */
  uint32_T RandSeed;                   /* '<S4>/White Noise' */
  uint8_T is_c32_ChargingAlgorithm;    /* '<S3>/Chart1' */
  uint8_T is_active_c32_ChargingAlgorithm;/* '<S3>/Chart1' */
  uint8_T is_c31_ChargingAlgorithm;    /* '<S3>/Chart' */
  uint8_T is_active_c31_ChargingAlgorithm;/* '<S3>/Chart' */
  uint8_T is_c5_ChargingAlgorithm;     /* '<S1>/ChargingAlgorithm' */
  uint8_T is_IR_Measurement;           /* '<S1>/ChargingAlgorithm' */
  uint8_T is_NormalCharging;           /* '<S1>/ChargingAlgorithm' */
  uint8_T is_SlowCharging;             /* '<S1>/ChargingAlgorithm' */
  uint8_T is_FastCharging;             /* '<S1>/ChargingAlgorithm' */
  uint8_T is_active_c5_ChargingAlgorithm;/* '<S1>/ChargingAlgorithm' */
  uint8_T temporalCounter_i1;          /* '<S1>/ChargingAlgorithm' */
} DW_ChargingAlgorithm_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState TriggeredSubsystem_Trig_ZCE;/* '<S3>/Triggered Subsystem' */
  ZCSigState TriggeredSubsystem1_Trig_ZCE;/* '<S3>/Triggered Subsystem1' */
} PrevZCX_ChargingAlgorithm_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  CC_OutputsBus CC_Outputs;            /* '<Root>/CC_Outputs' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
} ExtU_ChargingAlgorithm_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  int32_T ChargingCurrentReq_mA;       /* '<Root>/ChargingCurrentReq_mA' */
  ChargeState ChargingState;           /* '<Root>/ChargingState' */
  real32_T Avg_Rs;                     /* '<Root>/Avg_Rs' */
} ExtY_ChargingAlgorithm_T;

/* Real-time Model Data Structure */
struct tag_RTM_ChargingAlgorithm_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    SimTimeStep simTimeStep;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_ChargingAlgorithm_T ChargingAlgorithm_B;

/* Block states (default storage) */
extern DW_ChargingAlgorithm_T ChargingAlgorithm_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_ChargingAlgorithm_T ChargingAlgorithm_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_ChargingAlgorithm_T ChargingAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_ChargingAlgorithm_T ChargingAlgorithm_Y;

/* External data declarations for dependent source files */
extern const CC_OutputsBus ChargingAlgorithm_rtZCC_Outputs;/* CC_OutputsBus ground */

/* Model entry point functions */
extern void ChargingAlgorithm_initialize(void);
extern void ChargingAlgorithm_step(void);
extern void ChargingAlgorithm_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ChargingAlgorithm_T *const ChargingAlgorithm_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S8>/Data Type Duplicate' : Unused code path elimination
 * Block '<S9>/Data Type Duplicate' : Unused code path elimination
 * Block '<S10>/Add1' : Unused code path elimination
 * Block '<S11>/Add1' : Unused code path elimination
 * Block '<S11>/Divide' : Unused code path elimination
 * Block '<S11>/Divide1' : Unused code path elimination
 * Block '<S11>/Gain' : Unused code path elimination
 * Block '<S4>/Output' : Eliminated nontunable gain of 1
 * Block '<S12>/Gain' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('BMS_Algo_Release_v10_16_7/ChargingAlgorithm')    - opens subsystem BMS_Algo_Release_v10_16_7/ChargingAlgorithm
 * hilite_system('BMS_Algo_Release_v10_16_7/ChargingAlgorithm/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_16_7'
 * '<S1>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm'
 * '<S2>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/ChargingAlgorithm'
 * '<S3>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation'
 * '<S4>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Band-Limited White Noise1'
 * '<S5>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Chart'
 * '<S6>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Chart1'
 * '<S7>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Compare To Constant'
 * '<S8>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Discrete Derivative'
 * '<S9>'   : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Discrete Derivative1'
 * '<S10>'  : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Subsystem'
 * '<S11>'  : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Triggered Subsystem1'
 * '<S12>'  : 'BMS_Algo_Release_v10_16_7/ChargingAlgorithm/IR_Calculation/Triggered Subsystem'
 */
#endif                                 /* RTW_HEADER_ChargingAlgorithm_h_ */
