/*
 * DataPipeline_data.c
 *
 * Code generation for model "DataPipeline".
 *
 * Model version              : 4.1464
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Thu Jul 25 14:24:32 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#include "DataPipeline.h"

/* Block parameters (default storage) */
P_DataPipeline_T DataPipeline_P = {
  /* Variable: nCells_parallel
   * Referenced by:
   *   '<S1>/Gain1'
   *   '<S1>/Gain2'
   *   '<S1>/Gain3'
   *   '<S1>/Gain5'
   */
  1
};

/* Constant parameters (default storage) */
const ConstP_DataPipeline_T DataPipeline_ConstP = {
  /* Computed Parameter: Gain7_Gain
   * Referenced by: '<S1>/Gain7'
   */
  { { 0x33330000UL, 0x73UL } }
};
