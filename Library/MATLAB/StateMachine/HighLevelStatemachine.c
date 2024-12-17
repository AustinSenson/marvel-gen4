/*
 * HighLevelStatemachine.c
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

#include "HighLevelStatemachine.h"
#include "rtwtypes.h"
#include "HighLevelStatemachine_types.h"
#include <string.h>

/* Named constants for Chart: '<Root>/HighLevelStatemachine' */
#define HighLevelSt_IN_PermanentFailure ((uint8_T)2U)
#define HighLevelSta_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define HighLevelState_IN_DeepSleepMode ((uint8_T)7U)
#define HighLevelStatem_IN_FastCharging ((uint8_T)3U)
#define HighLevelStatem_IN_LowPowerMode ((uint8_T)8U)
#define HighLevelStatem_IN_Regeneration ((uint8_T)5U)
#define HighLevelStatem_IN_SlowCharging ((uint8_T)6U)
#define HighLevelStatema_IN_Discharging ((uint8_T)2U)
#define HighLevelStatema_IN_ServiceMode ((uint8_T)9U)
#define HighLevelStatemac_IN_Activating ((uint8_T)1U)
#define HighLevelStatemac_IN_Fault_Mode ((uint8_T)1U)
#define HighLevelStatemac_IN_Protection ((uint8_T)3U)
#define HighLevelStatemachi_IN_LimpHome ((uint8_T)1U)
#define HighLevelStatemachine_IN_Active ((uint8_T)2U)
#define HighLevelStatemachine_IN_Error ((uint8_T)3U)
#define HighLevelStatemachine_IN_Idle  ((uint8_T)4U)
#define HighLevelStatemachine_IN_Ready ((uint8_T)5U)

/* Block states (default storage) */
DW_HighLevelStatemachine_T HighLevelStatemachine_DW;

/* External inputs (root inport signals with default storage) */
ExtU_HighLevelStatemachine_T HighLevelStatemachine_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_HighLevelStatemachine_T HighLevelStatemachine_Y;

/* Real-time model */
static RT_MODEL_HighLevelStatemachin_T HighLevelStatemachine_M_;
RT_MODEL_HighLevelStatemachin_T *const HighLevelStatemachine_M =
  &HighLevelStatemachine_M_;

/* Forward declaration for local functions */
static void HighLevelStatemachin_Fault_Mode(void);
static void HighLevelStatemach_FastCharging(void);
static void HighLevelStatemach_SlowCharging(void);
static void HighLevel_HighLevelStatemachine(void);
static void HighLevelStatemachine_BMS_State(void);
const ProtectionState_Out HighLevelStatemachine_rtZProtec = { NoError,/* ThermalRunaway */
  NoError,                             /* TemperatureGradient */
  NoError,                             /* HighImbalanceFlag */
  NoError,                             /* ShortCircuitDetect */
  NoError,                             /* SuddenVoltageDrop */
  NoError,                             /* OV */
  NoError,                             /* UV */
  NoError,                             /* OCC */
  NoError,                             /* OCD */
  0.0F,                                /* i2t_Calculated_A2sec */
  NoError,                             /* eFuseChargeFlag */
  NoError,                             /* eFuseDischargeFlag */
  NoErr,                               /* TempState1 */
  NoErr,                               /* TempState2 */
  NoError,                             /* FlagGroup1 */
  NoError,                             /* FlagGroup2 */
  NoError,                             /* TempOverallState */
  NoError,                             /* ErrorDetect */
  NoError,                             /* DCLI_CurrentFlag */
  NoError                              /* DCLO_CurrentFlag */
};

