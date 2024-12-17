/*
 * ChargingAlgorithm.c
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

#include "ChargingAlgorithm.h"
#include "rtwtypes.h"
#include "ChargingAlgorithm_private.h"
#include <string.h>
#include "ChargingAlgorithm_types.h"
#include <math.h>
#include "multiword_types.h"
#include "zero_crossing_types.h"

/* Named constants for Chart: '<S1>/ChargingAlgorithm' */
#define Char_IN_IR_Measurement_Current1 ((uint8_T)2U)
#define Charg_IN_IR_Measurement_Current ((uint8_T)1U)
#define ChargingAlgo_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define ChargingAlgor_IN_BatteryRevival ((uint8_T)1U)
#define ChargingAlgor_IN_IR_Measurement ((uint8_T)2U)
#define ChargingAlgor_IN_NormalCharging ((uint8_T)3U)
#define ChargingAlgor_IN_PseudoCharging ((uint8_T)4U)
#define ChargingAlgori_IN_ChargingLobby ((uint8_T)1U)
#define ChargingAlgorit_IN_CC_Balancing ((uint8_T)3U)
#define ChargingAlgorit_IN_FastCharging ((uint8_T)1U)
#define ChargingAlgorit_IN_SlowCharging ((uint8_T)2U)
#define ChargingAlgorithm_IN_CC        ((uint8_T)2U)
#define ChargingAlgorithm_IN_CV        ((uint8_T)4U)

/* Named constants for Chart: '<S3>/Chart' */
#define ChargingAlgorithm_IN_V1_Record ((uint8_T)1U)
#define ChargingAlgorithm_IN_V2_Record ((uint8_T)2U)
#define ChargingAlgorithm_IN_V3_Record ((uint8_T)3U)

/* Named constants for Chart: '<S3>/Chart1' */
#define ChargingA_IN_Rs_Array_Creation1 ((uint8_T)2U)
#define ChargingAl_IN_Rs_Array_Creation ((uint8_T)1U)
#define ChargingAlgorithm_IN_Rs_Lobby  ((uint8_T)3U)

/* Block signals (default storage) */
B_ChargingAlgorithm_T ChargingAlgorithm_B;

/* Block states (default storage) */
DW_ChargingAlgorithm_T ChargingAlgorithm_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_ChargingAlgorithm_T ChargingAlgorithm_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_ChargingAlgorithm_T ChargingAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_ChargingAlgorithm_T ChargingAlgorithm_Y;

/* Real-time model */
static RT_MODEL_ChargingAlgorithm_T ChargingAlgorithm_M_;
RT_MODEL_ChargingAlgorithm_T *const ChargingAlgorithm_M = &ChargingAlgorithm_M_;

/* Forward declaration for local functions */
static void ChargingAlgorith_NormalCharging(void);
const CC_OutputsBus ChargingAlgorithm_rtZCC_Outputs = { 0,/* Initial_Capacity_mAh */
  0,                                   /* Total_CapacityRemains_mAh */
  0,                                   /* Total_Discharge_mAh */
  0,                                   /* SOC_cpct */
  CoulombCounting,                     /* CC_State */
  0,                                   /* MaxUsableCapacity_mAh */
  0,                                   /* TotalCapacityExchange_mAh */
  0,                                   /* SOH_cpct */
  0.0F                                 /* CycleCount */
};

real_T sMultiWord2Double(const uint32_T u1[], int32_T n1, int32_T e1)
{
  real_T y;
  int32_T exp_0;
  int32_T i;
  y = 0.0;
  exp_0 = e1;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    uint32_T cb;
    cb = 1U;
    for (i = 0; i < n1; i++) {
      uint32_T u1i;
      u1i = ~u1[i];
      cb += u1i;
      y -= ldexp(cb, exp_0);
      cb = (uint32_T)(cb < u1i);
      exp_0 += 32;
    }
  } else {
    for (i = 0; i < n1; i++) {
      y += ldexp(u1[i], exp_0);
      exp_0 += 32;
    }
  }

  return y;
}

void MultiWordSignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[])
{
  int32_T i;
  int32_T n1m1;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  uint32_T mask;
  uint32_T ys;
  mask = 1U << (31U - n2);
  ys = (u1[n1 - 1] & mask) != 0U ? MAX_uint32_T : 0U;
  mask = (mask << 1U) - 1U;
  y[n1 - 1] = (u1[n1 - 1] & mask) | (~mask & ys);
}

void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
                   n2, uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T j;
  int32_T k;
  uint32_T cb;
  uint32_T cb1;
  uint32_T yk;
  boolean_T isNegative1;
  boolean_T isNegative2;
  isNegative1 = ((u1[n1 - 1] & 2147483648U) != 0U);
  isNegative2 = ((u2[n2 - 1] & 2147483648U) != 0U);
  cb1 = 1U;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (i = 0; i < n1; i++) {
    int32_T ni;
    uint32_T a0;
    uint32_T a1;
    uint32_T cb2;
    uint32_T u1i;
    cb = 0U;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (uint32_T)(u1i < cb1);
    }

    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    cb2 = 1U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      uint32_T b1;
      uint32_T w01;
      uint32_T w10;
      u1i = u2[j];
      if (isNegative2) {
        u1i = ~u1i + cb2;
        cb2 = (uint32_T)(u1i < cb2);
      }

      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (uint32_T)(yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if (isNegative1 != isNegative2) {
    cb = 1U;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (uint32_T)(yk < cb);
    }
  }
}

real_T uMultiWord2Double(const uint32_T u1[], int32_T n1, int32_T e1)
{
  real_T y;
  int32_T exp_1;
  int32_T i;
  y = 0.0;
  exp_1 = e1;
  for (i = 0; i < n1; i++) {
    y += ldexp(u1[i], exp_1);
    exp_1 += 32;
  }

  return y;
}

