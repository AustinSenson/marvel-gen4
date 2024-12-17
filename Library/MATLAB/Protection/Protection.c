/*
 * Protection.c
 *
 * Code generation for model "Protection".
 *
 * Model version              : 4.1438
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Wed Jul 24 17:52:26 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: STMicroelectronics->Cortex_M3
 * Code generation objective: Execution efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#include "Protection.h"
#include "rtwtypes.h"
#include "Protection_types.h"
#include <math.h>
#include "Protection_private.h"
#include <string.h>
#include "zero_crossing_types.h"

/* Named constants for Chart: '<S1>/Chart' */
#define Protection_IN_Integrator_On    ((uint8_T)1U)
#define Protection_IN_Integrator_Reset ((uint8_T)2U)
#define Protection_IN_NO_ACTIVE_CHILD  ((uint8_T)0U)

/* Named constants for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
#define IN_FastCharge_OVThrs_Selection ((uint8_T)1U)
#define IN_OverCurrent_DynamicCurrentLi ((uint8_T)9U)
#define IN_SlowCharge_OVThrs_Selection ((uint8_T)2U)
#define IN_ThermalRunaway_PermanentFail ((uint8_T)3U)
#define Prote_IN_ThermalRunaway_NoError ((uint8_T)2U)
#define Prote_IN_ThermalRunaway_Warning ((uint8_T)4U)
#define Protect_IN_DCL_Protection_Lobby ((uint8_T)5U)
#define Protect_IN_No_Error_Temperature ((uint8_T)1U)
#define Protect_IN_ThermalRunaway_Error ((uint8_T)1U)
#define Protecti_IN_PermanentFail_eFuse ((uint8_T)10U)
#define Protection_IN_DCLI_Error       ((uint8_T)1U)
#define Protection_IN_DCLI_Warning     ((uint8_T)2U)
#define Protection_IN_DCLO_Error       ((uint8_T)3U)
#define Protection_IN_DCLO_Warning     ((uint8_T)4U)
#define Protection_IN_Error            ((uint8_T)1U)
#define Protection_IN_IntStat1         ((uint8_T)6U)
#define Protection_IN_IntStat2         ((uint8_T)7U)
#define Protection_IN_IntStat3         ((uint8_T)8U)
#define Protection_IN_IntStat4         ((uint8_T)9U)
#define Protection_IN_IntStat5         ((uint8_T)10U)
#define Protection_IN_IntState         ((uint8_T)11U)
#define Protection_IN_Lobby            ((uint8_T)2U)
#define Protection_IN_Lobby_f          ((uint8_T)1U)
#define Protection_IN_No_Error_Current ((uint8_T)2U)
#define Protection_IN_No_Error_Voltage ((uint8_T)1U)
#define Protection_IN_OCC_Error        ((uint8_T)3U)
#define Protection_IN_OCC_Recovery     ((uint8_T)4U)
#define Protection_IN_OCC_Warning      ((uint8_T)5U)
#define Protection_IN_OCD_Error        ((uint8_T)6U)
#define Protection_IN_OCD_Recovery     ((uint8_T)7U)
#define Protection_IN_OCD_Warning      ((uint8_T)8U)
#define Protection_IN_OTC_Error        ((uint8_T)2U)
#define Protection_IN_OTC_Recovery     ((uint8_T)3U)
#define Protection_IN_OTC_Warning      ((uint8_T)4U)
#define Protection_IN_OTD_Error        ((uint8_T)5U)
#define Protection_IN_OTD_Recovery     ((uint8_T)6U)
#define Protection_IN_OTD_Warning      ((uint8_T)7U)
#define Protection_IN_OV_Error         ((uint8_T)2U)
#define Protection_IN_OV_Recovery      ((uint8_T)3U)
#define Protection_IN_OV_Warning       ((uint8_T)4U)
#define Protection_IN_Recovery         ((uint8_T)3U)
#define Protection_IN_UTC_Error        ((uint8_T)8U)
#define Protection_IN_UTC_Recovery     ((uint8_T)9U)
#define Protection_IN_UTC_Warning      ((uint8_T)10U)
#define Protection_IN_UTD_Error        ((uint8_T)11U)
#define Protection_IN_UTD_Recovery     ((uint8_T)12U)
#define Protection_IN_UTD_Warning      ((uint8_T)13U)
#define Protection_IN_UV_Error         ((uint8_T)5U)
#define Protection_IN_UV_Recovery      ((uint8_T)6U)
#define Protection_IN_UV_Warning       ((uint8_T)7U)
#define Protection_IN_Warning          ((uint8_T)4U)
#define Protection_IN_eFuse_Melted     ((uint8_T)11U)
#define Protection_IN_eFuse_Melting    ((uint8_T)12U)

/* Block signals (default storage) */
B_Protection_T Protection_B;

/* Block states (default storage) */
DW_Protection_T Protection_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_Protection_T Protection_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_Protection_T Protection_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Protection_T Protection_Y;

/* Real-time model */
static RT_MODEL_Protection_T Protection_M_;
RT_MODEL_Protection_T *const Protection_M = &Protection_M_;

/* Forward declaration for local functions */
static void enter_atomic_SlowCharge_OVThrs_(void);
static void Protecti_OVThresholds_Selection(void);
static void Protection_Voltage_Protection(void);
static void Protection_eFuseWarning(void);
static void Protection_eFuseError(void);
static void Protection_eFuse_Melting(void);
static void Protection_i2tCalculation_DCL(void);
static void Protection_eFusePermanentFail(void);
static void Protection_Current_Protection(void);
static void Protection_UTD_Warning(void);
static void Protection_No_Error_Temperature(void);
static void Protection_UTD_Recovery(void);
static void P_Temperature_Protection_Group1(void);
static void Protection_UTD_Warning_j(void);
static void Protecti_No_Error_Temperature_j(void);
static void Protection_UTD_Recovery_p(void);
static void P_Temperature_Protection_Group2(void);
static void Protection_ThermalRunaway(void);
static void Protection_TemperatureGradient(void);
static void Protection_HighImbalanceFlag(void);
static void Protection_ShortCircuitDetect(void);
static void Protection_SuddenVoltageDrop(void);
static void Protection_ErrorDetect(void);
static void Protection_OverallState(void);
static void Protectio_DCL_CurrentProtection(void);
static void Pr_enter_internal_c2_Protection(void);
const ProtectionState_Out Protection_rtZProtectionState_O = { NoError,/* ThermalRunaway */
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

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void enter_atomic_SlowCharge_OVThrs_(void)
{
  boolean_T tmp;

  /* Inport: '<Root>/Thresholds' */
  /* :  Protections_OV_Warning_mV = Thresholds.Protections_SlowCh_OV_Warning_mV; */
  Protection_DW.Protections_OV_Warning_mV =
    Protection_U.Thresholds.Protections_SlowCh_OV_Warning_mV;

  /* Inport: '<Root>/DataPipeline' */
  tmp = !Protection_U.DataPipeline.VCU.isChargerConnected;
  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <
       Protection_DW.Protections_OV_Warning_mV) || (tmp &&
       (Protection_U.DataPipeline.Current_mA <= 0))) {
    Protection_DW.durationCounter_2_k5 = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
      Protection_DW.Protections_OV_Warning_mV) {
    Protection_DW.durationCounter_1_cg = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  /* :  Protections_OV_Error_mV = Thresholds.Protections_SlowCh_OV_Error_mV; */
  Protection_DW.Protections_OV_Error_mV =
    Protection_U.Thresholds.Protections_SlowCh_OV_Error_mV;

  /* Inport: '<Root>/DataPipeline' */
  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
       Protection_DW.Protections_OV_Error_mV) &&
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_1_i = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
      Protection_DW.Protections_OV_Error_mV) {
    Protection_DW.durationCounter_2_fr = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
       Protection_DW.Protections_OV_Error_mV) || (tmp &&
       (Protection_U.DataPipeline.Current_mA <= 0))) {
    Protection_DW.durationCounter_2_kq = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  /* :  Protections_OV_Recovery_mV = Thresholds.Protections_SlowCh_OV_Recovery_mV; */
  Protection_DW.Protections_OV_Recovery_mV =
    Protection_U.Thresholds.Protections_SlowCh_OV_Recovery_mV;

  /* Inport: '<Root>/DataPipeline' */
  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >
       Protection_DW.Protections_OV_Recovery_mV) &&
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_1_oz = 0;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protecti_OVThresholds_Selection(void)
{
  if (Protection_DW.is_OVThresholds_Selection == IN_FastCharge_OVThrs_Selection)
  {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == false || DataPipeline.VCU.FastCharge == false; */
    if ((!Protection_U.DataPipeline.VCU.isChargerConnected) ||
        (!Protection_U.DataPipeline.VCU.FastCharge)) {
      Protection_DW.is_OVThresholds_Selection = IN_SlowCharge_OVThrs_Selection;
      enter_atomic_SlowCharge_OVThrs_();
    }

    /* Inport: '<Root>/DataPipeline' */
    /* case IN_SlowCharge_OVThrs_Selection: */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == true && DataPipeline.VCU.FastCharge == true; */
  } else if (Protection_U.DataPipeline.VCU.isChargerConnected &&
             Protection_U.DataPipeline.VCU.FastCharge) {
    boolean_T tmp;
    Protection_DW.is_OVThresholds_Selection = IN_FastCharge_OVThrs_Selection;

    /* Inport: '<Root>/Thresholds' */
    /* :  Protections_OV_Warning_mV = Thresholds.Protections_FastCh_OV_Warning_mV; */
    Protection_DW.Protections_OV_Warning_mV =
      Protection_U.Thresholds.Protections_FastCh_OV_Warning_mV;
    tmp = !Protection_U.DataPipeline.VCU.isChargerConnected;
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <
         Protection_DW.Protections_OV_Warning_mV) || (tmp &&
         (Protection_U.DataPipeline.Current_mA <= 0))) {
      Protection_DW.durationCounter_2_k5 = 0;
    }

    if (Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
        Protection_DW.Protections_OV_Warning_mV) {
      Protection_DW.durationCounter_1_cg = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    /* :  Protections_OV_Error_mV = Thresholds.Protections_FastCh_OV_Error_mV; */
    Protection_DW.Protections_OV_Error_mV =
      Protection_U.Thresholds.Protections_FastCh_OV_Error_mV;
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
         Protection_DW.Protections_OV_Error_mV) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_1_i = 0;
    }

    if (Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
        Protection_DW.Protections_OV_Error_mV) {
      Protection_DW.durationCounter_2_fr = 0;
    }

    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
         Protection_DW.Protections_OV_Error_mV) || (tmp &&
         (Protection_U.DataPipeline.Current_mA <= 0))) {
      Protection_DW.durationCounter_2_kq = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    /* :  Protections_OV_Recovery_mV = Thresholds.Protections_FastCh_OV_Recovery_mV; */
    Protection_DW.Protections_OV_Recovery_mV =
      Protection_U.Thresholds.Protections_FastCh_OV_Recovery_mV;
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >
         Protection_DW.Protections_OV_Recovery_mV) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_1_oz = 0;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_Voltage_Protection(void)
{
  switch (Protection_DW.is_Voltage_Protection) {
   case Protection_IN_No_Error_Voltage:
    Protection_B.UVFlag = NoError;
    Protection_B.OVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = (duration(DataPipeline.VoltageSenseBus.mV_min <= Thresholds.Protections_UV_Warning_mV, msec) > Thresholds.Protections_UV_NoErrorTimeout_msec); */
    if (Protection_U.DataPipeline.VoltageSenseBus.mV_min >
        Protection_U.Thresholds.Protections_UV_Warning_mV) {
      Protection_DW.durationCounter_1_nq = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_nq * 100 >
        Protection_U.Thresholds.Protections_UV_NoErrorTimeout_msec) {
      Protection_DW.durationCounter_2_k = 0;
      Protection_DW.durationCounter_1_k0 = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_UV_Warning;

      /* :  UVFlag = ProtectionFlags.Warning; */
      Protection_B.UVFlag = Warning;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_max >= Protections_OV_Warning_mV && (DataPipeline.VCU.isChargerConnected==1 || DataPipeline.Current_mA>0), msec) > Thresholds.Protections_OV_NoErrorTimeout_msec; */
      if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <
           Protection_DW.Protections_OV_Warning_mV) ||
          ((!Protection_U.DataPipeline.VCU.isChargerConnected) &&
           (Protection_U.DataPipeline.Current_mA <= 0))) {
        Protection_DW.durationCounter_2_k5 = 0;
      }

      if (Protection_DW.durationCounter_2_k5 * 100 >
          Protection_U.Thresholds.Protections_OV_NoErrorTimeout_msec) {
        Protection_DW.durationCounter_2_kq = 0;
        Protection_DW.durationCounter_1_cg = 0;
        Protection_DW.is_Voltage_Protection = Protection_IN_OV_Warning;

        /* :  OVFlag = ProtectionFlags.Warning; */
        Protection_B.OVFlag = Warning;

        /* :  UVFlag = ProtectionFlags.NoError; */
        Protection_B.UVFlag = NoError;
      }
    }
    break;

   case Protection_IN_OV_Error:
    Protection_B.OVFlag = Error;
    Protection_B.UVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = (duration((DataPipeline.VoltageSenseBus.mV_max < Protections_OV_Error_mV || DataPipeline.VCU.isChargerConnected ==0), msec) > Thresholds.Protections_OV_ErrorTimeout_msec); */
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
         Protection_DW.Protections_OV_Error_mV) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_1_i = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_i * 100 >
        Protection_U.Thresholds.Protections_OV_ErrorTimeout_msec) {
      Protection_DW.durationCounter_2_fr = 0;
      Protection_DW.durationCounter_1_oz = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_OV_Recovery;

      /* :  OVFlag = ProtectionFlags.Recovery; */
      Protection_B.OVFlag = Recovery;

      /* :  UVFlag = ProtectionFlags.NoError; */
      Protection_B.UVFlag = NoError;
    }
    break;

   case Protection_IN_OV_Recovery:
    Protection_B.OVFlag = Recovery;
    Protection_B.UVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.mV_max <= Protections_OV_Recovery_mV || DataPipeline.VCU.isChargerConnected ==0), msec) > Thresholds.Protections_OV_RecoveryTimeout_msec; */
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >
         Protection_DW.Protections_OV_Recovery_mV) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_1_oz = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_oz * 100 >
        Protection_U.Thresholds.Protections_OV_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_2_k5 = 0;
      Protection_DW.durationCounter_1_nq = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_No_Error_Voltage;

      /* :  UVFlag = ProtectionFlags.NoError; */
      Protection_B.UVFlag = NoError;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = (duration((DataPipeline.VoltageSenseBus.mV_max > Protections_OV_Error_mV), msec) > Thresholds.Protections_OV_WarningTimeout_msec); */
      if (Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
          Protection_DW.Protections_OV_Error_mV) {
        Protection_DW.durationCounter_2_fr = 0;
      }

      if (Protection_DW.durationCounter_2_fr * 100 >
          Protection_U.Thresholds.Protections_OV_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_i = 0;
        Protection_DW.is_Voltage_Protection = Protection_IN_OV_Error;

        /* :  OVFlag = ProtectionFlags.Error; */
        Protection_B.OVFlag = Error;

        /* :  UVFlag = ProtectionFlags.NoError; */
        Protection_B.UVFlag = NoError;
      }
    }
    break;

   case Protection_IN_OV_Warning:
    Protection_B.OVFlag = Warning;
    Protection_B.UVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = (duration((DataPipeline.VoltageSenseBus.mV_max < Protections_OV_Warning_mV), msec) > Thresholds.Protections_OV_WarningTimeout_msec); */
    if (Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
        Protection_DW.Protections_OV_Warning_mV) {
      Protection_DW.durationCounter_1_cg = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_cg * 100 >
        Protection_U.Thresholds.Protections_OV_WarningTimeout_msec) {
      Protection_DW.durationCounter_2_k5 = 0;
      Protection_DW.durationCounter_1_nq = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_No_Error_Voltage;

      /* :  UVFlag = ProtectionFlags.NoError; */
      Protection_B.UVFlag = NoError;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.VoltageSenseBus.mV_max > Protections_OV_Error_mV && (DataPipeline.VCU.isChargerConnected ==1 || DataPipeline.Current_mA>0), msec) > Thresholds.Protections_OV_WarningTimeout_msec; */
      if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
           Protection_DW.Protections_OV_Error_mV) ||
          ((!Protection_U.DataPipeline.VCU.isChargerConnected) &&
           (Protection_U.DataPipeline.Current_mA <= 0))) {
        Protection_DW.durationCounter_2_kq = 0;
      }

      if (Protection_DW.durationCounter_2_kq * 100 >
          Protection_U.Thresholds.Protections_OV_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_i = 0;
        Protection_DW.is_Voltage_Protection = Protection_IN_OV_Error;

        /* :  OVFlag = ProtectionFlags.Error; */
        Protection_B.OVFlag = Error;

        /* :  UVFlag = ProtectionFlags.NoError; */
        Protection_B.UVFlag = NoError;
      }
    }
    break;

   case Protection_IN_UV_Error:
    Protection_B.UVFlag = Error;
    Protection_B.OVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = (duration((DataPipeline.VoltageSenseBus.mV_min > Thresholds.Protections_UV_Error_mV  || DataPipeline.VCU.isChargerConnected ==1), msec) > Thresholds.Protections_UV_ErrorTimeout_msec); */
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min <=
         Protection_U.Thresholds.Protections_UV_Error_mV) &&
        (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
      Protection_DW.durationCounter_1_od = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_od * 100 >
        Protection_U.Thresholds.Protections_UV_ErrorTimeout_msec) {
      Protection_DW.durationCounter_2_b = 0;
      Protection_DW.durationCounter_1_o0 = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_UV_Recovery;

      /* :  UVFlag = ProtectionFlags.Recovery; */
      Protection_B.UVFlag = Recovery;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    }
    break;

   case Protection_IN_UV_Recovery:
    Protection_B.UVFlag = Recovery;
    Protection_B.OVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.mV_min >= Thresholds.Protections_UV_Recovery_mV || DataPipeline.VCU.isChargerConnected ==1),msec) > Thresholds.Protections_UV_RecoveryTimeout_msec; */
    if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min <
         Protection_U.Thresholds.Protections_UV_Recovery_mV) &&
        (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
      Protection_DW.durationCounter_2_b = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_b * 100 >
        Protection_U.Thresholds.Protections_UV_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_2_k5 = 0;
      Protection_DW.durationCounter_1_nq = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_No_Error_Voltage;

      /* :  UVFlag = ProtectionFlags.NoError; */
      Protection_B.UVFlag = NoError;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.mV_min < Thresholds.Protections_UV_Error_mV), msec) > Thresholds.Protections_UV_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.VoltageSenseBus.mV_min >=
          Protection_U.Thresholds.Protections_UV_Error_mV) {
        Protection_DW.durationCounter_1_o0 = 0;
      }

      if (Protection_DW.durationCounter_1_o0 * 100 >
          Protection_U.Thresholds.Protections_UV_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_od = 0;
        Protection_DW.is_Voltage_Protection = Protection_IN_UV_Error;

        /* :  UVFlag = ProtectionFlags.Error; */
        Protection_B.UVFlag = Error;

        /* :  OVFlag = ProtectionFlags.NoError; */
        Protection_B.OVFlag = NoError;
      }
    }
    break;

   default:
    /* case IN_UV_Warning: */
    Protection_B.UVFlag = Warning;
    Protection_B.OVFlag = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = (duration((DataPipeline.VoltageSenseBus.mV_min > Thresholds.Protections_UV_Warning_mV), msec) > Thresholds.Protections_OV_WarningTimeout_msec); */
    if (Protection_U.DataPipeline.VoltageSenseBus.mV_min <=
        Protection_U.Thresholds.Protections_UV_Warning_mV) {
      Protection_DW.durationCounter_2_k = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_k * 100 >
        Protection_U.Thresholds.Protections_OV_WarningTimeout_msec) {
      Protection_DW.durationCounter_2_k5 = 0;
      Protection_DW.durationCounter_1_nq = 0;
      Protection_DW.is_Voltage_Protection = Protection_IN_No_Error_Voltage;

      /* :  UVFlag = ProtectionFlags.NoError; */
      Protection_B.UVFlag = NoError;

      /* :  OVFlag = ProtectionFlags.NoError; */
      Protection_B.OVFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.mV_min < Thresholds.Protections_UV_Error_mV && DataPipeline.VCU.isChargerConnected == 0), msec) > Thresholds.Protections_UV_WarningTimeout_msec; */
      if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min >=
           Protection_U.Thresholds.Protections_UV_Error_mV) ||
          Protection_U.DataPipeline.VCU.isChargerConnected) {
        Protection_DW.durationCounter_1_k0 = 0;
      }

      if (Protection_DW.durationCounter_1_k0 * 100 >
          Protection_U.Thresholds.Protections_UV_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_od = 0;
        Protection_DW.is_Voltage_Protection = Protection_IN_UV_Error;

        /* :  UVFlag = ProtectionFlags.Error; */
        Protection_B.UVFlag = Error;

        /* :  OVFlag = ProtectionFlags.NoError; */
        Protection_B.OVFlag = NoError;
      }
    }
    break;
  }
}

