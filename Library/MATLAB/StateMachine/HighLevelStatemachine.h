/*
 * HighLevelStatemachine.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "HighLevelStatemachine".
 *
 * Model version              : 4.1467
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jul 25 17:14:35 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: All passed
 */

#ifndef RTW_HEADER_HighLevelStatemachine_h_
#define RTW_HEADER_HighLevelStatemachine_h_
#ifndef HighLevelStatemachine_COMMON_INCLUDES_
#define HighLevelStatemachine_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                              /* HighLevelStatemachine_COMMON_INCLUDES_ */

#include "HighLevelStatemachine_types.h"
#include <stddef.h>
#include <string.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint32_T durationCounter_1;          /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_g;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2;          /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_m;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_g2;       /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_h;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_k;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_kf;       /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_b;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_o;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_3;          /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_f;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_fx;       /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_3_i;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_3_j;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_e;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_4;          /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_b;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_4_n;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_3_g;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_4_f;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_4_m;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_e;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_n;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_c;        /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_br;       /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_2_f0;       /* '<Root>/HighLevelStatemachine' */
  uint32_T durationCounter_1_e2;       /* '<Root>/HighLevelStatemachine' */
  uint8_T is_HighLevelStatemachine;    /* '<Root>/HighLevelStatemachine' */
  uint8_T is_Fault_Mode;               /* '<Root>/HighLevelStatemachine' */
  uint8_T is_BMS_State;                /* '<Root>/HighLevelStatemachine' */
  uint8_T is_active_c8_HighLevelStatemach;/* '<Root>/HighLevelStatemachine' */
} DW_HighLevelStatemachine_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  ThresholdsBus Thresholds;            /* '<Root>/Thresholds' */
  uint8_T ContactorCommand;            /* '<Root>/ContactorCommand' */
  ContState ContactorsState;           /* '<Root>/ContactorsState' */
  ProtectionState_Out ProtectionOutput;/* '<Root>/ProtectionOutput' */
  DataPipelineBus DataPipeline;        /* '<Root>/DataPipeline' */
} ExtU_HighLevelStatemachine_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  HighLevelMode HighLevelStateMode;        /* '<Root>/HighLevelStateMode' */
  BMSState BMS_State;                  /* '<Root>/BMS_State' */
} ExtY_HighLevelStatemachine_T;

/* Real-time Model Data Structure */
struct tag_RTM_HighLevelStatemachine_T {
  const char_T *errorStatus;
};

/* Block states (default storage) */
extern DW_HighLevelStatemachine_T HighLevelStatemachine_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_HighLevelStatemachine_T HighLevelStatemachine_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_HighLevelStatemachine_T HighLevelStatemachine_Y;

/* External data declarations for dependent source files */
extern const ProtectionState_Out HighLevelStatemachine_rtZProtec;/* ProtectionState_Out ground */

/* Model entry point functions */
extern void HighLevelStatemachine_initialize(void);
extern void HighLevelStatemachine_step(void);
extern void HighLevelStatemachine_terminate(void);

/* Real-time Model object */
extern RT_MODEL_HighLevelStatemachin_T *const HighLevelStatemachine_M;

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
 * hilite_system('BMS_Algo_Release_v10_16_9/HighLevelStatemachine/HighLevelStatemachine')    - opens subsystem BMS_Algo_Release_v10_16_9/HighLevelStatemachine/HighLevelStatemachine
 * hilite_system('BMS_Algo_Release_v10_16_9/HighLevelStatemachine/HighLevelStatemachine/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BMS_Algo_Release_v10_16_9/HighLevelStatemachine'
 * '<S1>'   : 'BMS_Algo_Release_v10_16_9/HighLevelStatemachine/HighLevelStatemachine'
 */
#endif                                 /* RTW_HEADER_HighLevelStatemachine_h_ */