void MultiWordUnsignedWrap(const uint32_T u1[], int32_T n1, uint32_T n2,
  uint32_T y[])
{
  int32_T i;
  int32_T n1m1;
  n1m1 = n1 - 1;
  for (i = 0; i < n1m1; i++) {
    y[i] = u1[i];
  }

  y[n1 - 1] = ((1U << (32U - n2)) - 1U) & u1[n1 - 1];
}

void uMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
                   n2, uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T j;
  int32_T k;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (i = 0; i < n1; i++) {
    int32_T ni;
    uint32_T a0;
    uint32_T a1;
    uint32_T cb;
    uint32_T u1i;
    cb = 0U;
    u1i = u1[i];
    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      uint32_T b1;
      uint32_T w01;
      uint32_T w10;
      uint32_T yk;
      u1i = u2[j];
      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (uint32_T)(yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (uint32_T)(yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }
}

real_T rt_urand_Upu32_Yd_f_pw(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return (real_T)*u * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

/* Function for Chart: '<S1>/ChargingAlgorithm' */
static void ChargingAlgorith_NormalCharging(void)
{
  boolean_T tmp;
  ChargingAlgorithm_B.IR_Calculation = false;

  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.isChargerConnected == true && ((CycleCount-Recorded_CycleCount) > Thresholds.nCycle_IR_Calculation),msec) >Thresholds.IR_entryTimeout_msec; */
  tmp = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;

  /* Inport: '<Root>/CC_Outputs' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  if (tmp || (ChargingAlgorithm_U.CC_Outputs.CycleCount -
              ChargingAlgorithm_DW.Recorded_CycleCount <=
              ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
    ChargingAlgorithm_DW.durationCounter_2_d = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_d * 100) >
      ChargingAlgorithm_U.Thresholds.IR_entryTimeout_msec) {
    ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
    ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
    ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
    ChargingAlgorithm_DW.durationCounter_1_oi = 0;
    ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
      ChargingAlgor_IN_IR_Measurement;

    /* Inport: '<Root>/CC_Outputs' */
    /* :  Recorded_CycleCount = CycleCount; */
    ChargingAlgorithm_DW.Recorded_CycleCount =
      ChargingAlgorithm_U.CC_Outputs.CycleCount;
    if (tmp || (ChargingAlgorithm_U.CC_Outputs.CycleCount -
                ChargingAlgorithm_DW.Recorded_CycleCount <=
                ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
      ChargingAlgorithm_DW.durationCounter_1_io = 0;
      ChargingAlgorithm_DW.durationCounter_2_d = 0;
    }

    /* :  i = 0; */
    ChargingAlgorithm_B.i = 0U;

    /* Outport: '<Root>/ChargingState' */
    /* :  ChargingState = ChargeState.IR_Measurement; */
    ChargingAlgorithm_Y.ChargingState = IR_Measurement;

    /* :  IR_Calculation = true; */
    ChargingAlgorithm_B.IR_Calculation = true;
    ChargingAlgorithm_DW.is_IR_Measurement = Charg_IN_IR_Measurement_Current;
    ChargingAlgorithm_DW.temporalCounter_i1 = 0U;
  } else {
    /* :  sf_internal_predicateOutput = duration(CC_Outputs.CC_State==3,msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
    ChargingAlgorithm_DW.durationCounter_1_k = 0;
    if (ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec <=
        0U) {
      ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
      ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
      ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
      ChargingAlgorithm_DW.durationCounter_2 = 0;
      ChargingAlgorithm_DW.durationCounter_1_io = 0;
      ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
        ChargingAlgor_IN_PseudoCharging;

      /* Outport: '<Root>/ChargingState' */
      /* :  ChargingState = ChargeState.PseudoCharging; */
      ChargingAlgorithm_Y.ChargingState = PseudoCharging;

      /* :  PseudoChargeCurrent = Thresholds.PseudoChargeCurrent_mA; */
      ChargingAlgorithm_B.PseudoChargeCurrent =
        ChargingAlgorithm_U.Thresholds.PseudoChargeCurrent_mA;
    } else if (ChargingAlgorithm_DW.is_NormalCharging ==
               ChargingAlgorit_IN_FastCharging) {
      /* Outport: '<Root>/ChargingState' */
      ChargingAlgorithm_Y.ChargingState = FastCharging;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.FastCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
          ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
        ChargingAlgorithm_DW.durationCounter_2_j = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_j * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.durationCounter_3 = 0;
        ChargingAlgorithm_DW.durationCounter_2_g = 0;
        ChargingAlgorithm_DW.durationCounter_1_e = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgori_IN_ChargingLobby;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.NoCharging; */
        ChargingAlgorithm_Y.ChargingState = NoCharging;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.SlowCharge == true && DataPipeline.VCU.FastCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if ((!ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) ||
            (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
          ChargingAlgorithm_DW.durationCounter_1_d = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_d * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
          ChargingAlgorithm_DW.durationCounter_2_gb = 0;
          ChargingAlgorithm_DW.durationCounter_1 = 0;
          ChargingAlgorithm_DW.is_NormalCharging =
            ChargingAlgorit_IN_SlowCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.SlowCharging; */
          ChargingAlgorithm_Y.ChargingState = SlowCharging;
          ChargingAlgorithm_DW.durationCounter_1_o = 0;
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.SlowCharge_BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowCharge_BatteryRevival_K;
        } else {
          switch (ChargingAlgorithm_DW.is_FastCharging) {
           case ChargingAlgor_IN_BatteryRevival:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_i = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_i * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_g = 0;
              ChargingAlgorithm_DW.is_FastCharging = ChargingAlgorithm_IN_CC;

              /* :  Constant_K = Thresholds.FastCharge_CC_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastCharge_CC_K;
            }
            break;

           case ChargingAlgorithm_IN_CC:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.Balancing_Min_Thr_mV, msec) >= Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
              ChargingAlgorithm_DW.durationCounter_1_g = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_g * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_jp = 0;
              ChargingAlgorithm_DW.is_FastCharging =
                ChargingAlgorit_IN_CC_Balancing;

              /* :  Constant_K = Thresholds.FastCharge_CC_Balancing_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastCharge_CC_Balancing_K;
            }
            break;

           case ChargingAlgorit_IN_CC_Balancing:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_jp = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_jp * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.is_FastCharging = ChargingAlgorithm_IN_CV;

              /* :  Constant_K = Thresholds.FastCharge_CV_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.FastCharge_CV_K;
            }
            break;

           default:
            /* case IN_CV: */
            break;
          }
        }
      }
    } else {
      /* Outport: '<Root>/ChargingState' */
      /* case IN_SlowCharging: */
      ChargingAlgorithm_Y.ChargingState = SlowCharging;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.SlowCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
          ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
        ChargingAlgorithm_DW.durationCounter_2_gb = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_gb * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.durationCounter_3 = 0;
        ChargingAlgorithm_DW.durationCounter_2_g = 0;
        ChargingAlgorithm_DW.durationCounter_1_e = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgori_IN_ChargingLobby;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.NoCharging; */
        ChargingAlgorithm_Y.ChargingState = NoCharging;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.FastCharge == true && DataPipeline.VCU.SlowCharge == false),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if ((!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) ||
            ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
          ChargingAlgorithm_DW.durationCounter_1 = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_1 * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
          ChargingAlgorithm_DW.durationCounter_2_j = 0;
          ChargingAlgorithm_DW.durationCounter_1_d = 0;
          ChargingAlgorithm_DW.is_NormalCharging =
            ChargingAlgorit_IN_FastCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.FastCharging; */
          ChargingAlgorithm_Y.ChargingState = FastCharging;
          ChargingAlgorithm_DW.durationCounter_1_i = 0;
          ChargingAlgorithm_DW.is_FastCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.FastCharge_BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.FastCharge_BatteryRevival_K;
        } else {
          switch (ChargingAlgorithm_DW.is_SlowCharging) {
           case ChargingAlgor_IN_BatteryRevival:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_o = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_o * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_j = 0;
              ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgorithm_IN_CC;

              /* :  Constant_K = Thresholds.SlowCharge_CC_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowCharge_CC_K;
            }
            break;

           case ChargingAlgorithm_IN_CC:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.Balancing_Min_Thr_mV, msec) >= Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
              ChargingAlgorithm_DW.durationCounter_1_j = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_j * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.durationCounter_1_c = 0;
              ChargingAlgorithm_DW.is_SlowCharging =
                ChargingAlgorit_IN_CC_Balancing;

              /* :  Constant_K = Thresholds.SlowCharge_CC_Balancing_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowCharge_CC_Balancing_K;
            }
            break;

           case ChargingAlgorit_IN_CC_Balancing:
            /* Inport: '<Root>/DataPipeline' */
            /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_min >= Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV, msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
            if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min <
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV)
            {
              ChargingAlgorithm_DW.durationCounter_1_c = 0;
            }

            if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_c * 100) >=
                ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
            {
              ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgorithm_IN_CV;

              /* :  Constant_K = Thresholds.SlowCharge_CV_K; */
              ChargingAlgorithm_B.Constant_K =
                ChargingAlgorithm_U.Thresholds.SlowCharge_CV_K;
            }
            break;

           default:
            /* case IN_CV: */
            break;
          }
        }
      }
    }
  }
}