real_T rt_roundd(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine'
 * function eFuseWarning
 */
static void Protection_eFuseWarning(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1; */
  if (Protection_U.DataPipeline.VCU.isChargerConnected) {
    /* :  eFuseChargeFlag= ProtectionFlags.Warning; */
    Protection_B.eFuseChargeFlag = Warning;
  } else {
    /* :  eFuseDischargeFlag= ProtectionFlags.Warning; */
    Protection_B.eFuseDischargeFlag = Warning;
  }

  /* End of Inport: '<Root>/DataPipeline' */
}

/*
 * Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine'
 * function eFuseError
 */
static void Protection_eFuseError(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1; */
  if (Protection_U.DataPipeline.VCU.isChargerConnected) {
    /* :  eFuseChargeFlag= ProtectionFlags.Error; */
    Protection_B.eFuseChargeFlag = Error;
  } else {
    /* :  eFuseDischargeFlag= ProtectionFlags.Error; */
    Protection_B.eFuseDischargeFlag = Error;
  }

  /* End of Inport: '<Root>/DataPipeline' */
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_eFuse_Melting(void)
{
  int32_T tmp;

  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = duration((abs(DataPipeline.Current_mA)/1000) < abs(DCL_Current_A),msec) >= Thresholds.i2tTimeout_msec; */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (rt_roundd((real_T)tmp / 1000.0) >= fabsf(Protection_DW.DCL_Current_A)) {
    Protection_DW.durationCounter_1_ag = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Protection_DW.durationCounter_1_ag * 100 >=
      Protection_U.Thresholds.i2tTimeout_msec) {
    Protection_DW.durationCounter_1_eu = 0;
    Protection_DW.is_Current_Protection = IN_OverCurrent_DynamicCurrentLi;

    /* :  eFuseChargeFlag= ProtectionFlags.NoError; */
    Protection_B.eFuseChargeFlag = NoError;

    /* :  eFuseDischargeFlag= ProtectionFlags.NoError; */
    Protection_B.eFuseDischargeFlag = NoError;
  } else {
    boolean_T guard1 = false;
    boolean_T tmp_0;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(((i2t_Calculated_A2sec >= DataPipeline.i2t_ChargingCurrent_A2sec && DataPipeline.VCU.isChargerConnected == 1)), msec)>= Thresholds.eFuseMeltingTimeout_msec || duration(((i2t_Calculated_A2sec >= DataPipeline.i2t_DisChargingCurrent_A2sec && DataPipeline.VCU.isChargerConnected == 0)), msec)>= Thresholds.eFuseMeltingTimeout_msec; */
    tmp_0 = !Protection_U.DataPipeline.VCU.isChargerConnected;
    if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
         Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) || tmp_0) {
      Protection_DW.durationCounter_2_d4 = 0;
    }

    guard1 = false;
    if (Protection_DW.durationCounter_2_d4 * 100 >=
        Protection_U.Thresholds.eFuseMeltingTimeout_msec) {
      guard1 = true;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
           Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) ||
          Protection_U.DataPipeline.VCU.isChargerConnected) {
        Protection_DW.durationCounter_3 = 0;
      }

      if (Protection_DW.durationCounter_3 * 100 >=
          Protection_U.Thresholds.eFuseMeltingTimeout_msec) {
        guard1 = true;
      } else {
        real32_T CurrentDiff_A;

        /* :  eFuseWarning; */
        Protection_eFuseWarning();

        /* Inport: '<Root>/DataPipeline' */
        /* :  DCL_Current_A = single((single(DataPipeline.isChargeEn) * single(DataPipeline.DCL_ChargingCurrent_A))+(single(~DataPipeline.isChargeEn) * single(DataPipeline.DCL_DisChargingCurrent_A))); */
        Protection_DW.DCL_Current_A = (real32_T)
          (Protection_U.DataPipeline.isChargeEn == 0) * (real32_T)
          Protection_U.DataPipeline.DCL_DisChargingCurrent_A + (real32_T)
          Protection_U.DataPipeline.isChargeEn * (real32_T)
          Protection_U.DataPipeline.DCL_ChargingCurrent_A;
        if (Protection_U.DataPipeline.Current_mA < 0) {
          if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
            tmp = MAX_int32_T;
          } else {
            tmp = -Protection_U.DataPipeline.Current_mA;
          }
        } else {
          tmp = Protection_U.DataPipeline.Current_mA;
        }

        if (rt_roundd((real_T)tmp / 1000.0) <= fabsf(Protection_DW.DCL_Current_A))
        {
          Protection_DW.durationCounter_1_eu = 0;
        }

        /* Inport: '<Root>/DataPipeline' */
        if (Protection_U.DataPipeline.Current_mA < 0) {
          if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
            tmp = MAX_int32_T;
          } else {
            tmp = -Protection_U.DataPipeline.Current_mA;
          }
        } else {
          tmp = Protection_U.DataPipeline.Current_mA;
        }

        if (rt_roundd((real_T)tmp / 1000.0) >= fabsf(Protection_DW.DCL_Current_A))
        {
          Protection_DW.durationCounter_1_ag = 0;
        }

        /* Inport: '<Root>/DataPipeline' */
        /* :  CurrentDiff_A = single(abs(DataPipeline.Current_mA)/1000) - single(abs(DCL_Current_A)); */
        if (Protection_U.DataPipeline.Current_mA < 0) {
          if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
            tmp = MAX_int32_T;
          } else {
            tmp = -Protection_U.DataPipeline.Current_mA;
          }
        } else {
          tmp = Protection_U.DataPipeline.Current_mA;
        }

        CurrentDiff_A = (real32_T)rt_roundd((real_T)tmp / 1000.0) - fabsf
          (Protection_DW.DCL_Current_A);

        /* Inport: '<Root>/DataPipeline' */
        /* :  i2t_Calculated_A2sec = i2t_Calculated_A2sec + (CurrentDiff_A * CurrentDiff_A * DataPipeline.LoopTimer_SOC_msec/1000); */
        Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec += CurrentDiff_A
          * CurrentDiff_A * Protection_U.DataPipeline.LoopTimer_SOC_msec /
          1000.0F;
        if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
             Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) || tmp_0) {
          Protection_DW.durationCounter_2_d4 = 0;
        }

        if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
             Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) ||
            Protection_U.DataPipeline.VCU.isChargerConnected) {
          Protection_DW.durationCounter_3 = 0;
        }
      }
    }

    if (guard1) {
      /* :  eFuseMeltCount = eFuseMeltCount + 1; */
      tmp = (int32_T)(Protection_DW.eFuseMeltCount + 1U);
      if (Protection_DW.eFuseMeltCount + 1U > 255U) {
        tmp = 255;
      }

      Protection_DW.eFuseMeltCount = (uint8_T)tmp;
      Protection_DW.is_Current_Protection = Protection_IN_eFuse_Melted;
      Protection_DW.temporalCounter_i1 = 0U;

      /* :  i2t_Calculated_A2sec = single(0); */
      Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec = 0.0F;

      /* Inport: '<Root>/DataPipeline' */
      if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
           Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) || tmp_0) {
        Protection_DW.durationCounter_2_d4 = 0;
      }

      if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
           Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) ||
          Protection_U.DataPipeline.VCU.isChargerConnected) {
        Protection_DW.durationCounter_3 = 0;
      }

      /* :  eFuseError; */
      Protection_eFuseError();
    }
  }

  /* End of Inport: '<Root>/Thresholds' */
}

/*
 * Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine'
 * function i2tCalculation_DCL
 */
static void Protection_i2tCalculation_DCL(void)
{
  /* :  sf_internal_predicateOutput = i2t_Calculated_A2sec >0; */
  if (Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec > 0.0F) {
    int32_T tmp;
    real32_T CurrentDiff_A;

    /* Inport: '<Root>/DataPipeline' */
    /* :  CurrentDiff_A = single(abs(DataPipeline.Current_mA)/1000) - single(abs(DCL_Current_A)); */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    CurrentDiff_A = (real32_T)rt_roundd((real_T)tmp / 1000.0) - fabsf
      (Protection_DW.DCL_Current_A);

    /* Inport: '<Root>/DataPipeline' */
    /* :  i2t_Calculated_A2sec = i2t_Calculated_A2sec - (CurrentDiff_A * CurrentDiff_A * DataPipeline.LoopTimer_SOC_msec/1000); */
    Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec -= CurrentDiff_A *
      CurrentDiff_A * Protection_U.DataPipeline.LoopTimer_SOC_msec / 1000.0F;
    if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
         Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) ||
        (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
      Protection_DW.durationCounter_2_d4 = 0;
    }

    if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
         Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) ||
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_3 = 0;
    }
  } else {
    /* :  i2t_Calculated_A2sec = single(0) */
    Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec = 0.0F;

    /* Inport: '<Root>/DataPipeline' */
    if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
         Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) ||
        (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
      Protection_DW.durationCounter_2_d4 = 0;
    }

    if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec <
         Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) ||
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_3 = 0;
    }
  }
}

/*
 * Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine'
 * function eFusePermanentFail
 */