/* Function for Chart: '<Root>/HighLevelStatemachine' */
static void HighLevelStatemachin_Fault_Mode(void)
{
  switch (HighLevelStatemachine_DW.is_Fault_Mode) {
   case HighLevelStatemachi_IN_LimpHome:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.VCUPermanentFail ==1, msec)> Thresholds.FaultsModeTimeout_msec; */
    if (!HighLevelStatemachine_U.DataPipeline.VCU.VCUPermanentFail) {
      HighLevelStatemachine_DW.durationCounter_2_n = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_2_n
         * 100) > HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_1_k = 0U;
      HighLevelStatemachine_DW.is_Fault_Mode = HighLevelSt_IN_PermanentFailure;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.PermanentFailFault; */
      HighLevelStatemachine_Y.HighLevelStateMode = PermanentFailFault;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
        HighLevelStatemachine_DW.durationCounter_1_o = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_1_o * 100)
          > HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
        HighLevelStatemachine_DW.is_Fault_Mode = HighLevelSta_IN_NO_ACTIVE_CHILD;
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatema_IN_ServiceMode;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ServiceMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.LimpHomeFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = LimpHomeFault;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      }
    }
    break;

   case HighLevelSt_IN_PermanentFailure:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
    if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
      HighLevelStatemachine_DW.durationCounter_1_k = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_1_k
         * 100) > HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
      HighLevelStatemachine_DW.is_Fault_Mode = HighLevelSta_IN_NO_ACTIVE_CHILD;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatema_IN_ServiceMode;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.ServiceMode; */
      HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.PermanentFailFault; */
      HighLevelStatemachine_Y.HighLevelStateMode = PermanentFailFault;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    }
    break;

   default:
    /* Inport: '<Root>/DataPipeline' */
    /* case IN_Protection: */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.TransmissionFail ==1 , msec)> Thresholds.FaultsModeTimeout_msec; */
    if (!HighLevelStatemachine_U.DataPipeline.VCU.TransmissionFail) {
      HighLevelStatemachine_DW.durationCounter_2_c = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_2_c
         * 100) > HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_2_n = 0U;
      HighLevelStatemachine_DW.durationCounter_1_o = 0U;
      HighLevelStatemachine_DW.is_Fault_Mode = HighLevelStatemachi_IN_LimpHome;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.LimpHomeFault; */
      HighLevelStatemachine_Y.HighLevelStateMode = LimpHomeFault;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
      if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
        HighLevelStatemachine_DW.durationCounter_1_b = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_1_b * 100)
          > HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
        HighLevelStatemachine_DW.is_Fault_Mode = HighLevelSta_IN_NO_ACTIVE_CHILD;
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatema_IN_ServiceMode;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ServiceMode; */
        HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ProtectionFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      }
    }
    break;
  }
}