/* Model step function */
void ChargingAlgorithm_step(void)
{
  int64m_T tmp_0;
  int64m_T tmp_1;
  uint64m_T tmp_4;
  uint64m_T tmp_5;
  real_T rtb_WhiteNoise;
  int32_T rtb_VoltageDiff_mV;
  real32_T tmp_8;
  uint32_T tmp;
  uint32_T tmp_2;
  uint32_T tmp_3;
  boolean_T tmp_6;
  boolean_T tmp_7;

  /* Chart: '<S1>/ChargingAlgorithm' incorporates:
   *  Inport: '<Root>/CC_Outputs'
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (ChargingAlgorithm_DW.temporalCounter_i1 < 7U) {
    ChargingAlgorithm_DW.temporalCounter_i1++;
  }

  if (ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm == 0U) {
    ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm = 1U;
    ChargingAlgorithm_DW.durationCounter_3 = 0;
    ChargingAlgorithm_DW.durationCounter_2_g = 0;
    ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
      ChargingAlgori_IN_ChargingLobby;

    /* Outport: '<Root>/ChargingState' */
    /* :  ChargingState = ChargeState.NoCharging; */
    ChargingAlgorithm_Y.ChargingState = NoCharging;
  } else {
    switch (ChargingAlgorithm_DW.is_c5_ChargingAlgorithm) {
     case ChargingAlgori_IN_ChargingLobby:
      /* Outport: '<Root>/ChargingState' */
      ChargingAlgorithm_Y.ChargingState = NoCharging;

      /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.FastCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      tmp_6 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
      if (tmp_6 || (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
        ChargingAlgorithm_DW.durationCounter_2_g = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_2_g * 100) >=
          ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.durationCounter_2_d = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgor_IN_NormalCharging;

        /* :  IR_Calculation = false; */
        ChargingAlgorithm_B.IR_Calculation = false;
        ChargingAlgorithm_DW.durationCounter_2_j = 0;
        ChargingAlgorithm_DW.durationCounter_1_d = 0;
        ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgorit_IN_FastCharging;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.FastCharging; */
        ChargingAlgorithm_Y.ChargingState = FastCharging;
        ChargingAlgorithm_DW.durationCounter_1_i = 0;
        ChargingAlgorithm_DW.is_FastCharging = ChargingAlgor_IN_BatteryRevival;

        /* :  Constant_K = Thresholds.FastCharge_BatteryRevival_K; */
        ChargingAlgorithm_B.Constant_K =
          ChargingAlgorithm_U.Thresholds.FastCharge_BatteryRevival_K;
      } else {
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.SlowCharge == true),msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        if (tmp_6 || (!ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge)) {
          ChargingAlgorithm_DW.durationCounter_3 = 0;
        }

        if ((real_T)(ChargingAlgorithm_DW.durationCounter_3 * 100) >=
            ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
        {
          ChargingAlgorithm_DW.durationCounter_2_d = 0;
          ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
            ChargingAlgor_IN_NormalCharging;

          /* :  IR_Calculation = false; */
          ChargingAlgorithm_B.IR_Calculation = false;
          ChargingAlgorithm_DW.durationCounter_2_gb = 0;
          ChargingAlgorithm_DW.durationCounter_1 = 0;
          ChargingAlgorithm_DW.is_NormalCharging =
            ChargingAlgorit_IN_SlowCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.SlowCharging; */
          ChargingAlgorithm_Y.ChargingState = SlowCharging;
          ChargingAlgorithm_DW.durationCounter_1_o = 0;
          ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgor_IN_BatteryRevival;

          /* :  Constant_K = Thresholds.SlowCharge_BatteryRevival_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowCharge_BatteryRevival_K;

          /* :  sf_internal_predicateOutput = duration(CC_Outputs.CC_State==3,msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
        } else if
            (ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec
             <= 0U) {
          ChargingAlgorithm_DW.durationCounter_2 = 0;
          ChargingAlgorithm_DW.durationCounter_1_io = 0;
          ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
            ChargingAlgor_IN_PseudoCharging;

          /* Outport: '<Root>/ChargingState' */
          /* :  ChargingState = ChargeState.PseudoCharging; */
          ChargingAlgorithm_Y.ChargingState = PseudoCharging;

          /* :  PseudoChargeCurrent = Thresholds.PseudoChargeCurrent_mA; */
          ChargingAlgorithm_B.PseudoChargeCurrent =
            ChargingAlgorithm_U.Thresholds.PseudoChargeCurrent_mA;
        } else {
          /* :  Constant_K = Thresholds.SlowCharge_NoCharge_K; */
          ChargingAlgorithm_B.Constant_K =
            ChargingAlgorithm_U.Thresholds.SlowCharge_NoCharge_K;
        }
      }
      break;

     case ChargingAlgor_IN_IR_Measurement:
      /* Outport: '<Root>/ChargingState' */
      ChargingAlgorithm_Y.ChargingState = IR_Measurement;
      ChargingAlgorithm_B.IR_Calculation = true;

      /* :  sf_internal_predicateOutput = duration(i>=5 && DataPipeline.VCU.isChargerConnected == true,msec) > Thresholds.IR_entryTimeout_msec; */
      tmp_6 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
      if ((ChargingAlgorithm_B.i < 5) || tmp_6) {
        ChargingAlgorithm_DW.durationCounter_1_oi = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_oi * 100) >
          ChargingAlgorithm_U.Thresholds.IR_entryTimeout_msec) {
        ChargingAlgorithm_DW.is_IR_Measurement = ChargingAlgo_IN_NO_ACTIVE_CHILD;
        ChargingAlgorithm_DW.durationCounter_2_d = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgor_IN_NormalCharging;

        /* :  IR_Calculation = false; */
        ChargingAlgorithm_B.IR_Calculation = false;
        ChargingAlgorithm_DW.durationCounter_2_gb = 0;
        ChargingAlgorithm_DW.durationCounter_1 = 0;
        ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgorit_IN_SlowCharging;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.SlowCharging; */
        ChargingAlgorithm_Y.ChargingState = SlowCharging;
        ChargingAlgorithm_DW.durationCounter_1_o = 0;
        ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgor_IN_BatteryRevival;

        /* :  Constant_K = Thresholds.SlowCharge_BatteryRevival_K; */
        ChargingAlgorithm_B.Constant_K =
          ChargingAlgorithm_U.Thresholds.SlowCharge_BatteryRevival_K;
      } else if (ChargingAlgorithm_DW.is_IR_Measurement ==
                 Charg_IN_IR_Measurement_Current) {
        /* :  sf_internal_predicateOutput = after(500,msec); */
        if (ChargingAlgorithm_DW.temporalCounter_i1 >= 5U) {
          /* :  i = i + 1 */
          rtb_VoltageDiff_mV = (int32_T)(ChargingAlgorithm_B.i + 1U);
          if (ChargingAlgorithm_B.i + 1U > 255U) {
            rtb_VoltageDiff_mV = 255;
          }

          ChargingAlgorithm_B.i = (uint8_T)rtb_VoltageDiff_mV;
          if ((ChargingAlgorithm_B.i < 5) || tmp_6) {
            ChargingAlgorithm_DW.durationCounter_1_oi = 0;
          }

          ChargingAlgorithm_DW.is_IR_Measurement =
            Char_IN_IR_Measurement_Current1;
          ChargingAlgorithm_DW.temporalCounter_i1 = 0U;
        } else {
          /* :  IR_Current = Thresholds.IR_Current_mA; */
          ChargingAlgorithm_B.IR_Current =
            ChargingAlgorithm_U.Thresholds.IR_Current_mA;
        }

        /* case IN_IR_Measurement_Current1: */
        /* :  sf_internal_predicateOutput = after(200,msec); */
      } else if (ChargingAlgorithm_DW.temporalCounter_i1 >= 2U) {
        ChargingAlgorithm_DW.is_IR_Measurement = Charg_IN_IR_Measurement_Current;
        ChargingAlgorithm_DW.temporalCounter_i1 = 0U;
      } else {
        /* :  IR_Current = Thresholds.IR_DropCurrent_mA; */
        ChargingAlgorithm_B.IR_Current =
          ChargingAlgorithm_U.Thresholds.IR_DropCurrent_mA;
      }
      break;

     case ChargingAlgor_IN_NormalCharging:
      ChargingAlgorith_NormalCharging();
      break;

     default:
      /* Outport: '<Root>/ChargingState' */
      /* case IN_PseudoCharging: */
      ChargingAlgorithm_Y.ChargingState = PseudoCharging;

      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.isChargerConnected == true && ((CycleCount-Recorded_CycleCount) > Thresholds.nCycle_IR_Calculation),msec) > Thresholds.IR_entryTimeout_msec; */
      tmp_6 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
      if (tmp_6 || (ChargingAlgorithm_U.CC_Outputs.CycleCount -
                    ChargingAlgorithm_DW.Recorded_CycleCount <=
                    ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
        ChargingAlgorithm_DW.durationCounter_1_io = 0;
      }

      if ((real_T)(ChargingAlgorithm_DW.durationCounter_1_io * 100) >
          ChargingAlgorithm_U.Thresholds.IR_entryTimeout_msec) {
        ChargingAlgorithm_DW.durationCounter_1_oi = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgor_IN_IR_Measurement;

        /* :  Recorded_CycleCount = CycleCount; */
        ChargingAlgorithm_DW.Recorded_CycleCount =
          ChargingAlgorithm_U.CC_Outputs.CycleCount;
        if (tmp_6 || (ChargingAlgorithm_U.CC_Outputs.CycleCount -
                      ChargingAlgorithm_DW.Recorded_CycleCount <=
                      ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
          ChargingAlgorithm_DW.durationCounter_1_io = 0;
          ChargingAlgorithm_DW.durationCounter_2_d = 0;
        }

        /* :  i = 0; */
        ChargingAlgorithm_B.i = 0U;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.IR_Measurement; */
        ChargingAlgorithm_Y.ChargingState = IR_Measurement;

        /* :  IR_Calculation = true; */
        ChargingAlgorithm_B.IR_Calculation = true;
        ChargingAlgorithm_DW.is_IR_Measurement = Charg_IN_IR_Measurement_Current;
        ChargingAlgorithm_DW.temporalCounter_i1 = 0U;

        /* :  sf_internal_predicateOutput = duration(CC_Outputs.CC_State~=3,msec)>=Thresholds.ChargeAlgorithm.TransitionTimeout_msec; */
      } else if ((real_T)(ChargingAlgorithm_DW.durationCounter_2 * 100) >=
                 ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.TransitionTimeout_msec)
      {
        ChargingAlgorithm_DW.durationCounter_3 = 0;
        ChargingAlgorithm_DW.durationCounter_2_g = 0;
        ChargingAlgorithm_DW.is_c5_ChargingAlgorithm =
          ChargingAlgori_IN_ChargingLobby;

        /* Outport: '<Root>/ChargingState' */
        /* :  ChargingState = ChargeState.NoCharging; */
        ChargingAlgorithm_Y.ChargingState = NoCharging;
      }
      break;
    }
  }

  tmp_6 = !ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge;
  if (ChargingAlgorithm_U.DataPipeline.VCU.FastCharge && tmp_6) {
    ChargingAlgorithm_DW.durationCounter_1++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1 = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge &&
      ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    ChargingAlgorithm_DW.durationCounter_1_d++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_d = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_o++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_o = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
    ChargingAlgorithm_DW.durationCounter_1_j++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_j = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_c++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_c = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.BatteryRevivalVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_i++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_i = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.Balancing_Min_Thr_mV) {
    ChargingAlgorithm_DW.durationCounter_1_g++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_g = 0;
  }

  if ((real_T)ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_min >=
      ChargingAlgorithm_U.Thresholds.ChargeAlgorithm.CVTransitionVoltage_mV) {
    ChargingAlgorithm_DW.durationCounter_1_jp++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_jp = 0;
  }

  ChargingAlgorithm_DW.durationCounter_1_e = 0;
  tmp_8 = ChargingAlgorithm_U.CC_Outputs.CycleCount -
    ChargingAlgorithm_DW.Recorded_CycleCount;
  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected && (tmp_8 >
       ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
    ChargingAlgorithm_DW.durationCounter_1_io++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_io = 0;
  }

  ChargingAlgorithm_DW.durationCounter_2++;
  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
      ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    ChargingAlgorithm_DW.durationCounter_2_g++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_g = 0;
  }

  ChargingAlgorithm_DW.durationCounter_1_k = 0;
  tmp_7 = !ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected;
  if (tmp_7 || (!ChargingAlgorithm_U.DataPipeline.VCU.FastCharge)) {
    ChargingAlgorithm_DW.durationCounter_2_j++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_j = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected &&
      ChargingAlgorithm_U.DataPipeline.VCU.SlowCharge) {
    ChargingAlgorithm_DW.durationCounter_3++;
  } else {
    ChargingAlgorithm_DW.durationCounter_3 = 0;
  }

  if (tmp_7 || tmp_6) {
    ChargingAlgorithm_DW.durationCounter_2_gb++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_gb = 0;
  }

  if ((ChargingAlgorithm_B.i >= 5) &&
      ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected) {
    ChargingAlgorithm_DW.durationCounter_1_oi++;
  } else {
    ChargingAlgorithm_DW.durationCounter_1_oi = 0;
  }

  if (ChargingAlgorithm_U.DataPipeline.VCU.isChargerConnected && (tmp_8 >
       ChargingAlgorithm_U.Thresholds.nCycle_IR_Calculation)) {
    ChargingAlgorithm_DW.durationCounter_2_d++;
  } else {
    ChargingAlgorithm_DW.durationCounter_2_d = 0;
  }

  /* End of Chart: '<S1>/ChargingAlgorithm' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    rtb_VoltageDiff_mV =
      ChargingAlgorithm_U.Thresholds.targetVoltagesFastCharging;
  } else {
    rtb_VoltageDiff_mV =
      ChargingAlgorithm_U.Thresholds.targetVoltageSlowCharging;
  }

  /* Sum: '<S1>/Minus' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Switch: '<S1>/Switch'
   */
  rtb_VoltageDiff_mV -= ChargingAlgorithm_U.DataPipeline.VoltageSenseBus.mV_max;

  /* Switch: '<S1>/Switch2' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  if (ChargingAlgorithm_U.DataPipeline.VCU.FastCharge) {
    tmp = ChargingAlgorithm_U.DataPipeline.VCU.FastChargersCurrentLimit_A;
  } else {
    tmp = ChargingAlgorithm_U.DataPipeline.VCU.SlowChargersCurrentLimit_A;
  }

  /* Gain: '<S1>/Gain2' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  tmp_2 = 1000U;
  tmp_3 = (uint32_T)ChargingAlgorithm_U.DataPipeline.DCL_ChargingCurrent_A;
  sMultiWordMul(&tmp_2, 1, &tmp_3, 1, &tmp_1.chunks[0U], 2);
  MultiWordSignedWrap(&tmp_1.chunks[0U], 2, 16U, &tmp_0.chunks[0U]);

  /* Gain: '<S1>/Gain3' incorporates:
   *  Switch: '<S1>/Switch2'
   */
  uMultiWordMul(&tmp_2, 1, &tmp, 1, &tmp_5.chunks[0U], 2);
  MultiWordUnsignedWrap(&tmp_5.chunks[0U], 2, 16U, &tmp_4.chunks[0U]);

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S1>/Constant'
   *  Product: '<S1>/Product'
   */
  if (rtb_VoltageDiff_mV > 0) {
    rtb_WhiteNoise = (int32_T)floorf(ChargingAlgorithm_B.Constant_K * (real32_T)
      rtb_VoltageDiff_mV);
  } else {
    rtb_WhiteNoise = 0.0;
  }

  /* Outport: '<Root>/ChargingCurrentReq_mA' incorporates:
   *  MinMax: '<S1>/Min'
   *  Switch: '<S1>/Switch1'
   */
  ChargingAlgorithm_Y.ChargingCurrentReq_mA = (int32_T)floor(fmin(fmin(fmin(fmin
    (ChargingAlgorithm_B.IR_Current, ChargingAlgorithm_B.PseudoChargeCurrent),
    rtb_WhiteNoise), sMultiWord2Double(&tmp_0.chunks[0U], 2, 0)),
    uMultiWord2Double(&tmp_4.chunks[0U], 2, 0)));

  /* Clock: '<S1>/Clock' */
  ChargingAlgorithm_B.Clock = ChargingAlgorithm_M->Timing.t[0];

  /* Outputs for Enabled SubSystem: '<S1>/IR_Calculation' incorporates:
   *  EnablePort: '<S3>/Enable'
   */
  /* Logic: '<S1>/AND' incorporates:
   *  Inport: '<Root>/DataPipeline'
   */
  if (ChargingAlgorithm_U.DataPipeline.IR_Calculation_En &&
      ChargingAlgorithm_B.IR_Calculation) {
    real_T rtb_Diff;
    real_T rtb_Diff_n;
    real_T rtb_TSamp;
    real_T rtb_TSamp_l;
    ZCEventType zcEvent;

    /* Sum: '<S3>/Add1' incorporates:
     *  RandomNumber: '<S4>/White Noise'
     */
    rtb_WhiteNoise = (real_T)ChargingAlgorithm_U.DataPipeline.Current_mA +
      ChargingAlgorithm_DW.NextOutput;

    /* SampleTimeMath: '<S8>/TSamp'
     *
     * About '<S8>/TSamp':
     *  y = u * K where K = 1 / ( w * Ts )
     */
    rtb_TSamp = (real_T)ChargingAlgorithm_U.DataPipeline.TerminalVoltage_mV *
      10.0;

    /* Sum: '<S8>/Diff' incorporates:
     *  UnitDelay: '<S8>/UD'
     */
    rtb_Diff = rtb_TSamp - ChargingAlgorithm_DW.UD_DSTATE;

    /* SampleTimeMath: '<S9>/TSamp'
     *
     * About '<S9>/TSamp':
     *  y = u * K where K = 1 / ( w * Ts )
     */
    rtb_TSamp_l = rtb_WhiteNoise * 10.0;

    /* Sum: '<S9>/Diff' incorporates:
     *  UnitDelay: '<S9>/UD'
     */
    rtb_Diff_n = rtb_TSamp_l - ChargingAlgorithm_DW.UD_DSTATE_f;

    /* Chart: '<S3>/Chart' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    if (ChargingAlgorithm_DW.is_active_c31_ChargingAlgorithm == 0U) {
      ChargingAlgorithm_DW.is_active_c31_ChargingAlgorithm = 1U;
      ChargingAlgorithm_DW.is_c31_ChargingAlgorithm =
        ChargingAlgorithm_IN_V1_Record;

      /* :  m = 2; */
      rtb_VoltageDiff_mV = 2;
    } else {
      switch (ChargingAlgorithm_DW.is_c31_ChargingAlgorithm) {
       case ChargingAlgorithm_IN_V1_Record:
        rtb_VoltageDiff_mV = 2;

        /* :  sf_internal_predicateOutput = VoltageDiff < -Thresholds.IR_VoltageDiff && CurrentDiff < -Thresholds.IR_CurrentDiff; */
        if ((rtb_Diff < -ChargingAlgorithm_U.Thresholds.IR_VoltageDiff) &&
            (rtb_Diff_n < -ChargingAlgorithm_U.Thresholds.IR_CurrentDiff)) {
          /* :  V1 = Voltage_mV; */
          ChargingAlgorithm_B.V1 =
            ChargingAlgorithm_U.DataPipeline.TerminalVoltage_mV;

          /* :  T1 = Clock; */
          /* :  I1 = Current_mA; */
          ChargingAlgorithm_B.I1 = rtb_WhiteNoise;
          ChargingAlgorithm_DW.is_c31_ChargingAlgorithm =
            ChargingAlgorithm_IN_V2_Record;

          /* :  m = 1; */
          rtb_VoltageDiff_mV = 1;
        }
        break;

       case ChargingAlgorithm_IN_V2_Record:
        rtb_VoltageDiff_mV = 1;

        /* :  sf_internal_predicateOutput = VoltageDiff > -Thresholds.IR_VoltageDiff && CurrentDiff <- Thresholds.IR_CurrentDiff; */
        if ((rtb_Diff > -ChargingAlgorithm_U.Thresholds.IR_VoltageDiff) &&
            (rtb_Diff_n < -ChargingAlgorithm_U.Thresholds.IR_CurrentDiff)) {
          /* :  V2 = Voltage_mV; */
          ChargingAlgorithm_B.V2 =
            ChargingAlgorithm_U.DataPipeline.TerminalVoltage_mV;

          /* :  I2 = Current_mA; */
          ChargingAlgorithm_B.I2 = rtb_WhiteNoise;
          ChargingAlgorithm_DW.is_c31_ChargingAlgorithm =
            ChargingAlgorithm_IN_V3_Record;

          /* :  m = 0; */
          rtb_VoltageDiff_mV = 0;
        }
        break;

       default:
        /* case IN_V3_Record: */
        rtb_VoltageDiff_mV = 0;

        /* :  sf_internal_predicateOutput = VoltageDiff >= Thresholds.IR_VoltageDiff && CurrentDiff >= Thresholds.IR_CurrentDiff; */
        if ((rtb_Diff >= ChargingAlgorithm_U.Thresholds.IR_VoltageDiff) &&
            (rtb_Diff_n >= ChargingAlgorithm_U.Thresholds.IR_CurrentDiff)) {
          /* :  V3 = Voltage_mV; */
          /* :  T3 = Clock; */
          /* :  I3 = Current_mA; */
          ChargingAlgorithm_DW.is_c31_ChargingAlgorithm =
            ChargingAlgorithm_IN_V1_Record;

          /* :  m = 2; */
          rtb_VoltageDiff_mV = 2;
        }
        break;
      }
    }

    /* End of Chart: '<S3>/Chart' */

    /* Outputs for Triggered SubSystem: '<S3>/Triggered Subsystem' incorporates:
     *  TriggerPort: '<S12>/Trigger'
     */
    zcEvent = rt_ZCFcn(FALLING_ZERO_CROSSING,
                       &ChargingAlgorithm_PrevZCX.TriggeredSubsystem_Trig_ZCE,
                       ((real_T)rtb_VoltageDiff_mV));
    if (zcEvent != NO_ZCEVENT) {
      /* Product: '<S12>/Divide' incorporates:
       *  Sum: '<S10>/Add'
       *  Sum: '<S10>/Add2'
       */
      ChargingAlgorithm_B.V1V2I = (real_T)(ChargingAlgorithm_B.V1 -
        ChargingAlgorithm_B.V2) / (ChargingAlgorithm_B.I1 -
        ChargingAlgorithm_B.I2);
    }

    /* End of Outputs for SubSystem: '<S3>/Triggered Subsystem' */

    /* Chart: '<S3>/Chart1' */
    if (ChargingAlgorithm_DW.is_active_c32_ChargingAlgorithm == 0U) {
      ChargingAlgorithm_DW.is_active_c32_ChargingAlgorithm = 1U;
      ChargingAlgorithm_DW.is_c32_ChargingAlgorithm =
        ChargingAlgorithm_IN_Rs_Lobby;
    } else {
      switch (ChargingAlgorithm_DW.is_c32_ChargingAlgorithm) {
       case ChargingAl_IN_Rs_Array_Creation:
        /* :  sf_internal_predicateOutput = m==0; */
        if (rtb_VoltageDiff_mV == 0) {
          ChargingAlgorithm_DW.is_c32_ChargingAlgorithm =
            ChargingAlgorithm_IN_Rs_Lobby;
        } else {
          /* :  sf_internal_predicateOutput = m==0 && i==5; */
          /* :  BatteryRs(i) = Rs; */
          ChargingAlgorithm_DW.BatteryRs[ChargingAlgorithm_B.i - 1] = (real32_T)
            ChargingAlgorithm_B.V1V2I;
        }
        break;

       case ChargingA_IN_Rs_Array_Creation1:
        break;

       default:
        /* case IN_Rs_Lobby: */
        /* :  sf_internal_predicateOutput = m == 1; */
        if (rtb_VoltageDiff_mV == 1) {
          ChargingAlgorithm_DW.is_c32_ChargingAlgorithm =
            ChargingAl_IN_Rs_Array_Creation;
        }
        break;
      }
    }

    /* End of Chart: '<S3>/Chart1' */

    /* Update for RandomNumber: '<S4>/White Noise' */
    ChargingAlgorithm_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw
      (&ChargingAlgorithm_DW.RandSeed);

    /* Update for UnitDelay: '<S8>/UD' */
    ChargingAlgorithm_DW.UD_DSTATE = rtb_TSamp;

    /* Update for UnitDelay: '<S9>/UD' */
    ChargingAlgorithm_DW.UD_DSTATE_f = rtb_TSamp_l;
  }

  /* End of Logic: '<S1>/AND' */
  /* End of Outputs for SubSystem: '<S1>/IR_Calculation' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++ChargingAlgorithm_M->Timing.clockTick0)) {
    ++ChargingAlgorithm_M->Timing.clockTickH0;
  }

  ChargingAlgorithm_M->Timing.t[0] = ChargingAlgorithm_M->Timing.clockTick0 *
    ChargingAlgorithm_M->Timing.stepSize0 +
    ChargingAlgorithm_M->Timing.clockTickH0 *
    ChargingAlgorithm_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.1s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.1, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    ChargingAlgorithm_M->Timing.clockTick1++;
    if (!ChargingAlgorithm_M->Timing.clockTick1) {
      ChargingAlgorithm_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void ChargingAlgorithm_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)ChargingAlgorithm_M, 0,
                sizeof(RT_MODEL_ChargingAlgorithm_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&ChargingAlgorithm_M->solverInfo,
                          &ChargingAlgorithm_M->Timing.simTimeStep);
    rtsiSetTPtr(&ChargingAlgorithm_M->solverInfo, &rtmGetTPtr
                (ChargingAlgorithm_M));
    rtsiSetStepSizePtr(&ChargingAlgorithm_M->solverInfo,
                       &ChargingAlgorithm_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&ChargingAlgorithm_M->solverInfo, (&rtmGetErrorStatus
      (ChargingAlgorithm_M)));
    rtsiSetRTModelPtr(&ChargingAlgorithm_M->solverInfo, ChargingAlgorithm_M);
  }

  rtsiSetSimTimeStep(&ChargingAlgorithm_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&ChargingAlgorithm_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(ChargingAlgorithm_M, &ChargingAlgorithm_M->Timing.tArray[0]);
  ChargingAlgorithm_M->Timing.stepSize0 = 0.1;

  /* block I/O */
  (void) memset(((void *) &ChargingAlgorithm_B), 0,
                sizeof(B_ChargingAlgorithm_T));

  /* states (dwork) */
  (void) memset((void *)&ChargingAlgorithm_DW, 0,
                sizeof(DW_ChargingAlgorithm_T));

  /* external inputs */
  (void)memset(&ChargingAlgorithm_U, 0, sizeof(ExtU_ChargingAlgorithm_T));
  ChargingAlgorithm_U.CC_Outputs = ChargingAlgorithm_rtZCC_Outputs;

  /* external outputs */
  (void)memset(&ChargingAlgorithm_Y, 0, sizeof(ExtY_ChargingAlgorithm_T));
  ChargingAlgorithm_Y.ChargingState = NoCharging;
  ChargingAlgorithm_PrevZCX.TriggeredSubsystem1_Trig_ZCE = POS_ZCSIG;
  ChargingAlgorithm_PrevZCX.TriggeredSubsystem_Trig_ZCE = UNINITIALIZED_ZCSIG;

  /* SystemInitialize for Chart: '<S1>/ChargingAlgorithm' */
  ChargingAlgorithm_DW.is_IR_Measurement = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.temporalCounter_i1 = 0U;
  ChargingAlgorithm_DW.is_NormalCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.is_FastCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.is_SlowCharging = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.is_active_c5_ChargingAlgorithm = 0U;
  ChargingAlgorithm_DW.is_c5_ChargingAlgorithm = ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_DW.Recorded_CycleCount = 0.0F;
  ChargingAlgorithm_B.Constant_K = 0.0F;

  /* SystemInitialize for Outport: '<Root>/ChargingState' incorporates:
   *  Chart: '<S1>/ChargingAlgorithm'
   */
  ChargingAlgorithm_Y.ChargingState = NoCharging;

  /* SystemInitialize for Chart: '<S1>/ChargingAlgorithm' */
  ChargingAlgorithm_B.PseudoChargeCurrent = 0U;
  ChargingAlgorithm_B.IR_Current = 0U;
  ChargingAlgorithm_B.IR_Calculation = false;
  ChargingAlgorithm_B.i = 0U;

  /* SystemInitialize for Enabled SubSystem: '<S1>/IR_Calculation' */
  /* InitializeConditions for RandomNumber: '<S4>/White Noise' */
  ChargingAlgorithm_DW.RandSeed = 1529675776U;
  ChargingAlgorithm_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw
    (&ChargingAlgorithm_DW.RandSeed);

  /* InitializeConditions for UnitDelay: '<S8>/UD' */
  ChargingAlgorithm_DW.UD_DSTATE = 0.0;

  /* InitializeConditions for UnitDelay: '<S9>/UD' */
  ChargingAlgorithm_DW.UD_DSTATE_f = 0.0;

  /* SystemInitialize for Chart: '<S3>/Chart' */
  ChargingAlgorithm_DW.is_active_c31_ChargingAlgorithm = 0U;
  ChargingAlgorithm_DW.is_c31_ChargingAlgorithm =
    ChargingAlgo_IN_NO_ACTIVE_CHILD;
  ChargingAlgorithm_B.V1 = 0;
  ChargingAlgorithm_B.V2 = 0;
  ChargingAlgorithm_B.I1 = 0.0;
  ChargingAlgorithm_B.I2 = 0.0;

  /* SystemInitialize for Triggered SubSystem: '<S3>/Triggered Subsystem' */
  /* SystemInitialize for Product: '<S12>/Divide' incorporates:
   *  Outport: '<S12>/Rs'
   */
  ChargingAlgorithm_B.V1V2I = 0.0;

  /* End of SystemInitialize for SubSystem: '<S3>/Triggered Subsystem' */

  /* SystemInitialize for Chart: '<S3>/Chart1' */
  ChargingAlgorithm_DW.is_active_c32_ChargingAlgorithm = 0U;
  ChargingAlgorithm_DW.is_c32_ChargingAlgorithm =
    ChargingAlgo_IN_NO_ACTIVE_CHILD;
  memset(&ChargingAlgorithm_DW.BatteryRs[0], 0, 20U * sizeof(real32_T));

  /* InitializeConditions for Outport: '<Root>/Avg_Rs' incorporates:
   *  Chart: '<S3>/Chart1'
   */
  ChargingAlgorithm_Y.Avg_Rs = 0.0F;

  /* End of SystemInitialize for SubSystem: '<S1>/IR_Calculation' */
}

/* Model terminate function */
void ChargingAlgorithm_terminate(void)
{
  /* (no terminate code required) */
}