static void Protection_eFusePermanentFail(void)
{
  /* Inport: '<Root>/DataPipeline' */
  /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1; */
  if (Protection_U.DataPipeline.VCU.isChargerConnected) {
    /* :  eFuseChargeFlag= ProtectionFlags.PermanentFail; */
    Protection_B.eFuseChargeFlag = PermanentFail;
  } else {
    /* :  eFuseDischargeFlag= ProtectionFlags.PermanentFail; */
    Protection_B.eFuseDischargeFlag = PermanentFail;
  }

  /* End of Inport: '<Root>/DataPipeline' */
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_Current_Protection(void)
{
  int32_T tmp;
  boolean_T guard1 = false;
  guard1 = false;
  switch (Protection_DW.is_Current_Protection) {
   case Protection_IN_Lobby_f:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.isDynamicCurrentEn == 1; */
    if (Protection_U.DataPipeline.isDynamicCurrentEn) {
      Protection_DW.durationCounter_1_eu = 0;
      Protection_DW.is_Current_Protection = IN_OverCurrent_DynamicCurrentLi;

      /* :  eFuseChargeFlag= ProtectionFlags.NoError; */
      Protection_B.eFuseChargeFlag = NoError;

      /* :  eFuseDischargeFlag= ProtectionFlags.NoError; */
      Protection_B.eFuseDischargeFlag = NoError;
    } else {
      /* :  sf_internal_predicateOutput = DataPipeline.isDynamicCurrentEn == 0; */
      Protection_DW.durationCounter_2_l = 0;
      Protection_DW.durationCounter_1_a = 0;
      Protection_DW.is_Current_Protection = Protection_IN_No_Error_Current;

      /* :  OCDFlag = ProtectionFlags.NoError; */
      Protection_B.OCDFlag = NoError;

      /* :  OCCFlag = ProtectionFlags.NoError; */
      Protection_B.OCCFlag = NoError;
    }
    break;

   case Protection_IN_No_Error_Current:
    Protection_B.OCDFlag = NoError;
    Protection_B.OCCFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 0 && duration(abs(DataPipeline.Current_mA) >= OCD_Warning_mA, msec) > Thresholds.Protections_OCD_NoErrorTimeout_msec; */
    if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp < Protection_DW.OCD_Warning_mA) {
        Protection_DW.durationCounter_1_a = 0;
      }

      if (Protection_DW.durationCounter_1_a * 100 >
          Protection_U.Thresholds.Protections_OCD_NoErrorTimeout_msec) {
        Protection_DW.durationCounter_2 = 0;
        Protection_DW.durationCounter_1_n = 0;
        Protection_DW.is_Current_Protection = Protection_IN_OCD_Warning;

        /* :  OCDFlag = ProtectionFlags.Warning; */
        Protection_B.OCDFlag = Warning;

        /* :  OCCFlag = ProtectionFlags.NoError; */
        Protection_B.OCCFlag = NoError;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }
    break;

   case Protection_IN_OCC_Error:
    Protection_B.OCCFlag = Error;
    Protection_B.OCDFlag = NoError;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = after(Thresholds.Protections_OCC_ErrorTimeout_msec, msec); */
    if ((uint32_T)((int32_T)Protection_DW.temporalCounter_i1 * 100) >= (uint32_T)
        Protection_U.Thresholds.Protections_OCC_ErrorTimeout_msec) {
      Protection_DW.durationCounter_2_d = 0;
      Protection_DW.durationCounter_1_o = 0;
      Protection_DW.is_Current_Protection = Protection_IN_OCC_Recovery;

      /* :  OCCFlag = ProtectionFlags.Recovery; */
      Protection_B.OCCFlag = Recovery;

      /* :  OCDFlag = ProtectionFlags.NoError; */
      Protection_B.OCDFlag = NoError;
    }
    break;

   case Protection_IN_OCC_Recovery:
    Protection_B.OCCFlag = Recovery;
    Protection_B.OCDFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) <= OCC_Error_mA, msec) > Thresholds.Protections_OCC_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    if (tmp > Protection_DW.OCC_Error_mA) {
      Protection_DW.durationCounter_1_o = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_o * 100 >
        Protection_U.Thresholds.Protections_OCC_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_2_l = 0;
      Protection_DW.durationCounter_1_a = 0;
      Protection_DW.is_Current_Protection = Protection_IN_No_Error_Current;

      /* :  OCDFlag = ProtectionFlags.NoError; */
      Protection_B.OCDFlag = NoError;

      /* :  OCCFlag = ProtectionFlags.NoError; */
      Protection_B.OCCFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) >= OCC_Error_mA, msec) > Thresholds.Protections_OCC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp < Protection_DW.OCC_Error_mA) {
        Protection_DW.durationCounter_2_d = 0;
      }

      if (Protection_DW.durationCounter_2_d * 100 >
          Protection_U.Thresholds.Protections_OCC_WarningTimeout_msec) {
        Protection_DW.is_Current_Protection = Protection_IN_OCC_Error;
        Protection_DW.temporalCounter_i1 = 0U;

        /* :  OCCFlag = ProtectionFlags.Error; */
        Protection_B.OCCFlag = Error;

        /* :  OCDFlag = ProtectionFlags.NoError; */
        Protection_B.OCDFlag = NoError;
      }
    }
    break;

   case Protection_IN_OCC_Warning:
    Protection_B.OCCFlag = Warning;
    Protection_B.OCDFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) >= OCC_Error_mA, msec) > Thresholds.Protections_OCC_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    if (tmp < Protection_DW.OCC_Error_mA) {
      Protection_DW.durationCounter_2_f = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_f * 100 >
        Protection_U.Thresholds.Protections_OCC_WarningTimeout_msec) {
      Protection_DW.is_Current_Protection = Protection_IN_OCC_Error;
      Protection_DW.temporalCounter_i1 = 0U;

      /* :  OCCFlag = ProtectionFlags.Error; */
      Protection_B.OCCFlag = Error;

      /* :  OCDFlag = ProtectionFlags.NoError; */
      Protection_B.OCDFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) <= OCC_Warning_mA, msec) > Thresholds.Protections_OCC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp > Protection_DW.OCC_Warning_mA) {
        Protection_DW.durationCounter_1_cr = 0;
      }

      if (Protection_DW.durationCounter_1_cr * 100 >
          Protection_U.Thresholds.Protections_OCC_WarningTimeout_msec) {
        Protection_DW.durationCounter_2_l = 0;
        Protection_DW.durationCounter_1_a = 0;
        Protection_DW.is_Current_Protection = Protection_IN_No_Error_Current;

        /* :  OCDFlag = ProtectionFlags.NoError; */
        Protection_B.OCDFlag = NoError;

        /* :  OCCFlag = ProtectionFlags.NoError; */
        Protection_B.OCCFlag = NoError;
      }
    }
    break;

   case Protection_IN_OCD_Error:
    Protection_B.OCDFlag = Error;
    Protection_B.OCCFlag = NoError;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = after(Thresholds.Protections_OCD_ErrorTimeout_msec, msec); */
    if ((uint32_T)((int32_T)Protection_DW.temporalCounter_i1 * 100) >= (uint32_T)
        Protection_U.Thresholds.Protections_OCD_ErrorTimeout_msec) {
      Protection_DW.durationCounter_2_c = 0;
      Protection_DW.durationCounter_1_e = 0;
      Protection_DW.is_Current_Protection = Protection_IN_OCD_Recovery;

      /* :  OCDFlag = ProtectionFlags.Recovery; */
      Protection_B.OCDFlag = Recovery;

      /* :  OCCFlag = ProtectionFlags.NoError; */
      Protection_B.OCCFlag = NoError;
    }
    break;

   case Protection_IN_OCD_Recovery:
    Protection_B.OCDFlag = Recovery;
    Protection_B.OCCFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) <= OCD_Error_mA, msec) > Thresholds.Protections_OCD_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    if (tmp > Protection_DW.OCD_Error_mA) {
      Protection_DW.durationCounter_2_c = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_c * 100 >
        Protection_U.Thresholds.Protections_OCD_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_2_l = 0;
      Protection_DW.durationCounter_1_a = 0;
      Protection_DW.is_Current_Protection = Protection_IN_No_Error_Current;

      /* :  OCDFlag = ProtectionFlags.NoError; */
      Protection_B.OCDFlag = NoError;

      /* :  OCCFlag = ProtectionFlags.NoError; */
      Protection_B.OCCFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = (duration(abs(DataPipeline.Current_mA) >= OCD_Error_mA, msec) > Thresholds.Protections_OCD_WarningTimeout_msec); */
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp < Protection_DW.OCD_Error_mA) {
        Protection_DW.durationCounter_1_e = 0;
      }

      if (Protection_DW.durationCounter_1_e * 100 >
          Protection_U.Thresholds.Protections_OCD_WarningTimeout_msec) {
        Protection_DW.is_Current_Protection = Protection_IN_OCD_Error;
        Protection_DW.temporalCounter_i1 = 0U;

        /* :  OCDFlag = ProtectionFlags.Error; */
        Protection_B.OCDFlag = Error;

        /* :  OCCFlag = ProtectionFlags.NoError; */
        Protection_B.OCCFlag = NoError;
      }
    }
    break;

   case Protection_IN_OCD_Warning:
    Protection_B.OCDFlag = Warning;
    Protection_B.OCCFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = (duration(abs(DataPipeline.Current_mA) >= OCD_Error_mA, msec) > Thresholds.Protections_OCD_WarningTimeout_msec); */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    if (tmp < Protection_DW.OCD_Error_mA) {
      Protection_DW.durationCounter_1_n = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_n * 100 >
        Protection_U.Thresholds.Protections_OCD_WarningTimeout_msec) {
      Protection_DW.is_Current_Protection = Protection_IN_OCD_Error;
      Protection_DW.temporalCounter_i1 = 0U;

      /* :  OCDFlag = ProtectionFlags.Error; */
      Protection_B.OCDFlag = Error;

      /* :  OCCFlag = ProtectionFlags.NoError; */
      Protection_B.OCCFlag = NoError;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(abs(DataPipeline.Current_mA) <= OCD_Warning_mA, msec) > Thresholds.Protections_OCD_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp > Protection_DW.OCD_Warning_mA) {
        Protection_DW.durationCounter_2 = 0;
      }

      if (Protection_DW.durationCounter_2 * 100 >
          Protection_U.Thresholds.Protections_OCD_WarningTimeout_msec) {
        Protection_DW.durationCounter_2_l = 0;
        Protection_DW.durationCounter_1_a = 0;
        Protection_DW.is_Current_Protection = Protection_IN_No_Error_Current;

        /* :  OCDFlag = ProtectionFlags.NoError; */
        Protection_B.OCDFlag = NoError;

        /* :  OCCFlag = ProtectionFlags.NoError; */
        Protection_B.OCCFlag = NoError;
      }
    }
    break;

   case IN_OverCurrent_DynamicCurrentLi:
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration((abs(DataPipeline.Current_mA)/1000) > abs(DCL_Current_A),msec) >= Thresholds.i2tTimeout_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    if (rt_roundd((real_T)tmp / 1000.0) <= fabsf(Protection_DW.DCL_Current_A)) {
      Protection_DW.durationCounter_1_eu = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_eu * 100 >=
        Protection_U.Thresholds.i2tTimeout_msec) {
      Protection_DW.durationCounter_3 = 0;
      Protection_DW.durationCounter_2_d4 = 0;
      Protection_DW.durationCounter_1_ag = 0;
      Protection_DW.is_Current_Protection = Protection_IN_eFuse_Melting;
    } else {
      /* :  i2tCalculation_DCL; */
      Protection_i2tCalculation_DCL();

      /* Inport: '<Root>/DataPipeline' */
      /* :  DCL_Current_A = (single(DataPipeline.isChargeEn) * single(DataPipeline.DCL_ChargingCurrent_A))+(single(~DataPipeline.isChargeEn) * single(DataPipeline.DCL_DisChargingCurrent_A)); */
      Protection_DW.DCL_Current_A = (real32_T)
        (Protection_U.DataPipeline.isChargeEn == 0) * (real32_T)
        Protection_U.DataPipeline.DCL_DisChargingCurrent_A + (real32_T)
        Protection_U.DataPipeline.isChargeEn * (real32_T)
        Protection_U.DataPipeline.DCL_ChargingCurrent_A;
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (rt_roundd((real_T)tmp / 1000.0) <= fabsf(Protection_DW.DCL_Current_A))
      {
        Protection_DW.durationCounter_1_eu = 0;
      }

      /* Inport: '<Root>/DataPipeline' */
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (rt_roundd((real_T)tmp / 1000.0) >= fabsf(Protection_DW.DCL_Current_A))
      {
        Protection_DW.durationCounter_1_ag = 0;
      }
    }
    break;

   case Protecti_IN_PermanentFail_eFuse:
    break;

   case Protection_IN_eFuse_Melted:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = after(Thresholds.i2tTimeout_msec,msec); */
    if ((uint32_T)((int32_T)Protection_DW.temporalCounter_i1 * 100) >= (uint32_T)
        Protection_U.Thresholds.i2tTimeout_msec) {
      Protection_DW.durationCounter_1_eu = 0;
      Protection_DW.is_Current_Protection = IN_OverCurrent_DynamicCurrentLi;

      /* :  eFuseChargeFlag= ProtectionFlags.NoError; */
      Protection_B.eFuseChargeFlag = NoError;

      /* :  eFuseDischargeFlag= ProtectionFlags.NoError; */
      Protection_B.eFuseDischargeFlag = NoError;

      /* :  sf_internal_predicateOutput = eFuseMeltCount >= Thresholds.PermanentFailCounts_eFuse; */
    } else if (Protection_DW.eFuseMeltCount >=
               Protection_U.Thresholds.PermanentFailCounts_eFuse) {
      Protection_DW.is_Current_Protection = Protecti_IN_PermanentFail_eFuse;

      /* :  eFusePermanentFail; */
      Protection_eFusePermanentFail();
    }
    break;

   default:
    /* case IN_eFuse_Melting: */
    Protection_eFuse_Melting();
    break;
  }

  if (guard1) {
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1 && duration(abs(DataPipeline.Current_mA) >= OCC_Warning_mA,msec) > Thresholds.Protections_OCC_NoErrorTimeout_msec; */
    if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      if (Protection_U.DataPipeline.Current_mA < 0) {
        if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
          tmp = MAX_int32_T;
        } else {
          tmp = -Protection_U.DataPipeline.Current_mA;
        }
      } else {
        tmp = Protection_U.DataPipeline.Current_mA;
      }

      if (tmp < Protection_DW.OCC_Warning_mA) {
        Protection_DW.durationCounter_2_l = 0;
      }

      if (Protection_DW.durationCounter_2_l * 100 >
          Protection_U.Thresholds.Protections_OCC_NoErrorTimeout_msec) {
        Protection_DW.durationCounter_2_f = 0;
        Protection_DW.durationCounter_1_cr = 0;
        Protection_DW.is_Current_Protection = Protection_IN_OCC_Warning;

        /* :  OCCFlag = ProtectionFlags.Warning; */
        Protection_B.OCCFlag = Warning;

        /* :  OCDFlag = ProtectionFlags.NoError; */
        Protection_B.OCDFlag = NoError;
      }
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_UTD_Warning(void)
{
  Protection_B.FlagGroup1 = Warning;
  Protection_B.TempState1 = UTD;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTD_Error_C,msec)>Thresholds.Temperature(1).Protections_UTD_WarningTimeout_msec; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_i0 = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Protection_DW.durationCounter_1_i0 * 100 >
      Protection_U.Thresholds.Temperature[0].Protections_UTD_WarningTimeout_msec)
  {
    Protection_DW.durationCounter_1_pi = 0;
    Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTD_Error;

    /* :  FlagGroup1 = ProtectionFlags.Error; */
    Protection_B.FlagGroup1 = Error;

    /* :  TempState1 = TempState.UTD; */
    Protection_B.TempState1 = UTD;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTD_Warning_C,msec)>=Thresholds.Temperature(1).Protections_UTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
        Protection_U.Thresholds.Temperature[0].Protections_UTD_Warning_C) {
      Protection_DW.durationCounter_2_fo = 0;
    }

    if (Protection_DW.durationCounter_2_fo * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_UTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_4 = 0;
      Protection_DW.durationCounter_3_f = 0;
      Protection_DW.durationCounter_2_lr = 0;
      Protection_DW.durationCounter_1_fk = 0;
      Protection_DW.is_Temperature_Protection_Group =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup1 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup1 = NoError;

      /* :  TempState1 = TempState.NoErr; */
      Protection_B.TempState1 = NoErr;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_No_Error_Temperature(void)
{
  Protection_B.FlagGroup1 = NoError;
  Protection_B.TempState1 = NoErr;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTC_Warning_C,msec)>= Thresholds.Temperature(1).Protections_UTC_NoErrorTimeout_msec && DataPipeline.VCU.isChargerConnected == 1; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_lr = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if ((Protection_DW.durationCounter_2_lr * 100 >=
       Protection_U.Thresholds.Temperature[0].
       Protections_UTC_NoErrorTimeout_msec) &&
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_2_j = 0;
    Protection_DW.durationCounter_1_l = 0;
    Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTC_Warning;

    /* :  FlagGroup1 = ProtectionFlags.Warning; */
    Protection_B.FlagGroup1 = Warning;

    /* :  TempState1 = TempState.UTC; */
    Protection_B.TempState1 = UTC;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTC_Warning_C,msec)>=Thresholds.Temperature(1).Protections_OTC_NoErrorTimeout_msec && DataPipeline.VCU.isChargerConnected == 1; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
        Protection_U.Thresholds.Temperature[0].Protections_OTC_Warning_C) {
      Protection_DW.durationCounter_4 = 0;
    }

    if ((Protection_DW.durationCounter_4 * 100 >=
         Protection_U.Thresholds.Temperature[0].
         Protections_OTC_NoErrorTimeout_msec) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_2_a = 0;
      Protection_DW.durationCounter_1_bg = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTC_Warning;

      /* :  FlagGroup1 = ProtectionFlags.Warning; */
      Protection_B.FlagGroup1 = Warning;

      /* :  TempState1 = TempState.OTC; */
      Protection_B.TempState1 = OTC;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTD_Warning_C,msec)>=Thresholds.Temperature(1).Protections_OTD_NoErrorTimeout_msec && DataPipeline.VCU.isChargerConnected == 0; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
          Protection_U.Thresholds.Temperature[0].Protections_OTD_Warning_C) {
        Protection_DW.durationCounter_3_f = 0;
      }

      if ((Protection_DW.durationCounter_3_f * 100 >=
           Protection_U.Thresholds.Temperature[0].
           Protections_OTD_NoErrorTimeout_msec) &&
          (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
        Protection_DW.durationCounter_2_n = 0;
        Protection_DW.durationCounter_1_cc = 0;
        Protection_DW.is_Temperature_Protection_Group =
          Protection_IN_OTD_Warning;

        /* :  FlagGroup1 = ProtectionFlags.Warning; */
        Protection_B.FlagGroup1 = Warning;

        /* :  TempState1 = TempState.OTD; */
        Protection_B.TempState1 = OTD;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTD_Warning_C,msec)>=Thresholds.Temperature(1).Protections_UTD_NoErrorTimeout_msec && DataPipeline.VCU.isChargerConnected == 0; */
        if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
            Protection_U.Thresholds.Temperature[0].Protections_UTD_Warning_C) {
          Protection_DW.durationCounter_1_fk = 0;
        }

        if ((Protection_DW.durationCounter_1_fk * 100 >=
             Protection_U.Thresholds.Temperature[0].
             Protections_UTD_NoErrorTimeout_msec) &&
            (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
          Protection_DW.durationCounter_2_fo = 0;
          Protection_DW.durationCounter_1_i0 = 0;
          Protection_DW.is_Temperature_Protection_Group =
            Protection_IN_UTD_Warning;

          /* :  FlagGroup1 = ProtectionFlags.Warning; */
          Protection_B.FlagGroup1 = Warning;

          /* :  TempState1 = TempState.UTD; */
          Protection_B.TempState1 = UTD;
        }
      }
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_UTD_Recovery(void)
{
  Protection_B.FlagGroup1 = Recovery;
  Protection_B.TempState1 = UTD;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTD_Recovery_C,msec)>=Thresholds.Temperature(1).Protections_UTD_RecoveryTimeout_msec; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Recovery_C) {
    Protection_DW.durationCounter_2_p = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Protection_DW.durationCounter_2_p * 100 >=
      Protection_U.Thresholds.Temperature[0].
      Protections_UTD_RecoveryTimeout_msec) {
    Protection_DW.durationCounter_4 = 0;
    Protection_DW.durationCounter_3_f = 0;
    Protection_DW.durationCounter_2_lr = 0;
    Protection_DW.durationCounter_1_fk = 0;
    Protection_DW.is_Temperature_Protection_Group =
      Protect_IN_No_Error_Temperature;

    /* :  FlagGroup1 = ProtectionFlags.NoError; */
    Protection_B.FlagGroup1 = NoError;

    /* :  TempState1 = TempState.NoErr; */
    Protection_B.TempState1 = NoErr;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTD_Error_C,msec)>Thresholds.Temperature(1).Protections_UTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
        Protection_U.Thresholds.Temperature[0].Protections_UTD_Error_C) {
      Protection_DW.durationCounter_1_b = 0;
    }

    if (Protection_DW.durationCounter_1_b * 100 >
        Protection_U.Thresholds.Temperature[0].
        Protections_UTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_pi = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTD_Error;

      /* :  FlagGroup1 = ProtectionFlags.Error; */
      Protection_B.FlagGroup1 = Error;

      /* :  TempState1 = TempState.UTD; */
      Protection_B.TempState1 = UTD;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void P_Temperature_Protection_Group1(void)
{
  switch (Protection_DW.is_Temperature_Protection_Group) {
   case Protect_IN_No_Error_Temperature:
    Protection_No_Error_Temperature();
    break;

   case Protection_IN_OTC_Error:
    Protection_B.FlagGroup1 = Error;
    Protection_B.TempState1 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTC_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
        Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
      Protection_DW.durationCounter_1_p3 = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_p3 * 100 >=
        Protection_U.Thresholds.Temperature[0].Protections_OTC_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_cm = 0;
      Protection_DW.durationCounter_1_a1 = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTC_Recovery;

      /* :  FlagGroup1 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup1 = Recovery;

      /* :  TempState1 = TempState.OTC; */
      Protection_B.TempState1 = OTC;
    }
    break;

   case Protection_IN_OTC_Recovery:
    Protection_B.FlagGroup1 = Recovery;
    Protection_B.TempState1 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTC_Recovery_C,msec)>=Thresholds.Temperature(1).Protections_OTC_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
        Protection_U.Thresholds.Temperature[0].Protections_OTC_Recovery_C) {
      Protection_DW.durationCounter_1_a1 = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_a1 * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_OTC_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4 = 0;
      Protection_DW.durationCounter_3_f = 0;
      Protection_DW.durationCounter_2_lr = 0;
      Protection_DW.durationCounter_1_fk = 0;
      Protection_DW.is_Temperature_Protection_Group =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup1 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup1 = NoError;

      /* :  TempState1 = TempState.NoErr; */
      Protection_B.TempState1 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
          Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
        Protection_DW.durationCounter_2_cm = 0;
      }

      if (Protection_DW.durationCounter_2_cm * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_OTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_p3 = 0;
        Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTC_Error;

        /* :  FlagGroup1 = ProtectionFlags.Error; */
        Protection_B.FlagGroup1 = Error;

        /* :  TempState1 = TempState.OTC; */
        Protection_B.TempState1 = OTC;
      }
    }
    break;

   case Protection_IN_OTC_Warning:
    Protection_B.FlagGroup1 = Warning;
    Protection_B.TempState1 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTC_Warning_C,msec)>=Thresholds.Temperature(1).Protections_OTC_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
        Protection_U.Thresholds.Temperature[0].Protections_OTC_Warning_C) {
      Protection_DW.durationCounter_1_bg = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_bg * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_OTC_WarningTimeout_msec) {
      Protection_DW.durationCounter_4 = 0;
      Protection_DW.durationCounter_3_f = 0;
      Protection_DW.durationCounter_2_lr = 0;
      Protection_DW.durationCounter_1_fk = 0;
      Protection_DW.is_Temperature_Protection_Group =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup1 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup1 = NoError;

      /* :  TempState1 = TempState.NoErr; */
      Protection_B.TempState1 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
          Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
        Protection_DW.durationCounter_2_a = 0;
      }

      if (Protection_DW.durationCounter_2_a * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_OTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_p3 = 0;
        Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTC_Error;

        /* :  FlagGroup1 = ProtectionFlags.Error; */
        Protection_B.FlagGroup1 = Error;

        /* :  TempState1 = TempState.OTC; */
        Protection_B.TempState1 = OTC;
      }
    }
    break;

   case Protection_IN_OTD_Error:
    Protection_B.FlagGroup1 = Error;
    Protection_B.TempState1 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTD_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
        Protection_U.Thresholds.Temperature[0].Protections_OTD_Error_C) {
      Protection_DW.durationCounter_1_ew = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_ew * 100 >=
        Protection_U.Thresholds.Temperature[0].Protections_OTD_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_kt = 0;
      Protection_DW.durationCounter_1_ga = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTD_Recovery;

      /* :  FlagGroup1 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup1 = Recovery;

      /* :  TempState1 = TempState.OTD; */
      Protection_B.TempState1 = OTD;
    }
    break;

   case Protection_IN_OTD_Recovery:
    Protection_B.FlagGroup1 = Recovery;
    Protection_B.TempState1 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTD_Recovery_C,msec)>=Thresholds.Temperature(1).Protections_OTD_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
        Protection_U.Thresholds.Temperature[0].Protections_OTD_Recovery_C) {
      Protection_DW.durationCounter_1_ga = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_ga * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_OTD_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4 = 0;
      Protection_DW.durationCounter_3_f = 0;
      Protection_DW.durationCounter_2_lr = 0;
      Protection_DW.durationCounter_1_fk = 0;
      Protection_DW.is_Temperature_Protection_Group =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup1 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup1 = NoError;

      /* :  TempState1 = TempState.NoErr; */
      Protection_B.TempState1 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTD_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
          Protection_U.Thresholds.Temperature[0].Protections_OTD_Error_C) {
        Protection_DW.durationCounter_2_kt = 0;
      }

      if (Protection_DW.durationCounter_2_kt * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_OTD_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_ew = 0;
        Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTD_Error;

        /* :  FlagGroup1 = ProtectionFlags.Error; */
        Protection_B.FlagGroup1 = Error;

        /* :  TempState1 = TempState.OTD; */
        Protection_B.TempState1 = OTD;
      }
    }
    break;

   case Protection_IN_OTD_Warning:
    Protection_B.FlagGroup1 = Warning;
    Protection_B.TempState1 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max >= Thresholds.Temperature(1).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(1).Protections_OTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <
        Protection_U.Thresholds.Temperature[0].Protections_OTD_Error_C) {
      Protection_DW.durationCounter_2_n = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_n * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_OTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_ew = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_OTD_Error;

      /* :  FlagGroup1 = ProtectionFlags.Error; */
      Protection_B.FlagGroup1 = Error;

      /* :  TempState1 = TempState.OTD; */
      Protection_B.TempState1 = OTD;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_max <= Thresholds.Temperature(1).Protections_OTD_Warning_C,msec)>=Thresholds.Temperature(1).Protections_OTD_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
          Protection_U.Thresholds.Temperature[0].Protections_OTD_Warning_C) {
        Protection_DW.durationCounter_1_cc = 0;
      }

      if (Protection_DW.durationCounter_1_cc * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_OTD_WarningTimeout_msec) {
        Protection_DW.durationCounter_4 = 0;
        Protection_DW.durationCounter_3_f = 0;
        Protection_DW.durationCounter_2_lr = 0;
        Protection_DW.durationCounter_1_fk = 0;
        Protection_DW.is_Temperature_Protection_Group =
          Protect_IN_No_Error_Temperature;

        /* :  FlagGroup1 = ProtectionFlags.NoError; */
        Protection_B.FlagGroup1 = NoError;

        /* :  TempState1 = TempState.NoErr; */
        Protection_B.TempState1 = NoErr;
      }
    }
    break;

   case Protection_IN_UTC_Error:
    Protection_B.FlagGroup1 = Error;
    Protection_B.TempState1 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_UTC_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
        Protection_U.Thresholds.Temperature[0].Protections_UTC_Error_C) {
      Protection_DW.durationCounter_1_ay = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_ay * 100 >=
        Protection_U.Thresholds.Temperature[0].Protections_UTC_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_m = 0;
      Protection_DW.durationCounter_1_cp = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTC_Recovery;

      /* :  FlagGroup1 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup1 = Recovery;

      /* :  TempState1 = TempState.UTC; */
      Protection_B.TempState1 = UTC;
    }
    break;

   case Protection_IN_UTC_Recovery:
    Protection_B.FlagGroup1 = Recovery;
    Protection_B.TempState1 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTC_Recovery_C,msec)>=Thresholds.Temperature(1).Protections_UTC_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
        Protection_U.Thresholds.Temperature[0].Protections_UTC_Recovery_C) {
      Protection_DW.durationCounter_2_m = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_m * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_UTC_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4 = 0;
      Protection_DW.durationCounter_3_f = 0;
      Protection_DW.durationCounter_2_lr = 0;
      Protection_DW.durationCounter_1_fk = 0;
      Protection_DW.is_Temperature_Protection_Group =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup1 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup1 = NoError;

      /* :  TempState1 = TempState.NoErr; */
      Protection_B.TempState1 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_UTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
          Protection_U.Thresholds.Temperature[0].Protections_UTC_Error_C) {
        Protection_DW.durationCounter_1_cp = 0;
      }

      if (Protection_DW.durationCounter_1_cp * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_UTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_ay = 0;
        Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTC_Error;

        /* :  FlagGroup1 = ProtectionFlags.Error; */
        Protection_B.FlagGroup1 = Error;

        /* :  TempState1 = TempState.UTC; */
        Protection_B.TempState1 = UTC;
      }
    }
    break;

   case Protection_IN_UTC_Warning:
    Protection_B.FlagGroup1 = Warning;
    Protection_B.TempState1 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min <= Thresholds.Temperature(1).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(1).Protections_UTC_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >
        Protection_U.Thresholds.Temperature[0].Protections_UTC_Error_C) {
      Protection_DW.durationCounter_1_l = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_l * 100 >=
        Protection_U.Thresholds.Temperature[0].
        Protections_UTC_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_ay = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTC_Error;

      /* :  FlagGroup1 = ProtectionFlags.Error; */
      Protection_B.FlagGroup1 = Error;

      /* :  TempState1 = TempState.UTC; */
      Protection_B.TempState1 = UTC;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTC_Warning_C,msec)>=Thresholds.Temperature(1).Protections_UTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
          Protection_U.Thresholds.Temperature[0].Protections_UTC_Warning_C) {
        Protection_DW.durationCounter_2_j = 0;
      }

      if (Protection_DW.durationCounter_2_j * 100 >=
          Protection_U.Thresholds.Temperature[0].
          Protections_UTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_4 = 0;
        Protection_DW.durationCounter_3_f = 0;
        Protection_DW.durationCounter_2_lr = 0;
        Protection_DW.durationCounter_1_fk = 0;
        Protection_DW.is_Temperature_Protection_Group =
          Protect_IN_No_Error_Temperature;

        /* :  FlagGroup1 = ProtectionFlags.NoError; */
        Protection_B.FlagGroup1 = NoError;

        /* :  TempState1 = TempState.NoErr; */
        Protection_B.TempState1 = NoErr;
      }
    }
    break;

   case Protection_IN_UTD_Error:
    Protection_B.FlagGroup1 = Error;
    Protection_B.TempState1 = UTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(1).T_min >= Thresholds.Temperature(1).Protections_UTD_Error_C,msec)>=Thresholds.Temperature(1).Protections_UTD_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <
        Protection_U.Thresholds.Temperature[0].Protections_UTD_Error_C) {
      Protection_DW.durationCounter_1_pi = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_pi * 100 >=
        Protection_U.Thresholds.Temperature[0].Protections_UTD_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_p = 0;
      Protection_DW.durationCounter_1_b = 0;
      Protection_DW.is_Temperature_Protection_Group = Protection_IN_UTD_Recovery;

      /* :  FlagGroup1 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup1 = Recovery;

      /* :  TempState1 = TempState.UTD; */
      Protection_B.TempState1 = UTD;
    }
    break;

   case Protection_IN_UTD_Recovery:
    Protection_UTD_Recovery();
    break;

   default:
    /* case IN_UTD_Warning: */
    Protection_UTD_Warning();
    break;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_UTD_Warning_j(void)
{
  Protection_B.FlagGroup2 = Warning;
  Protection_B.TempState2 = UTD;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTD_WarningTimeout_msec; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_o05 = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Protection_DW.durationCounter_1_o05 * 100 >=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_WarningTimeout_msec)
  {
    Protection_DW.durationCounter_1_j = 0;
    Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTD_Error;

    /* :  FlagGroup2 = ProtectionFlags.Error; */
    Protection_B.FlagGroup2 = Error;

    /* :  TempState2 = TempState.UTD; */
    Protection_B.TempState2 = UTD;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTD_Warning_C,msec)>=Thresholds.Temperature(2).Protections_UTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
        Protection_U.Thresholds.Temperature[1].Protections_UTD_Warning_C) {
      Protection_DW.durationCounter_2_ad = 0;
    }

    if (Protection_DW.durationCounter_2_ad * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_UTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_4_f = 0;
      Protection_DW.durationCounter_3_j = 0;
      Protection_DW.durationCounter_2_nn = 0;
      Protection_DW.durationCounter_1_bq = 0;
      Protection_DW.is_Temperature_Protection_Gro_g =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup2 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup2 = NoError;

      /* :  TempState2 = TempState.NoErr; */
      Protection_B.TempState2 = NoErr;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protecti_No_Error_Temperature_j(void)
{
  Protection_B.FlagGroup2 = NoError;
  Protection_B.TempState2 = NoErr;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTC_Warning_C,msec)>=Thresholds.Temperature(2).Protections_UTC_WarningTimeout_msec && DataPipeline.VCU.isChargerConnected == 1; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_nn = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if ((Protection_DW.durationCounter_2_nn * 100 >=
       Protection_U.Thresholds.Temperature[1].
       Protections_UTC_WarningTimeout_msec) &&
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_2_ke = 0;
    Protection_DW.durationCounter_1_k3 = 0;
    Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTC_Warning;

    /* :  FlagGroup2 = ProtectionFlags.Warning; */
    Protection_B.FlagGroup2 = Warning;

    /* :  TempState2 = TempState.UTC; */
    Protection_B.TempState2 = UTC;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTC_Warning_C,msec)>=Thresholds.Temperature(2).Protections_OTC_WarningTimeout_msec && DataPipeline.VCU.isChargerConnected == 1; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
        Protection_U.Thresholds.Temperature[1].Protections_OTC_Warning_C) {
      Protection_DW.durationCounter_4_f = 0;
    }

    if ((Protection_DW.durationCounter_4_f * 100 >=
         Protection_U.Thresholds.Temperature[1].
         Protections_OTC_WarningTimeout_msec) &&
        Protection_U.DataPipeline.VCU.isChargerConnected) {
      Protection_DW.durationCounter_2_d2 = 0;
      Protection_DW.durationCounter_1_bz = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTC_Warning;

      /* :  FlagGroup2 = ProtectionFlags.Warning; */
      Protection_B.FlagGroup2 = Warning;

      /* :  TempState2 = TempState.OTC; */
      Protection_B.TempState2 = OTC;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTD_Warning_C,msec)>Thresholds.Temperature(2).Protections_OTD_WarningTimeout_msec && DataPipeline.VCU.isChargerConnected == 0; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
          Protection_U.Thresholds.Temperature[1].Protections_OTD_Warning_C) {
        Protection_DW.durationCounter_3_j = 0;
      }

      if ((Protection_DW.durationCounter_3_j * 100 >
           Protection_U.Thresholds.Temperature[1].
           Protections_OTD_WarningTimeout_msec) &&
          (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
        Protection_DW.durationCounter_2_nng = 0;
        Protection_DW.durationCounter_1_hd = 0;
        Protection_DW.is_Temperature_Protection_Gro_g =
          Protection_IN_OTD_Warning;

        /* :  FlagGroup2 = ProtectionFlags.Warning; */
        Protection_B.FlagGroup2 = Warning;

        /* :  TempState2 = TempState.OTD; */
        Protection_B.TempState2 = OTD;
      } else {
        /* Inport: '<Root>/DataPipeline' */
        /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTD_Warning_C,msec)>=Thresholds.Temperature(2).Protections_UTD_WarningTimeout_msec && DataPipeline.VCU.isChargerConnected == 0; */
        if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
            Protection_U.Thresholds.Temperature[1].Protections_UTD_Warning_C) {
          Protection_DW.durationCounter_1_bq = 0;
        }

        if ((Protection_DW.durationCounter_1_bq * 100 >=
             Protection_U.Thresholds.Temperature[1].
             Protections_UTD_WarningTimeout_msec) &&
            (!Protection_U.DataPipeline.VCU.isChargerConnected)) {
          Protection_DW.durationCounter_2_ad = 0;
          Protection_DW.durationCounter_1_o05 = 0;
          Protection_DW.is_Temperature_Protection_Gro_g =
            Protection_IN_UTD_Warning;

          /* :  FlagGroup2 = ProtectionFlags.Warning; */
          Protection_B.FlagGroup2 = Warning;

          /* :  TempState2 = TempState.UTD; */
          Protection_B.TempState2 = UTD;
        }
      }
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_UTD_Recovery_p(void)
{
  Protection_B.FlagGroup2 = Recovery;
  Protection_B.TempState2 = UTD;

  /* Inport: '<Root>/DataPipeline' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTD_Recovery_C,msec)>=Thresholds.Temperature(2).Protections_UTD_RecoveryTimeout_msec; */
  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Recovery_C) {
    Protection_DW.durationCounter_2_jr = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  if (Protection_DW.durationCounter_2_jr * 100 >=
      Protection_U.Thresholds.Temperature[1].
      Protections_UTD_RecoveryTimeout_msec) {
    Protection_DW.durationCounter_4_f = 0;
    Protection_DW.durationCounter_3_j = 0;
    Protection_DW.durationCounter_2_nn = 0;
    Protection_DW.durationCounter_1_bq = 0;
    Protection_DW.is_Temperature_Protection_Gro_g =
      Protect_IN_No_Error_Temperature;

    /* :  FlagGroup2 = ProtectionFlags.NoError; */
    Protection_B.FlagGroup2 = NoError;

    /* :  TempState2 = TempState.NoErr; */
    Protection_B.TempState2 = NoErr;
  } else {
    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
        Protection_U.Thresholds.Temperature[1].Protections_UTD_Error_C) {
      Protection_DW.durationCounter_1_oa = 0;
    }

    if (Protection_DW.durationCounter_1_oa * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_UTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_j = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTD_Error;

      /* :  FlagGroup2 = ProtectionFlags.Error; */
      Protection_B.FlagGroup2 = Error;

      /* :  TempState2 = TempState.UTD; */
      Protection_B.TempState2 = UTD;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void P_Temperature_Protection_Group2(void)
{
  switch (Protection_DW.is_Temperature_Protection_Gro_g) {
   case Protect_IN_No_Error_Temperature:
    Protecti_No_Error_Temperature_j();
    break;

   case Protection_IN_OTC_Error:
    Protection_B.FlagGroup2 = Error;
    Protection_B.TempState2 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTC_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
        Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
      Protection_DW.durationCounter_1_by = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_by * 100 >=
        Protection_U.Thresholds.Temperature[1].Protections_OTC_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_h = 0;
      Protection_DW.durationCounter_1_bw = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTC_Recovery;

      /* :  FlagGroup2 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup2 = Recovery;

      /* :  TempState2 = TempState.OTC; */
      Protection_B.TempState2 = OTC;
    }
    break;

   case Protection_IN_OTC_Recovery:
    Protection_B.FlagGroup2 = Recovery;
    Protection_B.TempState2 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTC_Recovery_C,msec)>=Thresholds.Temperature(2).Protections_OTC_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
        Protection_U.Thresholds.Temperature[1].Protections_OTC_Recovery_C) {
      Protection_DW.durationCounter_1_bw = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_bw * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_OTC_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4_f = 0;
      Protection_DW.durationCounter_3_j = 0;
      Protection_DW.durationCounter_2_nn = 0;
      Protection_DW.durationCounter_1_bq = 0;
      Protection_DW.is_Temperature_Protection_Gro_g =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup2 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup2 = NoError;

      /* :  TempState2 = TempState.NoErr; */
      Protection_B.TempState2 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
          Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
        Protection_DW.durationCounter_2_h = 0;
      }

      if (Protection_DW.durationCounter_2_h * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_OTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_by = 0;
        Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTC_Error;

        /* :  FlagGroup2 = ProtectionFlags.Error; */
        Protection_B.FlagGroup2 = Error;

        /* :  TempState2 = TempState.OTC; */
        Protection_B.TempState2 = OTC;
      }
    }
    break;

   case Protection_IN_OTC_Warning:
    Protection_B.FlagGroup2 = Warning;
    Protection_B.TempState2 = OTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTC_Warning_C,msec)>=Thresholds.Temperature(2).Protections_OTC_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
        Protection_U.Thresholds.Temperature[1].Protections_OTC_Warning_C) {
      Protection_DW.durationCounter_1_bz = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_bz * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_OTC_WarningTimeout_msec) {
      Protection_DW.durationCounter_4_f = 0;
      Protection_DW.durationCounter_3_j = 0;
      Protection_DW.durationCounter_2_nn = 0;
      Protection_DW.durationCounter_1_bq = 0;
      Protection_DW.is_Temperature_Protection_Gro_g =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup2 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup2 = NoError;

      /* :  TempState2 = TempState.NoErr; */
      Protection_B.TempState2 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
          Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
        Protection_DW.durationCounter_2_d2 = 0;
      }

      if (Protection_DW.durationCounter_2_d2 * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_OTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_by = 0;
        Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTC_Error;

        /* :  FlagGroup2 = ProtectionFlags.Error; */
        Protection_B.FlagGroup2 = Error;

        /* :  TempState2 = TempState.OTC; */
        Protection_B.TempState2 = OTC;
      }
    }
    break;

   case Protection_IN_OTD_Error:
    Protection_B.FlagGroup2 = Error;
    Protection_B.TempState2 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTD_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
        Protection_U.Thresholds.Temperature[1].Protections_OTD_Error_C) {
      Protection_DW.durationCounter_1_jp = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_jp * 100 >=
        Protection_U.Thresholds.Temperature[1].Protections_OTD_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_mx = 0;
      Protection_DW.durationCounter_1_lb = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTD_Recovery;

      /* :  FlagGroup2 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup2 = Recovery;

      /* :  TempState2 = TempState.OTD; */
      Protection_B.TempState2 = OTD;
    }
    break;

   case Protection_IN_OTD_Recovery:
    Protection_B.FlagGroup2 = Recovery;
    Protection_B.TempState2 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTD_Recovery_C,msec)>=Thresholds.Temperature(2).Protections_OTD_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
        Protection_U.Thresholds.Temperature[1].Protections_OTD_Recovery_C) {
      Protection_DW.durationCounter_1_lb = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_lb * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_OTD_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4_f = 0;
      Protection_DW.durationCounter_3_j = 0;
      Protection_DW.durationCounter_2_nn = 0;
      Protection_DW.durationCounter_1_bq = 0;
      Protection_DW.is_Temperature_Protection_Gro_g =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup2 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup2 = NoError;

      /* :  TempState2 = TempState.NoErr; */
      Protection_B.TempState2 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTD_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
          Protection_U.Thresholds.Temperature[1].Protections_OTD_Error_C) {
        Protection_DW.durationCounter_2_mx = 0;
      }

      if (Protection_DW.durationCounter_2_mx * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_OTD_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_jp = 0;
        Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTD_Error;

        /* :  FlagGroup2 = ProtectionFlags.Error; */
        Protection_B.FlagGroup2 = Error;

        /* :  TempState2 = TempState.OTD; */
        Protection_B.TempState2 = OTD;
      }
    }
    break;

   case Protection_IN_OTD_Warning:
    Protection_B.FlagGroup2 = Warning;
    Protection_B.TempState2 = OTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max >= Thresholds.Temperature(2).Protections_OTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_OTD_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <
        Protection_U.Thresholds.Temperature[1].Protections_OTD_Error_C) {
      Protection_DW.durationCounter_2_nng = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_nng * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_OTD_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_jp = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_OTD_Error;

      /* :  FlagGroup2 = ProtectionFlags.Error; */
      Protection_B.FlagGroup2 = Error;

      /* :  TempState2 = TempState.OTD; */
      Protection_B.TempState2 = OTD;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_max <= Thresholds.Temperature(2).Protections_OTD_Warning_C,msec)>=Thresholds.Temperature(2).Protections_OTD_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >
          Protection_U.Thresholds.Temperature[1].Protections_OTD_Warning_C) {
        Protection_DW.durationCounter_1_hd = 0;
      }

      if (Protection_DW.durationCounter_1_hd * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_OTD_WarningTimeout_msec) {
        Protection_DW.durationCounter_4_f = 0;
        Protection_DW.durationCounter_3_j = 0;
        Protection_DW.durationCounter_2_nn = 0;
        Protection_DW.durationCounter_1_bq = 0;
        Protection_DW.is_Temperature_Protection_Gro_g =
          Protect_IN_No_Error_Temperature;

        /* :  FlagGroup2 = ProtectionFlags.NoError; */
        Protection_B.FlagGroup2 = NoError;

        /* :  TempState2 = TempState.NoErr; */
        Protection_B.TempState2 = NoErr;
      }
    }
    break;

   case Protection_IN_UTC_Error:
    Protection_B.FlagGroup2 = Error;
    Protection_B.TempState2 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTC_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
        Protection_U.Thresholds.Temperature[1].Protections_UTC_Error_C) {
      Protection_DW.durationCounter_1_cy = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_cy * 100 >=
        Protection_U.Thresholds.Temperature[1].Protections_UTC_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_fb = 0;
      Protection_DW.durationCounter_1_ev = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTC_Recovery;

      /* :  FlagGroup2 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup2 = Recovery;

      /* :  TempState2 = TempState.UTC; */
      Protection_B.TempState2 = UTC;
    }
    break;

   case Protection_IN_UTC_Recovery:
    Protection_B.FlagGroup2 = Recovery;
    Protection_B.TempState2 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTC_Recovery_C,msec)>=Thresholds.Temperature(2).Protections_UTC_RecoveryTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
        Protection_U.Thresholds.Temperature[1].Protections_UTC_Recovery_C) {
      Protection_DW.durationCounter_2_fb = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_2_fb * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_UTC_RecoveryTimeout_msec) {
      Protection_DW.durationCounter_4_f = 0;
      Protection_DW.durationCounter_3_j = 0;
      Protection_DW.durationCounter_2_nn = 0;
      Protection_DW.durationCounter_1_bq = 0;
      Protection_DW.is_Temperature_Protection_Gro_g =
        Protect_IN_No_Error_Temperature;

      /* :  FlagGroup2 = ProtectionFlags.NoError; */
      Protection_B.FlagGroup2 = NoError;

      /* :  TempState2 = TempState.NoErr; */
      Protection_B.TempState2 = NoErr;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
          Protection_U.Thresholds.Temperature[1].Protections_UTC_Error_C) {
        Protection_DW.durationCounter_1_ev = 0;
      }

      if (Protection_DW.durationCounter_1_ev * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_UTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_1_cy = 0;
        Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTC_Error;

        /* :  FlagGroup2 = ProtectionFlags.Error; */
        Protection_B.FlagGroup2 = Error;

        /* :  TempState2 = TempState.UTC; */
        Protection_B.TempState2 = UTC;
      }
    }
    break;

   case Protection_IN_UTC_Warning:
    Protection_B.FlagGroup2 = Warning;
    Protection_B.TempState2 = UTC;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min <= Thresholds.Temperature(2).Protections_UTC_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTC_WarningTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >
        Protection_U.Thresholds.Temperature[1].Protections_UTC_Error_C) {
      Protection_DW.durationCounter_1_k3 = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_k3 * 100 >=
        Protection_U.Thresholds.Temperature[1].
        Protections_UTC_WarningTimeout_msec) {
      Protection_DW.durationCounter_1_cy = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTC_Error;

      /* :  FlagGroup2 = ProtectionFlags.Error; */
      Protection_B.FlagGroup2 = Error;

      /* :  TempState2 = TempState.UTC; */
      Protection_B.TempState2 = UTC;
    } else {
      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTC_Warning_C,msec)>=Thresholds.Temperature(2).Protections_UTC_WarningTimeout_msec; */
      if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
          Protection_U.Thresholds.Temperature[1].Protections_UTC_Warning_C) {
        Protection_DW.durationCounter_2_ke = 0;
      }

      if (Protection_DW.durationCounter_2_ke * 100 >=
          Protection_U.Thresholds.Temperature[1].
          Protections_UTC_WarningTimeout_msec) {
        Protection_DW.durationCounter_4_f = 0;
        Protection_DW.durationCounter_3_j = 0;
        Protection_DW.durationCounter_2_nn = 0;
        Protection_DW.durationCounter_1_bq = 0;
        Protection_DW.is_Temperature_Protection_Gro_g =
          Protect_IN_No_Error_Temperature;

        /* :  FlagGroup2 = ProtectionFlags.NoError; */
        Protection_B.FlagGroup2 = NoError;

        /* :  TempState2 = TempState.NoErr; */
        Protection_B.TempState2 = NoErr;
      }
    }
    break;

   case Protection_IN_UTD_Error:
    Protection_B.FlagGroup2 = Error;
    Protection_B.TempState2 = UTD;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.TemperatureSenseBus(2).T_min >= Thresholds.Temperature(2).Protections_UTD_Error_C,msec)>=Thresholds.Temperature(2).Protections_UTD_ErrorTimeout_msec; */
    if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <
        Protection_U.Thresholds.Temperature[1].Protections_UTD_Error_C) {
      Protection_DW.durationCounter_1_j = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_j * 100 >=
        Protection_U.Thresholds.Temperature[1].Protections_UTD_ErrorTimeout_msec)
    {
      Protection_DW.durationCounter_2_jr = 0;
      Protection_DW.durationCounter_1_oa = 0;
      Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_UTD_Recovery;

      /* :  FlagGroup2 = ProtectionFlags.Recovery; */
      Protection_B.FlagGroup2 = Recovery;

      /* :  TempState2 = TempState.UTD; */
      Protection_B.TempState2 = UTD;
    }
    break;

   case Protection_IN_UTD_Recovery:
    Protection_UTD_Recovery_p();
    break;

   default:
    /* case IN_UTD_Warning: */
    Protection_UTD_Warning_j();
    break;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_ThermalRunaway(void)
{
  switch (Protection_DW.is_ThermalRunaway) {
   case Protect_IN_ThermalRunaway_Error:
    Protection_B.ThermalRunaway = Error;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = duration((ThermalRunaway == ProtectionFlags.Error),msec) >=Thresholds.ThermalRunawayErrorTimeout_msec; */
    if (Protection_DW.durationCounter_1_p * 100 >=
        Protection_U.Thresholds.ThermalRunawayErrorTimeout_msec) {
      Protection_DW.is_ThermalRunaway = IN_ThermalRunaway_PermanentFail;

      /* :  ThermalRunaway = ProtectionFlags.PermanentFail; */
      Protection_B.ThermalRunaway = PermanentFail;
      Protection_DW.durationCounter_1_p = 0;
    }
    break;

   case Prote_IN_ThermalRunaway_NoError:
    Protection_B.ThermalRunaway = NoError;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = duration((TempChange_C>=Thresholds.Protections_MaxAllowedTempChange_C && DataPipeline.TemperatureSenseBus(1).T_max >Thresholds.Protections_MaxOperatingTemp_C),msec) >=Thresholds.ThermalRunawayWarningTimeout_msec; */
    if (((real_T)Protection_B.Add >=
         Protection_U.Thresholds.Protections_MaxAllowedTempChange_C) &&
        (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
         Protection_U.Thresholds.Protections_MaxOperatingTemp_C)) {
    } else {
      Protection_DW.durationCounter_1 = 0;
    }

    if (Protection_DW.durationCounter_1 * 100 >=
        Protection_U.Thresholds.ThermalRunawayWarningTimeout_msec) {
      Protection_DW.durationCounter_1_c = 0;
      Protection_DW.is_ThermalRunaway = Prote_IN_ThermalRunaway_Warning;

      /* :  ThermalRunaway = ProtectionFlags.Warning; */
      Protection_B.ThermalRunaway = Warning;
      Protection_DW.durationCounter_1_p = 0;
    }
    break;

   case IN_ThermalRunaway_PermanentFail:
    Protection_B.ThermalRunaway = PermanentFail;
    break;

   default:
    /* case IN_ThermalRunaway_Warning: */
    Protection_B.ThermalRunaway = Warning;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = duration((TempChange_C >= Thresholds.Protections_MaxAllowedTempChange_C && DataPipeline.TemperatureSenseBus(1).T_max > Thresholds.Protections_MaxOperatingTemp_C),msec) >=Thresholds.ThermalRunawayErrorTimeout_msec; */
    if (((real_T)Protection_B.Add >=
         Protection_U.Thresholds.Protections_MaxAllowedTempChange_C) &&
        (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
         Protection_U.Thresholds.Protections_MaxOperatingTemp_C)) {
    } else {
      Protection_DW.durationCounter_1_c = 0;
    }

    if (Protection_DW.durationCounter_1_c * 100 >=
        Protection_U.Thresholds.ThermalRunawayErrorTimeout_msec) {
      Protection_DW.durationCounter_1_p = 0;
      Protection_DW.is_ThermalRunaway = Protect_IN_ThermalRunaway_Error;

      /* :  ThermalRunaway = ProtectionFlags.Error; */
      Protection_B.ThermalRunaway = Error;
    }
    break;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_TemperatureGradient(void)
{
  if (Protection_DW.is_TemperatureGradient == Protection_IN_Error) {
    Protection_B.TemperatureGradient = Error;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.DeltaTemperatureGroup1_C < Thresholds.Protections_MaxAllowedTempGradientGroup1_C, msec) >= Thresholds.TemperatureGradientTimeout_msec; */
    if (Protection_U.DataPipeline.DeltaTemperatureGroup1_C >=
        Protection_U.Thresholds.Protections_MaxAllowedTempGradientGroup1_C) {
      Protection_DW.durationCounter_1_g = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_g * 100 >=
        Protection_U.Thresholds.TemperatureGradientTimeout_msec) {
      Protection_DW.durationCounter_1_ec = 0;
      Protection_DW.is_TemperatureGradient = Protection_IN_Lobby;

      /* :  TemperatureGradient = ProtectionFlags.NoError; */
      Protection_B.TemperatureGradient = NoError;
    }
  } else {
    /* case IN_Lobby: */
    Protection_B.TemperatureGradient = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration(DataPipeline.DeltaTemperatureGroup1_C >= Thresholds.Protections_MaxAllowedTempGradientGroup1_C, msec) >= Thresholds.TemperatureGradientTimeout_msec; */
    if (Protection_U.DataPipeline.DeltaTemperatureGroup1_C <
        Protection_U.Thresholds.Protections_MaxAllowedTempGradientGroup1_C) {
      Protection_DW.durationCounter_1_ec = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_ec * 100 >=
        Protection_U.Thresholds.TemperatureGradientTimeout_msec) {
      Protection_DW.durationCounter_1_g = 0;
      Protection_DW.is_TemperatureGradient = Protection_IN_Error;

      /* :  TemperatureGradient = ProtectionFlags.Error; */
      Protection_B.TemperatureGradient = Error;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_HighImbalanceFlag(void)
{
  if (Protection_DW.is_HighImbalanceFlag == Protection_IN_Error) {
    Protection_B.HighImbalanceFlag = Error;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.VoltageDelta_mV < Thresholds.Protections_HighImbalanceError_mV),msec) >= Thresholds.HighImbalanceFlagTimeout_msec; */
    if (Protection_U.DataPipeline.VoltageSenseBus.VoltageDelta_mV >=
        Protection_U.Thresholds.Protections_HighImbalanceError_mV) {
      Protection_DW.durationCounter_1_pt = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_pt * 100 >=
        Protection_U.Thresholds.HighImbalanceFlagTimeout_msec) {
      Protection_DW.durationCounter_1_h = 0;
      Protection_DW.is_HighImbalanceFlag = Protection_IN_Lobby;

      /* :  HighImbalanceFlag = ProtectionFlags.NoError; */
      Protection_B.HighImbalanceFlag = NoError;
    }
  } else {
    /* case IN_Lobby: */
    Protection_B.HighImbalanceFlag = NoError;

    /* Inport: '<Root>/DataPipeline' incorporates:
     *  Inport: '<Root>/Thresholds'
     */
    /* :  sf_internal_predicateOutput = duration((DataPipeline.VoltageSenseBus.VoltageDelta_mV >= Thresholds.Protections_HighImbalanceError_mV),msec) >= Thresholds.HighImbalanceFlagTimeout_msec; */
    if (Protection_U.DataPipeline.VoltageSenseBus.VoltageDelta_mV <
        Protection_U.Thresholds.Protections_HighImbalanceError_mV) {
      Protection_DW.durationCounter_1_h = 0;
    }

    /* Inport: '<Root>/Thresholds' */
    if (Protection_DW.durationCounter_1_h * 100 >=
        Protection_U.Thresholds.HighImbalanceFlagTimeout_msec) {
      Protection_DW.durationCounter_1_pt = 0;
      Protection_DW.is_HighImbalanceFlag = Protection_IN_Error;

      /* :  HighImbalanceFlag = ProtectionFlags.Error; */
      Protection_B.HighImbalanceFlag = Error;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_ShortCircuitDetect(void)
{
  if (Protection_DW.is_ShortCircuitDetect == Protection_IN_Error) {
    int32_T tmp;
    Protection_B.ShortCircuitDetect = Error;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration((abs(DataPipeline.Current_mA) < Thresholds.Protections_ShortCircuitCurrent_mA),msec) >= Thresholds.ShortCircuitDetect_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    /* Inport: '<Root>/Thresholds' */
    if (tmp >= Protection_U.Thresholds.Protections_ShortCircuitCurrent_mA) {
      Protection_DW.durationCounter_1_fo = 0;
    }

    if (Protection_DW.durationCounter_1_fo * 100 >=
        Protection_U.Thresholds.ShortCircuitDetect_msec) {
      Protection_DW.durationCounter_1_ht = 0;
      Protection_DW.is_ShortCircuitDetect = Protection_IN_Lobby;

      /* :  ShortCircuitDetect = ProtectionFlags.NoError; */
      Protection_B.ShortCircuitDetect = NoError;
    }
  } else {
    int32_T tmp;

    /* case IN_Lobby: */
    Protection_B.ShortCircuitDetect = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = duration((abs(DataPipeline.Current_mA) >= Thresholds.Protections_ShortCircuitCurrent_mA),msec) >= Thresholds.ShortCircuitDetect_msec; */
    if (Protection_U.DataPipeline.Current_mA < 0) {
      if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
        tmp = MAX_int32_T;
      } else {
        tmp = -Protection_U.DataPipeline.Current_mA;
      }
    } else {
      tmp = Protection_U.DataPipeline.Current_mA;
    }

    /* Inport: '<Root>/Thresholds' */
    if (tmp < Protection_U.Thresholds.Protections_ShortCircuitCurrent_mA) {
      Protection_DW.durationCounter_1_ht = 0;
    }

    if (Protection_DW.durationCounter_1_ht * 100 >=
        Protection_U.Thresholds.ShortCircuitDetect_msec) {
      Protection_DW.durationCounter_1_fo = 0;
      Protection_DW.is_ShortCircuitDetect = Protection_IN_Error;

      /* :  ShortCircuitDetect = ProtectionFlags.Error; */
      Protection_B.ShortCircuitDetect = Error;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_SuddenVoltageDrop(void)
{
  if (Protection_DW.is_SuddenVoltageDrop == Protection_IN_Error) {
    Protection_B.SuddenVoltageDrop = Error;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = duration((VoltageDrop_mV < Thresholds.Protections_SuddenVoltageDrop_mV_by_time),msec) >= Thresholds.SuddenVoltageDropTimeout_msec; */
    if ((real_T)Protection_B.Add1 >=
        Protection_U.Thresholds.Protections_SuddenVoltageDrop_mV_by_time) {
      Protection_DW.durationCounter_1_k = 0;
    }

    if (Protection_DW.durationCounter_1_k * 100 >=
        Protection_U.Thresholds.SuddenVoltageDropTimeout_msec) {
      Protection_DW.durationCounter_1_f = 0;
      Protection_DW.is_SuddenVoltageDrop = Protection_IN_Lobby;

      /* :  SuddenVoltageDrop = ProtectionFlags.NoError; */
      Protection_B.SuddenVoltageDrop = NoError;
    }
  } else {
    /* case IN_Lobby: */
    Protection_B.SuddenVoltageDrop = NoError;

    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = duration((VoltageDrop_mV >= Thresholds.Protections_SuddenVoltageDrop_mV_by_time),msec) >= Thresholds.SuddenVoltageDropTimeout_msec; */
    if ((real_T)Protection_B.Add1 <
        Protection_U.Thresholds.Protections_SuddenVoltageDrop_mV_by_time) {
      Protection_DW.durationCounter_1_f = 0;
    }

    if (Protection_DW.durationCounter_1_f * 100 >=
        Protection_U.Thresholds.SuddenVoltageDropTimeout_msec) {
      Protection_DW.durationCounter_1_k = 0;
      Protection_DW.is_SuddenVoltageDrop = Protection_IN_Error;

      /* :  SuddenVoltageDrop = ProtectionFlags.Error; */
      Protection_B.SuddenVoltageDrop = Error;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_ErrorDetect(void)
{
  if (Protection_DW.is_ErrorDetect == Protection_IN_Error) {
    Protection_B.ErrorDetect = Error;

    /* :  sf_internal_predicateOutput = ThermalRunaway~=ProtectionFlags.Error && TemperatureGradient~=ProtectionFlags.Error && HighImbalanceFlag~=ProtectionFlags.Error && ShortCircuitDetect~=ProtectionFlags.Error && SuddenVoltageDrop~=ProtectionFlags.Error && TempOverallState~=ProtectionFlags.Error && OCCFlag~=ProtectionFlags.Error && OCDFlag~=ProtectionFlags.Error && UVFlag~=ProtectionFlags.Error && OVFlag~=ProtectionFlags.Error && eFuseChargeFlag~=ProtectionFlags.Error && eFuseDischargeFlag~=ProtectionFlags.Error; */
    if ((Protection_B.ThermalRunaway != Error) &&
        (Protection_B.TemperatureGradient != Error) &&
        (Protection_B.HighImbalanceFlag != Error) &&
        (Protection_B.ShortCircuitDetect != Error) &&
        (Protection_B.SuddenVoltageDrop != Error) &&
        (Protection_B.TempOverallState != Error) && (Protection_B.OCCFlag !=
         Error) && (Protection_B.OCDFlag != Error) && (Protection_B.UVFlag !=
         Error) && (Protection_B.OVFlag != Error) &&
        (Protection_B.eFuseChargeFlag != Error) &&
        (Protection_B.eFuseDischargeFlag != Error)) {
      Protection_DW.is_ErrorDetect = Protection_IN_Lobby;

      /* :  ErrorDetect = ProtectionFlags.NoError; */
      Protection_B.ErrorDetect = NoError;
    }
  } else {
    /* case IN_Lobby: */
    Protection_B.ErrorDetect = NoError;

    /* :  sf_internal_predicateOutput = ThermalRunaway==ProtectionFlags.Error||TemperatureGradient==ProtectionFlags.Error||HighImbalanceFlag==ProtectionFlags.Error||ShortCircuitDetect==ProtectionFlags.Error||SuddenVoltageDrop == ProtectionFlags.Error||TempOverallState==ProtectionFlags.Error||OCCFlag==ProtectionFlags.Error||OCDFlag==ProtectionFlags.Error||UVFlag==ProtectionFlags.Error||OVFlag==ProtectionFlags.Error ||eFuseChargeFlag==ProtectionFlags.Error || eFuseDischargeFlag==ProtectionFlags.Error; */
    if ((Protection_B.ThermalRunaway == Error) ||
        (Protection_B.TemperatureGradient == Error) ||
        (Protection_B.HighImbalanceFlag == Error) ||
        (Protection_B.ShortCircuitDetect == Error) ||
        (Protection_B.SuddenVoltageDrop == Error) ||
        (Protection_B.TempOverallState == Error) || (Protection_B.OCCFlag ==
         Error) || (Protection_B.OCDFlag == Error) || (Protection_B.UVFlag ==
         Error) || (Protection_B.OVFlag == Error) ||
        (Protection_B.eFuseChargeFlag == Error) ||
        (Protection_B.eFuseDischargeFlag == Error)) {
      Protection_DW.is_ErrorDetect = Protection_IN_Error;

      /* :  ErrorDetect = ProtectionFlags.Error; */
      Protection_B.ErrorDetect = Error;
    }
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protection_OverallState(void)
{
  switch (Protection_DW.is_OverallState) {
   case Protection_IN_Error:
    Protection_B.TempOverallState = Error;

    /* :  sf_internal_predicateOutput = (FlagGroup1 == 3 && FlagGroup2 == 0)||(FlagGroup1 == 0 && FlagGroup2==3); */
    if ((((int32_T)Protection_B.FlagGroup1 == 3) && ((int32_T)
          Protection_B.FlagGroup2 == 0)) || (((int32_T)Protection_B.FlagGroup1 ==
          0) && ((int32_T)Protection_B.FlagGroup2 == 3))) {
      Protection_DW.is_OverallState = Protection_IN_Recovery;

      /* :  TempOverallState = ProtectionFlags.Recovery; */
      Protection_B.TempOverallState = Recovery;

      /* :  sf_internal_predicateOutput = FlagGroup1 == 0 && FlagGroup2 == 0; */
    } else if (((int32_T)Protection_B.FlagGroup1 == 0) && ((int32_T)
                Protection_B.FlagGroup2 == 0)) {
      Protection_DW.is_OverallState = Protection_IN_Lobby;

      /* :  TempOverallState = ProtectionFlags.NoError; */
      Protection_B.TempOverallState = NoError;
    }
    break;

   case Protection_IN_Lobby:
    Protection_B.TempOverallState = NoError;

    /* :  sf_internal_predicateOutput = FlagGroup1 >= 1 || FlagGroup2 >= 1; */
    if (((int32_T)Protection_B.FlagGroup1 >= 1) || ((int32_T)
         Protection_B.FlagGroup2 >= 1)) {
      Protection_DW.is_OverallState = Protection_IN_Warning;

      /* :  TempOverallState = ProtectionFlags.Warning; */
      Protection_B.TempOverallState = Warning;
    }
    break;

   case Protection_IN_Recovery:
    Protection_B.TempOverallState = Recovery;

    /* :  sf_internal_predicateOutput = FlagGroup1 == 0 && FlagGroup2 == 0; */
    if (((int32_T)Protection_B.FlagGroup1 == 0) && ((int32_T)
         Protection_B.FlagGroup2 == 0)) {
      Protection_DW.is_OverallState = Protection_IN_Lobby;

      /* :  TempOverallState = ProtectionFlags.NoError; */
      Protection_B.TempOverallState = NoError;
    }
    break;

   default:
    /* case IN_Warning: */
    Protection_B.TempOverallState = Warning;

    /* :  sf_internal_predicateOutput = FlagGroup1 >= 2 || FlagGroup2 >= 2; */
    if (((int32_T)Protection_B.FlagGroup1 >= 2) || ((int32_T)
         Protection_B.FlagGroup2 >= 2)) {
      Protection_DW.is_OverallState = Protection_IN_Error;

      /* :  TempOverallState = ProtectionFlags.Error; */
      Protection_B.TempOverallState = Error;

      /* :  sf_internal_predicateOutput = FlagGroup1 == 0 && FlagGroup2 == 0; */
    } else if (((int32_T)Protection_B.FlagGroup1 == 0) && ((int32_T)
                Protection_B.FlagGroup2 == 0)) {
      Protection_DW.is_OverallState = Protection_IN_Lobby;

      /* :  TempOverallState = ProtectionFlags.NoError; */
      Protection_B.TempOverallState = NoError;
    }
    break;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Protectio_DCL_CurrentProtection(void)
{
  int32_T k_x;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T guard3 = false;
  boolean_T guard4 = false;
  boolean_T guard5 = false;
  boolean_T guard6 = false;
  boolean_T guard7 = false;
  boolean_T guard8 = false;
  guard1 = false;
  guard2 = false;
  guard3 = false;
  guard4 = false;
  guard5 = false;
  guard6 = false;
  guard7 = false;
  guard8 = false;
  switch (Protection_DW.is_DCL_CurrentProtection) {
   case Protection_IN_DCLI_Error:
    Protection_B.DCLI_CurrentFlag = Error;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 1 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_ChargingCurrent_A; */
    if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard2 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        guard2 = true;
      }
    }
    break;

   case Protection_IN_DCLI_Warning:
    Protection_B.DCLI_CurrentFlag = Warning;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 1 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_ChargingCurrent_A; */
    if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard3 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        guard3 = true;

        /* Inport: '<Root>/Thresholds' */
        /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_WarningToErrorTimeout_msec; */
      } else if (Protection_B.IntegratedTime >
                 Protection_U.Thresholds.DCL_WarningToErrorTimeout_msec) {
        /* :  TimeReset=0; */
        Protection_B.TimeReset = 0.0;
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLI_Error;

        /* :  DCLI_CurrentFlag = ProtectionFlags.Error; */
        Protection_B.DCLI_CurrentFlag = Error;
      }
    }
    break;

   case Protection_IN_DCLO_Error:
    Protection_B.DCLO_CurrentFlag = Error;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 0 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_DisChargingCurrent_A; */
    if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard4 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        guard4 = true;
      }
    }
    break;

   case Protection_IN_DCLO_Warning:
    Protection_B.DCLO_CurrentFlag = Warning;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 0 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_DisChargingCurrent_A; */
    if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard5 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        guard5 = true;

        /* Inport: '<Root>/Thresholds' */
        /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_WarningToErrorTimeout_msec; */
      } else if (Protection_B.IntegratedTime >
                 Protection_U.Thresholds.DCL_WarningToErrorTimeout_msec) {
        /* :  TimeReset=0; */
        Protection_B.TimeReset = 0.0;
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLO_Error;

        /* :  DCLO_CurrentFlag = ProtectionFlags.Error; */
        Protection_B.DCLO_CurrentFlag = Error;
      }
    }
    break;

   case Protect_IN_DCL_Protection_Lobby:
    Protection_B.DCLI_CurrentFlag = NoError;
    Protection_B.DCLO_CurrentFlag = NoError;

    /* Inport: '<Root>/DataPipeline' */
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 0 && abs(DataPipeline.Current_mA/1000)>DataPipeline.DCL_DisChargingCurrent_A; */
    if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x > Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        /* :  TimeReset = 1; */
        Protection_B.TimeReset = 1.0;
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntStat3;
      } else {
        guard6 = true;
      }
    } else {
      guard6 = true;
    }
    break;

   case Protection_IN_IntStat1:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_CurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_CurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

      /* :  TimeReset = 0; */
      Protection_B.TimeReset = 0.0;

      /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLI_CurrentFlag = NoError;

      /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLO_CurrentFlag = NoError;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1 || abs(DataPipeline.Current_mA/1000)>DataPipeline.DCL_ChargingCurrent_A; */
    } else if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard7 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x > Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        guard7 = true;
      }
    }
    break;

   case Protection_IN_IntStat2:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_RecoveryCurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_RecoveryCurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

      /* :  TimeReset = 0; */
      Protection_B.TimeReset = 0.0;

      /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLI_CurrentFlag = NoError;

      /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLO_CurrentFlag = NoError;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1 && abs(DataPipeline.Current_mA/1000)>DataPipeline.DCL_ChargingCurrent_A; */
    } else if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x > Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLI_Error;

        /* :  DCLI_CurrentFlag = ProtectionFlags.Error; */
        Protection_B.DCLI_CurrentFlag = Error;
      }
    }
    break;

   case Protection_IN_IntStat3:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_CurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_CurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLO_Warning;

      /* :  TimeReset = 1; */
      Protection_B.TimeReset = 1.0;

      /* :  DCLO_CurrentFlag = ProtectionFlags.Warning; */
      Protection_B.DCLO_CurrentFlag = Warning;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 0 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_DisChargingCurrent_A; */
    } else if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard8 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        guard8 = true;
      }
    }
    break;

   case Protection_IN_IntStat4:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_CurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_CurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

      /* :  TimeReset = 0; */
      Protection_B.TimeReset = 0.0;

      /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLI_CurrentFlag = NoError;

      /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLO_CurrentFlag = NoError;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 0 && abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_DisChargingCurrent_A; */
    } else if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        /* :  TimeReset = 0; */
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLO_Warning;

        /* :  TimeReset = 1; */
        Protection_B.TimeReset = 1.0;

        /* :  DCLO_CurrentFlag = ProtectionFlags.Warning; */
        Protection_B.DCLO_CurrentFlag = Warning;
      }
    }
    break;

   case Protection_IN_IntStat5:
    /* Inport: '<Root>/Thresholds' */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_RecoveryCurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_RecoveryCurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

      /* :  TimeReset = 0; */
      Protection_B.TimeReset = 0.0;

      /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLI_CurrentFlag = NoError;

      /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
      Protection_B.DCLO_CurrentFlag = NoError;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 0 && abs(DataPipeline.Current_mA/1000)>DataPipeline.DCL_DisChargingCurrent_A; */
    } else if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x > Protection_U.DataPipeline.DCL_DisChargingCurrent_A) {
        /* :  TimeReset = 0; */
        Protection_B.TimeReset = 0.0;
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLO_Error;

        /* :  DCLO_CurrentFlag = ProtectionFlags.Error; */
        Protection_B.DCLO_CurrentFlag = Error;
      }
    }
    break;

   default:
    /* Inport: '<Root>/Thresholds' */
    /* case IN_IntState: */
    /* :  sf_internal_predicateOutput = IntegratedTime>Thresholds.DCL_CurrentProtectionTimeout_msec; */
    if (Protection_B.IntegratedTime >
        Protection_U.Thresholds.DCL_CurrentProtectionTimeout_msec) {
      /* :  TimeReset = 0; */
      Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLI_Warning;

      /* :  TimeReset = 1; */
      Protection_B.TimeReset = 1.0;

      /* :  DCLI_CurrentFlag = ProtectionFlags.Warning; */
      Protection_B.DCLI_CurrentFlag = Warning;

      /* Inport: '<Root>/DataPipeline' */
      /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected ~= 1 || abs(DataPipeline.Current_mA/1000)<DataPipeline.DCL_ChargingCurrent_A; */
    } else if (!Protection_U.DataPipeline.VCU.isChargerConnected) {
      guard1 = true;
    } else {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x < Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        guard1 = true;
      }
    }
    break;
  }

  if (guard8) {
    /* :  TimeReset = 0; */
    Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

    /* :  TimeReset = 0; */
    Protection_B.TimeReset = 0.0;

    /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
    Protection_B.DCLI_CurrentFlag = NoError;

    /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
    Protection_B.DCLO_CurrentFlag = NoError;
  }

  if (guard7) {
    /* :  TimeReset = 0; */
    Protection_DW.is_DCL_CurrentProtection = Protection_IN_DCLI_Warning;

    /* :  TimeReset = 1; */
    Protection_B.TimeReset = 1.0;

    /* :  DCLI_CurrentFlag = ProtectionFlags.Warning; */
    Protection_B.DCLI_CurrentFlag = Warning;
  }

  if (guard6) {
    /* :  sf_internal_predicateOutput = DataPipeline.VCU.isChargerConnected == 1 && abs(DataPipeline.Current_mA/1000)>DataPipeline.DCL_ChargingCurrent_A; */
    if (Protection_U.DataPipeline.VCU.isChargerConnected) {
      k_x = (int32_T)rt_roundd((real_T)Protection_U.DataPipeline.Current_mA /
        1000.0);
      if (k_x < 0) {
        k_x = -k_x;
      }

      if (k_x > Protection_U.DataPipeline.DCL_ChargingCurrent_A) {
        /* :  TimeReset = 1; */
        Protection_B.TimeReset = 1.0;
        Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntState;
      }
    }
  }

  if (guard5) {
    /* :  TimeReset = 1; */
    Protection_B.TimeReset = 1.0;
    Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntStat4;
  }

  if (guard4) {
    /* :  TimeReset = 1; */
    Protection_B.TimeReset = 1.0;
    Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntStat5;
  }

  if (guard3) {
    /* :  TimeReset = 1; */
    Protection_B.TimeReset = 1.0;
    Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntStat1;
  }

  if (guard2) {
    /* :  TimeReset = 1; */
    Protection_B.TimeReset = 1.0;
    Protection_DW.is_DCL_CurrentProtection = Protection_IN_IntStat2;
  }

  if (guard1) {
    Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

    /* :  TimeReset = 0; */
    Protection_B.TimeReset = 0.0;

    /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
    Protection_B.DCLI_CurrentFlag = NoError;

    /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
    Protection_B.DCLO_CurrentFlag = NoError;
  }
}