/* Function for Chart: '<Root>/HighLevelStatemachine' */
static void HighLevelStatemach_FastCharging(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.SlowCharge == 1, msec)>Thresholds.NormalModeTimeout_msec; */
  if (!HighLevelStatemachine_U.DataPipeline.VCU.SlowCharge) {
    HighLevelStatemachine_DW.durationCounter_4_m = 0U;
  }

  /* Inport: '<Root>/Thresholds' */
  if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_4_m *
       100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4_f = 0U;
    HighLevelStatemachine_DW.durationCounter_3_g = 0U;
    HighLevelStatemachine_DW.durationCounter_2_e = 0U;
    HighLevelStatemachine_DW.durationCounter_1_g2 = 0U;
    HighLevelStatemachine_DW.is_HighLevelStatemachine =
      HighLevelStatem_IN_SlowCharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelStateMode = HighLevelMode.ChargingSlow; */
    HighLevelStatemachine_Y.HighLevelStateMode = ChargingSlow;
    HighLevelStatemachine_DW.durationCounter_2 = 0U;
    HighLevelStatemachine_DW.durationCounter_1_e = 0U;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
        HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
      HighLevelStatemachine_DW.durationCounter_3_i = 0U;
    }

    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_3_i
         * 100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_n = 0U;
      HighLevelStatemachine_DW.durationCounter_3 = 0U;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1 = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatemachine_IN_Idle;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.Idle; */
      HighLevelStatemachine_Y.HighLevelStateMode = Idle;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
      if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
        HighLevelStatemachine_DW.durationCounter_2_fx = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_2_fx * 100)
          > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_4 = 0U;
        HighLevelStatemachine_DW.durationCounter_3_j = 0U;
        HighLevelStatemachine_DW.durationCounter_2_f = 0U;
        HighLevelStatemachine_DW.durationCounter_1_g = 0U;
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatema_IN_Discharging;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.Discharging; */
        HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Inport: '<Root>/ProtectionOutput' */
        /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
        if ((int32_T)HighLevelStatemachine_U.ProtectionOutput.ErrorDetect != 1)
        {
          HighLevelStatemachine_DW.durationCounter_1_m = 0U;
        }

        /* End of Inport: '<Root>/ProtectionOutput' */
        if ((real_T)(uint32_T)((int32_T)
                               HighLevelStatemachine_DW.durationCounter_1_m *
                               100) >
            HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
          HighLevelStatemachine_DW.is_HighLevelStatemachine =
            HighLevelStatemac_IN_Fault_Mode;
          HighLevelStatemachine_DW.durationCounter_2_c = 0U;
          HighLevelStatemachine_DW.durationCounter_1_b = 0U;
          HighLevelStatemachine_DW.is_Fault_Mode =
            HighLevelStatemac_IN_Protection;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelStateMode = HighLevelMode.ProtectionFault; */
          HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
          HighLevelStatemachine_DW.durationCounter_2 = 0U;
          HighLevelStatemachine_DW.durationCounter_1_e = 0U;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<Root>/HighLevelStatemachine' */
static void HighLevelStatemach_SlowCharging(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.FastCharge == 1, msec)>Thresholds.NormalModeTimeout_msec; */
  if (!HighLevelStatemachine_U.DataPipeline.VCU.FastCharge) {
    HighLevelStatemachine_DW.durationCounter_4_f = 0U;
  }

  /* Inport: '<Root>/Thresholds' */
  if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_4_f *
       100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
    HighLevelStatemachine_DW.durationCounter_4_m = 0U;
    HighLevelStatemachine_DW.durationCounter_3_i = 0U;
    HighLevelStatemachine_DW.durationCounter_2_fx = 0U;
    HighLevelStatemachine_DW.durationCounter_1_m = 0U;
    HighLevelStatemachine_DW.is_HighLevelStatemachine =
      HighLevelStatem_IN_FastCharging;

    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  HighLevelStateMode = HighLevelMode.ChargingFast; */
    HighLevelStatemachine_Y.HighLevelStateMode = ChargingFast;
    HighLevelStatemachine_DW.durationCounter_2 = 0U;
    HighLevelStatemachine_DW.durationCounter_1_e = 0U;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
        HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
      HighLevelStatemachine_DW.durationCounter_3_g = 0U;
    }

    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_3_g
         * 100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_n = 0U;
      HighLevelStatemachine_DW.durationCounter_3 = 0U;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1 = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatemachine_IN_Idle;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.Idle; */
      HighLevelStatemachine_Y.HighLevelStateMode = Idle;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/ProtectionOutput' */
      /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
      if ((int32_T)HighLevelStatemachine_U.ProtectionOutput.ErrorDetect != 1) {
        HighLevelStatemachine_DW.durationCounter_1_g2 = 0U;
      }

      /* End of Inport: '<Root>/ProtectionOutput' */
      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_1_g2 * 100)
          > HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatemac_IN_Fault_Mode;
        HighLevelStatemachine_DW.durationCounter_2_c = 0U;
        HighLevelStatemachine_DW.durationCounter_1_b = 0U;
        HighLevelStatemachine_DW.is_Fault_Mode = HighLevelStatemac_IN_Protection;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ProtectionFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
        if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
          HighLevelStatemachine_DW.durationCounter_2_e = 0U;
        }

        if ((real_T)(uint32_T)((int32_T)
                               HighLevelStatemachine_DW.durationCounter_2_e *
                               100) >
            HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_4 = 0U;
          HighLevelStatemachine_DW.durationCounter_3_j = 0U;
          HighLevelStatemachine_DW.durationCounter_2_f = 0U;
          HighLevelStatemachine_DW.durationCounter_1_g = 0U;
          HighLevelStatemachine_DW.is_HighLevelStatemachine =
            HighLevelStatema_IN_Discharging;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelStateMode = HighLevelMode.Discharging; */
          HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
          HighLevelStatemachine_DW.durationCounter_2 = 0U;
          HighLevelStatemachine_DW.durationCounter_1_e = 0U;
        }
      }
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/* Function for Chart: '<Root>/HighLevelStatemachine' */
static void HighLevel_HighLevelStatemachine(void)
{
  switch (HighLevelStatemachine_DW.is_HighLevelStatemachine) {
   case HighLevelStatemac_IN_Fault_Mode:
    HighLevelStatemachin_Fault_Mode();
    break;

   case HighLevelStatema_IN_Discharging:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==1, msec)>Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1) {
      HighLevelStatemachine_DW.durationCounter_3_j = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_3_j
         * 100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4_f = 0U;
      HighLevelStatemachine_DW.durationCounter_3_g = 0U;
      HighLevelStatemachine_DW.durationCounter_2_e = 0U;
      HighLevelStatemachine_DW.durationCounter_1_g2 = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatem_IN_SlowCharging;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.ChargingSlow; */
      HighLevelStatemachine_Y.HighLevelStateMode = ChargingSlow;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.Current_mA < Thresholds.ChargeDetectionThreshold_mA, msec) > Thresholds.NormalModeTimeout_msec; */
      if (HighLevelStatemachine_U.DataPipeline.Current_mA >=
          HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
        HighLevelStatemachine_DW.durationCounter_2_f = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_2_f * 100)
          > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_4_n = 0U;
        HighLevelStatemachine_DW.durationCounter_3 = 0U;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1 = 0U;
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatemachine_IN_Idle;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.Idle; */
        HighLevelStatemachine_Y.HighLevelStateMode = Idle;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration((DataPipeline.VCU.isChargerConnected == 0 && DataPipeline.isChargeEn ==1), msec)>Thresholds.NormalModeTimeout_msec; */
        if (HighLevelStatemachine_U.DataPipeline.VCU.isChargerConnected ||
            (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1)) {
          HighLevelStatemachine_DW.durationCounter_4 = 0U;
        }

        if ((real_T)(uint32_T)((int32_T)
                               HighLevelStatemachine_DW.durationCounter_4 * 100)
            > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_2_b = 0U;
          HighLevelStatemachine_DW.durationCounter_1_h = 0U;
          HighLevelStatemachine_DW.is_HighLevelStatemachine =
            HighLevelStatem_IN_Regeneration;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelStateMode = HighLevelMode.Regeneration; */
          HighLevelStatemachine_Y.HighLevelStateMode = Regeneration;
          HighLevelStatemachine_DW.durationCounter_2 = 0U;
          HighLevelStatemachine_DW.durationCounter_1_e = 0U;
        } else {
          /* Inport: '<Root>/ProtectionOutput' */
          /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
          if ((int32_T)HighLevelStatemachine_U.ProtectionOutput.ErrorDetect != 1)
          {
            HighLevelStatemachine_DW.durationCounter_1_g = 0U;
          }

          if ((real_T)(uint32_T)((int32_T)
                                 HighLevelStatemachine_DW.durationCounter_1_g *
                                 100) >
              HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
            HighLevelStatemachine_DW.is_HighLevelStatemachine =
              HighLevelStatemac_IN_Fault_Mode;
            HighLevelStatemachine_DW.durationCounter_2_c = 0U;
            HighLevelStatemachine_DW.durationCounter_1_b = 0U;
            HighLevelStatemachine_DW.is_Fault_Mode =
              HighLevelStatemac_IN_Protection;

            /* Outport: '<Root>/HighLevelStateMode' */
            /* :  HighLevelStateMode = HighLevelMode.ProtectionFault; */
            HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
            HighLevelStatemachine_DW.durationCounter_2 = 0U;
            HighLevelStatemachine_DW.durationCounter_1_e = 0U;
          }
        }
      }
    }
    break;

   case HighLevelStatem_IN_FastCharging:
    HighLevelStatemach_FastCharging();
    break;

   case HighLevelStatemachine_IN_Idle:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==0, msec)>Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
      HighLevelStatemachine_DW.durationCounter_3 = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_3 *
         100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4 = 0U;
      HighLevelStatemachine_DW.durationCounter_3_j = 0U;
      HighLevelStatemachine_DW.durationCounter_2_f = 0U;
      HighLevelStatemachine_DW.durationCounter_1_g = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatema_IN_Discharging;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.Discharging; */
      HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn ==1, msec)>Thresholds.NormalModeTimeout_msec; */
      if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 1) {
        HighLevelStatemachine_DW.durationCounter_4_n = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_4_n * 100)
          > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
        HighLevelStatemachine_DW.durationCounter_4_f = 0U;
        HighLevelStatemachine_DW.durationCounter_3_g = 0U;
        HighLevelStatemachine_DW.durationCounter_2_e = 0U;
        HighLevelStatemachine_DW.durationCounter_1_g2 = 0U;
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatem_IN_SlowCharging;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ChargingSlow; */
        HighLevelStatemachine_Y.HighLevelStateMode = ChargingSlow;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      } else {
        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  sf_internal_predicateOutput = duration(HighLevelStateMode == HighLevelMode.Idle, msec) > Thresholds.PowerSaverModeTimeout_msec; */
        if (HighLevelStatemachine_Y.HighLevelStateMode != Idle) {
          HighLevelStatemachine_DW.durationCounter_2 = 0U;
        }

        if ((real_T)(uint32_T)((int32_T)
                               HighLevelStatemachine_DW.durationCounter_2 * 100)
            > HighLevelStatemachine_U.Thresholds.PowerSaverModeTimeout_msec) {
          HighLevelStatemachine_DW.durationCounter_1_e = 0U;
          HighLevelStatemachine_DW.is_HighLevelStatemachine =
            HighLevelStatem_IN_LowPowerMode;

          /* Outport: '<Root>/HighLevelStateMode' */
          /* :  HighLevelStateMode = HighLevelMode.LowPowerMode; */
          HighLevelStatemachine_Y.HighLevelStateMode = LowPowerMode;
          HighLevelStatemachine_DW.durationCounter_2 = 0U;
        } else {
          /* Inport: '<Root>/DataPipeline' */
          /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
          if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
            HighLevelStatemachine_DW.durationCounter_1 = 0U;
          }

          if ((real_T)(uint32_T)((int32_T)
                                 HighLevelStatemachine_DW.durationCounter_1 *
                                 100) >
              HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
            HighLevelStatemachine_DW.is_HighLevelStatemachine =
              HighLevelStatema_IN_ServiceMode;

            /* Outport: '<Root>/HighLevelStateMode' */
            /* :  HighLevelStateMode = HighLevelMode.ServiceMode; */
            HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
            HighLevelStatemachine_DW.durationCounter_2 = 0U;
            HighLevelStatemachine_DW.durationCounter_1_e = 0U;
          }
        }
      }
    }
    break;

   case HighLevelStatem_IN_Regeneration:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.isChargeEn == 0, msec)> Thresholds.NormalModeTimeout_msec; */
    if (HighLevelStatemachine_U.DataPipeline.isChargeEn != 0) {
      HighLevelStatemachine_DW.durationCounter_2_b = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_2_b
         * 100) > HighLevelStatemachine_U.Thresholds.NormalModeTimeout_msec) {
      HighLevelStatemachine_DW.durationCounter_4 = 0U;
      HighLevelStatemachine_DW.durationCounter_3_j = 0U;
      HighLevelStatemachine_DW.durationCounter_2_f = 0U;
      HighLevelStatemachine_DW.durationCounter_1_g = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatema_IN_Discharging;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.Discharging; */
      HighLevelStatemachine_Y.HighLevelStateMode = Discharging;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    } else {
      /* Inport: '<Root>/ProtectionOutput' */
      /* :  sf_internal_predicateOutput = duration(ProtectionOutput.ErrorDetect ==1,msec)>Thresholds.FaultsModeTimeout_msec; */
      if ((int32_T)HighLevelStatemachine_U.ProtectionOutput.ErrorDetect != 1) {
        HighLevelStatemachine_DW.durationCounter_1_h = 0U;
      }

      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_1_h * 100)
          > HighLevelStatemachine_U.Thresholds.FaultsModeTimeout_msec) {
        HighLevelStatemachine_DW.is_HighLevelStatemachine =
          HighLevelStatemac_IN_Fault_Mode;
        HighLevelStatemachine_DW.durationCounter_2_c = 0U;
        HighLevelStatemachine_DW.durationCounter_1_b = 0U;
        HighLevelStatemachine_DW.is_Fault_Mode = HighLevelStatemac_IN_Protection;

        /* Outport: '<Root>/HighLevelStateMode' */
        /* :  HighLevelStateMode = HighLevelMode.ProtectionFault; */
        HighLevelStatemachine_Y.HighLevelStateMode = ProtectionFault;
        HighLevelStatemachine_DW.durationCounter_2 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_e = 0U;
      }
    }
    break;

   case HighLevelStatem_IN_SlowCharging:
    HighLevelStatemach_SlowCharging();
    break;

   case HighLevelState_IN_DeepSleepMode:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.VCU.ServiceMode==1,msec)>Thresholds.ServiceModeTimeout_msec; */
    if (!HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
      HighLevelStatemachine_DW.durationCounter_1_kf = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)
                           HighLevelStatemachine_DW.durationCounter_1_kf * 100) >
        HighLevelStatemachine_U.Thresholds.ServiceModeTimeout_msec) {
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelStatema_IN_ServiceMode;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.ServiceMode; */
      HighLevelStatemachine_Y.HighLevelStateMode = ServiceMode;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    }
    break;

   case HighLevelStatem_IN_LowPowerMode:
    /* Outport: '<Root>/HighLevelStateMode' */
    /* :  sf_internal_predicateOutput = duration(HighLevelStateMode == HighLevelMode.LowPowerMode, msec) > Thresholds.PowerSaverModeTimeout_msec; */
    if (HighLevelStatemachine_Y.HighLevelStateMode != LowPowerMode) {
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    }

    /* Inport: '<Root>/Thresholds' */
    if ((real_T)(uint32_T)((int32_T)HighLevelStatemachine_DW.durationCounter_1_e
         * 100) > HighLevelStatemachine_U.Thresholds.PowerSaverModeTimeout_msec)
    {
      HighLevelStatemachine_DW.durationCounter_1_kf = 0U;
      HighLevelStatemachine_DW.is_HighLevelStatemachine =
        HighLevelState_IN_DeepSleepMode;

      /* Outport: '<Root>/HighLevelStateMode' */
      /* :  HighLevelStateMode = HighLevelMode.DeepSleepMode; */
      HighLevelStatemachine_Y.HighLevelStateMode = DeepSleepMode;
      HighLevelStatemachine_DW.durationCounter_2 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    }
    break;

   default:
    /* case IN_ServiceMode: */
    break;
  }
}