/* Function for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
static void Pr_enter_internal_c2_Protection(void)
{
  int32_T tmp;
  Protection_DW.is_OVThresholds_Selection = IN_SlowCharge_OVThrs_Selection;
  enter_atomic_SlowCharge_OVThrs_();
  Protection_DW.durationCounter_2_k5 = 0;
  Protection_DW.durationCounter_1_nq = 0;
  Protection_DW.is_Voltage_Protection = Protection_IN_No_Error_Voltage;

  /* :  UVFlag = ProtectionFlags.NoError; */
  Protection_B.UVFlag = NoError;

  /* :  OVFlag = ProtectionFlags.NoError; */
  Protection_B.OVFlag = NoError;
  Protection_DW.is_Current_Protection = Protection_IN_Lobby_f;

  /* Inport: '<Root>/Thresholds' */
  /* :  OCD_Warning_mA = Thresholds.Protections_OCD_Warning_mA; */
  Protection_DW.OCD_Warning_mA =
    Protection_U.Thresholds.Protections_OCD_Warning_mA;

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp < Protection_DW.OCD_Warning_mA) {
    Protection_DW.durationCounter_1_a = 0;
  }

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp > Protection_DW.OCD_Warning_mA) {
    Protection_DW.durationCounter_2 = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  /* :  OCD_Error_mA = Thresholds.Protections_OCD_Error_mA; */
  Protection_DW.OCD_Error_mA = Protection_U.Thresholds.Protections_OCD_Error_mA;

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp < Protection_DW.OCD_Error_mA) {
    Protection_DW.durationCounter_1_e = 0;
    Protection_DW.durationCounter_1_n = 0;
  }

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp > Protection_DW.OCD_Error_mA) {
    Protection_DW.durationCounter_2_c = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  /* :  OCC_Warning_mA = Thresholds.Protections_OCC_Warning_mA; */
  Protection_DW.OCC_Warning_mA =
    Protection_U.Thresholds.Protections_OCC_Warning_mA;

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp < Protection_DW.OCC_Warning_mA) {
    Protection_DW.durationCounter_2_l = 0;
  }

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp > Protection_DW.OCC_Warning_mA) {
    Protection_DW.durationCounter_1_cr = 0;
  }

  /* Inport: '<Root>/Thresholds' */
  /* :  OCC_Error_mA = Thresholds.Protections_OCC_Error_mA; */
  Protection_DW.OCC_Error_mA = Protection_U.Thresholds.Protections_OCC_Error_mA;

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp > Protection_DW.OCC_Error_mA) {
    Protection_DW.durationCounter_1_o = 0;
  }

  /* Inport: '<Root>/DataPipeline' */
  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      tmp = MAX_int32_T;
    } else {
      tmp = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    tmp = Protection_U.DataPipeline.Current_mA;
  }

  if (tmp < Protection_DW.OCC_Error_mA) {
    Protection_DW.durationCounter_2_f = 0;
    Protection_DW.durationCounter_2_d = 0;
  }

  Protection_DW.durationCounter_4 = 0;
  Protection_DW.durationCounter_3_f = 0;
  Protection_DW.durationCounter_2_lr = 0;
  Protection_DW.durationCounter_1_fk = 0;
  Protection_DW.is_Temperature_Protection_Group =
    Protect_IN_No_Error_Temperature;

  /* :  FlagGroup1 = ProtectionFlags.NoError; */
  Protection_B.FlagGroup1 = NoError;

  /* :  TempState1 = TempState.NoErr; */
  Protection_B.TempState1 = NoErr;
  Protection_DW.durationCounter_4_f = 0;
  Protection_DW.durationCounter_3_j = 0;
  Protection_DW.durationCounter_2_nn = 0;
  Protection_DW.durationCounter_1_bq = 0;
  Protection_DW.is_Temperature_Protection_Gro_g =
    Protect_IN_No_Error_Temperature;

  /* :  FlagGroup2 = ProtectionFlags.NoError; */
  Protection_B.FlagGroup2 = NoError;

  /* :  TempState2 = TempState.NoErr; */
  Protection_B.TempState2 = NoErr;
  Protection_DW.durationCounter_1 = 0;
  Protection_DW.is_ThermalRunaway = Prote_IN_ThermalRunaway_NoError;

  /* :  ThermalRunaway = ProtectionFlags.NoError; */
  Protection_B.ThermalRunaway = NoError;
  Protection_DW.durationCounter_1_p = 0;
  Protection_DW.durationCounter_1_ec = 0;
  Protection_DW.is_TemperatureGradient = Protection_IN_Lobby;

  /* :  TemperatureGradient = ProtectionFlags.NoError; */
  Protection_B.TemperatureGradient = NoError;
  Protection_DW.durationCounter_1_h = 0;
  Protection_DW.is_HighImbalanceFlag = Protection_IN_Lobby;

  /* :  HighImbalanceFlag = ProtectionFlags.NoError; */
  Protection_B.HighImbalanceFlag = NoError;
  Protection_DW.durationCounter_1_ht = 0;
  Protection_DW.is_ShortCircuitDetect = Protection_IN_Lobby;

  /* :  ShortCircuitDetect = ProtectionFlags.NoError; */
  Protection_B.ShortCircuitDetect = NoError;
  Protection_DW.durationCounter_1_f = 0;
  Protection_DW.is_SuddenVoltageDrop = Protection_IN_Lobby;

  /* :  SuddenVoltageDrop = ProtectionFlags.NoError; */
  Protection_B.SuddenVoltageDrop = NoError;
  Protection_DW.is_ErrorDetect = Protection_IN_Lobby;

  /* :  ErrorDetect = ProtectionFlags.NoError; */
  Protection_B.ErrorDetect = NoError;
  Protection_DW.is_OverallState = Protection_IN_Lobby;

  /* :  TempOverallState = ProtectionFlags.NoError; */
  Protection_B.TempOverallState = NoError;
  Protection_DW.is_DCL_CurrentProtection = Protect_IN_DCL_Protection_Lobby;

  /* :  TimeReset = 0; */
  Protection_B.TimeReset = 0.0;

  /* :  DCLI_CurrentFlag = ProtectionFlags.NoError; */
  Protection_B.DCLI_CurrentFlag = NoError;

  /* :  DCLO_CurrentFlag = ProtectionFlags.NoError; */
  Protection_B.DCLO_CurrentFlag = NoError;
}

/* Model step function */
void Protection_step(void)
{
  real_T tmp;
  int32_T rtb_Trigger;
  boolean_T rtb_NOT;
  boolean_T tmp_0;
  ZCEventType zcEvent;

  /* Chart: '<S1>/Chart' incorporates:
   *  Inport: '<Root>/Thresholds'
   */
  if (Protection_DW.temporalCounter_i1_p < 1U) {
    Protection_DW.temporalCounter_i1_p = 1U;
  }

  if (Protection_DW.is_active_c30_Protection == 0U) {
    Protection_DW.is_active_c30_Protection = 1U;
    Protection_DW.durationCounter_1_kc = 0;
    Protection_DW.is_c30_Protection = Protection_IN_Integrator_On;

    /* :  m = 1; */
    Protection_DW.m = 1U;

    /* :  Trigger = 0; */
    rtb_Trigger = 0;
  } else if (Protection_DW.is_c30_Protection == Protection_IN_Integrator_On) {
    rtb_Trigger = 0;

    /* :  sf_internal_predicateOutput = duration(m == 1,msec) == AverageTime_msec; */
    if (Protection_DW.m != 1) {
      Protection_DW.durationCounter_1_kc = 0;
    }

    if (Protection_DW.durationCounter_1_kc * 100 ==
        Protection_U.Thresholds.Protections_AverageTimeforProtection_msec) {
      Protection_DW.is_c30_Protection = Protection_IN_Integrator_Reset;
      Protection_DW.temporalCounter_i1_p = 0U;

      /* :  m = 0; */
      Protection_DW.m = 0U;
      Protection_DW.durationCounter_1_kc = 0;

      /* :  Trigger = 1; */
      rtb_Trigger = 1;
    }
  } else {
    /* case IN_Integrator_Reset: */
    /* :  sf_internal_predicateOutput = after(10,msec); */
    Protection_DW.durationCounter_1_kc = 0;
    Protection_DW.is_c30_Protection = Protection_IN_Integrator_On;

    /* :  m = 1; */
    Protection_DW.m = 1U;

    /* :  Trigger = 0; */
    rtb_Trigger = 0;
  }

  if (Protection_DW.m == 1) {
    Protection_DW.durationCounter_1_kc++;
  } else {
    Protection_DW.durationCounter_1_kc = 0;
  }

  /* End of Chart: '<S1>/Chart' */

  /* Outputs for Triggered SubSystem: '<S1>/Triggered Subsystem' incorporates:
   *  TriggerPort: '<S4>/Trigger'
   */
  zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                     &Protection_PrevZCX.TriggeredSubsystem_Trig_ZCE,
                     ((real_T)rtb_Trigger));
  if (zcEvent != NO_ZCEVENT) {
    /* SignalConversion generated from: '<S4>/Now_Tmax' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion'
     *  Inport: '<Root>/DataPipeline'
     */
    Protection_B.Now_Tmax = Protection_U.DataPipeline.TemperatureSenseBus[0].
      T_max;

    /* SignalConversion generated from: '<S4>/Now_mV_min' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion1'
     *  Inport: '<Root>/DataPipeline'
     */
    Protection_B.Now_mV_min = (real32_T)
      Protection_U.DataPipeline.VoltageSenseBus.mV_min;
  }

  /* End of Outputs for SubSystem: '<S1>/Triggered Subsystem' */

  /* Sum: '<S1>/Add' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  Inport: '<Root>/DataPipeline'
   */
  Protection_B.Add = (real32_T)Protection_U.DataPipeline.TemperatureSenseBus[0].
    T_max - Protection_B.Now_Tmax;

  /* Sum: '<S1>/Add1' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  Inport: '<Root>/DataPipeline'
   */
  Protection_B.Add1 = Protection_B.Now_mV_min - (real32_T)
    Protection_U.DataPipeline.VoltageSenseBus.mV_min;

  /* Logic: '<S1>/NOT' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   */
  rtb_NOT = (Protection_DW.UnitDelay_DSTATE == 0.0);

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  if (rtb_NOT || (Protection_DW.DiscreteTimeIntegrator_PrevRese != 0)) {
    Protection_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  }

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  Protection_B.IntegratedTime = Protection_DW.DiscreteTimeIntegrator_DSTATE;

  /* Chart: '<S1>/CurrentVoltageTemperature_Statemachine' incorporates:
   *  Inport: '<Root>/DataPipeline'
   *  Inport: '<Root>/Thresholds'
   */
  if (Protection_DW.temporalCounter_i1 < MAX_uint32_T) {
    Protection_DW.temporalCounter_i1++;
  }

  if (Protection_DW.is_active_c2_Protection == 0U) {
    Protection_DW.is_active_c2_Protection = 1U;
    Pr_enter_internal_c2_Protection();
  } else {
    Protecti_OVThresholds_Selection();
    Protection_Voltage_Protection();
    Protection_Current_Protection();
    P_Temperature_Protection_Group1();
    P_Temperature_Protection_Group2();
    Protection_ThermalRunaway();
    Protection_TemperatureGradient();
    Protection_HighImbalanceFlag();
    Protection_ShortCircuitDetect();
    Protection_SuddenVoltageDrop();
    Protection_ErrorDetect();
    Protection_OverallState();
    Protectio_DCL_CurrentProtection();
  }

  if (((real_T)Protection_B.Add >=
       Protection_U.Thresholds.Protections_MaxAllowedTempChange_C) &&
      (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
       Protection_U.Thresholds.Protections_MaxOperatingTemp_C)) {
    Protection_DW.durationCounter_1++;
  } else {
    Protection_DW.durationCounter_1 = 0;
  }

  if (((real_T)Protection_B.Add >=
       Protection_U.Thresholds.Protections_MaxAllowedTempChange_C) &&
      (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >
       Protection_U.Thresholds.Protections_MaxOperatingTemp_C)) {
    Protection_DW.durationCounter_1_c++;
  } else {
    Protection_DW.durationCounter_1_c = 0;
  }

  if (Protection_B.ThermalRunaway == Error) {
    Protection_DW.durationCounter_1_p++;
  } else {
    Protection_DW.durationCounter_1_p = 0;
  }

  if ((real_T)Protection_B.Add1 >=
      Protection_U.Thresholds.Protections_SuddenVoltageDrop_mV_by_time) {
    Protection_DW.durationCounter_1_f++;
  } else {
    Protection_DW.durationCounter_1_f = 0;
  }

  if ((real_T)Protection_B.Add1 <
      Protection_U.Thresholds.Protections_SuddenVoltageDrop_mV_by_time) {
    Protection_DW.durationCounter_1_k++;
  } else {
    Protection_DW.durationCounter_1_k = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCD_Error_mA) {
    Protection_DW.durationCounter_1_e++;
  } else {
    Protection_DW.durationCounter_1_e = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCD_Error_mA) {
    Protection_DW.durationCounter_1_n++;
  } else {
    Protection_DW.durationCounter_1_n = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCD_Warning_mA) {
    Protection_DW.durationCounter_1_a++;
  } else {
    Protection_DW.durationCounter_1_a = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger <= Protection_DW.OCD_Warning_mA) {
    Protection_DW.durationCounter_2++;
  } else {
    Protection_DW.durationCounter_2 = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger <= Protection_DW.OCD_Error_mA) {
    Protection_DW.durationCounter_2_c++;
  } else {
    Protection_DW.durationCounter_2_c = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger <= Protection_DW.OCC_Error_mA) {
    Protection_DW.durationCounter_1_o++;
  } else {
    Protection_DW.durationCounter_1_o = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCC_Warning_mA) {
    Protection_DW.durationCounter_2_l++;
  } else {
    Protection_DW.durationCounter_2_l = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger <= Protection_DW.OCC_Warning_mA) {
    Protection_DW.durationCounter_1_cr++;
  } else {
    Protection_DW.durationCounter_1_cr = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  tmp = fabsf(Protection_DW.DCL_Current_A);
  if (rt_roundd((real_T)rtb_Trigger / 1000.0) > tmp) {
    Protection_DW.durationCounter_1_eu++;
  } else {
    Protection_DW.durationCounter_1_eu = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rt_roundd((real_T)rtb_Trigger / 1000.0) < tmp) {
    Protection_DW.durationCounter_1_ag++;
  } else {
    Protection_DW.durationCounter_1_ag = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCC_Error_mA) {
    Protection_DW.durationCounter_2_f++;
  } else {
    Protection_DW.durationCounter_2_f = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_DW.OCC_Error_mA) {
    Protection_DW.durationCounter_2_d++;
  } else {
    Protection_DW.durationCounter_2_d = 0;
  }

  if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec >=
       Protection_U.DataPipeline.i2t_ChargingCurrent_A2sec) &&
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_2_d4++;
  } else {
    Protection_DW.durationCounter_2_d4 = 0;
  }

  tmp_0 = !Protection_U.DataPipeline.VCU.isChargerConnected;
  if (((real_T)Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec >=
       Protection_U.DataPipeline.i2t_DisChargingCurrent_A2sec) && tmp_0) {
    Protection_DW.durationCounter_3++;
  } else {
    Protection_DW.durationCounter_3 = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_min <
      Protection_U.Thresholds.Protections_UV_Error_mV) {
    Protection_DW.durationCounter_1_o0++;
  } else {
    Protection_DW.durationCounter_1_o0 = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min <
       Protection_U.Thresholds.Protections_UV_Error_mV) && tmp_0) {
    Protection_DW.durationCounter_1_k0++;
  } else {
    Protection_DW.durationCounter_1_k0 = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min >
       Protection_U.Thresholds.Protections_UV_Error_mV) ||
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_1_od++;
  } else {
    Protection_DW.durationCounter_1_od = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_min <=
      Protection_U.Thresholds.Protections_UV_Warning_mV) {
    Protection_DW.durationCounter_1_nq++;
  } else {
    Protection_DW.durationCounter_1_nq = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_min >
      Protection_U.Thresholds.Protections_UV_Warning_mV) {
    Protection_DW.durationCounter_2_k++;
  } else {
    Protection_DW.durationCounter_2_k = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_min >=
       Protection_U.Thresholds.Protections_UV_Recovery_mV) ||
      Protection_U.DataPipeline.VCU.isChargerConnected) {
    Protection_DW.durationCounter_2_b++;
  } else {
    Protection_DW.durationCounter_2_b = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >=
       Protection_DW.Protections_OV_Warning_mV) &&
      (Protection_U.DataPipeline.VCU.isChargerConnected ||
       (Protection_U.DataPipeline.Current_mA > 0))) {
    Protection_DW.durationCounter_2_k5++;
  } else {
    Protection_DW.durationCounter_2_k5 = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <=
       Protection_DW.Protections_OV_Recovery_mV) || tmp_0) {
    Protection_DW.durationCounter_1_oz++;
  } else {
    Protection_DW.durationCounter_1_oz = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_max <
      Protection_DW.Protections_OV_Warning_mV) {
    Protection_DW.durationCounter_1_cg++;
  } else {
    Protection_DW.durationCounter_1_cg = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max <
       Protection_DW.Protections_OV_Error_mV) || tmp_0) {
    Protection_DW.durationCounter_1_i++;
  } else {
    Protection_DW.durationCounter_1_i = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.mV_max >
      Protection_DW.Protections_OV_Error_mV) {
    Protection_DW.durationCounter_2_fr++;
  } else {
    Protection_DW.durationCounter_2_fr = 0;
  }

  if ((Protection_U.DataPipeline.VoltageSenseBus.mV_max >
       Protection_DW.Protections_OV_Error_mV) &&
      (Protection_U.DataPipeline.VCU.isChargerConnected ||
       (Protection_U.DataPipeline.Current_mA > 0))) {
    Protection_DW.durationCounter_2_kq++;
  } else {
    Protection_DW.durationCounter_2_kq = 0;
  }

  if (Protection_U.DataPipeline.DeltaTemperatureGroup1_C <
      Protection_U.Thresholds.Protections_MaxAllowedTempGradientGroup1_C) {
    Protection_DW.durationCounter_1_g++;
  } else {
    Protection_DW.durationCounter_1_g = 0;
  }

  if (Protection_U.DataPipeline.DeltaTemperatureGroup1_C >=
      Protection_U.Thresholds.Protections_MaxAllowedTempGradientGroup1_C) {
    Protection_DW.durationCounter_1_ec++;
  } else {
    Protection_DW.durationCounter_1_ec = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.VoltageDelta_mV >=
      Protection_U.Thresholds.Protections_HighImbalanceError_mV) {
    Protection_DW.durationCounter_1_h++;
  } else {
    Protection_DW.durationCounter_1_h = 0;
  }

  if (Protection_U.DataPipeline.VoltageSenseBus.VoltageDelta_mV <
      Protection_U.Thresholds.Protections_HighImbalanceError_mV) {
    Protection_DW.durationCounter_1_pt++;
  } else {
    Protection_DW.durationCounter_1_pt = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger >= Protection_U.Thresholds.Protections_ShortCircuitCurrent_mA)
  {
    Protection_DW.durationCounter_1_ht++;
  } else {
    Protection_DW.durationCounter_1_ht = 0;
  }

  if (Protection_U.DataPipeline.Current_mA < 0) {
    if (Protection_U.DataPipeline.Current_mA <= MIN_int32_T) {
      rtb_Trigger = MAX_int32_T;
    } else {
      rtb_Trigger = -Protection_U.DataPipeline.Current_mA;
    }
  } else {
    rtb_Trigger = Protection_U.DataPipeline.Current_mA;
  }

  if (rtb_Trigger < Protection_U.Thresholds.Protections_ShortCircuitCurrent_mA)
  {
    Protection_DW.durationCounter_1_fo++;
  } else {
    Protection_DW.durationCounter_1_fo = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <=
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_b++;
    Protection_DW.durationCounter_1_i0++;
  } else {
    Protection_DW.durationCounter_1_b = 0;
    Protection_DW.durationCounter_1_i0 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_pi++;
  } else {
    Protection_DW.durationCounter_1_pi = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <=
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Error_C) {
    Protection_DW.durationCounter_1_l++;
    Protection_DW.durationCounter_1_cp++;
  } else {
    Protection_DW.durationCounter_1_l = 0;
    Protection_DW.durationCounter_1_cp = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Error_C) {
    Protection_DW.durationCounter_1_ay++;
  } else {
    Protection_DW.durationCounter_1_ay = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Recovery_C) {
    Protection_DW.durationCounter_2_p++;
  } else {
    Protection_DW.durationCounter_2_p = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Warning_C) {
    Protection_DW.durationCounter_2_fo++;
  } else {
    Protection_DW.durationCounter_2_fo = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <=
      Protection_U.Thresholds.Temperature[0].Protections_UTD_Warning_C) {
    Protection_DW.durationCounter_1_fk++;
  } else {
    Protection_DW.durationCounter_1_fk = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min <=
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_lr++;
  } else {
    Protection_DW.durationCounter_2_lr = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_j++;
  } else {
    Protection_DW.durationCounter_2_j = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_min >=
      Protection_U.Thresholds.Temperature[0].Protections_UTC_Recovery_C) {
    Protection_DW.durationCounter_2_m++;
  } else {
    Protection_DW.durationCounter_2_m = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >=
      Protection_U.Thresholds.Temperature[0].Protections_OTD_Warning_C) {
    Protection_DW.durationCounter_3_f++;
  } else {
    Protection_DW.durationCounter_3_f = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTD_Warning_C) {
    Protection_DW.durationCounter_1_cc++;
  } else {
    Protection_DW.durationCounter_1_cc = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTD_Recovery_C) {
    Protection_DW.durationCounter_1_ga++;
  } else {
    Protection_DW.durationCounter_1_ga = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Warning_C) {
    Protection_DW.durationCounter_4++;
  } else {
    Protection_DW.durationCounter_4 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Recovery_C) {
    Protection_DW.durationCounter_1_a1++;
  } else {
    Protection_DW.durationCounter_1_a1 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Warning_C) {
    Protection_DW.durationCounter_1_bg++;
  } else {
    Protection_DW.durationCounter_1_bg = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_2_a++;
  } else {
    Protection_DW.durationCounter_2_a = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_1_p3++;
  } else {
    Protection_DW.durationCounter_1_p3 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >=
      Protection_U.Thresholds.Temperature[0].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_2_cm++;
  } else {
    Protection_DW.durationCounter_2_cm = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max <=
      Protection_U.Thresholds.Temperature[0].Protections_OTD_Error_C) {
    Protection_DW.durationCounter_1_ew++;
  } else {
    Protection_DW.durationCounter_1_ew = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[0].T_max >=
      Protection_U.Thresholds.Temperature[0].Protections_OTD_Error_C) {
    Protection_DW.durationCounter_2_kt++;
    Protection_DW.durationCounter_2_n++;
  } else {
    Protection_DW.durationCounter_2_kt = 0;
    Protection_DW.durationCounter_2_n = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_oa++;
    Protection_DW.durationCounter_1_o05++;
  } else {
    Protection_DW.durationCounter_1_oa = 0;
    Protection_DW.durationCounter_1_o05 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Error_C) {
    Protection_DW.durationCounter_1_j++;
  } else {
    Protection_DW.durationCounter_1_j = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <=
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Error_C) {
    Protection_DW.durationCounter_1_k3++;
    Protection_DW.durationCounter_1_ev++;
  } else {
    Protection_DW.durationCounter_1_k3 = 0;
    Protection_DW.durationCounter_1_ev = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Error_C) {
    Protection_DW.durationCounter_1_cy++;
  } else {
    Protection_DW.durationCounter_1_cy = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Warning_C) {
    Protection_DW.durationCounter_2_ad++;
  } else {
    Protection_DW.durationCounter_2_ad = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Warning_C) {
    Protection_DW.durationCounter_1_bq++;
  } else {
    Protection_DW.durationCounter_1_bq = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTD_Recovery_C) {
    Protection_DW.durationCounter_2_jr++;
  } else {
    Protection_DW.durationCounter_2_jr = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min <=
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_nn++;
  } else {
    Protection_DW.durationCounter_2_nn = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Warning_C) {
    Protection_DW.durationCounter_2_ke++;
  } else {
    Protection_DW.durationCounter_2_ke = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_min >=
      Protection_U.Thresholds.Temperature[1].Protections_UTC_Recovery_C) {
    Protection_DW.durationCounter_2_fb++;
  } else {
    Protection_DW.durationCounter_2_fb = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >=
      Protection_U.Thresholds.Temperature[1].Protections_OTD_Warning_C) {
    Protection_DW.durationCounter_3_j++;
  } else {
    Protection_DW.durationCounter_3_j = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTD_Warning_C) {
    Protection_DW.durationCounter_1_hd++;
  } else {
    Protection_DW.durationCounter_1_hd = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTD_Recovery_C) {
    Protection_DW.durationCounter_1_lb++;
  } else {
    Protection_DW.durationCounter_1_lb = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Warning_C) {
    Protection_DW.durationCounter_4_f++;
  } else {
    Protection_DW.durationCounter_4_f = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Recovery_C) {
    Protection_DW.durationCounter_1_bw++;
  } else {
    Protection_DW.durationCounter_1_bw = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Warning_C) {
    Protection_DW.durationCounter_1_bz++;
  } else {
    Protection_DW.durationCounter_1_bz = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_2_d2++;
  } else {
    Protection_DW.durationCounter_2_d2 = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_1_by++;
  } else {
    Protection_DW.durationCounter_1_by = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >=
      Protection_U.Thresholds.Temperature[1].Protections_OTC_Error_C) {
    Protection_DW.durationCounter_2_h++;
  } else {
    Protection_DW.durationCounter_2_h = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max <=
      Protection_U.Thresholds.Temperature[1].Protections_OTD_Error_C) {
    Protection_DW.durationCounter_1_jp++;
  } else {
    Protection_DW.durationCounter_1_jp = 0;
  }

  if (Protection_U.DataPipeline.TemperatureSenseBus[1].T_max >=
      Protection_U.Thresholds.Temperature[1].Protections_OTD_Error_C) {
    Protection_DW.durationCounter_2_mx++;
    Protection_DW.durationCounter_2_nng++;
  } else {
    Protection_DW.durationCounter_2_mx = 0;
    Protection_DW.durationCounter_2_nng = 0;
  }

  /* End of Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */

  /* BusCreator: '<S1>/Bus Creator' incorporates:
   *  Outport: '<Root>/ProtectionState_Out'
   */
  Protection_Y.ProtectionState_Out_j.ThermalRunaway =
    Protection_B.ThermalRunaway;
  Protection_Y.ProtectionState_Out_j.TemperatureGradient =
    Protection_B.TemperatureGradient;
  Protection_Y.ProtectionState_Out_j.HighImbalanceFlag =
    Protection_B.HighImbalanceFlag;
  Protection_Y.ProtectionState_Out_j.ShortCircuitDetect =
    Protection_B.ShortCircuitDetect;
  Protection_Y.ProtectionState_Out_j.SuddenVoltageDrop =
    Protection_B.SuddenVoltageDrop;
  Protection_Y.ProtectionState_Out_j.OV = Protection_B.OVFlag;
  Protection_Y.ProtectionState_Out_j.UV = Protection_B.UVFlag;
  Protection_Y.ProtectionState_Out_j.OCC = Protection_B.OCCFlag;
  Protection_Y.ProtectionState_Out_j.OCD = Protection_B.OCDFlag;
  Protection_Y.ProtectionState_Out_j.eFuseChargeFlag =
    Protection_B.eFuseChargeFlag;
  Protection_Y.ProtectionState_Out_j.eFuseDischargeFlag =
    Protection_B.eFuseDischargeFlag;
  Protection_Y.ProtectionState_Out_j.TempState1 = Protection_B.TempState1;
  Protection_Y.ProtectionState_Out_j.TempState2 = Protection_B.TempState2;
  Protection_Y.ProtectionState_Out_j.FlagGroup1 = Protection_B.FlagGroup1;
  Protection_Y.ProtectionState_Out_j.FlagGroup2 = Protection_B.FlagGroup2;
  Protection_Y.ProtectionState_Out_j.TempOverallState =
    Protection_B.TempOverallState;
  Protection_Y.ProtectionState_Out_j.ErrorDetect = Protection_B.ErrorDetect;
  Protection_Y.ProtectionState_Out_j.DCLI_CurrentFlag =
    Protection_B.DCLI_CurrentFlag;
  Protection_Y.ProtectionState_Out_j.DCLO_CurrentFlag =
    Protection_B.DCLO_CurrentFlag;

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  Protection_DW.UnitDelay_DSTATE = Protection_B.TimeReset;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
   *  Inport: '<Root>/Protection_LoopTime_msec'
   */
  Protection_DW.DiscreteTimeIntegrator_DSTATE += Protection_U.Protection_LoopTime_msec;
  Protection_DW.DiscreteTimeIntegrator_PrevRese = (int8_T)rtb_NOT;
}