/* Function for Chart: '<Root>/HighLevelStatemachine' */
static void HighLevelStatemachine_BMS_State(void)
{
  switch (HighLevelStatemachine_DW.is_BMS_State) {
   case HighLevelStatemac_IN_Activating:
    /* SystemInitialize for Outport: '<Root>/BMS_State' */
    HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVATING;

    /* Inport: '<Root>/ContactorsState' */
    /* :  sf_internal_predicateOutput = ContactorsState == 5; */
    if ((int32_T)HighLevelStatemachine_U.ContactorsState == 5) {
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Active;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ACTIVE; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVE;
    } else {
      /* Inport: '<Root>/ContactorCommand' */
      /* :  sf_internal_predicateOutput = duration(ContactorCommand==1&& (ContactorsState==0 || ContactorsState==8),msec)>Thresholds.BMS_ActivatingToError_Timeout_msec; */
      if ((HighLevelStatemachine_U.ContactorCommand != 1) || (((int32_T)
            HighLevelStatemachine_U.ContactorsState != 0) && ((int32_T)
            HighLevelStatemachine_U.ContactorsState != 8))) {
        HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
      }

      /* Inport: '<Root>/Thresholds' */
      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_2_f0 * 100)
          >
          HighLevelStatemachine_U.Thresholds.BMS_ActivatingToError_Timeout_msec)
      {
        HighLevelStatemachine_DW.durationCounter_1_e2 = 0U;
        HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Error;

        /* SystemInitialize for Outport: '<Root>/BMS_State' */
        /* :  BMS_State = BMSState.MARVEL_ERROR; */
        HighLevelStatemachine_Y.BMS_State = MARVEL_ERROR;
      } else {
        /* Inport: '<Root>/ContactorCommand' */
        /* :  sf_internal_predicateOutput = duration((ContactorCommand==0&& ContactorsState==0),msec)>Thresholds.BMS_ActivatingToError_Timeout_msec; */
        if ((HighLevelStatemachine_U.ContactorCommand != 0) || ((int32_T)
             HighLevelStatemachine_U.ContactorsState != 0)) {
          HighLevelStatemachine_DW.durationCounter_1_br = 0U;
        }

        if ((real_T)(uint32_T)((int32_T)
                               HighLevelStatemachine_DW.durationCounter_1_br *
                               100) >
            HighLevelStatemachine_U.Thresholds.BMS_ActivatingToError_Timeout_msec)
        {
          HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Ready;

          /* SystemInitialize for Outport: '<Root>/BMS_State' */
          /* :  BMS_State = BMSState.MARVEL_READY; */
          HighLevelStatemachine_Y.BMS_State = MARVEL_READY;
        }
      }
    }
    break;

   case HighLevelStatemachine_IN_Active:
    /* SystemInitialize for Outport: '<Root>/BMS_State' */
    HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVE;

    /* Inport: '<Root>/ContactorCommand' incorporates:
     *  Inport: '<Root>/ContactorsState'
     */
    /* :  sf_internal_predicateOutput = ContactorCommand==1&&(ContactorsState==0||ContactorsState==8); */
    if ((HighLevelStatemachine_U.ContactorCommand == 1) && (((int32_T)
          HighLevelStatemachine_U.ContactorsState == 0) || ((int32_T)
          HighLevelStatemachine_U.ContactorsState == 8))) {
      HighLevelStatemachine_DW.durationCounter_1_e2 = 0U;
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Error;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ERROR; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ERROR;

      /* :  sf_internal_predicateOutput = ContactorCommand==0&&ContactorsState~=5; */
    } else if ((HighLevelStatemachine_U.ContactorCommand == 0) && ((int32_T)
                HighLevelStatemachine_U.ContactorsState != 5)) {
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Ready;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_READY; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_READY;

      /* :  sf_internal_predicateOutput = ContactorsState<6 && ContactorCommand==1 && ContactorsState~=5; */
    } else if (((int32_T)HighLevelStatemachine_U.ContactorsState < 6) &&
               (HighLevelStatemachine_U.ContactorCommand == 1) && ((int32_T)
                HighLevelStatemachine_U.ContactorsState != 5)) {
      HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_br = 0U;
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemac_IN_Activating;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ACTIVATING; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVATING;
    }
    break;

   case HighLevelStatemachine_IN_Error:
    /* SystemInitialize for Outport: '<Root>/BMS_State' */
    HighLevelStatemachine_Y.BMS_State = MARVEL_ERROR;

    /* Inport: '<Root>/ProtectionOutput' incorporates:
     *  Inport: '<Root>/ContactorCommand'
     *  Inport: '<Root>/ContactorsState'
     */
    /* :  sf_internal_predicateOutput = ProtectionOutput.ErrorDetect == ProtectionFlags.NoError && ContactorsState == 0 && ContactorCommand==0; */
    if ((HighLevelStatemachine_U.ProtectionOutput.ErrorDetect == NoError) &&
        ((int32_T)HighLevelStatemachine_U.ContactorsState == 0) &&
        (HighLevelStatemachine_U.ContactorCommand == 0)) {
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Ready;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_READY; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_READY;
    } else {
      /* :  sf_internal_predicateOutput = duration((ContactorCommand==1&&ContactorsState>0&&ContactorsState<=6),msec)>Thresholds.BMS_ErrorToActivating_Timeout_msec; */
      if ((HighLevelStatemachine_U.ContactorCommand != 1) || ((int32_T)
           HighLevelStatemachine_U.ContactorsState <= 0) || ((int32_T)
           HighLevelStatemachine_U.ContactorsState > 6)) {
        HighLevelStatemachine_DW.durationCounter_1_e2 = 0U;
      }

      /* Inport: '<Root>/Thresholds' */
      if ((real_T)(uint32_T)((int32_T)
                             HighLevelStatemachine_DW.durationCounter_1_e2 * 100)
          >
          HighLevelStatemachine_U.Thresholds.BMS_ErrorToActivating_Timeout_msec)
      {
        HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
        HighLevelStatemachine_DW.durationCounter_1_br = 0U;
        HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemac_IN_Activating;

        /* SystemInitialize for Outport: '<Root>/BMS_State' */
        /* :  BMS_State = BMSState.MARVEL_ACTIVATING; */
        HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVATING;
      }
    }
    break;

   case HighLevelStatemachine_IN_Idle:
    /* Inport: '<Root>/ContactorCommand' */
    /* :  sf_internal_predicateOutput = ContactorCommand==0; */
    if (HighLevelStatemachine_U.ContactorCommand == 0) {
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Ready;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_READY; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_READY;

      /* :  sf_internal_predicateOutput = ContactorCommand==1; */
    } else if (HighLevelStatemachine_U.ContactorCommand == 1) {
      HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_br = 0U;
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemac_IN_Activating;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ACTIVATING; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVATING;
    }
    break;

   default:
    /* SystemInitialize for Outport: '<Root>/BMS_State' */
    /* case IN_Ready: */
    HighLevelStatemachine_Y.BMS_State = MARVEL_READY;

    /* Inport: '<Root>/ContactorsState' incorporates:
     *  Inport: '<Root>/ContactorCommand'
     */
    /* :  sf_internal_predicateOutput = ContactorsState > 0 && ContactorCommand==1; */
    if (((int32_T)HighLevelStatemachine_U.ContactorsState > 0) &&
        (HighLevelStatemachine_U.ContactorCommand == 1)) {
      HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
      HighLevelStatemachine_DW.durationCounter_1_br = 0U;
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemac_IN_Activating;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ACTIVATING; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ACTIVATING;

      /* Inport: '<Root>/ProtectionOutput' */
      /* :  sf_internal_predicateOutput = ProtectionOutput.ErrorDetect == ProtectionFlags.Error || ContactorsState >6; */
    } else if ((HighLevelStatemachine_U.ProtectionOutput.ErrorDetect == Error) ||
               ((int32_T)HighLevelStatemachine_U.ContactorsState > 6)) {
      HighLevelStatemachine_DW.durationCounter_1_e2 = 0U;
      HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Error;

      /* SystemInitialize for Outport: '<Root>/BMS_State' */
      /* :  BMS_State = BMSState.MARVEL_ERROR; */
      HighLevelStatemachine_Y.BMS_State = MARVEL_ERROR;
    }
    break;
  }
}