/* Model initialize function */
void Protection_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Protection_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &Protection_B), 0,
                sizeof(B_Protection_T));

  {
    Protection_B.TempState2 = NoErr;
    Protection_B.TempState1 = NoErr;
    Protection_B.OCCFlag = NoError;
    Protection_B.OCDFlag = NoError;
    Protection_B.OVFlag = NoError;
    Protection_B.UVFlag = NoError;
    Protection_B.FlagGroup1 = NoError;
    Protection_B.FlagGroup2 = NoError;
    Protection_B.ThermalRunaway = NoError;
    Protection_B.TemperatureGradient = NoError;
    Protection_B.HighImbalanceFlag = NoError;
    Protection_B.ShortCircuitDetect = NoError;
    Protection_B.SuddenVoltageDrop = NoError;
    Protection_B.ErrorDetect = NoError;
    Protection_B.TempOverallState = NoError;
    Protection_B.eFuseDischargeFlag = NoError;
    Protection_B.eFuseChargeFlag = NoError;
    Protection_B.DCLI_CurrentFlag = NoError;
    Protection_B.DCLO_CurrentFlag = NoError;
  }

  /* states (dwork) */
  (void) memset((void *)&Protection_DW, 0,
                sizeof(DW_Protection_T));

  /* external inputs */
  (void)memset(&Protection_U, 0, sizeof(ExtU_Protection_T));

  /* external outputs */
  Protection_Y.ProtectionState_Out_j = Protection_rtZProtectionState_O;
  Protection_PrevZCX.TriggeredSubsystem_Trig_ZCE = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay' */
  Protection_DW.UnitDelay_DSTATE = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  Protection_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  Protection_DW.DiscreteTimeIntegrator_PrevRese = 0;

  /* SystemInitialize for Chart: '<S1>/Chart' */
  Protection_DW.temporalCounter_i1_p = 0U;
  Protection_DW.is_active_c30_Protection = 0U;
  Protection_DW.is_c30_Protection = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.m = 0U;

  /* SystemInitialize for Triggered SubSystem: '<S1>/Triggered Subsystem' */
  /* SystemInitialize for SignalConversion generated from: '<S4>/Now_Tmax' incorporates:
   *  Outport: '<S4>/Old_Tmax'
   */
  Protection_B.Now_Tmax = 0.0F;

  /* SystemInitialize for SignalConversion generated from: '<S4>/Now_mV_min' incorporates:
   *  Outport: '<S4>/Old_mV_min'
   */
  Protection_B.Now_mV_min = 0.0F;

  /* End of SystemInitialize for SubSystem: '<S1>/Triggered Subsystem' */

  /* SystemInitialize for Chart: '<S1>/CurrentVoltageTemperature_Statemachine' */
  Protection_DW.is_Current_Protection = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.temporalCounter_i1 = 0U;
  Protection_DW.is_DCL_CurrentProtection = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_ErrorDetect = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_HighImbalanceFlag = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_OVThresholds_Selection = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_OverallState = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_ShortCircuitDetect = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_SuddenVoltageDrop = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_TemperatureGradient = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_Temperature_Protection_Group = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_Temperature_Protection_Gro_g = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_ThermalRunaway = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_Voltage_Protection = Protection_IN_NO_ACTIVE_CHILD;
  Protection_DW.is_active_c2_Protection = 0U;
  Protection_DW.OCC_Error_mA = 0;
  Protection_DW.OCC_Warning_mA = 0;
  Protection_DW.OCD_Error_mA = 0;
  Protection_DW.OCD_Warning_mA = 0;
  Protection_DW.eFuseMeltCount = 0U;
  Protection_DW.DCL_Current_A = 0.0F;
  Protection_DW.Protections_OV_Recovery_mV = 3500;
  Protection_DW.Protections_OV_Warning_mV = 3400;
  Protection_DW.Protections_OV_Error_mV = 3600;
  Protection_B.OCCFlag = NoError;
  Protection_B.OCDFlag = NoError;
  Protection_B.OVFlag = NoError;
  Protection_B.UVFlag = NoError;
  Protection_B.TempState2 = NoErr;
  Protection_B.TempState1 = NoErr;
  Protection_B.FlagGroup1 = NoError;
  Protection_B.FlagGroup2 = NoError;
  Protection_B.ThermalRunaway = NoError;
  Protection_B.TemperatureGradient = NoError;
  Protection_B.HighImbalanceFlag = NoError;
  Protection_B.ShortCircuitDetect = NoError;
  Protection_B.SuddenVoltageDrop = NoError;
  Protection_B.ErrorDetect = NoError;
  Protection_B.TempOverallState = NoError;
  Protection_Y.ProtectionState_Out_j.i2t_Calculated_A2sec = 0.0F;
  Protection_B.eFuseDischargeFlag = NoError;
  Protection_B.eFuseChargeFlag = NoError;
  Protection_B.DCLI_CurrentFlag = NoError;
  Protection_B.DCLO_CurrentFlag = NoError;
  Protection_B.TimeReset = 0.0;
}

/* Model terminate function */
void Protection_terminate(void)
{
  /* (no terminate code required) */
}