/* Model step function */
void HighLevelStatemachine_step(void)
{
  /* Chart: '<Root>/HighLevelStatemachine' incorporates:
   *  Inport: '<Root>/ContactorCommand'
   *  Inport: '<Root>/ContactorsState'
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/ProtectionOutput'
   *  Inport: '<Root>/Thresholds'
   *  Outport: '<Root>/HighLevelStateMode'
   */
  if (HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach == 0U) {
    HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach = 1U;
    HighLevelStatemachine_DW.durationCounter_4_n = 0U;
    HighLevelStatemachine_DW.durationCounter_3 = 0U;
    HighLevelStatemachine_DW.durationCounter_2 = 0U;
    HighLevelStatemachine_DW.durationCounter_1 = 0U;
    HighLevelStatemachine_DW.is_HighLevelStatemachine =
      HighLevelStatemachine_IN_Idle;

    /* :  HighLevelStateMode = HighLevelMode.Idle; */
    HighLevelStatemachine_Y.HighLevelStateMode = Idle;
    HighLevelStatemachine_DW.durationCounter_1_e = 0U;
    HighLevelStatemachine_DW.is_BMS_State = HighLevelStatemachine_IN_Idle;
  } else {
    HighLevel_HighLevelStatemachine();
    HighLevelStatemachine_BMS_State();
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
    HighLevelStatemachine_DW.durationCounter_1++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1 = 0U;
  }

  if ((int32_T)HighLevelStatemachine_U.ProtectionOutput.ErrorDetect == 1) {
    HighLevelStatemachine_DW.durationCounter_1_g++;
    HighLevelStatemachine_DW.durationCounter_1_m++;
    HighLevelStatemachine_DW.durationCounter_1_g2++;
    HighLevelStatemachine_DW.durationCounter_1_h++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_g = 0U;
    HighLevelStatemachine_DW.durationCounter_1_m = 0U;
    HighLevelStatemachine_DW.durationCounter_1_g2 = 0U;
    HighLevelStatemachine_DW.durationCounter_1_h = 0U;
  }

  if (HighLevelStatemachine_Y.HighLevelStateMode == Idle) {
    HighLevelStatemachine_DW.durationCounter_2++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2 = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.ServiceMode) {
    HighLevelStatemachine_DW.durationCounter_1_k++;
    HighLevelStatemachine_DW.durationCounter_1_kf++;
    HighLevelStatemachine_DW.durationCounter_1_b++;
    HighLevelStatemachine_DW.durationCounter_1_o++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_k = 0U;
    HighLevelStatemachine_DW.durationCounter_1_kf = 0U;
    HighLevelStatemachine_DW.durationCounter_1_b = 0U;
    HighLevelStatemachine_DW.durationCounter_1_o = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_3++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3 = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_2_f++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_f = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_2_fx++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_fx = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_3_i++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_i = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1) {
    HighLevelStatemachine_DW.durationCounter_3_j++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_j = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_2_e++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_e = 0U;
  }

  if ((!HighLevelStatemachine_U.DataPipeline.VCU.isChargerConnected) &&
      (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1)) {
    HighLevelStatemachine_DW.durationCounter_4++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4 = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 0) {
    HighLevelStatemachine_DW.durationCounter_2_b++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_b = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.isChargeEn == 1) {
    HighLevelStatemachine_DW.durationCounter_4_n++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4_n = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.Current_mA <
      HighLevelStatemachine_U.Thresholds.ChargeDetectionThreshold_mA) {
    HighLevelStatemachine_DW.durationCounter_3_g++;
  } else {
    HighLevelStatemachine_DW.durationCounter_3_g = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.FastCharge) {
    HighLevelStatemachine_DW.durationCounter_4_f++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4_f = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.SlowCharge) {
    HighLevelStatemachine_DW.durationCounter_4_m++;
  } else {
    HighLevelStatemachine_DW.durationCounter_4_m = 0U;
  }

  if (HighLevelStatemachine_Y.HighLevelStateMode == LowPowerMode) {
    HighLevelStatemachine_DW.durationCounter_1_e++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_e = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.VCUPermanentFail) {
    HighLevelStatemachine_DW.durationCounter_2_n++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_n = 0U;
  }

  if (HighLevelStatemachine_U.DataPipeline.VCU.TransmissionFail) {
    HighLevelStatemachine_DW.durationCounter_2_c++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_c = 0U;
  }

  if ((HighLevelStatemachine_U.ContactorCommand == 0) && ((int32_T)
       HighLevelStatemachine_U.ContactorsState == 0)) {
    HighLevelStatemachine_DW.durationCounter_1_br++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_br = 0U;
  }

  if ((HighLevelStatemachine_U.ContactorCommand == 1) && (((int32_T)
        HighLevelStatemachine_U.ContactorsState == 0) || ((int32_T)
        HighLevelStatemachine_U.ContactorsState == 8))) {
    HighLevelStatemachine_DW.durationCounter_2_f0++;
  } else {
    HighLevelStatemachine_DW.durationCounter_2_f0 = 0U;
  }

  if ((HighLevelStatemachine_U.ContactorCommand == 1) && ((int32_T)
       HighLevelStatemachine_U.ContactorsState > 0) && ((int32_T)
       HighLevelStatemachine_U.ContactorsState <= 6)) {
    HighLevelStatemachine_DW.durationCounter_1_e2++;
  } else {
    HighLevelStatemachine_DW.durationCounter_1_e2 = 0U;
  }

  /* End of Chart: '<Root>/HighLevelStatemachine' */
}

/* Model initialize function */
void HighLevelStatemachine_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(HighLevelStatemachine_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&HighLevelStatemachine_DW, 0,
                sizeof(DW_HighLevelStatemachine_T));

  /* external inputs */
  (void)memset(&HighLevelStatemachine_U, 0, sizeof(ExtU_HighLevelStatemachine_T));
  HighLevelStatemachine_U.ContactorsState = OpenSubstate;
  HighLevelStatemachine_U.ProtectionOutput = HighLevelStatemachine_rtZProtec;

  /* external outputs */
  HighLevelStatemachine_Y.HighLevelStateMode = Idle;
  HighLevelStatemachine_Y.BMS_State = MARVEL_READY;

  /* SystemInitialize for Chart: '<Root>/HighLevelStatemachine' */
  HighLevelStatemachine_DW.is_BMS_State = HighLevelSta_IN_NO_ACTIVE_CHILD;
  HighLevelStatemachine_DW.is_HighLevelStatemachine =
    HighLevelSta_IN_NO_ACTIVE_CHILD;
  HighLevelStatemachine_DW.is_Fault_Mode = HighLevelSta_IN_NO_ACTIVE_CHILD;
  HighLevelStatemachine_DW.is_active_c8_HighLevelStatemach = 0U;

  /* SystemInitialize for Outport: '<Root>/HighLevelStateMode' incorporates:
   *  Chart: '<Root>/HighLevelStatemachine'
   */
  HighLevelStatemachine_Y.HighLevelStateMode = Idle;

  /* SystemInitialize for Outport: '<Root>/BMS_State' incorporates:
   *  Chart: '<Root>/HighLevelStatemachine'
   */
  HighLevelStatemachine_Y.BMS_State = MARVEL_READY;
}

/* Model terminate function */
void HighLevelStatemachine_terminate(void)
{
  /* (no terminate code required) */
}
