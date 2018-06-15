/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    MKE06Z80M48SF0RM, Rev.0.1, July 2013
**     Version:             rev. 1.0, 2013-07-30
**
**     Abstract:
**         CMSIS Peripheral Access Layer for MKE06Z4
**
**     Copyright: 1997 - 2013 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2013-07-30)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file MKE06Z4.h
 * @version 1.0
 * @date 2013-07-30
 * @brief CMSIS Peripheral Access Layer for MKE06Z4
 *
 * CMSIS Peripheral Access Layer for MKE06Z4
 */

#if !defined(SKEAZ1284_H_)
#define SKEAZ1284_H_                               /**< Symbol preventing repeated inclusion */

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0100u
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0000u


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
typedef enum IRQn {
  /* Core interrupts */
  NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
  HardFault_IRQn               = -13,              /**< Cortex-M0 SV Hard Fault Interrupt */
  SVCall_IRQn                  = -5,               /**< Cortex-M0 SV Call Interrupt */
  PendSV_IRQn                  = -2,               /**< Cortex-M0 Pend SV Interrupt */
  SysTick_IRQn                 = -1,               /**< Cortex-M0 System Tick Interrupt */

  /* Device specific interrupts */
  Reserved16_IRQn              = 0,                /**< Reserved interrupt 16 */
  Reserved17_IRQn              = 1,                /**< Reserved interrupt 17 */
  Reserved18_IRQn              = 2,                /**< Reserved interrupt 18 */
  Reserved19_IRQn              = 3,                /**< Reserved interrupt 19 */
  Reserved20_IRQn              = 4,                /**< Reserved interrupt 20 */
  FTMRE_IRQn                   = 5,                /**< FTMRE command complete/read collision interrupt */
  LVD_LVW_IRQn                 = 6,                /**< Low Voltage Detect, Low Voltage Warning */
  IRQ_IRQn                     = 7,                /**< External interrupt */
  I2C0_IRQn                    = 8,                /**< I2C0 interrupt */
  I2C1_IRQn                    = 9,                /**< I2C1 interrupt */
  SPI0_IRQn                    = 10,               /**< SPI0 interrupt */
  SPI1_IRQn                    = 11,               /**< SPI1 interrupt */
  UART0_IRQn                   = 12,               /**< UART0 status/error interrupt */
  UART1_IRQn                   = 13,               /**< UART1 status/error interrupt */
  UART2_IRQn                   = 14,               /**< UART2 status/error interrupt */
  ADC0_IRQn                    = 15,               /**< ADC0 interrupt */
  ACMP0_IRQn                   = 16,               /**< ACMP0 interrupt */
  FTM0_IRQn                    = 17,               /**< FTM0 Single interrupt vector for all sources */
  FTM1_IRQn                    = 18,               /**< FTM1 Single interrupt vector for all sources */
  FTM2_IRQn                    = 19,               /**< FTM2 Single interrupt vector for all sources */
  RTC_IRQn                     = 20,               /**< RTC overflow */
  ACMP1_IRQn                   = 21,               /**< ACMP1 interrupt */
  PIT_CH0_IRQn                 = 22,               /**< PIT CH0 overflow */
  PIT_CH1_IRQn                 = 23,               /**< PIT CH1 overflow */
  KBI0_IRQn                    = 24,               /**< Keyboard interrupt 0 */
  KBI1_IRQn                    = 25,               /**< Keyboard interrupt 1 */
  Reserved42_IRQn              = 26,               /**< Reserved interrupt 42 */
  ICS_IRQn                     = 27,               /**< ICS interrupt */
  Watchdog_IRQn                = 28,               /**< WDOG Interrupt */
  PWT_IRQn                     = 29,               /**< Pulse Width Timer Interrupt */
  MSCAN_RX_IRQn                = 30,               /**< MSCAN Rx Interrupt */
  MSCAN_TX_IRQn                = 31                /**< MSCAN Tx, Err and Wake-up interrupt */
} IRQn_Type;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Cortex M0 Core Configuration
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Cortex_Core_Configuration Cortex M0 Core Configuration
 * @{
 */

#define __CM0PLUS_REV                  0x0000    /**< Core revision r0p0 */
#define __MPU_PRESENT                  0         /**< Defines if an MPU is present or not */
#define __VTOR_PRESENT                 1         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               2         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */

#include "core_cm0plus.h"              /* Core Peripheral Access Layer */
#include "system_SKEAZ1284.h"            /* Device specific configuration file */

/*!
 * @}
 */ /* end of group Cortex_Core_Configuration */


/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- ACMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ACMP_Peripheral_Access_Layer ACMP Peripheral Access Layer
 * @{
 */

/** ACMP - Register Layout Typedef */
typedef struct {
  __IO uint8_t CS;                                 /**< ACMP Control and Status Register, offset: 0x0 */
  __IO uint8_t C0;                                 /**< ACMP Control Register 0, offset: 0x1 */
  __IO uint8_t C1;                                 /**< ACMP Control Register 1, offset: 0x2 */
  __IO uint8_t C2;                                 /**< ACMP Control Register 2, offset: 0x3 */
} ACMP_Type;

/* ----------------------------------------------------------------------------
   -- ACMP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ACMP_Register_Masks ACMP Register Masks
 * @{
 */

/* CS Bit Fields */
#define ACMP_CS_ACMOD_MASK                       0x3u
#define ACMP_CS_ACMOD_SHIFT                      0
#define ACMP_CS_ACMOD(x)                         (((uint8_t)(((uint8_t)(x))<<ACMP_CS_ACMOD_SHIFT))&ACMP_CS_ACMOD_MASK)
#define ACMP_CS_ACOPE_MASK                       0x4u
#define ACMP_CS_ACOPE_SHIFT                      2
#define ACMP_CS_ACO_MASK                         0x8u
#define ACMP_CS_ACO_SHIFT                        3
#define ACMP_CS_ACIE_MASK                        0x10u
#define ACMP_CS_ACIE_SHIFT                       4
#define ACMP_CS_ACF_MASK                         0x20u
#define ACMP_CS_ACF_SHIFT                        5
#define ACMP_CS_HYST_MASK                        0x40u
#define ACMP_CS_HYST_SHIFT                       6
#define ACMP_CS_ACE_MASK                         0x80u
#define ACMP_CS_ACE_SHIFT                        7
/* C0 Bit Fields */
#define ACMP_C0_ACNSEL_MASK                      0x3u
#define ACMP_C0_ACNSEL_SHIFT                     0
#define ACMP_C0_ACNSEL(x)                        (((uint8_t)(((uint8_t)(x))<<ACMP_C0_ACNSEL_SHIFT))&ACMP_C0_ACNSEL_MASK)
#define ACMP_C0_ACPSEL_MASK                      0x30u
#define ACMP_C0_ACPSEL_SHIFT                     4
#define ACMP_C0_ACPSEL(x)                        (((uint8_t)(((uint8_t)(x))<<ACMP_C0_ACPSEL_SHIFT))&ACMP_C0_ACPSEL_MASK)
/* C1 Bit Fields */
#define ACMP_C1_DACVAL_MASK                      0x3Fu
#define ACMP_C1_DACVAL_SHIFT                     0
#define ACMP_C1_DACVAL(x)                        (((uint8_t)(((uint8_t)(x))<<ACMP_C1_DACVAL_SHIFT))&ACMP_C1_DACVAL_MASK)
#define ACMP_C1_DACREF_MASK                      0x40u
#define ACMP_C1_DACREF_SHIFT                     6
#define ACMP_C1_DACEN_MASK                       0x80u
#define ACMP_C1_DACEN_SHIFT                      7
/* C2 Bit Fields */
#define ACMP_C2_ACIPE_MASK                       0x7u
#define ACMP_C2_ACIPE_SHIFT                      0
#define ACMP_C2_ACIPE(x)                         (((uint8_t)(((uint8_t)(x))<<ACMP_C2_ACIPE_SHIFT))&ACMP_C2_ACIPE_MASK)

/*!
 * @}
 */ /* end of group ACMP_Register_Masks */


/* ACMP - Peripheral instance base addresses */
/** Peripheral ACMP0 base address */
#define ACMP0_BASE                               (0x40073000u)
/** Peripheral ACMP0 base pointer */
#define ACMP0                                    ((ACMP_Type *)ACMP0_BASE)
/** Peripheral ACMP1 base address */
#define ACMP1_BASE                               (0x40074000u)
/** Peripheral ACMP1 base pointer */
#define ACMP1                                    ((ACMP_Type *)ACMP1_BASE)
/** Array initializer of ACMP peripheral base pointers */
#define ACMP_BASES                               { ACMP0, ACMP1 }

/*!
 * @}
 */ /* end of group ACMP_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- ADC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Peripheral_Access_Layer ADC Peripheral Access Layer
 * @{
 */

/** ADC - Register Layout Typedef */
typedef struct {
  __IO uint32_t SC1;                               /**< Status and Control Register 1, offset: 0x0 */
  __IO uint32_t SC2;                               /**< Status and Control Register 2, offset: 0x4 */
  __IO uint32_t SC3;                               /**< Status and Control Register 3, offset: 0x8 */
  __IO uint32_t SC4;                               /**< Status and Control Register 4, offset: 0xC */
  __I  uint32_t R;                                 /**< Conversion Result Register, offset: 0x10 */
  __IO uint32_t CV;                                /**< Compare Value Register, offset: 0x14 */
  __IO uint32_t APCTL1;                            /**< Pin Control 1 Register, offset: 0x18 */
  __IO uint32_t SC5;                               /**< Status and Control Register 5, offset: 0x1C */
} ADC_Type;

/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/* SC1 Bit Fields */
#define ADC_SC1_ADCH_MASK                        0x1Fu
#define ADC_SC1_ADCH_SHIFT                       0
#define ADC_SC1_ADCH(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC1_ADCH_SHIFT))&ADC_SC1_ADCH_MASK)
#define ADC_SC1_ADCO_MASK                        0x20u
#define ADC_SC1_ADCO_SHIFT                       5
#define ADC_SC1_AIEN_MASK                        0x40u
#define ADC_SC1_AIEN_SHIFT                       6
#define ADC_SC1_COCO_MASK                        0x80u
#define ADC_SC1_COCO_SHIFT                       7
/* SC2 Bit Fields */
#define ADC_SC2_REFSEL_MASK                      0x3u
#define ADC_SC2_REFSEL_SHIFT                     0
#define ADC_SC2_REFSEL(x)                        (((uint32_t)(((uint32_t)(x))<<ADC_SC2_REFSEL_SHIFT))&ADC_SC2_REFSEL_MASK)
#define ADC_SC2_FFULL_MASK                       0x4u
#define ADC_SC2_FFULL_SHIFT                      2
#define ADC_SC2_FEMPTY_MASK                      0x8u
#define ADC_SC2_FEMPTY_SHIFT                     3
#define ADC_SC2_ACFGT_MASK                       0x10u
#define ADC_SC2_ACFGT_SHIFT                      4
#define ADC_SC2_ACFE_MASK                        0x20u
#define ADC_SC2_ACFE_SHIFT                       5
#define ADC_SC2_ADTRG_MASK                       0x40u
#define ADC_SC2_ADTRG_SHIFT                      6
#define ADC_SC2_ADACT_MASK                       0x80u
#define ADC_SC2_ADACT_SHIFT                      7
/* SC3 Bit Fields */
#define ADC_SC3_ADICLK_MASK                      0x3u
#define ADC_SC3_ADICLK_SHIFT                     0
#define ADC_SC3_ADICLK(x)                        (((uint32_t)(((uint32_t)(x))<<ADC_SC3_ADICLK_SHIFT))&ADC_SC3_ADICLK_MASK)
#define ADC_SC3_MODE_MASK                        0xCu
#define ADC_SC3_MODE_SHIFT                       2
#define ADC_SC3_MODE(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_MODE_SHIFT))&ADC_SC3_MODE_MASK)
#define ADC_SC3_ADLSMP_MASK                      0x10u
#define ADC_SC3_ADLSMP_SHIFT                     4
#define ADC_SC3_ADIV_MASK                        0x60u
#define ADC_SC3_ADIV_SHIFT                       5
#define ADC_SC3_ADIV(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_ADIV_SHIFT))&ADC_SC3_ADIV_MASK)
#define ADC_SC3_ADLPC_MASK                       0x80u
#define ADC_SC3_ADLPC_SHIFT                      7
/* SC4 Bit Fields */
#define ADC_SC4_AFDEP_MASK                       0x7u
#define ADC_SC4_AFDEP_SHIFT                      0
#define ADC_SC4_AFDEP(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_SC4_AFDEP_SHIFT))&ADC_SC4_AFDEP_MASK)
#define ADC_SC4_ACFSEL_MASK                      0x20u
#define ADC_SC4_ACFSEL_SHIFT                     5
#define ADC_SC4_ASCANE_MASK                      0x40u
#define ADC_SC4_ASCANE_SHIFT                     6
#define ADC_SC4_HTRGME_MASK                      0x100u
#define ADC_SC4_HTRGME_SHIFT                     8
/* R Bit Fields */
#define ADC_R_ADR_MASK                           0xFFFu
#define ADC_R_ADR_SHIFT                          0
#define ADC_R_ADR(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_R_ADR_SHIFT))&ADC_R_ADR_MASK)
/* CV Bit Fields */
#define ADC_CV_CV_MASK                           0xFFFu
#define ADC_CV_CV_SHIFT                          0
#define ADC_CV_CV(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_CV_CV_SHIFT))&ADC_CV_CV_MASK)
/* APCTL1 Bit Fields */
#define ADC_APCTL1_ADPC_MASK                     0xFFFFu
#define ADC_APCTL1_ADPC_SHIFT                    0
#define ADC_APCTL1_ADPC(x)                       (((uint32_t)(((uint32_t)(x))<<ADC_APCTL1_ADPC_SHIFT))&ADC_APCTL1_ADPC_MASK)
/* SC5 Bit Fields */
#define ADC_SC5_HTRGMASKSEL_MASK                 0x1u
#define ADC_SC5_HTRGMASKSEL_SHIFT                0
#define ADC_SC5_HTRGMASKE_MASK                   0x2u
#define ADC_SC5_HTRGMASKE_SHIFT                  1

/*!
 * @}
 */ /* end of group ADC_Register_Masks */


/* ADC - Peripheral instance base addresses */
/** Peripheral ADC base address */
#define ADC_BASE                                 (0x4003B000u)
/** Peripheral ADC base pointer */
#define ADC                                      ((ADC_Type *)ADC_BASE)
/** Array initializer of ADC peripheral base pointers */
#define ADC_BASES                                { ADC }

/*!
 * @}
 */ /* end of group ADC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- CRC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Peripheral_Access_Layer CRC Peripheral Access Layer
 * @{
 */

/** CRC - Register Layout Typedef */
typedef struct {
  union {                                          /* offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      __IO uint16_t DATAL;                             /**< CRC_DATAL register., offset: 0x0 */
      __IO uint16_t DATAH;                             /**< CRC_DATAH register., offset: 0x2 */
    } ACCESS16BIT;
    __IO uint32_t DATA;                              /**< CRC Data register, offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      __IO uint8_t DATALL;                             /**< CRC_DATALL register., offset: 0x0 */
      __IO uint8_t DATALU;                             /**< CRC_DATALU register., offset: 0x1 */
      __IO uint8_t DATAHL;                             /**< CRC_DATAHL register., offset: 0x2 */
      __IO uint8_t DATAHU;                             /**< CRC_DATAHU register., offset: 0x3 */
    } ACCESS8BIT;
  };
  union {                                          /* offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint16_t GPOLYL;                            /**< CRC_GPOLYL register., offset: 0x4 */
      __IO uint16_t GPOLYH;                            /**< CRC_GPOLYH register., offset: 0x6 */
    } GPOLY_ACCESS16BIT;
    __IO uint32_t GPOLY;                             /**< CRC Polynomial register, offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint8_t GPOLYLL;                            /**< CRC_GPOLYLL register., offset: 0x4 */
      __IO uint8_t GPOLYLU;                            /**< CRC_GPOLYLU register., offset: 0x5 */
      __IO uint8_t GPOLYHL;                            /**< CRC_GPOLYHL register., offset: 0x6 */
      __IO uint8_t GPOLYHU;                            /**< CRC_GPOLYHU register., offset: 0x7 */
    } GPOLY_ACCESS8BIT;
  };
  union {                                          /* offset: 0x8 */
    __IO uint32_t CTRL;                              /**< CRC Control register, offset: 0x8 */
    struct {                                         /* offset: 0x8 */
           uint8_t RESERVED_0[3];
      __IO uint8_t CTRLHU;                             /**< CRC_CTRLHU register., offset: 0xB */
    } CTRL_ACCESS8BIT;
  };
} CRC_Type;

/* ----------------------------------------------------------------------------
   -- CRC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Register_Masks CRC Register Masks
 * @{
 */

/* DATAL Bit Fields */
#define CRC_DATAL_DATAL_MASK                     0xFFFFu
#define CRC_DATAL_DATAL_SHIFT                    0
#define CRC_DATAL_DATAL(x)                       (((uint16_t)(((uint16_t)(x))<<CRC_DATAL_DATAL_SHIFT))&CRC_DATAL_DATAL_MASK)
/* DATAH Bit Fields */
#define CRC_DATAH_DATAH_MASK                     0xFFFFu
#define CRC_DATAH_DATAH_SHIFT                    0
#define CRC_DATAH_DATAH(x)                       (((uint16_t)(((uint16_t)(x))<<CRC_DATAH_DATAH_SHIFT))&CRC_DATAH_DATAH_MASK)
/* DATA Bit Fields */
#define CRC_DATA_LL_MASK                         0xFFu
#define CRC_DATA_LL_SHIFT                        0
#define CRC_DATA_LL(x)                           (((uint32_t)(((uint32_t)(x))<<CRC_DATA_LL_SHIFT))&CRC_DATA_LL_MASK)
#define CRC_DATA_LU_MASK                         0xFF00u
#define CRC_DATA_LU_SHIFT                        8
#define CRC_DATA_LU(x)                           (((uint32_t)(((uint32_t)(x))<<CRC_DATA_LU_SHIFT))&CRC_DATA_LU_MASK)
#define CRC_DATA_HL_MASK                         0xFF0000u
#define CRC_DATA_HL_SHIFT                        16
#define CRC_DATA_HL(x)                           (((uint32_t)(((uint32_t)(x))<<CRC_DATA_HL_SHIFT))&CRC_DATA_HL_MASK)
#define CRC_DATA_HU_MASK                         0xFF000000u
#define CRC_DATA_HU_SHIFT                        24
#define CRC_DATA_HU(x)                           (((uint32_t)(((uint32_t)(x))<<CRC_DATA_HU_SHIFT))&CRC_DATA_HU_MASK)
/* DATALL Bit Fields */
#define CRC_DATALL_DATALL_MASK                   0xFFu
#define CRC_DATALL_DATALL_SHIFT                  0
#define CRC_DATALL_DATALL(x)                     (((uint8_t)(((uint8_t)(x))<<CRC_DATALL_DATALL_SHIFT))&CRC_DATALL_DATALL_MASK)
/* DATALU Bit Fields */
#define CRC_DATALU_DATALU_MASK                   0xFFu
#define CRC_DATALU_DATALU_SHIFT                  0
#define CRC_DATALU_DATALU(x)                     (((uint8_t)(((uint8_t)(x))<<CRC_DATALU_DATALU_SHIFT))&CRC_DATALU_DATALU_MASK)
/* DATAHL Bit Fields */
#define CRC_DATAHL_DATAHL_MASK                   0xFFu
#define CRC_DATAHL_DATAHL_SHIFT                  0
#define CRC_DATAHL_DATAHL(x)                     (((uint8_t)(((uint8_t)(x))<<CRC_DATAHL_DATAHL_SHIFT))&CRC_DATAHL_DATAHL_MASK)
/* DATAHU Bit Fields */
#define CRC_DATAHU_DATAHU_MASK                   0xFFu
#define CRC_DATAHU_DATAHU_SHIFT                  0
#define CRC_DATAHU_DATAHU(x)                     (((uint8_t)(((uint8_t)(x))<<CRC_DATAHU_DATAHU_SHIFT))&CRC_DATAHU_DATAHU_MASK)
/* GPOLYL Bit Fields */
#define CRC_GPOLYL_GPOLYL_MASK                   0xFFFFu
#define CRC_GPOLYL_GPOLYL_SHIFT                  0
#define CRC_GPOLYL_GPOLYL(x)                     (((uint16_t)(((uint16_t)(x))<<CRC_GPOLYL_GPOLYL_SHIFT))&CRC_GPOLYL_GPOLYL_MASK)
/* GPOLYH Bit Fields */
#define CRC_GPOLYH_GPOLYH_MASK                   0xFFFFu
#define CRC_GPOLYH_GPOLYH_SHIFT                  0
#define CRC_GPOLYH_GPOLYH(x)                     (((uint16_t)(((uint16_t)(x))<<CRC_GPOLYH_GPOLYH_SHIFT))&CRC_GPOLYH_GPOLYH_MASK)
/* GPOLY Bit Fields */
#define CRC_GPOLY_LOW_MASK                       0xFFFFu
#define CRC_GPOLY_LOW_SHIFT                      0
#define CRC_GPOLY_LOW(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_LOW_SHIFT))&CRC_GPOLY_LOW_MASK)
#define CRC_GPOLY_HIGH_MASK                      0xFFFF0000u
#define CRC_GPOLY_HIGH_SHIFT                     16
#define CRC_GPOLY_HIGH(x)                        (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_HIGH_SHIFT))&CRC_GPOLY_HIGH_MASK)
/* GPOLYLL Bit Fields */
#define CRC_GPOLYLL_GPOLYLL_MASK                 0xFFu
#define CRC_GPOLYLL_GPOLYLL_SHIFT                0
#define CRC_GPOLYLL_GPOLYLL(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYLL_GPOLYLL_SHIFT))&CRC_GPOLYLL_GPOLYLL_MASK)
/* GPOLYLU Bit Fields */
#define CRC_GPOLYLU_GPOLYLU_MASK                 0xFFu
#define CRC_GPOLYLU_GPOLYLU_SHIFT                0
#define CRC_GPOLYLU_GPOLYLU(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYLU_GPOLYLU_SHIFT))&CRC_GPOLYLU_GPOLYLU_MASK)
/* GPOLYHL Bit Fields */
#define CRC_GPOLYHL_GPOLYHL_MASK                 0xFFu
#define CRC_GPOLYHL_GPOLYHL_SHIFT                0
#define CRC_GPOLYHL_GPOLYHL(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYHL_GPOLYHL_SHIFT))&CRC_GPOLYHL_GPOLYHL_MASK)
/* GPOLYHU Bit Fields */
#define CRC_GPOLYHU_GPOLYHU_MASK                 0xFFu
#define CRC_GPOLYHU_GPOLYHU_SHIFT                0
#define CRC_GPOLYHU_GPOLYHU(x)                   (((uint8_t)(((uint8_t)(x))<<CRC_GPOLYHU_GPOLYHU_SHIFT))&CRC_GPOLYHU_GPOLYHU_MASK)
/* CTRL Bit Fields */
#define CRC_CTRL_TCRC_MASK                       0x1000000u
#define CRC_CTRL_TCRC_SHIFT                      24
#define CRC_CTRL_WAS_MASK                        0x2000000u
#define CRC_CTRL_WAS_SHIFT                       25
#define CRC_CTRL_FXOR_MASK                       0x4000000u
#define CRC_CTRL_FXOR_SHIFT                      26
#define CRC_CTRL_TOTR_MASK                       0x30000000u
#define CRC_CTRL_TOTR_SHIFT                      28
#define CRC_CTRL_TOTR(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOTR_SHIFT))&CRC_CTRL_TOTR_MASK)
#define CRC_CTRL_TOT_MASK                        0xC0000000u
#define CRC_CTRL_TOT_SHIFT                       30
#define CRC_CTRL_TOT(x)                          (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOT_SHIFT))&CRC_CTRL_TOT_MASK)
/* CTRLHU Bit Fields */
#define CRC_CTRLHU_TCRC_MASK                     0x1u
#define CRC_CTRLHU_TCRC_SHIFT                    0
#define CRC_CTRLHU_WAS_MASK                      0x2u
#define CRC_CTRLHU_WAS_SHIFT                     1
#define CRC_CTRLHU_FXOR_MASK                     0x4u
#define CRC_CTRLHU_FXOR_SHIFT                    2
#define CRC_CTRLHU_TOTR_MASK                     0x30u
#define CRC_CTRLHU_TOTR_SHIFT                    4
#define CRC_CTRLHU_TOTR(x)                       (((uint8_t)(((uint8_t)(x))<<CRC_CTRLHU_TOTR_SHIFT))&CRC_CTRLHU_TOTR_MASK)
#define CRC_CTRLHU_TOT_MASK                      0xC0u
#define CRC_CTRLHU_TOT_SHIFT                     6
#define CRC_CTRLHU_TOT(x)                        (((uint8_t)(((uint8_t)(x))<<CRC_CTRLHU_TOT_SHIFT))&CRC_CTRLHU_TOT_MASK)

/*!
 * @}
 */ /* end of group CRC_Register_Masks */


/* CRC - Peripheral instance base addresses */
/** Peripheral CRC base address */
#define CRC_BASE                                 (0x40032000u)
/** Peripheral CRC base pointer */
#define CRC0                                     ((CRC_Type *)CRC_BASE)
/** Array initializer of CRC peripheral base pointers */
#define CRC_BASES                                { CRC0 }

/*!
 * @}
 */ /* end of group CRC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- FGPIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Peripheral_Access_Layer FGPIO Peripheral Access Layer
 * @{
 */

/** FGPIO - Register Layout Typedef */
typedef struct {
  __IO uint32_t PDOR;                              /**< Port Data Output Register, offset: 0x0 */
  __O  uint32_t PSOR;                              /**< Port Set Output Register, offset: 0x4 */
  __O  uint32_t PCOR;                              /**< Port Clear Output Register, offset: 0x8 */
  __O  uint32_t PTOR;                              /**< Port Toggle Output Register, offset: 0xC */
  __I  uint32_t PDIR;                              /**< Port Data Input Register, offset: 0x10 */
  __IO uint32_t PDDR;                              /**< Port Data Direction Register, offset: 0x14 */
  __IO uint32_t PIDR;                              /**< Port Input Disable Register, offset: 0x18 */
} FGPIO_Type;

/* ----------------------------------------------------------------------------
   -- FGPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Register_Masks FGPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define FGPIO_PDOR_PDO_MASK                      0xFFFFFFFFu
#define FGPIO_PDOR_PDO_SHIFT                     0
#define FGPIO_PDOR_PDO(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDOR_PDO_SHIFT))&FGPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define FGPIO_PSOR_PTSO_MASK                     0xFFFFFFFFu
#define FGPIO_PSOR_PTSO_SHIFT                    0
#define FGPIO_PSOR_PTSO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PSOR_PTSO_SHIFT))&FGPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define FGPIO_PCOR_PTCO_MASK                     0xFFFFFFFFu
#define FGPIO_PCOR_PTCO_SHIFT                    0
#define FGPIO_PCOR_PTCO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PCOR_PTCO_SHIFT))&FGPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define FGPIO_PTOR_PTTO_MASK                     0xFFFFFFFFu
#define FGPIO_PTOR_PTTO_SHIFT                    0
#define FGPIO_PTOR_PTTO(x)                       (((uint32_t)(((uint32_t)(x))<<FGPIO_PTOR_PTTO_SHIFT))&FGPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define FGPIO_PDIR_PDI_MASK                      0xFFFFFFFFu
#define FGPIO_PDIR_PDI_SHIFT                     0
#define FGPIO_PDIR_PDI(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDIR_PDI_SHIFT))&FGPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define FGPIO_PDDR_PDD_MASK                      0xFFFFFFFFu
#define FGPIO_PDDR_PDD_SHIFT                     0
#define FGPIO_PDDR_PDD(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PDDR_PDD_SHIFT))&FGPIO_PDDR_PDD_MASK)
/* PIDR Bit Fields */
#define FGPIO_PIDR_PID_MASK                      0xFFFFFFFFu
#define FGPIO_PIDR_PID_SHIFT                     0
#define FGPIO_PIDR_PID(x)                        (((uint32_t)(((uint32_t)(x))<<FGPIO_PIDR_PID_SHIFT))&FGPIO_PIDR_PID_MASK)

/*!
 * @}
 */ /* end of group FGPIO_Register_Masks */


/* FGPIO - Peripheral instance base addresses */
/** Peripheral FGPIOA base address */
#define FGPIOA_BASE                              (0xF8000000u)
/** Peripheral FGPIOA base pointer */
#define FGPIOA                                   ((FGPIO_Type *)FGPIOA_BASE)
/** Peripheral FGPIOB base address */
#define FGPIOB_BASE                              (0xF8000040u)
/** Peripheral FGPIOB base pointer */
#define FGPIOB                                   ((FGPIO_Type *)FGPIOB_BASE)
/** Peripheral FGPIOC base address */
#define FGPIOC_BASE                              (0xF8000080u)
/** Peripheral FGPIOC base pointer */
#define FGPIOC                                   ((FGPIO_Type *)FGPIOC_BASE)
/** Array initializer of FGPIO peripheral base pointers */
#define FGPIO_BASES                              { FGPIOA, FGPIOB, FGPIOC }

/*!
 * @}
 */ /* end of group FGPIO_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- FTM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTM_Peripheral_Access_Layer FTM Peripheral Access Layer
 * @{
 */

/** FTM - Register Layout Typedef */
typedef struct {
  __IO uint32_t SC;                                /**< Status And Control, offset: 0x0 */
  __IO uint32_t CNT;                               /**< Counter, offset: 0x4 */
  __IO uint32_t MOD;                               /**< Modulo, offset: 0x8 */
  struct {                                         /* offset: 0xC, array step: 0x8 */
    __IO uint32_t CnSC;                              /**< Channel (n) Status And Control, array offset: 0xC, array step: 0x8 */
    __IO uint32_t CnV;                               /**< Channel (n) Value, array offset: 0x10, array step: 0x8 */
  } CONTROLS[6];
       uint8_t RESERVED_0[16];
  __IO uint32_t CNTIN;                             /**< Counter Initial Value, offset: 0x4C */
  __IO uint32_t STATUS;                            /**< Capture And Compare Status, offset: 0x50 */
  __IO uint32_t MODE;                              /**< Features Mode Selection, offset: 0x54 */
  __IO uint32_t SYNC;                              /**< Synchronization, offset: 0x58 */
  __IO uint32_t OUTINIT;                           /**< Initial State For Channels Output, offset: 0x5C */
  __IO uint32_t OUTMASK;                           /**< Output Mask, offset: 0x60 */
  __IO uint32_t COMBINE;                           /**< Function For Linked Channels, offset: 0x64 */
  __IO uint32_t DEADTIME;                          /**< Deadtime Insertion Control, offset: 0x68 */
  __IO uint32_t EXTTRIG;                           /**< FTM External Trigger, offset: 0x6C */
  __IO uint32_t POL;                               /**< Channels Polarity, offset: 0x70 */
  __IO uint32_t FMS;                               /**< Fault Mode Status, offset: 0x74 */
  __IO uint32_t FILTER;                            /**< Input Capture Filter Control, offset: 0x78 */
  __IO uint32_t FLTCTRL;                           /**< Fault Control, offset: 0x7C */
       uint8_t RESERVED_1[4];
  __IO uint32_t CONF;                              /**< Configuration, offset: 0x84 */
  __IO uint32_t FLTPOL;                            /**< FTM Fault Input Polarity, offset: 0x88 */
  __IO uint32_t SYNCONF;                           /**< Synchronization Configuration, offset: 0x8C */
  __IO uint32_t INVCTRL;                           /**< FTM Inverting Control, offset: 0x90 */
  __IO uint32_t SWOCTRL;                           /**< FTM Software Output Control, offset: 0x94 */
  __IO uint32_t PWMLOAD;                           /**< FTM PWM Load, offset: 0x98 */
} FTM_Type;

/* ----------------------------------------------------------------------------
   -- FTM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTM_Register_Masks FTM Register Masks
 * @{
 */

/* SC Bit Fields */
#define FTM_SC_PS_MASK                           0x7u
#define FTM_SC_PS_SHIFT                          0
#define FTM_SC_PS(x)                             (((uint32_t)(((uint32_t)(x))<<FTM_SC_PS_SHIFT))&FTM_SC_PS_MASK)
#define FTM_SC_CLKS_MASK                         0x18u
#define FTM_SC_CLKS_SHIFT                        3
#define FTM_SC_CLKS(x)                           (((uint32_t)(((uint32_t)(x))<<FTM_SC_CLKS_SHIFT))&FTM_SC_CLKS_MASK)
#define FTM_SC_CPWMS_MASK                        0x20u
#define FTM_SC_CPWMS_SHIFT                       5
#define FTM_SC_TOIE_MASK                         0x40u
#define FTM_SC_TOIE_SHIFT                        6
#define FTM_SC_TOF_MASK                          0x80u
#define FTM_SC_TOF_SHIFT                         7
/* CNT Bit Fields */
#define FTM_CNT_COUNT_MASK                       0xFFFFu
#define FTM_CNT_COUNT_SHIFT                      0
#define FTM_CNT_COUNT(x)                         (((uint32_t)(((uint32_t)(x))<<FTM_CNT_COUNT_SHIFT))&FTM_CNT_COUNT_MASK)
/* MOD Bit Fields */
#define FTM_MOD_MOD_MASK                         0xFFFFu
#define FTM_MOD_MOD_SHIFT                        0
#define FTM_MOD_MOD(x)                           (((uint32_t)(((uint32_t)(x))<<FTM_MOD_MOD_SHIFT))&FTM_MOD_MOD_MASK)
/* CnSC Bit Fields */
#define FTM_CnSC_ELSA_MASK                       0x4u
#define FTM_CnSC_ELSA_SHIFT                      2
#define FTM_CnSC_ELSB_MASK                       0x8u
#define FTM_CnSC_ELSB_SHIFT                      3
#define FTM_CnSC_MSA_MASK                        0x10u
#define FTM_CnSC_MSA_SHIFT                       4
#define FTM_CnSC_MSB_MASK                        0x20u
#define FTM_CnSC_MSB_SHIFT                       5
#define FTM_CnSC_CHIE_MASK                       0x40u
#define FTM_CnSC_CHIE_SHIFT                      6
#define FTM_CnSC_CHF_MASK                        0x80u
#define FTM_CnSC_CHF_SHIFT                       7
/* CnV Bit Fields */
#define FTM_CnV_VAL_MASK                         0xFFFFu
#define FTM_CnV_VAL_SHIFT                        0
#define FTM_CnV_VAL(x)                           (((uint32_t)(((uint32_t)(x))<<FTM_CnV_VAL_SHIFT))&FTM_CnV_VAL_MASK)
/* CNTIN Bit Fields */
#define FTM_CNTIN_INIT_MASK                      0xFFFFu
#define FTM_CNTIN_INIT_SHIFT                     0
#define FTM_CNTIN_INIT(x)                        (((uint32_t)(((uint32_t)(x))<<FTM_CNTIN_INIT_SHIFT))&FTM_CNTIN_INIT_MASK)
/* STATUS Bit Fields */
#define FTM_STATUS_CH0F_MASK                     0x1u
#define FTM_STATUS_CH0F_SHIFT                    0
#define FTM_STATUS_CH1F_MASK                     0x2u
#define FTM_STATUS_CH1F_SHIFT                    1
#define FTM_STATUS_CH2F_MASK                     0x4u
#define FTM_STATUS_CH2F_SHIFT                    2
#define FTM_STATUS_CH3F_MASK                     0x8u
#define FTM_STATUS_CH3F_SHIFT                    3
#define FTM_STATUS_CH4F_MASK                     0x10u
#define FTM_STATUS_CH4F_SHIFT                    4
#define FTM_STATUS_CH5F_MASK                     0x20u
#define FTM_STATUS_CH5F_SHIFT                    5
#define FTM_STATUS_CH6F_MASK                     0x40u
#define FTM_STATUS_CH6F_SHIFT                    6
#define FTM_STATUS_CH7F_MASK                     0x80u
#define FTM_STATUS_CH7F_SHIFT                    7
/* MODE Bit Fields */
#define FTM_MODE_FTMEN_MASK                      0x1u
#define FTM_MODE_FTMEN_SHIFT                     0
#define FTM_MODE_INIT_MASK                       0x2u
#define FTM_MODE_INIT_SHIFT                      1
#define FTM_MODE_WPDIS_MASK                      0x4u
#define FTM_MODE_WPDIS_SHIFT                     2
#define FTM_MODE_PWMSYNC_MASK                    0x8u
#define FTM_MODE_PWMSYNC_SHIFT                   3
#define FTM_MODE_CAPTEST_MASK                    0x10u
#define FTM_MODE_CAPTEST_SHIFT                   4
#define FTM_MODE_FAULTM_MASK                     0x60u
#define FTM_MODE_FAULTM_SHIFT                    5
#define FTM_MODE_FAULTM(x)                       (((uint32_t)(((uint32_t)(x))<<FTM_MODE_FAULTM_SHIFT))&FTM_MODE_FAULTM_MASK)
#define FTM_MODE_FAULTIE_MASK                    0x80u
#define FTM_MODE_FAULTIE_SHIFT                   7
/* SYNC Bit Fields */
#define FTM_SYNC_CNTMIN_MASK                     0x1u
#define FTM_SYNC_CNTMIN_SHIFT                    0
#define FTM_SYNC_CNTMAX_MASK                     0x2u
#define FTM_SYNC_CNTMAX_SHIFT                    1
#define FTM_SYNC_REINIT_MASK                     0x4u
#define FTM_SYNC_REINIT_SHIFT                    2
#define FTM_SYNC_SYNCHOM_MASK                    0x8u
#define FTM_SYNC_SYNCHOM_SHIFT                   3
#define FTM_SYNC_TRIG0_MASK                      0x10u
#define FTM_SYNC_TRIG0_SHIFT                     4
#define FTM_SYNC_TRIG1_MASK                      0x20u
#define FTM_SYNC_TRIG1_SHIFT                     5
#define FTM_SYNC_TRIG2_MASK                      0x40u
#define FTM_SYNC_TRIG2_SHIFT                     6
#define FTM_SYNC_SWSYNC_MASK                     0x80u
#define FTM_SYNC_SWSYNC_SHIFT                    7
/* OUTINIT Bit Fields */
#define FTM_OUTINIT_CH0OI_MASK                   0x1u
#define FTM_OUTINIT_CH0OI_SHIFT                  0
#define FTM_OUTINIT_CH1OI_MASK                   0x2u
#define FTM_OUTINIT_CH1OI_SHIFT                  1
#define FTM_OUTINIT_CH2OI_MASK                   0x4u
#define FTM_OUTINIT_CH2OI_SHIFT                  2
#define FTM_OUTINIT_CH3OI_MASK                   0x8u
#define FTM_OUTINIT_CH3OI_SHIFT                  3
#define FTM_OUTINIT_CH4OI_MASK                   0x10u
#define FTM_OUTINIT_CH4OI_SHIFT                  4
#define FTM_OUTINIT_CH5OI_MASK                   0x20u
#define FTM_OUTINIT_CH5OI_SHIFT                  5
#define FTM_OUTINIT_CH6OI_MASK                   0x40u
#define FTM_OUTINIT_CH6OI_SHIFT                  6
#define FTM_OUTINIT_CH7OI_MASK                   0x80u
#define FTM_OUTINIT_CH7OI_SHIFT                  7
/* OUTMASK Bit Fields */
#define FTM_OUTMASK_CH0OM_MASK                   0x1u
#define FTM_OUTMASK_CH0OM_SHIFT                  0
#define FTM_OUTMASK_CH1OM_MASK                   0x2u
#define FTM_OUTMASK_CH1OM_SHIFT                  1
#define FTM_OUTMASK_CH2OM_MASK                   0x4u
#define FTM_OUTMASK_CH2OM_SHIFT                  2
#define FTM_OUTMASK_CH3OM_MASK                   0x8u
#define FTM_OUTMASK_CH3OM_SHIFT                  3
#define FTM_OUTMASK_CH4OM_MASK                   0x10u
#define FTM_OUTMASK_CH4OM_SHIFT                  4
#define FTM_OUTMASK_CH5OM_MASK                   0x20u
#define FTM_OUTMASK_CH5OM_SHIFT                  5
#define FTM_OUTMASK_CH6OM_MASK                   0x40u
#define FTM_OUTMASK_CH6OM_SHIFT                  6
#define FTM_OUTMASK_CH7OM_MASK                   0x80u
#define FTM_OUTMASK_CH7OM_SHIFT                  7
/* COMBINE Bit Fields */
#define FTM_COMBINE_COMBINE0_MASK                0x1u
#define FTM_COMBINE_COMBINE0_SHIFT               0
#define FTM_COMBINE_COMP0_MASK                   0x2u
#define FTM_COMBINE_COMP0_SHIFT                  1
#define FTM_COMBINE_DECAPEN0_MASK                0x4u
#define FTM_COMBINE_DECAPEN0_SHIFT               2
#define FTM_COMBINE_DECAP0_MASK                  0x8u
#define FTM_COMBINE_DECAP0_SHIFT                 3
#define FTM_COMBINE_DTEN0_MASK                   0x10u
#define FTM_COMBINE_DTEN0_SHIFT                  4
#define FTM_COMBINE_SYNCEN0_MASK                 0x20u
#define FTM_COMBINE_SYNCEN0_SHIFT                5
#define FTM_COMBINE_FAULTEN0_MASK                0x40u
#define FTM_COMBINE_FAULTEN0_SHIFT               6
#define FTM_COMBINE_COMBINE1_MASK                0x100u
#define FTM_COMBINE_COMBINE1_SHIFT               8
#define FTM_COMBINE_COMP1_MASK                   0x200u
#define FTM_COMBINE_COMP1_SHIFT                  9
#define FTM_COMBINE_DECAPEN1_MASK                0x400u
#define FTM_COMBINE_DECAPEN1_SHIFT               10
#define FTM_COMBINE_DECAP1_MASK                  0x800u
#define FTM_COMBINE_DECAP1_SHIFT                 11
#define FTM_COMBINE_DTEN1_MASK                   0x1000u
#define FTM_COMBINE_DTEN1_SHIFT                  12
#define FTM_COMBINE_SYNCEN1_MASK                 0x2000u
#define FTM_COMBINE_SYNCEN1_SHIFT                13
#define FTM_COMBINE_FAULTEN1_MASK                0x4000u
#define FTM_COMBINE_FAULTEN1_SHIFT               14
#define FTM_COMBINE_COMBINE2_MASK                0x10000u
#define FTM_COMBINE_COMBINE2_SHIFT               16
#define FTM_COMBINE_COMP2_MASK                   0x20000u
#define FTM_COMBINE_COMP2_SHIFT                  17
#define FTM_COMBINE_DECAPEN2_MASK                0x40000u
#define FTM_COMBINE_DECAPEN2_SHIFT               18
#define FTM_COMBINE_DECAP2_MASK                  0x80000u
#define FTM_COMBINE_DECAP2_SHIFT                 19
#define FTM_COMBINE_DTEN2_MASK                   0x100000u
#define FTM_COMBINE_DTEN2_SHIFT                  20
#define FTM_COMBINE_SYNCEN2_MASK                 0x200000u
#define FTM_COMBINE_SYNCEN2_SHIFT                21
#define FTM_COMBINE_FAULTEN2_MASK                0x400000u
#define FTM_COMBINE_FAULTEN2_SHIFT               22
#define FTM_COMBINE_COMBINE3_MASK                0x1000000u
#define FTM_COMBINE_COMBINE3_SHIFT               24
#define FTM_COMBINE_COMP3_MASK                   0x2000000u
#define FTM_COMBINE_COMP3_SHIFT                  25
#define FTM_COMBINE_DECAPEN3_MASK                0x4000000u
#define FTM_COMBINE_DECAPEN3_SHIFT               26
#define FTM_COMBINE_DECAP3_MASK                  0x8000000u
#define FTM_COMBINE_DECAP3_SHIFT                 27
#define FTM_COMBINE_DTEN3_MASK                   0x10000000u
#define FTM_COMBINE_DTEN3_SHIFT                  28
#define FTM_COMBINE_SYNCEN3_MASK                 0x20000000u
#define FTM_COMBINE_SYNCEN3_SHIFT                29
#define FTM_COMBINE_FAULTEN3_MASK                0x40000000u
#define FTM_COMBINE_FAULTEN3_SHIFT               30
/* DEADTIME Bit Fields */
#define FTM_DEADTIME_DTVAL_MASK                  0x3Fu
#define FTM_DEADTIME_DTVAL_SHIFT                 0
#define FTM_DEADTIME_DTVAL(x)                    (((uint32_t)(((uint32_t)(x))<<FTM_DEADTIME_DTVAL_SHIFT))&FTM_DEADTIME_DTVAL_MASK)
#define FTM_DEADTIME_DTPS_MASK                   0xC0u
#define FTM_DEADTIME_DTPS_SHIFT                  6
#define FTM_DEADTIME_DTPS(x)                     (((uint32_t)(((uint32_t)(x))<<FTM_DEADTIME_DTPS_SHIFT))&FTM_DEADTIME_DTPS_MASK)
/* EXTTRIG Bit Fields */
#define FTM_EXTTRIG_CH2TRIG_MASK                 0x1u
#define FTM_EXTTRIG_CH2TRIG_SHIFT                0
#define FTM_EXTTRIG_CH3TRIG_MASK                 0x2u
#define FTM_EXTTRIG_CH3TRIG_SHIFT                1
#define FTM_EXTTRIG_CH4TRIG_MASK                 0x4u
#define FTM_EXTTRIG_CH4TRIG_SHIFT                2
#define FTM_EXTTRIG_CH5TRIG_MASK                 0x8u
#define FTM_EXTTRIG_CH5TRIG_SHIFT                3
#define FTM_EXTTRIG_CH0TRIG_MASK                 0x10u
#define FTM_EXTTRIG_CH0TRIG_SHIFT                4
#define FTM_EXTTRIG_CH1TRIG_MASK                 0x20u
#define FTM_EXTTRIG_CH1TRIG_SHIFT                5
#define FTM_EXTTRIG_INITTRIGEN_MASK              0x40u
#define FTM_EXTTRIG_INITTRIGEN_SHIFT             6
#define FTM_EXTTRIG_TRIGF_MASK                   0x80u
#define FTM_EXTTRIG_TRIGF_SHIFT                  7
/* POL Bit Fields */
#define FTM_POL_POL0_MASK                        0x1u
#define FTM_POL_POL0_SHIFT                       0
#define FTM_POL_POL1_MASK                        0x2u
#define FTM_POL_POL1_SHIFT                       1
#define FTM_POL_POL2_MASK                        0x4u
#define FTM_POL_POL2_SHIFT                       2
#define FTM_POL_POL3_MASK                        0x8u
#define FTM_POL_POL3_SHIFT                       3
#define FTM_POL_POL4_MASK                        0x10u
#define FTM_POL_POL4_SHIFT                       4
#define FTM_POL_POL5_MASK                        0x20u
#define FTM_POL_POL5_SHIFT                       5
#define FTM_POL_POL6_MASK                        0x40u
#define FTM_POL_POL6_SHIFT                       6
#define FTM_POL_POL7_MASK                        0x80u
#define FTM_POL_POL7_SHIFT                       7
/* FMS Bit Fields */
#define FTM_FMS_FAULTF0_MASK                     0x1u
#define FTM_FMS_FAULTF0_SHIFT                    0
#define FTM_FMS_FAULTF1_MASK                     0x2u
#define FTM_FMS_FAULTF1_SHIFT                    1
#define FTM_FMS_FAULTF2_MASK                     0x4u
#define FTM_FMS_FAULTF2_SHIFT                    2
#define FTM_FMS_FAULTF3_MASK                     0x8u
#define FTM_FMS_FAULTF3_SHIFT                    3
#define FTM_FMS_FAULTIN_MASK                     0x20u
#define FTM_FMS_FAULTIN_SHIFT                    5
#define FTM_FMS_WPEN_MASK                        0x40u
#define FTM_FMS_WPEN_SHIFT                       6
#define FTM_FMS_FAULTF_MASK                      0x80u
#define FTM_FMS_FAULTF_SHIFT                     7
/* FILTER Bit Fields */
#define FTM_FILTER_CH0FVAL_MASK                  0xFu
#define FTM_FILTER_CH0FVAL_SHIFT                 0
#define FTM_FILTER_CH0FVAL(x)                    (((uint32_t)(((uint32_t)(x))<<FTM_FILTER_CH0FVAL_SHIFT))&FTM_FILTER_CH0FVAL_MASK)
#define FTM_FILTER_CH1FVAL_MASK                  0xF0u
#define FTM_FILTER_CH1FVAL_SHIFT                 4
#define FTM_FILTER_CH1FVAL(x)                    (((uint32_t)(((uint32_t)(x))<<FTM_FILTER_CH1FVAL_SHIFT))&FTM_FILTER_CH1FVAL_MASK)
#define FTM_FILTER_CH2FVAL_MASK                  0xF00u
#define FTM_FILTER_CH2FVAL_SHIFT                 8
#define FTM_FILTER_CH2FVAL(x)                    (((uint32_t)(((uint32_t)(x))<<FTM_FILTER_CH2FVAL_SHIFT))&FTM_FILTER_CH2FVAL_MASK)
#define FTM_FILTER_CH3FVAL_MASK                  0xF000u
#define FTM_FILTER_CH3FVAL_SHIFT                 12
#define FTM_FILTER_CH3FVAL(x)                    (((uint32_t)(((uint32_t)(x))<<FTM_FILTER_CH3FVAL_SHIFT))&FTM_FILTER_CH3FVAL_MASK)
/* FLTCTRL Bit Fields */
#define FTM_FLTCTRL_FAULT0EN_MASK                0x1u
#define FTM_FLTCTRL_FAULT0EN_SHIFT               0
#define FTM_FLTCTRL_FAULT1EN_MASK                0x2u
#define FTM_FLTCTRL_FAULT1EN_SHIFT               1
#define FTM_FLTCTRL_FAULT2EN_MASK                0x4u
#define FTM_FLTCTRL_FAULT2EN_SHIFT               2
#define FTM_FLTCTRL_FAULT3EN_MASK                0x8u
#define FTM_FLTCTRL_FAULT3EN_SHIFT               3
#define FTM_FLTCTRL_FFLTR0EN_MASK                0x10u
#define FTM_FLTCTRL_FFLTR0EN_SHIFT               4
#define FTM_FLTCTRL_FFLTR1EN_MASK                0x20u
#define FTM_FLTCTRL_FFLTR1EN_SHIFT               5
#define FTM_FLTCTRL_FFLTR2EN_MASK                0x40u
#define FTM_FLTCTRL_FFLTR2EN_SHIFT               6
#define FTM_FLTCTRL_FFLTR3EN_MASK                0x80u
#define FTM_FLTCTRL_FFLTR3EN_SHIFT               7
#define FTM_FLTCTRL_FFVAL_MASK                   0xF00u
#define FTM_FLTCTRL_FFVAL_SHIFT                  8
#define FTM_FLTCTRL_FFVAL(x)                     (((uint32_t)(((uint32_t)(x))<<FTM_FLTCTRL_FFVAL_SHIFT))&FTM_FLTCTRL_FFVAL_MASK)
/* CONF Bit Fields */
#define FTM_CONF_NUMTOF_MASK                     0x1Fu
#define FTM_CONF_NUMTOF_SHIFT                    0
#define FTM_CONF_NUMTOF(x)                       (((uint32_t)(((uint32_t)(x))<<FTM_CONF_NUMTOF_SHIFT))&FTM_CONF_NUMTOF_MASK)
#define FTM_CONF_BDMMODE_MASK                    0xC0u
#define FTM_CONF_BDMMODE_SHIFT                   6
#define FTM_CONF_BDMMODE(x)                      (((uint32_t)(((uint32_t)(x))<<FTM_CONF_BDMMODE_SHIFT))&FTM_CONF_BDMMODE_MASK)
#define FTM_CONF_GTBEEN_MASK                     0x200u
#define FTM_CONF_GTBEEN_SHIFT                    9
#define FTM_CONF_GTBEOUT_MASK                    0x400u
#define FTM_CONF_GTBEOUT_SHIFT                   10
/* FLTPOL Bit Fields */
#define FTM_FLTPOL_FLT0POL_MASK                  0x1u
#define FTM_FLTPOL_FLT0POL_SHIFT                 0
#define FTM_FLTPOL_FLT1POL_MASK                  0x2u
#define FTM_FLTPOL_FLT1POL_SHIFT                 1
#define FTM_FLTPOL_FLT2POL_MASK                  0x4u
#define FTM_FLTPOL_FLT2POL_SHIFT                 2
#define FTM_FLTPOL_FLT3POL_MASK                  0x8u
#define FTM_FLTPOL_FLT3POL_SHIFT                 3
/* SYNCONF Bit Fields */
#define FTM_SYNCONF_HWTRIGMODE_MASK              0x1u
#define FTM_SYNCONF_HWTRIGMODE_SHIFT             0
#define FTM_SYNCONF_CNTINC_MASK                  0x4u
#define FTM_SYNCONF_CNTINC_SHIFT                 2
#define FTM_SYNCONF_INVC_MASK                    0x10u
#define FTM_SYNCONF_INVC_SHIFT                   4
#define FTM_SYNCONF_SWOC_MASK                    0x20u
#define FTM_SYNCONF_SWOC_SHIFT                   5
#define FTM_SYNCONF_SYNCMODE_MASK                0x80u
#define FTM_SYNCONF_SYNCMODE_SHIFT               7
#define FTM_SYNCONF_SWRSTCNT_MASK                0x100u
#define FTM_SYNCONF_SWRSTCNT_SHIFT               8
#define FTM_SYNCONF_SWWRBUF_MASK                 0x200u
#define FTM_SYNCONF_SWWRBUF_SHIFT                9
#define FTM_SYNCONF_SWOM_MASK                    0x400u
#define FTM_SYNCONF_SWOM_SHIFT                   10
#define FTM_SYNCONF_SWINVC_MASK                  0x800u
#define FTM_SYNCONF_SWINVC_SHIFT                 11
#define FTM_SYNCONF_SWSOC_MASK                   0x1000u
#define FTM_SYNCONF_SWSOC_SHIFT                  12
#define FTM_SYNCONF_HWRSTCNT_MASK                0x10000u
#define FTM_SYNCONF_HWRSTCNT_SHIFT               16
#define FTM_SYNCONF_HWWRBUF_MASK                 0x20000u
#define FTM_SYNCONF_HWWRBUF_SHIFT                17
#define FTM_SYNCONF_HWOM_MASK                    0x40000u
#define FTM_SYNCONF_HWOM_SHIFT                   18
#define FTM_SYNCONF_HWINVC_MASK                  0x80000u
#define FTM_SYNCONF_HWINVC_SHIFT                 19
#define FTM_SYNCONF_HWSOC_MASK                   0x100000u
#define FTM_SYNCONF_HWSOC_SHIFT                  20
/* INVCTRL Bit Fields */
#define FTM_INVCTRL_INV0EN_MASK                  0x1u
#define FTM_INVCTRL_INV0EN_SHIFT                 0
#define FTM_INVCTRL_INV1EN_MASK                  0x2u
#define FTM_INVCTRL_INV1EN_SHIFT                 1
#define FTM_INVCTRL_INV2EN_MASK                  0x4u
#define FTM_INVCTRL_INV2EN_SHIFT                 2
#define FTM_INVCTRL_INV3EN_MASK                  0x8u
#define FTM_INVCTRL_INV3EN_SHIFT                 3
/* SWOCTRL Bit Fields */
#define FTM_SWOCTRL_CH0OC_MASK                   0x1u
#define FTM_SWOCTRL_CH0OC_SHIFT                  0
#define FTM_SWOCTRL_CH1OC_MASK                   0x2u
#define FTM_SWOCTRL_CH1OC_SHIFT                  1
#define FTM_SWOCTRL_CH2OC_MASK                   0x4u
#define FTM_SWOCTRL_CH2OC_SHIFT                  2
#define FTM_SWOCTRL_CH3OC_MASK                   0x8u
#define FTM_SWOCTRL_CH3OC_SHIFT                  3
#define FTM_SWOCTRL_CH4OC_MASK                   0x10u
#define FTM_SWOCTRL_CH4OC_SHIFT                  4
#define FTM_SWOCTRL_CH5OC_MASK                   0x20u
#define FTM_SWOCTRL_CH5OC_SHIFT                  5
#define FTM_SWOCTRL_CH6OC_MASK                   0x40u
#define FTM_SWOCTRL_CH6OC_SHIFT                  6
#define FTM_SWOCTRL_CH7OC_MASK                   0x80u
#define FTM_SWOCTRL_CH7OC_SHIFT                  7
#define FTM_SWOCTRL_CH0OCV_MASK                  0x100u
#define FTM_SWOCTRL_CH0OCV_SHIFT                 8
#define FTM_SWOCTRL_CH1OCV_MASK                  0x200u
#define FTM_SWOCTRL_CH1OCV_SHIFT                 9
#define FTM_SWOCTRL_CH2OCV_MASK                  0x400u
#define FTM_SWOCTRL_CH2OCV_SHIFT                 10
#define FTM_SWOCTRL_CH3OCV_MASK                  0x800u
#define FTM_SWOCTRL_CH3OCV_SHIFT                 11
#define FTM_SWOCTRL_CH4OCV_MASK                  0x1000u
#define FTM_SWOCTRL_CH4OCV_SHIFT                 12
#define FTM_SWOCTRL_CH5OCV_MASK                  0x2000u
#define FTM_SWOCTRL_CH5OCV_SHIFT                 13
#define FTM_SWOCTRL_CH6OCV_MASK                  0x4000u
#define FTM_SWOCTRL_CH6OCV_SHIFT                 14
#define FTM_SWOCTRL_CH7OCV_MASK                  0x8000u
#define FTM_SWOCTRL_CH7OCV_SHIFT                 15
/* PWMLOAD Bit Fields */
#define FTM_PWMLOAD_CH0SEL_MASK                  0x1u
#define FTM_PWMLOAD_CH0SEL_SHIFT                 0
#define FTM_PWMLOAD_CH1SEL_MASK                  0x2u
#define FTM_PWMLOAD_CH1SEL_SHIFT                 1
#define FTM_PWMLOAD_CH2SEL_MASK                  0x4u
#define FTM_PWMLOAD_CH2SEL_SHIFT                 2
#define FTM_PWMLOAD_CH3SEL_MASK                  0x8u
#define FTM_PWMLOAD_CH3SEL_SHIFT                 3
#define FTM_PWMLOAD_CH4SEL_MASK                  0x10u
#define FTM_PWMLOAD_CH4SEL_SHIFT                 4
#define FTM_PWMLOAD_CH5SEL_MASK                  0x20u
#define FTM_PWMLOAD_CH5SEL_SHIFT                 5
#define FTM_PWMLOAD_CH6SEL_MASK                  0x40u
#define FTM_PWMLOAD_CH6SEL_SHIFT                 6
#define FTM_PWMLOAD_CH7SEL_MASK                  0x80u
#define FTM_PWMLOAD_CH7SEL_SHIFT                 7
#define FTM_PWMLOAD_LDOK_MASK                    0x200u
#define FTM_PWMLOAD_LDOK_SHIFT                   9

/*!
 * @}
 */ /* end of group FTM_Register_Masks */


/* FTM - Peripheral instance base addresses */
/** Peripheral FTM0 base address */
#define FTM0_BASE                                (0x40038000u)
/** Peripheral FTM0 base pointer */
#define FTM0                                     ((FTM_Type *)FTM0_BASE)
/** Peripheral FTM1 base address */
#define FTM1_BASE                                (0x40039000u)
/** Peripheral FTM1 base pointer */
#define FTM1                                     ((FTM_Type *)FTM1_BASE)
/** Peripheral FTM2 base address */
#define FTM2_BASE                                (0x4003A000u)
/** Peripheral FTM2 base pointer */
#define FTM2                                     ((FTM_Type *)FTM2_BASE)
/** Array initializer of FTM peripheral base pointers */
#define FTM_BASES                                { FTM0, FTM1, FTM2 }

/*!
 * @}
 */ /* end of group FTM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- FTMRE Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTMRE_Peripheral_Access_Layer FTMRE Peripheral Access Layer
 * @{
 */

/** FTMRE - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[1];
  __IO uint8_t FCCOBIX;                            /**< Flash CCOB Index Register, offset: 0x1 */
  __I  uint8_t FSEC;                               /**< Flash Security Register, offset: 0x2 */
  __IO uint8_t FCLKDIV;                            /**< Flash Clock Divider Register, offset: 0x3 */
       uint8_t RESERVED_1[1];
  __IO uint8_t FSTAT;                              /**< Flash Status Register, offset: 0x5 */
       uint8_t RESERVED_2[1];
  __IO uint8_t FCNFG;                              /**< Flash Configuration Register, offset: 0x7 */
  __IO uint8_t FCCOBLO;                            /**< Flash Common Command Object Register: Low, offset: 0x8 */
  __IO uint8_t FCCOBHI;                            /**< Flash Common Command Object Register:High, offset: 0x9 */
       uint8_t RESERVED_3[1];
  __IO uint8_t FPROT;                              /**< Flash Protection Register, offset: 0xB */
       uint8_t RESERVED_4[3];
  __I  uint8_t FOPT;                               /**< Flash Option Register, offset: 0xF */
} FTMRE_Type;

/* ----------------------------------------------------------------------------
   -- FTMRE Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTMRE_Register_Masks FTMRE Register Masks
 * @{
 */

/* FCCOBIX Bit Fields */
#define FTMRE_FCCOBIX_CCOBIX_MASK                0x7u
#define FTMRE_FCCOBIX_CCOBIX_SHIFT               0
#define FTMRE_FCCOBIX_CCOBIX(x)                  (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBIX_CCOBIX_SHIFT))&FTMRE_FCCOBIX_CCOBIX_MASK)
/* FSEC Bit Fields */
#define FTMRE_FSEC_SEC_MASK                      0x3u
#define FTMRE_FSEC_SEC_SHIFT                     0
#define FTMRE_FSEC_SEC(x)                        (((uint8_t)(((uint8_t)(x))<<FTMRE_FSEC_SEC_SHIFT))&FTMRE_FSEC_SEC_MASK)
#define FTMRE_FSEC_KEYEN_MASK                    0xC0u
#define FTMRE_FSEC_KEYEN_SHIFT                   6
#define FTMRE_FSEC_KEYEN(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FSEC_KEYEN_SHIFT))&FTMRE_FSEC_KEYEN_MASK)
/* FCLKDIV Bit Fields */
#define FTMRE_FCLKDIV_FDIV_MASK                  0x3Fu
#define FTMRE_FCLKDIV_FDIV_SHIFT                 0
#define FTMRE_FCLKDIV_FDIV(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCLKDIV_FDIV_SHIFT))&FTMRE_FCLKDIV_FDIV_MASK)
#define FTMRE_FCLKDIV_FDIVLCK_MASK               0x40u
#define FTMRE_FCLKDIV_FDIVLCK_SHIFT              6
#define FTMRE_FCLKDIV_FDIVLD_MASK                0x80u
#define FTMRE_FCLKDIV_FDIVLD_SHIFT               7
/* FSTAT Bit Fields */
#define FTMRE_FSTAT_MGSTAT_MASK                  0x3u
#define FTMRE_FSTAT_MGSTAT_SHIFT                 0
#define FTMRE_FSTAT_MGSTAT(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FSTAT_MGSTAT_SHIFT))&FTMRE_FSTAT_MGSTAT_MASK)
#define FTMRE_FSTAT_MGBUSY_MASK                  0x8u
#define FTMRE_FSTAT_MGBUSY_SHIFT                 3
#define FTMRE_FSTAT_FPVIOL_MASK                  0x10u
#define FTMRE_FSTAT_FPVIOL_SHIFT                 4
#define FTMRE_FSTAT_ACCERR_MASK                  0x20u
#define FTMRE_FSTAT_ACCERR_SHIFT                 5
#define FTMRE_FSTAT_CCIF_MASK                    0x80u
#define FTMRE_FSTAT_CCIF_SHIFT                   7
/* FCNFG Bit Fields */
#define FTMRE_FCNFG_ERSAREQ_MASK                 0x20u
#define FTMRE_FCNFG_ERSAREQ_SHIFT                5
#define FTMRE_FCNFG_CCIE_MASK                    0x80u
#define FTMRE_FCNFG_CCIE_SHIFT                   7
/* FCCOBLO Bit Fields */
#define FTMRE_FCCOBLO_CCOB_MASK                  0xFFu
#define FTMRE_FCCOBLO_CCOB_SHIFT                 0
#define FTMRE_FCCOBLO_CCOB(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBLO_CCOB_SHIFT))&FTMRE_FCCOBLO_CCOB_MASK)
/* FCCOBHI Bit Fields */
#define FTMRE_FCCOBHI_CCOB_MASK                  0xFFu
#define FTMRE_FCCOBHI_CCOB_SHIFT                 0
#define FTMRE_FCCOBHI_CCOB(x)                    (((uint8_t)(((uint8_t)(x))<<FTMRE_FCCOBHI_CCOB_SHIFT))&FTMRE_FCCOBHI_CCOB_MASK)
/* FPROT Bit Fields */
#define FTMRE_FPROT_FPLS_MASK                    0x3u
#define FTMRE_FPROT_FPLS_SHIFT                   0
#define FTMRE_FPROT_FPLS(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FPROT_FPLS_SHIFT))&FTMRE_FPROT_FPLS_MASK)
#define FTMRE_FPROT_FPLDIS_MASK                  0x4u
#define FTMRE_FPROT_FPLDIS_SHIFT                 2
#define FTMRE_FPROT_FPHS_MASK                    0x18u
#define FTMRE_FPROT_FPHS_SHIFT                   3
#define FTMRE_FPROT_FPHS(x)                      (((uint8_t)(((uint8_t)(x))<<FTMRE_FPROT_FPHS_SHIFT))&FTMRE_FPROT_FPHS_MASK)
#define FTMRE_FPROT_FPHDIS_MASK                  0x20u
#define FTMRE_FPROT_FPHDIS_SHIFT                 5
#define FTMRE_FPROT_RNV6_MASK                    0x40u
#define FTMRE_FPROT_RNV6_SHIFT                   6
#define FTMRE_FPROT_FPOPEN_MASK                  0x80u
#define FTMRE_FPROT_FPOPEN_SHIFT                 7
/* FOPT Bit Fields */
#define FTMRE_FOPT_NV_MASK                       0xFFu
#define FTMRE_FOPT_NV_SHIFT                      0
#define FTMRE_FOPT_NV(x)                         (((uint8_t)(((uint8_t)(x))<<FTMRE_FOPT_NV_SHIFT))&FTMRE_FOPT_NV_MASK)

/*!
 * @}
 */ /* end of group FTMRE_Register_Masks */


/* FTMRE - Peripheral instance base addresses */
/** Peripheral FTMRE base address */
#define FTMRE_BASE                               (0x40020000u)
/** Peripheral FTMRE base pointer */
#define FTMRE                                    ((FTMRE_Type *)FTMRE_BASE)
/** Array initializer of FTMRE peripheral base pointers */
#define FTMRE_BASES                              { FTMRE }

/*!
 * @}
 */ /* end of group FTMRE_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- GPIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Peripheral_Access_Layer GPIO Peripheral Access Layer
 * @{
 */

/** GPIO - Register Layout Typedef */
typedef struct GPIO_Type{
  union{
  __IO uint32_t PDOR;                              /**< Port Data Output Register, offset: 0x0 */
  struct{
            uint32_t PDOR0 : 1;
            uint32_t PDOR1: 1;
            uint32_t PDOR2 : 1;
            uint32_t PDOR3 : 1;
            uint32_t PDOR4 : 1;
            uint32_t PDOR5 : 1;
            uint32_t PDOR6 : 1;
            uint32_t PDOR7 : 1;
            uint32_t PDOR8 : 1;
            uint32_t PDOR9 : 1;
            uint32_t PDOR10: 1;
            uint32_t PDOR11: 1;
            uint32_t PDOR12: 1;
            uint32_t PDOR13: 1;
            uint32_t PDOR14: 1;
            uint32_t PDOR15: 1;
            uint32_t PDOR16: 1;
            uint32_t PDOR17: 1;
            uint32_t PDOR18: 1;
            uint32_t PDOR19: 1;
            uint32_t PDOR20: 1;
            uint32_t PDOR21: 1;
            uint32_t PDOR22: 1;
            uint32_t PDOR23: 1;
            uint32_t PDOR24: 1;
            uint32_t PDOR25: 1;
            uint32_t PDOR26: 1;
            uint32_t PDOR27: 1;
            uint32_t PDOR28: 1;
            uint32_t PDOR29: 1;
            uint32_t PDOR30: 1;
            uint32_t PDOR31: 1;
        } PDORs;
      struct                                            // 用于直接赋值位的   ——by 蓝宙
        {
            uint8_t Byte0;
            uint8_t Byte1;
            uint8_t Byte2;
            uint8_t Byte3;
        } PDORByte;
     };
  __O  uint32_t PSOR;                              /**< Port Set Output Register, offset: 0x4 */
  __O  uint32_t PCOR;                              /**< Port Clear Output Register, offset: 0x8 */
  __O  uint32_t PTOR;                              /**< Port Toggle Output Register, offset: 0xC */
 union{
  __I  uint32_t PDIR;                              /**< Port Data Input Register, offset: 0x10 */
   struct                                            // 用于直接赋值位的   ——by 蓝宙
        {
            uint32_t PDIR0 : 1;
            uint32_t PDIR1: 1;
            uint32_t PDIR2 : 1;
            uint32_t PDIR3 : 1;
            uint32_t PDIR4 : 1;
            uint32_t PDIR5 : 1;
            uint32_t PDIR6 : 1;
            uint32_t PDIR7 : 1;
            uint32_t PDIR8 : 1;
            uint32_t PDIR9 : 1;
            uint32_t PDIR10: 1;
            uint32_t PDIR11: 1;
            uint32_t PDIR12: 1;
            uint32_t PDIR13: 1;
            uint32_t PDIR14: 1;
            uint32_t PDIR15: 1;
            uint32_t PDIR16: 1;
            uint32_t PDIR17: 1;
            uint32_t PDIR18: 1;
            uint32_t PDIR19: 1;
            uint32_t PDIR20: 1;
            uint32_t PDIR21: 1;
            uint32_t PDIR22: 1;
            uint32_t PDIR23: 1;
            uint32_t PDIR24: 1;
            uint32_t PDIR25: 1;
            uint32_t PDIR26: 1;
            uint32_t PDIR27: 1;
            uint32_t PDIR28: 1;
            uint32_t PDIR29: 1;
            uint32_t PDIR30: 1;
            uint32_t PDIR31: 1;
        } PDIRs;
        struct                                            // 用于直接赋值位的   ——by 蓝宙
        {
            uint8_t Byte0;
            uint8_t Byte1;
            uint8_t Byte2;
            uint8_t Byte3;
        } PDIRByte;
  };
   union{
  __IO uint32_t PDDR;                              /**< Port Data Direction Register, offset: 0x14 */
          struct                                            // 用于直接赋值位的   ——by 蓝宙
        {
            uint32_t DDR0 : 1;
            uint32_t DDR1: 1;
            uint32_t DDR2 : 1;
            uint32_t DDR3 : 1;
            uint32_t DDR4 : 1;
            uint32_t DDR5 : 1;
            uint32_t DDR6 : 1;
            uint32_t DDR7 : 1;
            uint32_t DDR8 : 1;
            uint32_t DDR9 : 1;
            uint32_t DDR10: 1;
            uint32_t DDR11: 1;
            uint32_t DDR12: 1;
            uint32_t DDR13: 1;
            uint32_t DDR14: 1;
            uint32_t DDR15: 1;
            uint32_t DDR16: 1;
            uint32_t DDR17: 1;
            uint32_t DDR18: 1;
            uint32_t DDR19: 1;
            uint32_t DDR20: 1;
            uint32_t DDR21: 1;
            uint32_t DDR22: 1;
            uint32_t DDR23: 1;
            uint32_t DDR24: 1;
            uint32_t DDR25: 1;
            uint32_t DDR26: 1;
            uint32_t DDR27: 1;
            uint32_t DDR28: 1;
            uint32_t DDR29: 1;
            uint32_t DDR30: 1;
            uint32_t DDR31: 1;
        } DDRs;
        struct                                            //
        {
            uint8_t Byte0;
            uint8_t Byte1;
            uint8_t Byte2;
            uint8_t Byte3;
        } DDRByte;
   };

  __IO uint32_t PIDR;                              /**< Port Input Disable Register, offset: 0x18 */

  }GPIO_Type;

/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */
   //用于初始化 
/* GPIO - Register accessors */
#define GPIO_PDOR_REG(base)                      ((base)->PDOR)
#define GPIO_PSOR_REG(base)                      ((base)->PSOR)
#define GPIO_PCOR_REG(base)                      ((base)->PCOR)
#define GPIO_PTOR_REG(base)                      ((base)->PTOR)
#define GPIO_PDIR_REG(base)                      ((base)->PDIR)
#define GPIO_PDDR_REG(base)                      ((base)->PDDR)
#define GPIO_PIDR_REG(base)                      ((base)->PIDR)
/*!
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/* PDOR Bit Fields */
#define GPIO_PDOR_PDO_MASK                       0xFFFFFFFFu
#define GPIO_PDOR_PDO_SHIFT                      0
#define GPIO_PDOR_PDO(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDOR_PDO_SHIFT))&GPIO_PDOR_PDO_MASK)
/* PSOR Bit Fields */
#define GPIO_PSOR_PTSO_MASK                      0xFFFFFFFFu
#define GPIO_PSOR_PTSO_SHIFT                     0
#define GPIO_PSOR_PTSO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PSOR_PTSO_SHIFT))&GPIO_PSOR_PTSO_MASK)
/* PCOR Bit Fields */
#define GPIO_PCOR_PTCO_MASK                      0xFFFFFFFFu
#define GPIO_PCOR_PTCO_SHIFT                     0
#define GPIO_PCOR_PTCO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PCOR_PTCO_SHIFT))&GPIO_PCOR_PTCO_MASK)
/* PTOR Bit Fields */
#define GPIO_PTOR_PTTO_MASK                      0xFFFFFFFFu
#define GPIO_PTOR_PTTO_SHIFT                     0
#define GPIO_PTOR_PTTO(x)                        (((uint32_t)(((uint32_t)(x))<<GPIO_PTOR_PTTO_SHIFT))&GPIO_PTOR_PTTO_MASK)
/* PDIR Bit Fields */
#define GPIO_PDIR_PDI_MASK                       0xFFFFFFFFu
#define GPIO_PDIR_PDI_SHIFT                      0
#define GPIO_PDIR_PDI(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDIR_PDI_SHIFT))&GPIO_PDIR_PDI_MASK)
/* PDDR Bit Fields */
#define GPIO_PDDR_PDD_MASK                       0xFFFFFFFFu
#define GPIO_PDDR_PDD_SHIFT                      0
#define GPIO_PDDR_PDD(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PDDR_PDD_SHIFT))&GPIO_PDDR_PDD_MASK)
/* PIDR Bit Fields */
#define GPIO_PIDR_PID_MASK                       0xFFFFFFFFu
#define GPIO_PIDR_PID_SHIFT                      0
#define GPIO_PIDR_PID(x)                         (((uint32_t)(((uint32_t)(x))<<GPIO_PIDR_PID_SHIFT))&GPIO_PIDR_PID_MASK)

/*!
 * @}
 */ /* end of group GPIO_Register_Masks */


/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIOA base address */
#define GPIOA_BASE                               (0x400FF000u)
/** Peripheral GPIOA base pointer */
#define GPIOA                                    ((GPIO_Type *)GPIOA_BASE)
/** Peripheral GPIOB base address */
#define GPIOB_BASE                               (0x400FF040u)
/** Peripheral GPIOB base pointer */
#define GPIOB                                    ((GPIO_Type *)GPIOB_BASE)
/** Peripheral GPIOC base address */
#define GPIOC_BASE                               (0x400FF080u)
/** Peripheral GPIOC base pointer */
#define GPIOC                                    ((GPIO_Type *)GPIOC_BASE)
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASES                               { GPIOA, GPIOB, GPIOC }

/*!
 * @}
 */ /* end of group GPIO_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- I2C Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Peripheral_Access_Layer I2C Peripheral Access Layer
 * @{
 */

/** I2C - Register Layout Typedef */
typedef struct {
  __IO uint8_t A1;                                 /**< I2C Address Register 1, offset: 0x0 */
  __IO uint8_t F;                                  /**< I2C Frequency Divider register, offset: 0x1 */
  __IO uint8_t C1;                                 /**< I2C Control Register 1, offset: 0x2 */
  __IO uint8_t S;                                  /**< I2C Status register, offset: 0x3 */
  __IO uint8_t D;                                  /**< I2C Data I/O register, offset: 0x4 */
  __IO uint8_t C2;                                 /**< I2C Control Register 2, offset: 0x5 */
  __IO uint8_t FLT;                                /**< I2C Programmable Input Glitch Filter register, offset: 0x6 */
  __IO uint8_t RA;                                 /**< I2C Range Address register, offset: 0x7 */
  __IO uint8_t SMB;                                /**< I2C SMBus Control and Status register, offset: 0x8 */
  __IO uint8_t A2;                                 /**< I2C Address Register 2, offset: 0x9 */
  __IO uint8_t SLTH;                               /**< I2C SCL Low Timeout Register High, offset: 0xA */
  __IO uint8_t SLTL;                               /**< I2C SCL Low Timeout Register Low, offset: 0xB */
} I2C_Type;

/* ----------------------------------------------------------------------------
   -- I2C Register Masks
   --------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Masks I2C Register Masks
 * @{
 */

/* A1 Bit Fields */
#define I2C_A1_AD_MASK                           0xFEu
#define I2C_A1_AD_SHIFT                          1
#define I2C_A1_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_A1_AD_SHIFT))&I2C_A1_AD_MASK)
/* F Bit Fields */
#define I2C_F_ICR_MASK                           0x3Fu
#define I2C_F_ICR_SHIFT                          0
#define I2C_F_ICR(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_F_ICR_SHIFT))&I2C_F_ICR_MASK)
#define I2C_F_MULT_MASK                          0xC0u
#define I2C_F_MULT_SHIFT                         6
#define I2C_F_MULT(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_F_MULT_SHIFT))&I2C_F_MULT_MASK)
/* C1 Bit Fields */
#define I2C_C1_WUEN_MASK                         0x2u
#define I2C_C1_WUEN_SHIFT                        1
#define I2C_C1_RSTA_MASK                         0x4u
#define I2C_C1_RSTA_SHIFT                        2
#define I2C_C1_TXAK_MASK                         0x8u
#define I2C_C1_TXAK_SHIFT                        3
#define I2C_C1_TX_MASK                           0x10u
#define I2C_C1_TX_SHIFT                          4
#define I2C_C1_MST_MASK                          0x20u
#define I2C_C1_MST_SHIFT                         5
#define I2C_C1_IICIE_MASK                        0x40u
#define I2C_C1_IICIE_SHIFT                       6
#define I2C_C1_IICEN_MASK                        0x80u
#define I2C_C1_IICEN_SHIFT                       7
/* S Bit Fields */
#define I2C_S_RXAK_MASK                          0x1u
#define I2C_S_RXAK_SHIFT                         0
#define I2C_S_IICIF_MASK                         0x2u
#define I2C_S_IICIF_SHIFT                        1
#define I2C_S_SRW_MASK                           0x4u
#define I2C_S_SRW_SHIFT                          2
#define I2C_S_RAM_MASK                           0x8u
#define I2C_S_RAM_SHIFT                          3
#define I2C_S_ARBL_MASK                          0x10u
#define I2C_S_ARBL_SHIFT                         4
#define I2C_S_BUSY_MASK                          0x20u
#define I2C_S_BUSY_SHIFT                         5
#define I2C_S_IAAS_MASK                          0x40u
#define I2C_S_IAAS_SHIFT                         6
#define I2C_S_TCF_MASK                           0x80u
#define I2C_S_TCF_SHIFT                          7
/* D Bit Fields */
#define I2C_D_DATA_MASK                          0xFFu
#define I2C_D_DATA_SHIFT                         0
#define I2C_D_DATA(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_D_DATA_SHIFT))&I2C_D_DATA_MASK)
/* C2 Bit Fields */
#define I2C_C2_AD_MASK                           0x7u
#define I2C_C2_AD_SHIFT                          0
#define I2C_C2_AD(x)                             (((uint8_t)(((uint8_t)(x))<<I2C_C2_AD_SHIFT))&I2C_C2_AD_MASK)
#define I2C_C2_RMEN_MASK                         0x8u
#define I2C_C2_RMEN_SHIFT                        3
#define I2C_C2_SBRC_MASK                         0x10u
#define I2C_C2_SBRC_SHIFT                        4
#define I2C_C2_ADEXT_MASK                        0x40u
#define I2C_C2_ADEXT_SHIFT                       6
#define I2C_C2_GCAEN_MASK                        0x80u
#define I2C_C2_GCAEN_SHIFT                       7
/* FLT Bit Fields */
#define I2C_FLT_FLT_MASK                         0xFu
#define I2C_FLT_FLT_SHIFT                        0
#define I2C_FLT_FLT(x)                           (((uint8_t)(((uint8_t)(x))<<I2C_FLT_FLT_SHIFT))&I2C_FLT_FLT_MASK)
#define I2C_FLT_STARTF_MASK                      0x10u
#define I2C_FLT_STARTF_SHIFT                     4
#define I2C_FLT_SSIE_MASK                        0x20u
#define I2C_FLT_SSIE_SHIFT                       5
#define I2C_FLT_STOPF_MASK                       0x40u
#define I2C_FLT_STOPF_SHIFT                      6
#define I2C_FLT_SHEN_MASK                        0x80u
#define I2C_FLT_SHEN_SHIFT                       7
/* RA Bit Fields */
#define I2C_RA_RAD_MASK                          0xFEu
#define I2C_RA_RAD_SHIFT                         1
#define I2C_RA_RAD(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_RA_RAD_SHIFT))&I2C_RA_RAD_MASK)
/* SMB Bit Fields */
#define I2C_SMB_SHTF2IE_MASK                     0x1u
#define I2C_SMB_SHTF2IE_SHIFT                    0
#define I2C_SMB_SHTF2_MASK                       0x2u
#define I2C_SMB_SHTF2_SHIFT                      1
#define I2C_SMB_SHTF1_MASK                       0x4u
#define I2C_SMB_SHTF1_SHIFT                      2
#define I2C_SMB_SLTF_MASK                        0x8u
#define I2C_SMB_SLTF_SHIFT                       3
#define I2C_SMB_TCKSEL_MASK                      0x10u
#define I2C_SMB_TCKSEL_SHIFT                     4
#define I2C_SMB_SIICAEN_MASK                     0x20u
#define I2C_SMB_SIICAEN_SHIFT                    5
#define I2C_SMB_ALERTEN_MASK                     0x40u
#define I2C_SMB_ALERTEN_SHIFT                    6
#define I2C_SMB_FACK_MASK                        0x80u
#define I2C_SMB_FACK_SHIFT                       7
/* A2 Bit Fields */
#define I2C_A2_SAD_MASK                          0xFEu
#define I2C_A2_SAD_SHIFT                         1
#define I2C_A2_SAD(x)                            (((uint8_t)(((uint8_t)(x))<<I2C_A2_SAD_SHIFT))&I2C_A2_SAD_MASK)
/* SLTH Bit Fields */
#define I2C_SLTH_SSLT_MASK                       0xFFu
#define I2C_SLTH_SSLT_SHIFT                      0
#define I2C_SLTH_SSLT(x)                         (((uint8_t)(((uint8_t)(x))<<I2C_SLTH_SSLT_SHIFT))&I2C_SLTH_SSLT_MASK)
/* SLTL Bit Fields */
#define I2C_SLTL_SSLT_MASK                       0xFFu
#define I2C_SLTL_SSLT_SHIFT                      0
#define I2C_SLTL_SSLT(x)                         (((uint8_t)(((uint8_t)(x))<<I2C_SLTL_SSLT_SHIFT))&I2C_SLTL_SSLT_MASK)

/*!
 * @}
 */ /* end of group I2C_Register_Masks */


/* I2C - Peripheral instance base addresses */
/** Peripheral I2C0 base address */
#define I2C0_BASE                                (0x40066000u)
/** Peripheral I2C0 base pointer */
#define I2C0                                     ((I2C_Type *)I2C0_BASE)
/** Peripheral I2C1 base address */
#define I2C1_BASE                                (0x40067000u)
/** Peripheral I2C1 base pointer */
#define I2C1                                     ((I2C_Type *)I2C1_BASE)
/** Array initializer of I2C peripheral base pointers */
#define I2C_BASES                                { I2C0, I2C1 }

/*!
 * @}
 */ /* end of group I2C_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- ICS Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ICS_Peripheral_Access_Layer ICS Peripheral Access Layer
 * @{
 */

/** ICS - Register Layout Typedef */
typedef struct {
  __IO uint8_t C1;                                 /**< ICS Control Register 1, offset: 0x0 */
  __IO uint8_t C2;                                 /**< ICS Control Register 2, offset: 0x1 */
  __IO uint8_t C3;                                 /**< ICS Control Register 3, offset: 0x2 */
  __IO uint8_t C4;                                 /**< ICS Control Register 4, offset: 0x3 */
  __IO  uint8_t S;                                  /**< ICS Status Register, offset: 0x4 */
} ICS_Type;

/* ----------------------------------------------------------------------------
   -- ICS Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ICS_Register_Masks ICS Register Masks
 * @{
 */

/* C1 Bit Fields */
#define ICS_C1_IREFSTEN_MASK                     0x1u
#define ICS_C1_IREFSTEN_SHIFT                    0
#define ICS_C1_IRCLKEN_MASK                      0x2u
#define ICS_C1_IRCLKEN_SHIFT                     1
#define ICS_C1_IREFS_MASK                        0x4u
#define ICS_C1_IREFS_SHIFT                       2
#define ICS_C1_RDIV_MASK                         0x38u
#define ICS_C1_RDIV_SHIFT                        3
#define ICS_C1_RDIV(x)                           (((uint8_t)(((uint8_t)(x))<<ICS_C1_RDIV_SHIFT))&ICS_C1_RDIV_MASK)
#define ICS_C1_CLKS_MASK                         0xC0u
#define ICS_C1_CLKS_SHIFT                        6
#define ICS_C1_CLKS(x)                           (((uint8_t)(((uint8_t)(x))<<ICS_C1_CLKS_SHIFT))&ICS_C1_CLKS_MASK)
/* C2 Bit Fields */
#define ICS_C2_LP_MASK                           0x10u
#define ICS_C2_LP_SHIFT                          4
#define ICS_C2_BDIV_MASK                         0xE0u
#define ICS_C2_BDIV_SHIFT                        5
#define ICS_C2_BDIV(x)                           (((uint8_t)(((uint8_t)(x))<<ICS_C2_BDIV_SHIFT))&ICS_C2_BDIV_MASK)
/* C3 Bit Fields */
#define ICS_C3_SCTRIM_MASK                       0xFFu
#define ICS_C3_SCTRIM_SHIFT                      0
#define ICS_C3_SCTRIM(x)                         (((uint8_t)(((uint8_t)(x))<<ICS_C3_SCTRIM_SHIFT))&ICS_C3_SCTRIM_MASK)
/* C4 Bit Fields */
#define ICS_C4_SCFTRIM_MASK                      0x1u
#define ICS_C4_SCFTRIM_SHIFT                     0
#define ICS_C4_CME_MASK                          0x20u
#define ICS_C4_CME_SHIFT                         5
#define ICS_C4_LOLIE_MASK                        0x80u
#define ICS_C4_LOLIE_SHIFT                       7
/* S Bit Fields */
#define ICS_S_CLKST_MASK                         0xCu
#define ICS_S_CLKST_SHIFT                        2
#define ICS_S_CLKST(x)                           (((uint8_t)(((uint8_t)(x))<<ICS_S_CLKST_SHIFT))&ICS_S_CLKST_MASK)
#define ICS_S_IREFST_MASK                        0x10u
#define ICS_S_IREFST_SHIFT                       4
#define ICS_S_LOCK_MASK                          0x40u
#define ICS_S_LOCK_SHIFT                         6
#define ICS_S_LOLS_MASK                          0x80u
#define ICS_S_LOLS_SHIFT                         7

/*!
 * @}
 */ /* end of group ICS_Register_Masks */


/* ICS - Peripheral instance base addresses */
/** Peripheral ICS base address */
#define ICS_BASE                                 (0x40064000u)
/** Peripheral ICS base pointer */
#define ICS                                      ((ICS_Type *)ICS_BASE)
/** Array initializer of ICS peripheral base pointers */
#define ICS_BASES                                { ICS }

/*!
 * @}
 */ /* end of group ICS_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- IRQ Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup IRQ_Peripheral_Access_Layer IRQ Peripheral Access Layer
 * @{
 */

/** IRQ - Register Layout Typedef */
typedef struct {
  __IO uint8_t SC;                                 /**< Interrupt Pin Request Status and Control Register, offset: 0x0 */
} IRQ_Type;

/* ----------------------------------------------------------------------------
   -- IRQ Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup IRQ_Register_Masks IRQ Register Masks
 * @{
 */

/* SC Bit Fields */
#define IRQ_SC_IRQMOD_MASK                       0x1u
#define IRQ_SC_IRQMOD_SHIFT                      0
#define IRQ_SC_IRQIE_MASK                        0x2u
#define IRQ_SC_IRQIE_SHIFT                       1
#define IRQ_SC_IRQACK_MASK                       0x4u
#define IRQ_SC_IRQACK_SHIFT                      2
#define IRQ_SC_IRQF_MASK                         0x8u
#define IRQ_SC_IRQF_SHIFT                        3
#define IRQ_SC_IRQPE_MASK                        0x10u
#define IRQ_SC_IRQPE_SHIFT                       4
#define IRQ_SC_IRQEDG_MASK                       0x20u
#define IRQ_SC_IRQEDG_SHIFT                      5
#define IRQ_SC_IRQPDD_MASK                       0x40u
#define IRQ_SC_IRQPDD_SHIFT                      6

/*!
 * @}
 */ /* end of group IRQ_Register_Masks */


/* IRQ - Peripheral instance base addresses */
/** Peripheral IRQ base address */
#define IRQ_BASE                                 (0x40031000u)
/** Peripheral IRQ base pointer */
#define IRQ                                      ((IRQ_Type *)IRQ_BASE)
/** Array initializer of IRQ peripheral base pointers */
#define IRQ_BASES                                { IRQ }

/*!
 * @}
 */ /* end of group IRQ_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- KBI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup KBI_Peripheral_Access_Layer KBI Peripheral Access Layer
 * @{
 */

/** KBI - Register Layout Typedef */
typedef struct {
  __IO uint32_t PE;                                /**< KBI Pin Enable Register, offset: 0x0 */
  __IO uint32_t ES;                                /**< KBI Edge Select Register, offset: 0x4 */
  __IO uint32_t SC;                                /**< KBI Status and Control Register, offset: 0x8 */
  __I  uint32_t SP;                                /**< KBI Source Pin Register, offset: 0xC */
} KBI_Type;

/* ----------------------------------------------------------------------------
   -- KBI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup KBI_Register_Masks KBI Register Masks
 * @{
 */

/* PE Bit Fields */
#define KBI_PE_KBIPE_MASK                        0xFFFFFFFFu
#define KBI_PE_KBIPE_SHIFT                       0
#define KBI_PE_KBIPE(x)                          (((uint32_t)(((uint32_t)(x))<<KBI_PE_KBIPE_SHIFT))&KBI_PE_KBIPE_MASK)
/* ES Bit Fields */
#define KBI_ES_KBEDG_MASK                        0xFFFFFFFFu
#define KBI_ES_KBEDG_SHIFT                       0
#define KBI_ES_KBEDG(x)                          (((uint32_t)(((uint32_t)(x))<<KBI_ES_KBEDG_SHIFT))&KBI_ES_KBEDG_MASK)
/* SC Bit Fields */
#define KBI_SC_KBMOD_MASK                        0x1u
#define KBI_SC_KBMOD_SHIFT                       0
#define KBI_SC_KBIE_MASK                         0x2u
#define KBI_SC_KBIE_SHIFT                        1
#define KBI_SC_KBACK_MASK                        0x4u
#define KBI_SC_KBACK_SHIFT                       2
#define KBI_SC_KBF_MASK                          0x8u
#define KBI_SC_KBF_SHIFT                         3
#define KBI_SC_KBSPEN_MASK                       0x10u
#define KBI_SC_KBSPEN_SHIFT                      4
#define KBI_SC_RSTKBSP_MASK                      0x20u
#define KBI_SC_RSTKBSP_SHIFT                     5
/* SP Bit Fields */
#define KBI_SP_SP_MASK                           0xFFFFFFFFu
#define KBI_SP_SP_SHIFT                          0
#define KBI_SP_SP(x)                             (((uint32_t)(((uint32_t)(x))<<KBI_SP_SP_SHIFT))&KBI_SP_SP_MASK)

/*!
 * @}
 */ /* end of group KBI_Register_Masks */


/* KBI - Peripheral instance base addresses */
/** Peripheral KBI0 base address */
#define KBI0_BASE                                (0x40079000u)
/** Peripheral KBI0 base pointer */
#define KBI0                                     ((KBI_Type *)KBI0_BASE)
/** Peripheral KBI1 base address */
#define KBI1_BASE                                (0x4007A000u)
/** Peripheral KBI1 base pointer */
#define KBI1                                     ((KBI_Type *)KBI1_BASE)
/** Array initializer of KBI peripheral base pointers */
#define KBI_BASES                                { KBI0, KBI1 }

/*!
 * @}
 */ /* end of group KBI_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MCM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Peripheral_Access_Layer MCM Peripheral Access Layer
 * @{
 */

/** MCM - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[8];
  __I  uint16_t PLASC;                             /**< Crossbar Switch (AXBS) Slave Configuration, offset: 0x8 */
  __I  uint16_t PLAMC;                             /**< Crossbar Switch (AXBS) Master Configuration, offset: 0xA */
  __IO uint32_t PLACR;                             /**< Platform Control Register, offset: 0xC */
} MCM_Type;

/* ----------------------------------------------------------------------------
   -- MCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Masks MCM Register Masks
 * @{
 */

/* PLASC Bit Fields */
#define MCM_PLASC_ASC_MASK                       0xFFu
#define MCM_PLASC_ASC_SHIFT                      0
#define MCM_PLASC_ASC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLASC_ASC_SHIFT))&MCM_PLASC_ASC_MASK)
/* PLAMC Bit Fields */
#define MCM_PLAMC_AMC_MASK                       0xFFu
#define MCM_PLAMC_AMC_SHIFT                      0
#define MCM_PLAMC_AMC(x)                         (((uint16_t)(((uint16_t)(x))<<MCM_PLAMC_AMC_SHIFT))&MCM_PLAMC_AMC_MASK)
/* PLACR Bit Fields */
#define MCM_PLACR_CFCC_MASK                      0x400u
#define MCM_PLACR_CFCC_SHIFT                     10
#define MCM_PLACR_DFCDA_MASK                     0x800u
#define MCM_PLACR_DFCDA_SHIFT                    11
#define MCM_PLACR_DFCIC_MASK                     0x1000u
#define MCM_PLACR_DFCIC_SHIFT                    12
#define MCM_PLACR_DFCC_MASK                      0x2000u
#define MCM_PLACR_DFCC_SHIFT                     13
#define MCM_PLACR_EFDS_MASK                      0x4000u
#define MCM_PLACR_EFDS_SHIFT                     14
#define MCM_PLACR_DFCS_MASK                      0x8000u
#define MCM_PLACR_DFCS_SHIFT                     15
#define MCM_PLACR_ESFC_MASK                      0x10000u
#define MCM_PLACR_ESFC_SHIFT                     16

/*!
 * @}
 */ /* end of group MCM_Register_Masks */


/* MCM - Peripheral instance base addresses */
/** Peripheral MCM base address */
#define MCM_BASE                                 (0xF0003000u)
/** Peripheral MCM base pointer */
#define MCM                                      ((MCM_Type *)MCM_BASE)
/** Array initializer of MCM peripheral base pointers */
#define MCM_BASES                                { MCM }

/*!
 * @}
 */ /* end of group MCM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MSCAN Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MSCAN_Peripheral_Access_Layer MSCAN Peripheral Access Layer
 * @{
 */

/** MSCAN - Register Layout Typedef */
typedef struct {
  __IO uint8_t CANCTL0;                            /**< MSCAN Control Register 0, offset: 0x0 */
  __IO uint8_t CANCTL1;                            /**< MSCAN Control Register 1, offset: 0x1 */
  __IO uint8_t CANBTR0;                            /**< MSCAN Bus Timing Register 0, offset: 0x2 */
  __IO uint8_t CANBTR1;                            /**< MSCAN Bus Timing Register 1, offset: 0x3 */
  __IO uint8_t CANRFLG;                            /**< MSCAN Receiver Flag Register, offset: 0x4 */
  __IO uint8_t CANRIER;                            /**< MSCAN Receiver Interrupt Enable Register, offset: 0x5 */
  __IO uint8_t CANTFLG;                            /**< MSCAN Transmitter Flag Register, offset: 0x6 */
  __IO uint8_t CANTIER;                            /**< MSCAN Transmitter Interrupt Enable Register, offset: 0x7 */
  __IO uint8_t CANTARQ;                            /**< MSCAN Transmitter Message Abort Request Register, offset: 0x8 */
  __I  uint8_t CANTAAK;                            /**< MSCAN Transmitter Message Abort Acknowledge Register, offset: 0x9 */
  __IO uint8_t CANTBSEL;                           /**< MSCAN Transmit Buffer Selection Register, offset: 0xA */
  __IO uint8_t CANIDAC;                            /**< MSCAN Identifier Acceptance Control Register, offset: 0xB */
       uint8_t RESERVED_0[1];
  __IO uint8_t CANMISC;                            /**< MSCAN Miscellaneous Register, offset: 0xD */
  __I  uint8_t CANRXERR;                           /**< MSCAN Receive Error Counter, offset: 0xE */
  __I  uint8_t CANTXERR;                           /**< MSCAN Transmit Error Counter, offset: 0xF */
  __IO uint8_t CANIDAR_BANK_1[4];                  /**< MSCAN Identifier Acceptance Register n of First Bank, array offset: 0x10, array step: 0x1 */
  __IO uint8_t CANIDMR_BANK_1[4];                  /**< MSCAN Identifier Mask Register n of First Bank, array offset: 0x14, array step: 0x1 */
  __IO uint8_t CANIDAR_BANK_2[4];                  /**< MSCAN Identifier Acceptance Register n of Second Bank, array offset: 0x18, array step: 0x1 */
  __IO uint8_t CANIDMR_BANK_2[4];                  /**< MSCAN Identifier Mask Register n of Second Bank, array offset: 0x1C, array step: 0x1 */
  union {                                          /* offset: 0x20 */
    __IO uint8_t REIDR0;                             /**< Receive Extended Identifier Register 0, offset: 0x20 */
    __IO uint8_t RSIDR0;                             /**< Receive Standard Identifier Register 0, offset: 0x20 */
  };
  union {                                          /* offset: 0x21 */
    __IO uint8_t REIDR1;                             /**< Receive Extended Identifier Register 1, offset: 0x21 */
    __IO uint8_t RSIDR1;                             /**< Standard Identifier Register 1, offset: 0x21 */
    __IO uint8_t TSIDR1;                             /**< Transmit Standard Identifier Register 1, offset: 0x21 */
  };
  __IO uint8_t REIDR2;                             /**< Receive Extended Identifier Register 2, offset: 0x22 */
  __IO uint8_t REIDR3;                             /**< Receive Extended Identifier Register 3, offset: 0x23 */
  __IO uint8_t REDSR[8];                           /**< Receive Extended Data Segment Register N, array offset: 0x24, array step: 0x1 */
  __IO uint8_t RDLR;                               /**< This register keeps the data length field of the CAN frame., offset: 0x2C */
       uint8_t RESERVED_1[1];
  __IO uint8_t RTSRH;                              /**< Receive Time Stamp Register High, offset: 0x2E */
  __IO uint8_t RTSRL;                              /**< Receive Time Stamp Register Low, offset: 0x2F */
  union {                                          /* offset: 0x30 */
    __IO uint8_t TEIDR0;                             /**< Transmit Extended Identifier Register 0, offset: 0x30 */
    __IO uint8_t TSIDR0;                             /**< Transmit Standard Identifier Register 0, offset: 0x30 */
  };
  __IO uint8_t TEIDR1;                             /**< Transmit Extended Identifier Register 1, offset: 0x31 */
  __IO uint8_t TEIDR2;                             /**< Transmit Extended Identifier Register 2, offset: 0x32 */
  __IO uint8_t TEIDR3;                             /**< Transmit Extended Identifier Register 3, offset: 0x33 */
  __IO uint8_t TEDSR[8];                           /**< Transmit Extended Data Segment Register N, array offset: 0x34, array step: 0x1 */
  __IO uint8_t TDLR;                               /**< This register keeps the data length field of the CAN frame., offset: 0x3C */
  __IO uint8_t TBPR;                               /**< Transmit Buffer Priority Register, offset: 0x3D */
  __IO uint8_t TTSRH;                              /**< Transmit Time Stamp Register High, offset: 0x3E */
  __IO uint8_t TTSRL;                              /**< Transmit Time Stamp Register Low, offset: 0x3F */
} MSCAN_Type;

/* ----------------------------------------------------------------------------
   -- MSCAN Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MSCAN_Register_Masks MSCAN Register Masks
 * @{
 */

/* CANCTL0 Bit Fields */
#define MSCAN_CANCTL0_INITRQ_MASK                0x1u
#define MSCAN_CANCTL0_INITRQ_SHIFT               0
#define MSCAN_CANCTL0_SLPRQ_MASK                 0x2u
#define MSCAN_CANCTL0_SLPRQ_SHIFT                1
#define MSCAN_CANCTL0_WUPE_MASK                  0x4u
#define MSCAN_CANCTL0_WUPE_SHIFT                 2
#define MSCAN_CANCTL0_TIME_MASK                  0x8u
#define MSCAN_CANCTL0_TIME_SHIFT                 3
#define MSCAN_CANCTL0_SYNCH_MASK                 0x10u
#define MSCAN_CANCTL0_SYNCH_SHIFT                4
#define MSCAN_CANCTL0_CSWAI_MASK                 0x20u
#define MSCAN_CANCTL0_CSWAI_SHIFT                5
#define MSCAN_CANCTL0_RXACT_MASK                 0x40u
#define MSCAN_CANCTL0_RXACT_SHIFT                6
#define MSCAN_CANCTL0_RXFRM_MASK                 0x80u
#define MSCAN_CANCTL0_RXFRM_SHIFT                7
/* CANCTL1 Bit Fields */
#define MSCAN_CANCTL1_INITAK_MASK                0x1u
#define MSCAN_CANCTL1_INITAK_SHIFT               0
#define MSCAN_CANCTL1_SLPAK_MASK                 0x2u
#define MSCAN_CANCTL1_SLPAK_SHIFT                1
#define MSCAN_CANCTL1_WUPM_MASK                  0x4u
#define MSCAN_CANCTL1_WUPM_SHIFT                 2
#define MSCAN_CANCTL1_BORM_MASK                  0x8u
#define MSCAN_CANCTL1_BORM_SHIFT                 3
#define MSCAN_CANCTL1_LISTEN_MASK                0x10u
#define MSCAN_CANCTL1_LISTEN_SHIFT               4
#define MSCAN_CANCTL1_LOOPB_MASK                 0x20u
#define MSCAN_CANCTL1_LOOPB_SHIFT                5
#define MSCAN_CANCTL1_CLKSRC_MASK                0x40u
#define MSCAN_CANCTL1_CLKSRC_SHIFT               6
#define MSCAN_CANCTL1_CANE_MASK                  0x80u
#define MSCAN_CANCTL1_CANE_SHIFT                 7
/* CANBTR0 Bit Fields */
#define MSCAN_CANBTR0_BRP_MASK                   0x3Fu
#define MSCAN_CANBTR0_BRP_SHIFT                  0
#define MSCAN_CANBTR0_BRP(x)                     (((uint8_t)(((uint8_t)(x))<<MSCAN_CANBTR0_BRP_SHIFT))&MSCAN_CANBTR0_BRP_MASK)
#define MSCAN_CANBTR0_SJW_MASK                   0xC0u
#define MSCAN_CANBTR0_SJW_SHIFT                  6
#define MSCAN_CANBTR0_SJW(x)                     (((uint8_t)(((uint8_t)(x))<<MSCAN_CANBTR0_SJW_SHIFT))&MSCAN_CANBTR0_SJW_MASK)
/* CANBTR1 Bit Fields */
#define MSCAN_CANBTR1_TSEG1_MASK                 0xFu
#define MSCAN_CANBTR1_TSEG1_SHIFT                0
#define MSCAN_CANBTR1_TSEG1(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANBTR1_TSEG1_SHIFT))&MSCAN_CANBTR1_TSEG1_MASK)
#define MSCAN_CANBTR1_TSEG2_MASK                 0x70u
#define MSCAN_CANBTR1_TSEG2_SHIFT                4
#define MSCAN_CANBTR1_TSEG2(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANBTR1_TSEG2_SHIFT))&MSCAN_CANBTR1_TSEG2_MASK)
#define MSCAN_CANBTR1_SAMP_MASK                  0x80u
#define MSCAN_CANBTR1_SAMP_SHIFT                 7
/* CANRFLG Bit Fields */
#define MSCAN_CANRFLG_RXF_MASK                   0x1u
#define MSCAN_CANRFLG_RXF_SHIFT                  0
#define MSCAN_CANRFLG_OVRIF_MASK                 0x2u
#define MSCAN_CANRFLG_OVRIF_SHIFT                1
#define MSCAN_CANRFLG_TSTAT_MASK                 0xCu
#define MSCAN_CANRFLG_TSTAT_SHIFT                2
#define MSCAN_CANRFLG_TSTAT(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANRFLG_TSTAT_SHIFT))&MSCAN_CANRFLG_TSTAT_MASK)
#define MSCAN_CANRFLG_RSTAT_MASK                 0x30u
#define MSCAN_CANRFLG_RSTAT_SHIFT                4
#define MSCAN_CANRFLG_RSTAT(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANRFLG_RSTAT_SHIFT))&MSCAN_CANRFLG_RSTAT_MASK)
#define MSCAN_CANRFLG_CSCIF_MASK                 0x40u
#define MSCAN_CANRFLG_CSCIF_SHIFT                6
#define MSCAN_CANRFLG_WUPIF_MASK                 0x80u
#define MSCAN_CANRFLG_WUPIF_SHIFT                7
/* CANRIER Bit Fields */
#define MSCAN_CANRIER_RXFIE_MASK                 0x1u
#define MSCAN_CANRIER_RXFIE_SHIFT                0
#define MSCAN_CANRIER_OVRIE_MASK                 0x2u
#define MSCAN_CANRIER_OVRIE_SHIFT                1
#define MSCAN_CANRIER_TSTATE_MASK                0xCu
#define MSCAN_CANRIER_TSTATE_SHIFT               2
#define MSCAN_CANRIER_TSTATE(x)                  (((uint8_t)(((uint8_t)(x))<<MSCAN_CANRIER_TSTATE_SHIFT))&MSCAN_CANRIER_TSTATE_MASK)
#define MSCAN_CANRIER_RSTATE_MASK                0x30u
#define MSCAN_CANRIER_RSTATE_SHIFT               4
#define MSCAN_CANRIER_RSTATE(x)                  (((uint8_t)(((uint8_t)(x))<<MSCAN_CANRIER_RSTATE_SHIFT))&MSCAN_CANRIER_RSTATE_MASK)
#define MSCAN_CANRIER_CSCIE_MASK                 0x40u
#define MSCAN_CANRIER_CSCIE_SHIFT                6
#define MSCAN_CANRIER_WUPIE_MASK                 0x80u
#define MSCAN_CANRIER_WUPIE_SHIFT                7
/* CANTFLG Bit Fields */
#define MSCAN_CANTFLG_TXE_MASK                   0x7u
#define MSCAN_CANTFLG_TXE_SHIFT                  0
#define MSCAN_CANTFLG_TXE(x)                     (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTFLG_TXE_SHIFT))&MSCAN_CANTFLG_TXE_MASK)
/* CANTIER Bit Fields */
#define MSCAN_CANTIER_TXEIE_MASK                 0x7u
#define MSCAN_CANTIER_TXEIE_SHIFT                0
#define MSCAN_CANTIER_TXEIE(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTIER_TXEIE_SHIFT))&MSCAN_CANTIER_TXEIE_MASK)
/* CANTARQ Bit Fields */
#define MSCAN_CANTARQ_ABTRQ_MASK                 0x7u
#define MSCAN_CANTARQ_ABTRQ_SHIFT                0
#define MSCAN_CANTARQ_ABTRQ(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTARQ_ABTRQ_SHIFT))&MSCAN_CANTARQ_ABTRQ_MASK)
/* CANTAAK Bit Fields */
#define MSCAN_CANTAAK_ABTAK_MASK                 0x7u
#define MSCAN_CANTAAK_ABTAK_SHIFT                0
#define MSCAN_CANTAAK_ABTAK(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTAAK_ABTAK_SHIFT))&MSCAN_CANTAAK_ABTAK_MASK)
/* CANTBSEL Bit Fields */
#define MSCAN_CANTBSEL_TX_MASK                   0x7u
#define MSCAN_CANTBSEL_TX_SHIFT                  0
#define MSCAN_CANTBSEL_TX(x)                     (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTBSEL_TX_SHIFT))&MSCAN_CANTBSEL_TX_MASK)
/* CANIDAC Bit Fields */
#define MSCAN_CANIDAC_IDHIT_MASK                 0x7u
#define MSCAN_CANIDAC_IDHIT_SHIFT                0
#define MSCAN_CANIDAC_IDHIT(x)                   (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDAC_IDHIT_SHIFT))&MSCAN_CANIDAC_IDHIT_MASK)
#define MSCAN_CANIDAC_IDAM_MASK                  0x30u
#define MSCAN_CANIDAC_IDAM_SHIFT                 4
#define MSCAN_CANIDAC_IDAM(x)                    (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDAC_IDAM_SHIFT))&MSCAN_CANIDAC_IDAM_MASK)
/* CANMISC Bit Fields */
#define MSCAN_CANMISC_BOHOLD_MASK                0x1u
#define MSCAN_CANMISC_BOHOLD_SHIFT               0
/* CANRXERR Bit Fields */
#define MSCAN_CANRXERR_RXERR_MASK                0xFFu
#define MSCAN_CANRXERR_RXERR_SHIFT               0
#define MSCAN_CANRXERR_RXERR(x)                  (((uint8_t)(((uint8_t)(x))<<MSCAN_CANRXERR_RXERR_SHIFT))&MSCAN_CANRXERR_RXERR_MASK)
/* CANTXERR Bit Fields */
#define MSCAN_CANTXERR_TXERR_MASK                0xFFu
#define MSCAN_CANTXERR_TXERR_SHIFT               0
#define MSCAN_CANTXERR_TXERR(x)                  (((uint8_t)(((uint8_t)(x))<<MSCAN_CANTXERR_TXERR_SHIFT))&MSCAN_CANTXERR_TXERR_MASK)
/* CANIDAR_BANK_1 Bit Fields */
#define MSCAN_CANIDAR_BANK_1_AC_MASK             0xFFu
#define MSCAN_CANIDAR_BANK_1_AC_SHIFT            0
#define MSCAN_CANIDAR_BANK_1_AC(x)               (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDAR_BANK_1_AC_SHIFT))&MSCAN_CANIDAR_BANK_1_AC_MASK)
/* CANIDMR_BANK_1 Bit Fields */
#define MSCAN_CANIDMR_BANK_1_AM_MASK             0xFFu
#define MSCAN_CANIDMR_BANK_1_AM_SHIFT            0
#define MSCAN_CANIDMR_BANK_1_AM(x)               (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDMR_BANK_1_AM_SHIFT))&MSCAN_CANIDMR_BANK_1_AM_MASK)
/* CANIDAR_BANK_2 Bit Fields */
#define MSCAN_CANIDAR_BANK_2_AC_MASK             0xFFu
#define MSCAN_CANIDAR_BANK_2_AC_SHIFT            0
#define MSCAN_CANIDAR_BANK_2_AC(x)               (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDAR_BANK_2_AC_SHIFT))&MSCAN_CANIDAR_BANK_2_AC_MASK)
/* CANIDMR_BANK_2 Bit Fields */
#define MSCAN_CANIDMR_BANK_2_AM_MASK             0xFFu
#define MSCAN_CANIDMR_BANK_2_AM_SHIFT            0
#define MSCAN_CANIDMR_BANK_2_AM(x)               (((uint8_t)(((uint8_t)(x))<<MSCAN_CANIDMR_BANK_2_AM_SHIFT))&MSCAN_CANIDMR_BANK_2_AM_MASK)
/* REIDR0 Bit Fields */
#define MSCAN_REIDR0_REID28_REID21_MASK          0xFFu
#define MSCAN_REIDR0_REID28_REID21_SHIFT         0
#define MSCAN_REIDR0_REID28_REID21(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_REIDR0_REID28_REID21_SHIFT))&MSCAN_REIDR0_REID28_REID21_MASK)
/* RSIDR0 Bit Fields */
#define MSCAN_RSIDR0_RSID10_RSID3_MASK           0xFFu
#define MSCAN_RSIDR0_RSID10_RSID3_SHIFT          0
#define MSCAN_RSIDR0_RSID10_RSID3(x)             (((uint8_t)(((uint8_t)(x))<<MSCAN_RSIDR0_RSID10_RSID3_SHIFT))&MSCAN_RSIDR0_RSID10_RSID3_MASK)
/* REIDR1 Bit Fields */
#define MSCAN_REIDR1_REID17_REID15_MASK          0x7u
#define MSCAN_REIDR1_REID17_REID15_SHIFT         0
#define MSCAN_REIDR1_REID17_REID15(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_REIDR1_REID17_REID15_SHIFT))&MSCAN_REIDR1_REID17_REID15_MASK)
#define MSCAN_REIDR1_REIDE_MASK                  0x8u
#define MSCAN_REIDR1_REIDE_SHIFT                 3
#define MSCAN_REIDR1_RSRR_MASK                   0x10u
#define MSCAN_REIDR1_RSRR_SHIFT                  4
#define MSCAN_REIDR1_REID20_REID18_MASK          0xE0u
#define MSCAN_REIDR1_REID20_REID18_SHIFT         5
#define MSCAN_REIDR1_REID20_REID18(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_REIDR1_REID20_REID18_SHIFT))&MSCAN_REIDR1_REID20_REID18_MASK)
/* RSIDR1 Bit Fields */
#define MSCAN_RSIDR1_RSIDE_MASK                  0x8u
#define MSCAN_RSIDR1_RSIDE_SHIFT                 3
#define MSCAN_RSIDR1_RSRTR_MASK                  0x10u
#define MSCAN_RSIDR1_RSRTR_SHIFT                 4
#define MSCAN_RSIDR1_RSID2_RSID0_MASK            0xE0u
#define MSCAN_RSIDR1_RSID2_RSID0_SHIFT           5
#define MSCAN_RSIDR1_RSID2_RSID0(x)              (((uint8_t)(((uint8_t)(x))<<MSCAN_RSIDR1_RSID2_RSID0_SHIFT))&MSCAN_RSIDR1_RSID2_RSID0_MASK)
/* TSIDR1 Bit Fields */
#define MSCAN_TSIDR1_TSIDE_MASK                  0x8u
#define MSCAN_TSIDR1_TSIDE_SHIFT                 3
#define MSCAN_TSIDR1_TSRTR_MASK                  0x10u
#define MSCAN_TSIDR1_TSRTR_SHIFT                 4
#define MSCAN_TSIDR1_TSID2_TSID0_MASK            0xE0u
#define MSCAN_TSIDR1_TSID2_TSID0_SHIFT           5
#define MSCAN_TSIDR1_TSID2_TSID0(x)              (((uint8_t)(((uint8_t)(x))<<MSCAN_TSIDR1_TSID2_TSID0_SHIFT))&MSCAN_TSIDR1_TSID2_TSID0_MASK)
/* REIDR2 Bit Fields */
#define MSCAN_REIDR2_REID14_REID7_MASK           0xFFu
#define MSCAN_REIDR2_REID14_REID7_SHIFT          0
#define MSCAN_REIDR2_REID14_REID7(x)             (((uint8_t)(((uint8_t)(x))<<MSCAN_REIDR2_REID14_REID7_SHIFT))&MSCAN_REIDR2_REID14_REID7_MASK)
/* REIDR3 Bit Fields */
#define MSCAN_REIDR3_RERTR_MASK                  0x1u
#define MSCAN_REIDR3_RERTR_SHIFT                 0
#define MSCAN_REIDR3_REID6_REID0_MASK            0xFEu
#define MSCAN_REIDR3_REID6_REID0_SHIFT           1
#define MSCAN_REIDR3_REID6_REID0(x)              (((uint8_t)(((uint8_t)(x))<<MSCAN_REIDR3_REID6_REID0_SHIFT))&MSCAN_REIDR3_REID6_REID0_MASK)
/* REDSR Bit Fields */
#define MSCAN_REDSR_RDB_MASK                     0xFFu
#define MSCAN_REDSR_RDB_SHIFT                    0
#define MSCAN_REDSR_RDB(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_REDSR_RDB_SHIFT))&MSCAN_REDSR_RDB_MASK)
/* RDLR Bit Fields */
#define MSCAN_RDLR_RDLC_MASK                     0xFu
#define MSCAN_RDLR_RDLC_SHIFT                    0
#define MSCAN_RDLR_RDLC(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_RDLR_RDLC_SHIFT))&MSCAN_RDLR_RDLC_MASK)
/* RTSRH Bit Fields */
#define MSCAN_RTSRH_RTS_MASK                     0xFFu
#define MSCAN_RTSRH_RTS_SHIFT                    0
#define MSCAN_RTSRH_RTS(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_RTSRH_RTS_SHIFT))&MSCAN_RTSRH_RTS_MASK)
/* RTSRL Bit Fields */
#define MSCAN_RTSRL_RTS_MASK                     0xFFu
#define MSCAN_RTSRL_RTS_SHIFT                    0
#define MSCAN_RTSRL_RTS(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_RTSRL_RTS_SHIFT))&MSCAN_RTSRL_RTS_MASK)
/* TEIDR0 Bit Fields */
#define MSCAN_TEIDR0_TEID28_TEID21_MASK          0xFFu
#define MSCAN_TEIDR0_TEID28_TEID21_SHIFT         0
#define MSCAN_TEIDR0_TEID28_TEID21(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_TEIDR0_TEID28_TEID21_SHIFT))&MSCAN_TEIDR0_TEID28_TEID21_MASK)
/* TSIDR0 Bit Fields */
#define MSCAN_TSIDR0_TSID10_TSID3_MASK           0xFFu
#define MSCAN_TSIDR0_TSID10_TSID3_SHIFT          0
#define MSCAN_TSIDR0_TSID10_TSID3(x)             (((uint8_t)(((uint8_t)(x))<<MSCAN_TSIDR0_TSID10_TSID3_SHIFT))&MSCAN_TSIDR0_TSID10_TSID3_MASK)
/* TEIDR1 Bit Fields */
#define MSCAN_TEIDR1_TEID17_TEID15_MASK          0x7u
#define MSCAN_TEIDR1_TEID17_TEID15_SHIFT         0
#define MSCAN_TEIDR1_TEID17_TEID15(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_TEIDR1_TEID17_TEID15_SHIFT))&MSCAN_TEIDR1_TEID17_TEID15_MASK)
#define MSCAN_TEIDR1_TEIDE_MASK                  0x8u
#define MSCAN_TEIDR1_TEIDE_SHIFT                 3
#define MSCAN_TEIDR1_TSRR_MASK                   0x10u
#define MSCAN_TEIDR1_TSRR_SHIFT                  4
#define MSCAN_TEIDR1_TEID20_TEID18_MASK          0xE0u
#define MSCAN_TEIDR1_TEID20_TEID18_SHIFT         5
#define MSCAN_TEIDR1_TEID20_TEID18(x)            (((uint8_t)(((uint8_t)(x))<<MSCAN_TEIDR1_TEID20_TEID18_SHIFT))&MSCAN_TEIDR1_TEID20_TEID18_MASK)
/* TEIDR2 Bit Fields */
#define MSCAN_TEIDR2_TEID14_TEID7_MASK           0xFFu
#define MSCAN_TEIDR2_TEID14_TEID7_SHIFT          0
#define MSCAN_TEIDR2_TEID14_TEID7(x)             (((uint8_t)(((uint8_t)(x))<<MSCAN_TEIDR2_TEID14_TEID7_SHIFT))&MSCAN_TEIDR2_TEID14_TEID7_MASK)
/* TEIDR3 Bit Fields */
#define MSCAN_TEIDR3_TERTR_MASK                  0x1u
#define MSCAN_TEIDR3_TERTR_SHIFT                 0
#define MSCAN_TEIDR3_TEID6_TEID0_MASK            0xFEu
#define MSCAN_TEIDR3_TEID6_TEID0_SHIFT           1
#define MSCAN_TEIDR3_TEID6_TEID0(x)              (((uint8_t)(((uint8_t)(x))<<MSCAN_TEIDR3_TEID6_TEID0_SHIFT))&MSCAN_TEIDR3_TEID6_TEID0_MASK)
/* TEDSR Bit Fields */
#define MSCAN_TEDSR_TDB_MASK                     0xFFu
#define MSCAN_TEDSR_TDB_SHIFT                    0
#define MSCAN_TEDSR_TDB(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_TEDSR_TDB_SHIFT))&MSCAN_TEDSR_TDB_MASK)
/* TDLR Bit Fields */
#define MSCAN_TDLR_DLC_MASK                      0xFu
#define MSCAN_TDLR_DLC_SHIFT                     0
#define MSCAN_TDLR_DLC(x)                        (((uint8_t)(((uint8_t)(x))<<MSCAN_TDLR_DLC_SHIFT))&MSCAN_TDLR_DLC_MASK)
/* TBPR Bit Fields */
#define MSCAN_TBPR_PRIO_MASK                     0xFFu
#define MSCAN_TBPR_PRIO_SHIFT                    0
#define MSCAN_TBPR_PRIO(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_TBPR_PRIO_SHIFT))&MSCAN_TBPR_PRIO_MASK)
/* TTSRH Bit Fields */
#define MSCAN_TTSRH_TTS_MASK                     0xFFu
#define MSCAN_TTSRH_TTS_SHIFT                    0
#define MSCAN_TTSRH_TTS(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_TTSRH_TTS_SHIFT))&MSCAN_TTSRH_TTS_MASK)
/* TTSRL Bit Fields */
#define MSCAN_TTSRL_TTS_MASK                     0xFFu
#define MSCAN_TTSRL_TTS_SHIFT                    0
#define MSCAN_TTSRL_TTS(x)                       (((uint8_t)(((uint8_t)(x))<<MSCAN_TTSRL_TTS_SHIFT))&MSCAN_TTSRL_TTS_MASK)

/*!
 * @}
 */ /* end of group MSCAN_Register_Masks */


/* MSCAN - Peripheral instance base addresses */
/** Peripheral MSCAN base address */
#define MSCAN_BASE                               (0x40024000u)
/** Peripheral MSCAN base pointer */
#define MSCAN                                    ((MSCAN_Type *)MSCAN_BASE)
/** Array initializer of MSCAN peripheral base pointers */
#define MSCAN_BASES                              { MSCAN }

/*!
 * @}
 */ /* end of group MSCAN_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- NV Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Peripheral_Access_Layer NV Peripheral Access Layer
 * @{
 */

/** NV - Register Layout Typedef */
typedef struct {
  __I  uint8_t BACKKEY0;                           /**< Backdoor Comparison Key 0, offset: 0x0 */
  __I  uint8_t BACKKEY1;                           /**< Backdoor Comparison Key 1, offset: 0x1 */
  __I  uint8_t BACKKEY2;                           /**< Backdoor Comparison Key 2, offset: 0x2 */
  __I  uint8_t BACKKEY3;                           /**< Backdoor Comparison Key 3, offset: 0x3 */
  __I  uint8_t BACKKEY4;                           /**< Backdoor Comparison Key 4, offset: 0x4 */
  __I  uint8_t BACKKEY5;                           /**< Backdoor Comparison Key 5, offset: 0x5 */
  __I  uint8_t BACKKEY6;                           /**< Backdoor Comparison Key 6, offset: 0x6 */
  __I  uint8_t BACKKEY7;                           /**< Backdoor Comparison Key 7, offset: 0x7 */
       uint8_t RESERVED_0[5];
  __I  uint8_t FPROT;                              /**< Non-volatile P-Flash Protection Register, offset: 0xD */
  __I  uint8_t FSEC;                               /**< Non-volatile Flash Security Register, offset: 0xE */
  __I  uint8_t FOPT;                               /**< Non-volatile Flash Option Register, offset: 0xF */
} NV_Type;

/* ----------------------------------------------------------------------------
   -- NV Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Masks NV Register Masks
 * @{
 */

/* BACKKEY0 Bit Fields */
#define NV_BACKKEY0_KEY_MASK                     0xFFu
#define NV_BACKKEY0_KEY_SHIFT                    0
#define NV_BACKKEY0_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY0_KEY_SHIFT))&NV_BACKKEY0_KEY_MASK)
/* BACKKEY1 Bit Fields */
#define NV_BACKKEY1_KEY_MASK                     0xFFu
#define NV_BACKKEY1_KEY_SHIFT                    0
#define NV_BACKKEY1_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY1_KEY_SHIFT))&NV_BACKKEY1_KEY_MASK)
/* BACKKEY2 Bit Fields */
#define NV_BACKKEY2_KEY_MASK                     0xFFu
#define NV_BACKKEY2_KEY_SHIFT                    0
#define NV_BACKKEY2_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY2_KEY_SHIFT))&NV_BACKKEY2_KEY_MASK)
/* BACKKEY3 Bit Fields */
#define NV_BACKKEY3_KEY_MASK                     0xFFu
#define NV_BACKKEY3_KEY_SHIFT                    0
#define NV_BACKKEY3_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY3_KEY_SHIFT))&NV_BACKKEY3_KEY_MASK)
/* BACKKEY4 Bit Fields */
#define NV_BACKKEY4_KEY_MASK                     0xFFu
#define NV_BACKKEY4_KEY_SHIFT                    0
#define NV_BACKKEY4_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY4_KEY_SHIFT))&NV_BACKKEY4_KEY_MASK)
/* BACKKEY5 Bit Fields */
#define NV_BACKKEY5_KEY_MASK                     0xFFu
#define NV_BACKKEY5_KEY_SHIFT                    0
#define NV_BACKKEY5_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY5_KEY_SHIFT))&NV_BACKKEY5_KEY_MASK)
/* BACKKEY6 Bit Fields */
#define NV_BACKKEY6_KEY_MASK                     0xFFu
#define NV_BACKKEY6_KEY_SHIFT                    0
#define NV_BACKKEY6_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY6_KEY_SHIFT))&NV_BACKKEY6_KEY_MASK)
/* BACKKEY7 Bit Fields */
#define NV_BACKKEY7_KEY_MASK                     0xFFu
#define NV_BACKKEY7_KEY_SHIFT                    0
#define NV_BACKKEY7_KEY(x)                       (((uint8_t)(((uint8_t)(x))<<NV_BACKKEY7_KEY_SHIFT))&NV_BACKKEY7_KEY_MASK)
/* FPROT Bit Fields */
#define NV_FPROT_FPLS_MASK                       0x3u
#define NV_FPROT_FPLS_SHIFT                      0
#define NV_FPROT_FPLS(x)                         (((uint8_t)(((uint8_t)(x))<<NV_FPROT_FPLS_SHIFT))&NV_FPROT_FPLS_MASK)
#define NV_FPROT_FPLDIS_MASK                     0x4u
#define NV_FPROT_FPLDIS_SHIFT                    2
#define NV_FPROT_FPHS_MASK                       0x18u
#define NV_FPROT_FPHS_SHIFT                      3
#define NV_FPROT_FPHS(x)                         (((uint8_t)(((uint8_t)(x))<<NV_FPROT_FPHS_SHIFT))&NV_FPROT_FPHS_MASK)
#define NV_FPROT_FPHDIS_MASK                     0x20u
#define NV_FPROT_FPHDIS_SHIFT                    5
#define NV_FPROT_FPOPEN_MASK                     0x80u
#define NV_FPROT_FPOPEN_SHIFT                    7
/* FSEC Bit Fields */
#define NV_FSEC_SEC_MASK                         0x3u
#define NV_FSEC_SEC_SHIFT                        0
#define NV_FSEC_SEC(x)                           (((uint8_t)(((uint8_t)(x))<<NV_FSEC_SEC_SHIFT))&NV_FSEC_SEC_MASK)
#define NV_FSEC_KEYEN_MASK                       0xC0u
#define NV_FSEC_KEYEN_SHIFT                      6
#define NV_FSEC_KEYEN(x)                         (((uint8_t)(((uint8_t)(x))<<NV_FSEC_KEYEN_SHIFT))&NV_FSEC_KEYEN_MASK)

/*!
 * @}
 */ /* end of group NV_Register_Masks */


/* NV - Peripheral instance base addresses */
/** Peripheral FTMRE_FlashConfig base address */
#define FTMRE_FlashConfig_BASE                   (0x400u)
/** Peripheral FTMRE_FlashConfig base pointer */
#define FTMRE_FlashConfig                        ((NV_Type *)FTMRE_FlashConfig_BASE)
/** Array initializer of NV peripheral base pointers */
#define NV_BASES                                 { FTMRE_FlashConfig }

/*!
 * @}
 */ /* end of group NV_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- OSC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Peripheral_Access_Layer OSC Peripheral Access Layer
 * @{
 */

/** OSC - Register Layout Typedef */
typedef struct {
  __IO uint8_t CR;                                 /**< OSC Control Register, offset: 0x0 */
} OSC_Type;

/* ----------------------------------------------------------------------------
   -- OSC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Masks OSC Register Masks
 * @{
 */

/* CR Bit Fields */
#define OSC_CR_OSCINIT_MASK                      0x1u
#define OSC_CR_OSCINIT_SHIFT                     0
#define OSC_CR_HGO_MASK                          0x2u
#define OSC_CR_HGO_SHIFT                         1
#define OSC_CR_RANGE_MASK                        0x4u
#define OSC_CR_RANGE_SHIFT                       2
#define OSC_CR_OSCOS_MASK                        0x10u
#define OSC_CR_OSCOS_SHIFT                       4
#define OSC_CR_OSCSTEN_MASK                      0x20u
#define OSC_CR_OSCSTEN_SHIFT                     5
#define OSC_CR_OSCEN_MASK                        0x80u
#define OSC_CR_OSCEN_SHIFT                       7

/*!
 * @}
 */ /* end of group OSC_Register_Masks */


/* OSC - Peripheral instance base addresses */
/** Peripheral OSC base address */
#define OSC_BASE                                 (0x40065000u)
/** Peripheral OSC base pointer */
#define OSC                                      ((OSC_Type *)OSC_BASE)
/** Array initializer of OSC peripheral base pointers */
#define OSC_BASES                                { OSC }

/*!
 * @}
 */ /* end of group OSC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PIT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Peripheral_Access_Layer PIT Peripheral Access Layer
 * @{
 */

/** PIT - Register Layout Typedef */
typedef struct {
  __IO uint32_t MCR;                               /**< PIT Module Control Register, offset: 0x0 */
       uint8_t RESERVED_0[252];
  struct {                                         /* offset: 0x100, array step: 0x10 */
    __IO uint32_t LDVAL;                             /**< Timer Load Value Register, array offset: 0x100, array step: 0x10 */
    __I  uint32_t CVAL;                              /**< Current Timer Value Register, array offset: 0x104, array step: 0x10 */
    __IO uint32_t TCTRL;                             /**< Timer Control Register, array offset: 0x108, array step: 0x10 */
    __IO uint32_t TFLG;                              /**< Timer Flag Register, array offset: 0x10C, array step: 0x10 */
  } CHANNEL[2];
} PIT_Type;

/* ----------------------------------------------------------------------------
   -- PIT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Register_Masks PIT Register Masks
 * @{
 */

/* MCR Bit Fields */
#define PIT_MCR_FRZ_MASK                         0x1u
#define PIT_MCR_FRZ_SHIFT                        0
#define PIT_MCR_MDIS_MASK                        0x2u
#define PIT_MCR_MDIS_SHIFT                       1
/* LDVAL Bit Fields */
#define PIT_LDVAL_TSV_MASK                       0xFFFFFFFFu
#define PIT_LDVAL_TSV_SHIFT                      0
#define PIT_LDVAL_TSV(x)                         (((uint32_t)(((uint32_t)(x))<<PIT_LDVAL_TSV_SHIFT))&PIT_LDVAL_TSV_MASK)
/* CVAL Bit Fields */
#define PIT_CVAL_TVL_MASK                        0xFFFFFFFFu
#define PIT_CVAL_TVL_SHIFT                       0
#define PIT_CVAL_TVL(x)                          (((uint32_t)(((uint32_t)(x))<<PIT_CVAL_TVL_SHIFT))&PIT_CVAL_TVL_MASK)
/* TCTRL Bit Fields */
#define PIT_TCTRL_TEN_MASK                       0x1u
#define PIT_TCTRL_TEN_SHIFT                      0
#define PIT_TCTRL_TIE_MASK                       0x2u
#define PIT_TCTRL_TIE_SHIFT                      1
#define PIT_TCTRL_CHN_MASK                       0x4u
#define PIT_TCTRL_CHN_SHIFT                      2
/* TFLG Bit Fields */
#define PIT_TFLG_TIF_MASK                        0x1u
#define PIT_TFLG_TIF_SHIFT                       0

/*!
 * @}
 */ /* end of group PIT_Register_Masks */


/* PIT - Peripheral instance base addresses */
/** Peripheral PIT base address */
#define PIT_BASE                                 (0x40037000u)
/** Peripheral PIT base pointer */
#define PIT                                      ((PIT_Type *)PIT_BASE)
/** Array initializer of PIT peripheral base pointers */
#define PIT_BASES                                { PIT }

/*!
 * @}
 */ /* end of group PIT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PMC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Peripheral_Access_Layer PMC Peripheral Access Layer
 * @{
 */

/** PMC - Register Layout Typedef */
typedef struct {
  __IO uint8_t SPMSC1;                             /**< System Power Management Status and Control 1 Register, offset: 0x0 */
  __IO uint8_t SPMSC2;                             /**< System Power Management Status and Control 2 Register, offset: 0x1 */
} PMC_Type;

/* ----------------------------------------------------------------------------
   -- PMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Masks PMC Register Masks
 * @{
 */

/* SPMSC1 Bit Fields */
#define PMC_SPMSC1_BGBE_MASK                     0x1u
#define PMC_SPMSC1_BGBE_SHIFT                    0
#define PMC_SPMSC1_LVDE_MASK                     0x4u
#define PMC_SPMSC1_LVDE_SHIFT                    2
#define PMC_SPMSC1_LVDSE_MASK                    0x8u
#define PMC_SPMSC1_LVDSE_SHIFT                   3
#define PMC_SPMSC1_LVDRE_MASK                    0x10u
#define PMC_SPMSC1_LVDRE_SHIFT                   4
#define PMC_SPMSC1_LVWIE_MASK                    0x20u
#define PMC_SPMSC1_LVWIE_SHIFT                   5
#define PMC_SPMSC1_LVWACK_MASK                   0x40u
#define PMC_SPMSC1_LVWACK_SHIFT                  6
#define PMC_SPMSC1_LVWF_MASK                     0x80u
#define PMC_SPMSC1_LVWF_SHIFT                    7
/* SPMSC2 Bit Fields */
#define PMC_SPMSC2_LVWV_MASK                     0x30u
#define PMC_SPMSC2_LVWV_SHIFT                    4
#define PMC_SPMSC2_LVWV(x)                       (((uint8_t)(((uint8_t)(x))<<PMC_SPMSC2_LVWV_SHIFT))&PMC_SPMSC2_LVWV_MASK)
#define PMC_SPMSC2_LVDV_MASK                     0x40u
#define PMC_SPMSC2_LVDV_SHIFT                    6

/*!
 * @}
 */ /* end of group PMC_Register_Masks */


/* PMC - Peripheral instance base addresses */
/** Peripheral PMC base address */
#define PMC_BASE                                 (0x4007D000u)
/** Peripheral PMC base pointer */
#define PMC                                      ((PMC_Type *)PMC_BASE)
/** Array initializer of PMC peripheral base pointers */
#define PMC_BASES                                { PMC }

/*!
 * @}
 */ /* end of group PMC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PORT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Peripheral_Access_Layer PORT Peripheral Access Layer
 * @{
 */

/** PORT - Register Layout Typedef */
typedef struct {
  __IO uint32_t IOFLT0;                            /**< Port Filter Register 0, offset: 0x0 */
  __IO uint32_t IOFLT1;                            /**< Port Filter Register 1, offset: 0x4 */
  __IO uint32_t PUE0;                              /**< Port Pullup Enable Register 0, offset: 0x8 */
  __IO uint32_t PUE1;                              /**< Port Pullup Enable Register 1, offset: 0xC */
  __IO uint32_t PUE2;                              /**< Port Pullup Enable Register 2, offset: 0x10 */
  __IO uint32_t HDRVE;                             /**< Port High Drive Enable Register, offset: 0x14 */
} PORT_Type;

/* ----------------------------------------------------------------------------
   -- PORT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Masks PORT Register Masks
 * @{
 */

/* IOFLT0 Bit Fields */
#define PORT_IOFLT0_FLTA_MASK                    0x3u
#define PORT_IOFLT0_FLTA_SHIFT                   0
#define PORT_IOFLT0_FLTA(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTA_SHIFT))&PORT_IOFLT0_FLTA_MASK)
#define PORT_IOFLT0_FLTB_MASK                    0xCu
#define PORT_IOFLT0_FLTB_SHIFT                   2
#define PORT_IOFLT0_FLTB(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTB_SHIFT))&PORT_IOFLT0_FLTB_MASK)
#define PORT_IOFLT0_FLTC_MASK                    0x30u
#define PORT_IOFLT0_FLTC_SHIFT                   4
#define PORT_IOFLT0_FLTC(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTC_SHIFT))&PORT_IOFLT0_FLTC_MASK)
#define PORT_IOFLT0_FLTD_MASK                    0xC0u
#define PORT_IOFLT0_FLTD_SHIFT                   6
#define PORT_IOFLT0_FLTD(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTD_SHIFT))&PORT_IOFLT0_FLTD_MASK)
#define PORT_IOFLT0_FLTE_MASK                    0x300u
#define PORT_IOFLT0_FLTE_SHIFT                   8
#define PORT_IOFLT0_FLTE(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTE_SHIFT))&PORT_IOFLT0_FLTE_MASK)
#define PORT_IOFLT0_FLTF_MASK                    0xC00u
#define PORT_IOFLT0_FLTF_SHIFT                   10
#define PORT_IOFLT0_FLTF(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTF_SHIFT))&PORT_IOFLT0_FLTF_MASK)
#define PORT_IOFLT0_FLTG_MASK                    0x3000u
#define PORT_IOFLT0_FLTG_SHIFT                   12
#define PORT_IOFLT0_FLTG(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTG_SHIFT))&PORT_IOFLT0_FLTG_MASK)
#define PORT_IOFLT0_FLTH_MASK                    0xC000u
#define PORT_IOFLT0_FLTH_SHIFT                   14
#define PORT_IOFLT0_FLTH(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTH_SHIFT))&PORT_IOFLT0_FLTH_MASK)
#define PORT_IOFLT0_FLTRST_MASK                  0x30000u
#define PORT_IOFLT0_FLTRST_SHIFT                 16
#define PORT_IOFLT0_FLTRST(x)                    (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTRST_SHIFT))&PORT_IOFLT0_FLTRST_MASK)
#define PORT_IOFLT0_FLTKBI0_MASK                 0xC0000u
#define PORT_IOFLT0_FLTKBI0_SHIFT                18
#define PORT_IOFLT0_FLTKBI0(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTKBI0_SHIFT))&PORT_IOFLT0_FLTKBI0_MASK)
#define PORT_IOFLT0_FLTKBI1_MASK                 0x300000u
#define PORT_IOFLT0_FLTKBI1_SHIFT                20
#define PORT_IOFLT0_FLTKBI1(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTKBI1_SHIFT))&PORT_IOFLT0_FLTKBI1_MASK)
#define PORT_IOFLT0_FLTNMI_MASK                  0xC00000u
#define PORT_IOFLT0_FLTNMI_SHIFT                 22
#define PORT_IOFLT0_FLTNMI(x)                    (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTNMI_SHIFT))&PORT_IOFLT0_FLTNMI_MASK)
#define PORT_IOFLT0_FLTDIV1_MASK                 0x3000000u
#define PORT_IOFLT0_FLTDIV1_SHIFT                24
#define PORT_IOFLT0_FLTDIV1(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTDIV1_SHIFT))&PORT_IOFLT0_FLTDIV1_MASK)
#define PORT_IOFLT0_FLTDIV2_MASK                 0x1C000000u
#define PORT_IOFLT0_FLTDIV2_SHIFT                26
#define PORT_IOFLT0_FLTDIV2(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTDIV2_SHIFT))&PORT_IOFLT0_FLTDIV2_MASK)
#define PORT_IOFLT0_FLTDIV3_MASK                 0xE0000000u
#define PORT_IOFLT0_FLTDIV3_SHIFT                29
#define PORT_IOFLT0_FLTDIV3(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT0_FLTDIV3_SHIFT))&PORT_IOFLT0_FLTDIV3_MASK)
/* IOFLT1 Bit Fields */
#define PORT_IOFLT1_FLTI_MASK                    0x3u
#define PORT_IOFLT1_FLTI_SHIFT                   0
#define PORT_IOFLT1_FLTI(x)                      (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTI_SHIFT))&PORT_IOFLT1_FLTI_MASK)
#define PORT_IOFLT1_FLTIRQ_MASK                  0x30u
#define PORT_IOFLT1_FLTIRQ_SHIFT                 4
#define PORT_IOFLT1_FLTIRQ(x)                    (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTIRQ_SHIFT))&PORT_IOFLT1_FLTIRQ_MASK)
#define PORT_IOFLT1_FLTFTM0_MASK                 0xC0u
#define PORT_IOFLT1_FLTFTM0_SHIFT                6
#define PORT_IOFLT1_FLTFTM0(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTFTM0_SHIFT))&PORT_IOFLT1_FLTFTM0_MASK)
#define PORT_IOFLT1_FLTFTM1_MASK                 0x300u
#define PORT_IOFLT1_FLTFTM1_SHIFT                8
#define PORT_IOFLT1_FLTFTM1(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTFTM1_SHIFT))&PORT_IOFLT1_FLTFTM1_MASK)
#define PORT_IOFLT1_FLTPWT_MASK                  0xC00u
#define PORT_IOFLT1_FLTPWT_SHIFT                 10
#define PORT_IOFLT1_FLTPWT(x)                    (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTPWT_SHIFT))&PORT_IOFLT1_FLTPWT_MASK)
#define PORT_IOFLT1_FLTI2C0_MASK                 0x3000u
#define PORT_IOFLT1_FLTI2C0_SHIFT                12
#define PORT_IOFLT1_FLTI2C0(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTI2C0_SHIFT))&PORT_IOFLT1_FLTI2C0_MASK)
#define PORT_IOFLT1_FLTI2C1_MASK                 0xC000u
#define PORT_IOFLT1_FLTI2C1_SHIFT                14
#define PORT_IOFLT1_FLTI2C1(x)                   (((uint32_t)(((uint32_t)(x))<<PORT_IOFLT1_FLTI2C1_SHIFT))&PORT_IOFLT1_FLTI2C1_MASK)
/* PUE0 Bit Fields */
#define PORT_PUE0_PTAPE0_MASK                    0x1u
#define PORT_PUE0_PTAPE0_SHIFT                   0
#define PORT_PUE0_PTAPE1_MASK                    0x2u
#define PORT_PUE0_PTAPE1_SHIFT                   1
#define PORT_PUE0_PTAPE2_MASK                    0x4u
#define PORT_PUE0_PTAPE2_SHIFT                   2
#define PORT_PUE0_PTAPE3_MASK                    0x8u
#define PORT_PUE0_PTAPE3_SHIFT                   3
#define PORT_PUE0_PTAPE4_MASK                    0x10u
#define PORT_PUE0_PTAPE4_SHIFT                   4
#define PORT_PUE0_PTAPE5_MASK                    0x20u
#define PORT_PUE0_PTAPE5_SHIFT                   5
#define PORT_PUE0_PTAPE6_MASK                    0x40u
#define PORT_PUE0_PTAPE6_SHIFT                   6
#define PORT_PUE0_PTAPE7_MASK                    0x80u
#define PORT_PUE0_PTAPE7_SHIFT                   7
#define PORT_PUE0_PTBPE0_MASK                    0x100u
#define PORT_PUE0_PTBPE0_SHIFT                   8
#define PORT_PUE0_PTBPE1_MASK                    0x200u
#define PORT_PUE0_PTBPE1_SHIFT                   9
#define PORT_PUE0_PTBPE2_MASK                    0x400u
#define PORT_PUE0_PTBPE2_SHIFT                   10
#define PORT_PUE0_PTBPE3_MASK                    0x800u
#define PORT_PUE0_PTBPE3_SHIFT                   11
#define PORT_PUE0_PTBPE4_MASK                    0x1000u
#define PORT_PUE0_PTBPE4_SHIFT                   12
#define PORT_PUE0_PTBPE5_MASK                    0x2000u
#define PORT_PUE0_PTBPE5_SHIFT                   13
#define PORT_PUE0_PTBPE6_MASK                    0x4000u
#define PORT_PUE0_PTBPE6_SHIFT                   14
#define PORT_PUE0_PTBPE7_MASK                    0x8000u
#define PORT_PUE0_PTBPE7_SHIFT                   15
#define PORT_PUE0_PTCPE0_MASK                    0x10000u
#define PORT_PUE0_PTCPE0_SHIFT                   16
#define PORT_PUE0_PTCPE1_MASK                    0x20000u
#define PORT_PUE0_PTCPE1_SHIFT                   17
#define PORT_PUE0_PTCPE2_MASK                    0x40000u
#define PORT_PUE0_PTCPE2_SHIFT                   18
#define PORT_PUE0_PTCPE3_MASK                    0x80000u
#define PORT_PUE0_PTCPE3_SHIFT                   19
#define PORT_PUE0_PTCPE4_MASK                    0x100000u
#define PORT_PUE0_PTCPE4_SHIFT                   20
#define PORT_PUE0_PTCPE5_MASK                    0x200000u
#define PORT_PUE0_PTCPE5_SHIFT                   21
#define PORT_PUE0_PTCPE6_MASK                    0x400000u
#define PORT_PUE0_PTCPE6_SHIFT                   22
#define PORT_PUE0_PTCPE7_MASK                    0x800000u
#define PORT_PUE0_PTCPE7_SHIFT                   23
#define PORT_PUE0_PTDPE0_MASK                    0x1000000u
#define PORT_PUE0_PTDPE0_SHIFT                   24
#define PORT_PUE0_PTDPE1_MASK                    0x2000000u
#define PORT_PUE0_PTDPE1_SHIFT                   25
#define PORT_PUE0_PTDPE2_MASK                    0x4000000u
#define PORT_PUE0_PTDPE2_SHIFT                   26
#define PORT_PUE0_PTDPE3_MASK                    0x8000000u
#define PORT_PUE0_PTDPE3_SHIFT                   27
#define PORT_PUE0_PTDPE4_MASK                    0x10000000u
#define PORT_PUE0_PTDPE4_SHIFT                   28
#define PORT_PUE0_PTDPE5_MASK                    0x20000000u
#define PORT_PUE0_PTDPE5_SHIFT                   29
#define PORT_PUE0_PTDPE6_MASK                    0x40000000u
#define PORT_PUE0_PTDPE6_SHIFT                   30
#define PORT_PUE0_PTDPE7_MASK                    0x80000000u
#define PORT_PUE0_PTDPE7_SHIFT                   31
/* PUE1 Bit Fields */
#define PORT_PUE1_PTEPE0_MASK                    0x1u
#define PORT_PUE1_PTEPE0_SHIFT                   0
#define PORT_PUE1_PTEPE1_MASK                    0x2u
#define PORT_PUE1_PTEPE1_SHIFT                   1
#define PORT_PUE1_PTEPE2_MASK                    0x4u
#define PORT_PUE1_PTEPE2_SHIFT                   2
#define PORT_PUE1_PTEPE3_MASK                    0x8u
#define PORT_PUE1_PTEPE3_SHIFT                   3
#define PORT_PUE1_PTEPE4_MASK                    0x10u
#define PORT_PUE1_PTEPE4_SHIFT                   4
#define PORT_PUE1_PTEPE5_MASK                    0x20u
#define PORT_PUE1_PTEPE5_SHIFT                   5
#define PORT_PUE1_PTEPE6_MASK                    0x40u
#define PORT_PUE1_PTEPE6_SHIFT                   6
#define PORT_PUE1_PTEPE7_MASK                    0x80u
#define PORT_PUE1_PTEPE7_SHIFT                   7
#define PORT_PUE1_PTFPE0_MASK                    0x100u
#define PORT_PUE1_PTFPE0_SHIFT                   8
#define PORT_PUE1_PTFPE1_MASK                    0x200u
#define PORT_PUE1_PTFPE1_SHIFT                   9
#define PORT_PUE1_PTFPE2_MASK                    0x400u
#define PORT_PUE1_PTFPE2_SHIFT                   10
#define PORT_PUE1_PTFPE3_MASK                    0x800u
#define PORT_PUE1_PTFPE3_SHIFT                   11
#define PORT_PUE1_PTFPE4_MASK                    0x1000u
#define PORT_PUE1_PTFPE4_SHIFT                   12
#define PORT_PUE1_PTFPE5_MASK                    0x2000u
#define PORT_PUE1_PTFPE5_SHIFT                   13
#define PORT_PUE1_PTFPE6_MASK                    0x4000u
#define PORT_PUE1_PTFPE6_SHIFT                   14
#define PORT_PUE1_PTFPE7_MASK                    0x8000u
#define PORT_PUE1_PTFPE7_SHIFT                   15
#define PORT_PUE1_PTGPE0_MASK                    0x10000u
#define PORT_PUE1_PTGPE0_SHIFT                   16
#define PORT_PUE1_PTGPE1_MASK                    0x20000u
#define PORT_PUE1_PTGPE1_SHIFT                   17
#define PORT_PUE1_PTGPE2_MASK                    0x40000u
#define PORT_PUE1_PTGPE2_SHIFT                   18
#define PORT_PUE1_PTGPE3_MASK                    0x80000u
#define PORT_PUE1_PTGPE3_SHIFT                   19
#define PORT_PUE1_PTGPE4_MASK                    0x100000u
#define PORT_PUE1_PTGPE4_SHIFT                   20
#define PORT_PUE1_PTGPE5_MASK                    0x200000u
#define PORT_PUE1_PTGPE5_SHIFT                   21
#define PORT_PUE1_PTGPE6_MASK                    0x400000u
#define PORT_PUE1_PTGPE6_SHIFT                   22
#define PORT_PUE1_PTGPE7_MASK                    0x800000u
#define PORT_PUE1_PTGPE7_SHIFT                   23
#define PORT_PUE1_PTHPE0_MASK                    0x1000000u
#define PORT_PUE1_PTHPE0_SHIFT                   24
#define PORT_PUE1_PTHPE1_MASK                    0x2000000u
#define PORT_PUE1_PTHPE1_SHIFT                   25
#define PORT_PUE1_PTHPE2_MASK                    0x4000000u
#define PORT_PUE1_PTHPE2_SHIFT                   26
#define PORT_PUE1_PTHPE3_MASK                    0x8000000u
#define PORT_PUE1_PTHPE3_SHIFT                   27
#define PORT_PUE1_PTHPE4_MASK                    0x10000000u
#define PORT_PUE1_PTHPE4_SHIFT                   28
#define PORT_PUE1_PTHPE5_MASK                    0x20000000u
#define PORT_PUE1_PTHPE5_SHIFT                   29
#define PORT_PUE1_PTHPE6_MASK                    0x40000000u
#define PORT_PUE1_PTHPE6_SHIFT                   30
#define PORT_PUE1_PTHPE7_MASK                    0x80000000u
#define PORT_PUE1_PTHPE7_SHIFT                   31
/* PUE2 Bit Fields */
#define PORT_PUE2_PTIPE0_MASK                    0x1u
#define PORT_PUE2_PTIPE0_SHIFT                   0
#define PORT_PUE2_PTIPE1_MASK                    0x2u
#define PORT_PUE2_PTIPE1_SHIFT                   1
#define PORT_PUE2_PTIPE2_MASK                    0x4u
#define PORT_PUE2_PTIPE2_SHIFT                   2
#define PORT_PUE2_PTIPE3_MASK                    0x8u
#define PORT_PUE2_PTIPE3_SHIFT                   3
#define PORT_PUE2_PTIPE4_MASK                    0x10u
#define PORT_PUE2_PTIPE4_SHIFT                   4
#define PORT_PUE2_PTIPE5_MASK                    0x20u
#define PORT_PUE2_PTIPE5_SHIFT                   5
#define PORT_PUE2_PTIPE6_MASK                    0x40u
#define PORT_PUE2_PTIPE6_SHIFT                   6
/* HDRVE Bit Fields */
#define PORT_HDRVE_PTB4_MASK                     0x1u
#define PORT_HDRVE_PTB4_SHIFT                    0
#define PORT_HDRVE_PTB5_MASK                     0x2u
#define PORT_HDRVE_PTB5_SHIFT                    1
#define PORT_HDRVE_PTD0_MASK                     0x4u
#define PORT_HDRVE_PTD0_SHIFT                    2
#define PORT_HDRVE_PTD1_MASK                     0x8u
#define PORT_HDRVE_PTD1_SHIFT                    3
#define PORT_HDRVE_PTE0_MASK                     0x10u
#define PORT_HDRVE_PTE0_SHIFT                    4
#define PORT_HDRVE_PTE1_MASK                     0x20u
#define PORT_HDRVE_PTE1_SHIFT                    5
#define PORT_HDRVE_PTH0_MASK                     0x40u
#define PORT_HDRVE_PTH0_SHIFT                    6
#define PORT_HDRVE_PTH1_MASK                     0x80u
#define PORT_HDRVE_PTH1_SHIFT                    7

/*!
 * @}
 */ /* end of group PORT_Register_Masks */


/* PORT - Peripheral instance base addresses */
/** Peripheral PORT base address */
#define PORT_BASE                                (0x40049000u)
/** Peripheral PORT base pointer */
#define PORT                                     ((PORT_Type *)PORT_BASE)
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASES                               { PORT }

/*!
 * @}
 */ /* end of group PORT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PWT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PWT_Peripheral_Access_Layer PWT Peripheral Access Layer
 * @{
 */

/** PWT - Register Layout Typedef */
typedef struct {
  __IO uint32_t R1;                                /**< Pulse Width Timer Register 1, offset: 0x0 */
  __I  uint32_t R2;                                /**< Pulse Width Timer Register 2, offset: 0x4 */
} PWT_Type;

/* ----------------------------------------------------------------------------
   -- PWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PWT_Register_Masks PWT Register Masks
 * @{
 */

/* R1 Bit Fields */
#define PWT_R1_PWTOV_MASK                        0x1u
#define PWT_R1_PWTOV_SHIFT                       0
#define PWT_R1_PWTRDY_MASK                       0x2u
#define PWT_R1_PWTRDY_SHIFT                      1
#define PWT_R1_PWTSR_MASK                        0x8u
#define PWT_R1_PWTSR_SHIFT                       3
#define PWT_R1_POVIE_MASK                        0x10u
#define PWT_R1_POVIE_SHIFT                       4
#define PWT_R1_PRDYIE_MASK                       0x20u
#define PWT_R1_PRDYIE_SHIFT                      5
#define PWT_R1_PWTIE_MASK                        0x40u
#define PWT_R1_PWTIE_SHIFT                       6
#define PWT_R1_PWTEN_MASK                        0x80u
#define PWT_R1_PWTEN_SHIFT                       7
#define PWT_R1_PRE_MASK                          0x700u
#define PWT_R1_PRE_SHIFT                         8
#define PWT_R1_PRE(x)                            (((uint32_t)(((uint32_t)(x))<<PWT_R1_PRE_SHIFT))&PWT_R1_PRE_MASK)
#define PWT_R1_EDGE_MASK                         0x1800u
#define PWT_R1_EDGE_SHIFT                        11
#define PWT_R1_EDGE(x)                           (((uint32_t)(((uint32_t)(x))<<PWT_R1_EDGE_SHIFT))&PWT_R1_EDGE_MASK)
#define PWT_R1_PINSEL_MASK                       0x6000u
#define PWT_R1_PINSEL_SHIFT                      13
#define PWT_R1_PINSEL(x)                         (((uint32_t)(((uint32_t)(x))<<PWT_R1_PINSEL_SHIFT))&PWT_R1_PINSEL_MASK)
#define PWT_R1_PCLKS_MASK                        0x8000u
#define PWT_R1_PCLKS_SHIFT                       15
#define PWT_R1_PPW_MASK                          0xFFFF0000u
#define PWT_R1_PPW_SHIFT                         16
#define PWT_R1_PPW(x)                            (((uint32_t)(((uint32_t)(x))<<PWT_R1_PPW_SHIFT))&PWT_R1_PPW_MASK)
/* R2 Bit Fields */
#define PWT_R2_NPW_MASK                          0xFFFFu
#define PWT_R2_NPW_SHIFT                         0
#define PWT_R2_NPW(x)                            (((uint32_t)(((uint32_t)(x))<<PWT_R2_NPW_SHIFT))&PWT_R2_NPW_MASK)
#define PWT_R2_PWTC_MASK                         0xFFFF0000u
#define PWT_R2_PWTC_SHIFT                        16
#define PWT_R2_PWTC(x)                           (((uint32_t)(((uint32_t)(x))<<PWT_R2_PWTC_SHIFT))&PWT_R2_PWTC_MASK)

/*!
 * @}
 */ /* end of group PWT_Register_Masks */


/* PWT - Peripheral instance base addresses */
/** Peripheral PWT base address */
#define PWT_BASE                                 (0x40033000u)
/** Peripheral PWT base pointer */
#define PWT                                      ((PWT_Type *)PWT_BASE)
/** Array initializer of PWT peripheral base pointers */
#define PWT_BASES                                { PWT }

/*!
 * @}
 */ /* end of group PWT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- ROM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Peripheral_Access_Layer ROM Peripheral Access Layer
 * @{
 */

/** ROM - Register Layout Typedef */
typedef struct {
  __I  uint32_t ENTRY[1];                          /**< Entry, array offset: 0x0, array step: 0x4 */
  __I  uint32_t TABLEMARK;                         /**< End of Table Marker Register, offset: 0x4 */
       uint8_t RESERVED_0[4036];
  __I  uint32_t SYSACCESS;                         /**< System Access Register, offset: 0xFCC */
  __I  uint32_t PERIPHID4;                         /**< Peripheral ID Register, offset: 0xFD0 */
  __I  uint32_t PERIPHID5;                         /**< Peripheral ID Register, offset: 0xFD4 */
  __I  uint32_t PERIPHID6;                         /**< Peripheral ID Register, offset: 0xFD8 */
  __I  uint32_t PERIPHID7;                         /**< Peripheral ID Register, offset: 0xFDC */
  __I  uint32_t PERIPHID0;                         /**< Peripheral ID Register, offset: 0xFE0 */
  __I  uint32_t PERIPHID1;                         /**< Peripheral ID Register, offset: 0xFE4 */
  __I  uint32_t PERIPHID2;                         /**< Peripheral ID Register, offset: 0xFE8 */
  __I  uint32_t PERIPHID3;                         /**< Peripheral ID Register, offset: 0xFEC */
  __I  uint32_t COMPID[4];                         /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} ROM_Type;

/* ----------------------------------------------------------------------------
   -- ROM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Masks ROM Register Masks
 * @{
 */

/* ENTRY Bit Fields */
#define ROM_ENTRY_ENTRY_MASK                     0xFFFFFFFFu
#define ROM_ENTRY_ENTRY_SHIFT                    0
#define ROM_ENTRY_ENTRY(x)                       (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY_ENTRY_SHIFT))&ROM_ENTRY_ENTRY_MASK)
/* TABLEMARK Bit Fields */
#define ROM_TABLEMARK_MARK_MASK                  0xFFFFFFFFu
#define ROM_TABLEMARK_MARK_SHIFT                 0
#define ROM_TABLEMARK_MARK(x)                    (((uint32_t)(((uint32_t)(x))<<ROM_TABLEMARK_MARK_SHIFT))&ROM_TABLEMARK_MARK_MASK)
/* SYSACCESS Bit Fields */
#define ROM_SYSACCESS_SYSACCESS_MASK             0xFFFFFFFFu
#define ROM_SYSACCESS_SYSACCESS_SHIFT            0
#define ROM_SYSACCESS_SYSACCESS(x)               (((uint32_t)(((uint32_t)(x))<<ROM_SYSACCESS_SYSACCESS_SHIFT))&ROM_SYSACCESS_SYSACCESS_MASK)
/* PERIPHID4 Bit Fields */
#define ROM_PERIPHID4_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID4_PERIPHID_SHIFT             0
#define ROM_PERIPHID4_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID4_PERIPHID_SHIFT))&ROM_PERIPHID4_PERIPHID_MASK)
/* PERIPHID5 Bit Fields */
#define ROM_PERIPHID5_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID5_PERIPHID_SHIFT             0
#define ROM_PERIPHID5_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID5_PERIPHID_SHIFT))&ROM_PERIPHID5_PERIPHID_MASK)
/* PERIPHID6 Bit Fields */
#define ROM_PERIPHID6_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID6_PERIPHID_SHIFT             0
#define ROM_PERIPHID6_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID6_PERIPHID_SHIFT))&ROM_PERIPHID6_PERIPHID_MASK)
/* PERIPHID7 Bit Fields */
#define ROM_PERIPHID7_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID7_PERIPHID_SHIFT             0
#define ROM_PERIPHID7_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID7_PERIPHID_SHIFT))&ROM_PERIPHID7_PERIPHID_MASK)
/* PERIPHID0 Bit Fields */
#define ROM_PERIPHID0_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID0_PERIPHID_SHIFT             0
#define ROM_PERIPHID0_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID0_PERIPHID_SHIFT))&ROM_PERIPHID0_PERIPHID_MASK)
/* PERIPHID1 Bit Fields */
#define ROM_PERIPHID1_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID1_PERIPHID_SHIFT             0
#define ROM_PERIPHID1_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID1_PERIPHID_SHIFT))&ROM_PERIPHID1_PERIPHID_MASK)
/* PERIPHID2 Bit Fields */
#define ROM_PERIPHID2_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID2_PERIPHID_SHIFT             0
#define ROM_PERIPHID2_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID2_PERIPHID_SHIFT))&ROM_PERIPHID2_PERIPHID_MASK)
/* PERIPHID3 Bit Fields */
#define ROM_PERIPHID3_PERIPHID_MASK              0xFFFFFFFFu
#define ROM_PERIPHID3_PERIPHID_SHIFT             0
#define ROM_PERIPHID3_PERIPHID(x)                (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID3_PERIPHID_SHIFT))&ROM_PERIPHID3_PERIPHID_MASK)
/* COMPID Bit Fields */
#define ROM_COMPID_COMPID_MASK                   0xFFFFFFFFu
#define ROM_COMPID_COMPID_SHIFT                  0
#define ROM_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x))<<ROM_COMPID_COMPID_SHIFT))&ROM_COMPID_COMPID_MASK)

/*!
 * @}
 */ /* end of group ROM_Register_Masks */


/* ROM - Peripheral instance base addresses */
/** Peripheral ROM base address */
#define ROM_BASE                                 (0xF0002000u)
/** Peripheral ROM base pointer */
#define ROM                                      ((ROM_Type *)ROM_BASE)
/** Array initializer of ROM peripheral base pointers */
#define ROM_BASES                                { ROM }

/*!
 * @}
 */ /* end of group ROM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- RTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Peripheral_Access_Layer RTC Peripheral Access Layer
 * @{
 */

/** RTC - Register Layout Typedef */
typedef struct {
  __IO uint32_t SC;                                /**< RTC Status and Control Register, offset: 0x0 */
  __IO uint32_t MOD;                               /**< RTC Modulo Register, offset: 0x4 */
  __I  uint32_t CNT;                               /**< RTC Counter Register, offset: 0x8 */
} RTC_Type;

/* ----------------------------------------------------------------------------
   -- RTC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Register_Masks RTC Register Masks
 * @{
 */

/* SC Bit Fields */
#define RTC_SC_RTCO_MASK                         0x10u
#define RTC_SC_RTCO_SHIFT                        4
#define RTC_SC_RTIE_MASK                         0x40u
#define RTC_SC_RTIE_SHIFT                        6
#define RTC_SC_RTIF_MASK                         0x80u
#define RTC_SC_RTIF_SHIFT                        7
#define RTC_SC_RTCPS_MASK                        0x700u
#define RTC_SC_RTCPS_SHIFT                       8
#define RTC_SC_RTCPS(x)                          (((uint32_t)(((uint32_t)(x))<<RTC_SC_RTCPS_SHIFT))&RTC_SC_RTCPS_MASK)
#define RTC_SC_RTCLKS_MASK                       0xC000u
#define RTC_SC_RTCLKS_SHIFT                      14
#define RTC_SC_RTCLKS(x)                         (((uint32_t)(((uint32_t)(x))<<RTC_SC_RTCLKS_SHIFT))&RTC_SC_RTCLKS_MASK)
/* MOD Bit Fields */
#define RTC_MOD_MOD_MASK                         0xFFFFu
#define RTC_MOD_MOD_SHIFT                        0
#define RTC_MOD_MOD(x)                           (((uint32_t)(((uint32_t)(x))<<RTC_MOD_MOD_SHIFT))&RTC_MOD_MOD_MASK)
/* CNT Bit Fields */
#define RTC_CNT_CNT_MASK                         0xFFFFu
#define RTC_CNT_CNT_SHIFT                        0
#define RTC_CNT_CNT(x)                           (((uint32_t)(((uint32_t)(x))<<RTC_CNT_CNT_SHIFT))&RTC_CNT_CNT_MASK)

/*!
 * @}
 */ /* end of group RTC_Register_Masks */


/* RTC - Peripheral instance base addresses */
/** Peripheral RTC base address */
#define RTC_BASE                                 (0x4003D000u)
/** Peripheral RTC base pointer */
#define RTC                                      ((RTC_Type *)RTC_BASE)
/** Array initializer of RTC peripheral base pointers */
#define RTC_BASES                                { RTC }

/*!
 * @}
 */ /* end of group RTC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SIM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Peripheral_Access_Layer SIM Peripheral Access Layer
 * @{
 */

/** SIM - Register Layout Typedef */
typedef struct {
  __I  uint32_t SRSID;                             /**< System Reset Status and ID Register, offset: 0x0 */
  __IO uint32_t SOPT0;                             /**< System Options Register 0, offset: 0x4 */
  __IO uint32_t SOPT1;                             /**< System Options Register, offset: 0x8 */
  __IO uint32_t PINSEL;                            /**< Pin Selection Register 0, offset: 0xC */
  __IO uint32_t PINSEL1;                           /**< Pin Selection Register 1, offset: 0x10 */
  __IO uint32_t SCGC;                              /**< System Clock Gating Control Register, offset: 0x14 */
  __I  uint32_t UUIDL;                             /**< Universally Unique Identifier Low Register, offset: 0x18 */
  __I  uint32_t UUIDH;                             /**< Universally Unique Identifier Middle Low Register, offset: 0x1C */
  __I  uint32_t UUIDMH;                            /**< Universally Unique Identifier Middle High Register, offset: 0x20 */
  __IO uint32_t CLKDIV;                            /**< Clock Divider Register, offset: 0x24 */
} SIM_Type;

/* ----------------------------------------------------------------------------
   -- SIM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Masks SIM Register Masks
 * @{
 */

/* SRSID Bit Fields */
#define SIM_SRSID_LVD_MASK                       0x2u
#define SIM_SRSID_LVD_SHIFT                      1
#define SIM_SRSID_LOC_MASK                       0x4u
#define SIM_SRSID_LOC_SHIFT                      2
#define SIM_SRSID_WDOG_MASK                      0x20u
#define SIM_SRSID_WDOG_SHIFT                     5
#define SIM_SRSID_PIN_MASK                       0x40u
#define SIM_SRSID_PIN_SHIFT                      6
#define SIM_SRSID_POR_MASK                       0x80u
#define SIM_SRSID_POR_SHIFT                      7
#define SIM_SRSID_LOCKUP_MASK                    0x200u
#define SIM_SRSID_LOCKUP_SHIFT                   9
#define SIM_SRSID_SW_MASK                        0x400u
#define SIM_SRSID_SW_SHIFT                       10
#define SIM_SRSID_MDMAP_MASK                     0x800u
#define SIM_SRSID_MDMAP_SHIFT                    11
#define SIM_SRSID_SACKERR_MASK                   0x2000u
#define SIM_SRSID_SACKERR_SHIFT                  13
#define SIM_SRSID_PINID_MASK                     0xF0000u
#define SIM_SRSID_PINID_SHIFT                    16
#define SIM_SRSID_PINID(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SRSID_PINID_SHIFT))&SIM_SRSID_PINID_MASK)
#define SIM_SRSID_RevID_MASK                     0xF00000u
#define SIM_SRSID_RevID_SHIFT                    20
#define SIM_SRSID_RevID(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SRSID_RevID_SHIFT))&SIM_SRSID_RevID_MASK)
#define SIM_SRSID_SUBFAMID_MASK                  0xF000000u
#define SIM_SRSID_SUBFAMID_SHIFT                 24
#define SIM_SRSID_SUBFAMID(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_SRSID_SUBFAMID_SHIFT))&SIM_SRSID_SUBFAMID_MASK)
#define SIM_SRSID_FAMID_MASK                     0xF0000000u
#define SIM_SRSID_FAMID_SHIFT                    28
#define SIM_SRSID_FAMID(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SRSID_FAMID_SHIFT))&SIM_SRSID_FAMID_MASK)
/* SOPT0 Bit Fields */
#define SIM_SOPT0_NMIE_MASK                      0x2u
#define SIM_SOPT0_NMIE_SHIFT                     1
#define SIM_SOPT0_RSTPE_MASK                     0x4u
#define SIM_SOPT0_RSTPE_SHIFT                    2
#define SIM_SOPT0_SWDE_MASK                      0x8u
#define SIM_SOPT0_SWDE_SHIFT                     3
#define SIM_SOPT0_ACTRG_MASK                     0x20u
#define SIM_SOPT0_ACTRG_SHIFT                    5
#define SIM_SOPT0_RXDFE_MASK                     0x300u
#define SIM_SOPT0_RXDFE_SHIFT                    8
#define SIM_SOPT0_RXDFE(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SOPT0_RXDFE_SHIFT))&SIM_SOPT0_RXDFE_MASK)
#define SIM_SOPT0_RTCC_MASK                      0x400u
#define SIM_SOPT0_RTCC_SHIFT                     10
#define SIM_SOPT0_ACIC_MASK                      0x800u
#define SIM_SOPT0_ACIC_SHIFT                     11
#define SIM_SOPT0_RXDCE_MASK                     0x1000u
#define SIM_SOPT0_RXDCE_SHIFT                    12
#define SIM_SOPT0_FTMSYNC_MASK                   0x4000u
#define SIM_SOPT0_FTMSYNC_SHIFT                  14
#define SIM_SOPT0_TXDME_MASK                     0x8000u
#define SIM_SOPT0_TXDME_SHIFT                    15
#define SIM_SOPT0_BUSREF_MASK                    0x70000u
#define SIM_SOPT0_BUSREF_SHIFT                   16
#define SIM_SOPT0_BUSREF(x)                      (((uint32_t)(((uint32_t)(x))<<SIM_SOPT0_BUSREF_SHIFT))&SIM_SOPT0_BUSREF_MASK)
#define SIM_SOPT0_CLKOE_MASK                     0x80000u
#define SIM_SOPT0_CLKOE_SHIFT                    19
#define SIM_SOPT0_ADHWT_MASK                     0x700000u
#define SIM_SOPT0_ADHWT_SHIFT                    20
#define SIM_SOPT0_ADHWT(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SOPT0_ADHWT_SHIFT))&SIM_SOPT0_ADHWT_MASK)
#define SIM_SOPT0_DLYACT_MASK                    0x800000u
#define SIM_SOPT0_DLYACT_SHIFT                   23
#define SIM_SOPT0_DELAY_MASK                     0xFF000000u
#define SIM_SOPT0_DELAY_SHIFT                    24
#define SIM_SOPT0_DELAY(x)                       (((uint32_t)(((uint32_t)(x))<<SIM_SOPT0_DELAY_SHIFT))&SIM_SOPT0_DELAY_MASK)
/* SOPT1 Bit Fields */
#define SIM_SOPT1_I2C04WEN_MASK                  0x1u
#define SIM_SOPT1_I2C04WEN_SHIFT                 0
#define SIM_SOPT1_I2C0OINV_MASK                  0x2u
#define SIM_SOPT1_I2C0OINV_SHIFT                 1
#define SIM_SOPT1_ACPWTS_MASK                    0x8u
#define SIM_SOPT1_ACPWTS_SHIFT                   3
#define SIM_SOPT1_UARTPWTS_MASK                  0x30u
#define SIM_SOPT1_UARTPWTS_SHIFT                 4
#define SIM_SOPT1_UARTPWTS(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_UARTPWTS_SHIFT))&SIM_SOPT1_UARTPWTS_MASK)
/* PINSEL Bit Fields */
#define SIM_PINSEL_IRQPS_MASK                    0x7u
#define SIM_PINSEL_IRQPS_SHIFT                   0
#define SIM_PINSEL_IRQPS(x)                      (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL_IRQPS_SHIFT))&SIM_PINSEL_IRQPS_MASK)
#define SIM_PINSEL_RTCPS_MASK                    0x10u
#define SIM_PINSEL_RTCPS_SHIFT                   4
#define SIM_PINSEL_I2C0PS_MASK                   0x20u
#define SIM_PINSEL_I2C0PS_SHIFT                  5
#define SIM_PINSEL_SPI0PS_MASK                   0x40u
#define SIM_PINSEL_SPI0PS_SHIFT                  6
#define SIM_PINSEL_UART0PS_MASK                  0x80u
#define SIM_PINSEL_UART0PS_SHIFT                 7
#define SIM_PINSEL_FTM0PS0_MASK                  0x100u
#define SIM_PINSEL_FTM0PS0_SHIFT                 8
#define SIM_PINSEL_FTM0PS1_MASK                  0x200u
#define SIM_PINSEL_FTM0PS1_SHIFT                 9
#define SIM_PINSEL_FTM1PS0_MASK                  0x400u
#define SIM_PINSEL_FTM1PS0_SHIFT                 10
#define SIM_PINSEL_FTM1PS1_MASK                  0x800u
#define SIM_PINSEL_FTM1PS1_SHIFT                 11
#define SIM_PINSEL_FTM0CLKPS_MASK                0x3000000u
#define SIM_PINSEL_FTM0CLKPS_SHIFT               24
#define SIM_PINSEL_FTM0CLKPS(x)                  (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL_FTM0CLKPS_SHIFT))&SIM_PINSEL_FTM0CLKPS_MASK)
#define SIM_PINSEL_FTM1CLKPS_MASK                0xC000000u
#define SIM_PINSEL_FTM1CLKPS_SHIFT               26
#define SIM_PINSEL_FTM1CLKPS(x)                  (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL_FTM1CLKPS_SHIFT))&SIM_PINSEL_FTM1CLKPS_MASK)
#define SIM_PINSEL_FTM2CLKPS_MASK                0x30000000u
#define SIM_PINSEL_FTM2CLKPS_SHIFT               28
#define SIM_PINSEL_FTM2CLKPS(x)                  (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL_FTM2CLKPS_SHIFT))&SIM_PINSEL_FTM2CLKPS_MASK)
#define SIM_PINSEL_PWTCLKPS_MASK                 0xC0000000u
#define SIM_PINSEL_PWTCLKPS_SHIFT                30
#define SIM_PINSEL_PWTCLKPS(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL_PWTCLKPS_SHIFT))&SIM_PINSEL_PWTCLKPS_MASK)
/* PINSEL1 Bit Fields */
#define SIM_PINSEL1_FTM2PS0_MASK                 0x3u
#define SIM_PINSEL1_FTM2PS0_SHIFT                0
#define SIM_PINSEL1_FTM2PS0(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL1_FTM2PS0_SHIFT))&SIM_PINSEL1_FTM2PS0_MASK)
#define SIM_PINSEL1_FTM2PS1_MASK                 0xCu
#define SIM_PINSEL1_FTM2PS1_SHIFT                2
#define SIM_PINSEL1_FTM2PS1(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL1_FTM2PS1_SHIFT))&SIM_PINSEL1_FTM2PS1_MASK)
#define SIM_PINSEL1_FTM2PS2_MASK                 0x30u
#define SIM_PINSEL1_FTM2PS2_SHIFT                4
#define SIM_PINSEL1_FTM2PS2(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL1_FTM2PS2_SHIFT))&SIM_PINSEL1_FTM2PS2_MASK)
#define SIM_PINSEL1_FTM2PS3_MASK                 0xC0u
#define SIM_PINSEL1_FTM2PS3_SHIFT                6
#define SIM_PINSEL1_FTM2PS3(x)                   (((uint32_t)(((uint32_t)(x))<<SIM_PINSEL1_FTM2PS3_SHIFT))&SIM_PINSEL1_FTM2PS3_MASK)
#define SIM_PINSEL1_FTM2PS4_MASK                 0x100u
#define SIM_PINSEL1_FTM2PS4_SHIFT                8
#define SIM_PINSEL1_FTM2PS5_MASK                 0x200u
#define SIM_PINSEL1_FTM2PS5_SHIFT                9
#define SIM_PINSEL1_I2C1PS_MASK                  0x400u
#define SIM_PINSEL1_I2C1PS_SHIFT                 10
#define SIM_PINSEL1_SPI1PS_MASK                  0x800u
#define SIM_PINSEL1_SPI1PS_SHIFT                 11
#define SIM_PINSEL1_UART1PS_MASK                 0x1000u
#define SIM_PINSEL1_UART1PS_SHIFT                12
#define SIM_PINSEL1_UART2PS_MASK                 0x2000u
#define SIM_PINSEL1_UART2PS_SHIFT                13
#define SIM_PINSEL1_PWTIN0PS_MASK                0x4000u
#define SIM_PINSEL1_PWTIN0PS_SHIFT               14
#define SIM_PINSEL1_PWTIN1PS_MASK                0x8000u
#define SIM_PINSEL1_PWTIN1PS_SHIFT               15
#define SIM_PINSEL1_MSCANPS_MASK                 0x10000u
#define SIM_PINSEL1_MSCANPS_SHIFT                16
/* SCGC Bit Fields */
#define SIM_SCGC_RTC_MASK                        0x1u
#define SIM_SCGC_RTC_SHIFT                       0
#define SIM_SCGC_PIT_MASK                        0x2u
#define SIM_SCGC_PIT_SHIFT                       1
#define SIM_SCGC_PWT_MASK                        0x10u
#define SIM_SCGC_PWT_SHIFT                       4
#define SIM_SCGC_FTM0_MASK                       0x20u
#define SIM_SCGC_FTM0_SHIFT                      5
#define SIM_SCGC_FTM1_MASK                       0x40u
#define SIM_SCGC_FTM1_SHIFT                      6
#define SIM_SCGC_FTM2_MASK                       0x80u
#define SIM_SCGC_FTM2_SHIFT                      7
#define SIM_SCGC_CRC_MASK                        0x400u
#define SIM_SCGC_CRC_SHIFT                       10
#define SIM_SCGC_FLASH_MASK                      0x1000u
#define SIM_SCGC_FLASH_SHIFT                     12
#define SIM_SCGC_SWD_MASK                        0x2000u
#define SIM_SCGC_SWD_SHIFT                       13
#define SIM_SCGC_MSCAN_MASK                      0x8000u
#define SIM_SCGC_MSCAN_SHIFT                     15
#define SIM_SCGC_I2C0_MASK                       0x10000u
#define SIM_SCGC_I2C0_SHIFT                      16
#define SIM_SCGC_I2C1_MASK                       0x20000u
#define SIM_SCGC_I2C1_SHIFT                      17
#define SIM_SCGC_SPI0_MASK                       0x40000u
#define SIM_SCGC_SPI0_SHIFT                      18
#define SIM_SCGC_SPI1_MASK                       0x80000u
#define SIM_SCGC_SPI1_SHIFT                      19
#define SIM_SCGC_UART0_MASK                      0x100000u
#define SIM_SCGC_UART0_SHIFT                     20
#define SIM_SCGC_UART1_MASK                      0x200000u
#define SIM_SCGC_UART1_SHIFT                     21
#define SIM_SCGC_UART2_MASK                      0x400000u
#define SIM_SCGC_UART2_SHIFT                     22
#define SIM_SCGC_KBI0_MASK                       0x1000000u
#define SIM_SCGC_KBI0_SHIFT                      24
#define SIM_SCGC_KBI1_MASK                       0x2000000u
#define SIM_SCGC_KBI1_SHIFT                      25
#define SIM_SCGC_IRQ_MASK                        0x8000000u
#define SIM_SCGC_IRQ_SHIFT                       27
#define SIM_SCGC_ADC_MASK                        0x20000000u
#define SIM_SCGC_ADC_SHIFT                       29
#define SIM_SCGC_ACMP0_MASK                      0x40000000u
#define SIM_SCGC_ACMP0_SHIFT                     30
#define SIM_SCGC_ACMP1_MASK                      0x80000000u
#define SIM_SCGC_ACMP1_SHIFT                     31
/* UUIDL Bit Fields */
#define SIM_UUIDL_ID_MASK                        0xFFFFFFFFu
#define SIM_UUIDL_ID_SHIFT                       0
#define SIM_UUIDL_ID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UUIDL_ID_SHIFT))&SIM_UUIDL_ID_MASK)
/* UUIDH Bit Fields */
#define SIM_UUIDH_ID_MASK                        0xFFFFFFFFu
#define SIM_UUIDH_ID_SHIFT                       0
#define SIM_UUIDH_ID(x)                          (((uint32_t)(((uint32_t)(x))<<SIM_UUIDH_ID_SHIFT))&SIM_UUIDH_ID_MASK)
/* UUIDMH Bit Fields */
#define SIM_UUIDMH_ID_MASK                       0xFFFFu
#define SIM_UUIDMH_ID_SHIFT                      0
#define SIM_UUIDMH_ID(x)                         (((uint32_t)(((uint32_t)(x))<<SIM_UUIDMH_ID_SHIFT))&SIM_UUIDMH_ID_MASK)
/* CLKDIV Bit Fields */
#define SIM_CLKDIV_OUTDIV3_MASK                  0x100000u
#define SIM_CLKDIV_OUTDIV3_SHIFT                 20
#define SIM_CLKDIV_OUTDIV2_MASK                  0x1000000u
#define SIM_CLKDIV_OUTDIV2_SHIFT                 24
#define SIM_CLKDIV_OUTDIV1_MASK                  0x30000000u
#define SIM_CLKDIV_OUTDIV1_SHIFT                 28
#define SIM_CLKDIV_OUTDIV1(x)                    (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV_OUTDIV1_SHIFT))&SIM_CLKDIV_OUTDIV1_MASK)

/*!
 * @}
 */ /* end of group SIM_Register_Masks */


/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base address */
#define SIM_BASE                                 (0x40048000u)
/** Peripheral SIM base pointer */
#define SIM                                      ((SIM_Type *)SIM_BASE)
/** Array initializer of SIM peripheral base pointers */
#define SIM_BASES                                { SIM }

/*!
 * @}
 */ /* end of group SIM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Peripheral_Access_Layer SPI Peripheral Access Layer
 * @{
 */

/** SPI - Register Layout Typedef */
typedef struct {
  __IO uint8_t C1;                                 /**< SPI Control Register 1, offset: 0x0 */
  __IO uint8_t C2;                                 /**< SPI Control Register 2, offset: 0x1 */
  __IO uint8_t BR;                                 /**< SPI Baud Rate Register, offset: 0x2 */
  __I  uint8_t S;                                  /**< SPI Status Register, offset: 0x3 */
       uint8_t RESERVED_0[1];
  __IO uint8_t D;                                  /**< SPI Data Register, offset: 0x5 */
       uint8_t RESERVED_1[1];
  __IO uint8_t M;                                  /**< SPI Match Register, offset: 0x7 */
} SPI_Type;

/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/* C1 Bit Fields */
#define SPI_C1_LSBFE_MASK                        0x1u
#define SPI_C1_LSBFE_SHIFT                       0
#define SPI_C1_SSOE_MASK                         0x2u
#define SPI_C1_SSOE_SHIFT                        1
#define SPI_C1_CPHA_MASK                         0x4u
#define SPI_C1_CPHA_SHIFT                        2
#define SPI_C1_CPOL_MASK                         0x8u
#define SPI_C1_CPOL_SHIFT                        3
#define SPI_C1_MSTR_MASK                         0x10u
#define SPI_C1_MSTR_SHIFT                        4
#define SPI_C1_SPTIE_MASK                        0x20u
#define SPI_C1_SPTIE_SHIFT                       5
#define SPI_C1_SPE_MASK                          0x40u
#define SPI_C1_SPE_SHIFT                         6
#define SPI_C1_SPIE_MASK                         0x80u
#define SPI_C1_SPIE_SHIFT                        7
/* C2 Bit Fields */
#define SPI_C2_SPC0_MASK                         0x1u
#define SPI_C2_SPC0_SHIFT                        0
#define SPI_C2_SPISWAI_MASK                      0x2u
#define SPI_C2_SPISWAI_SHIFT                     1
#define SPI_C2_BIDIROE_MASK                      0x8u
#define SPI_C2_BIDIROE_SHIFT                     3
#define SPI_C2_MODFEN_MASK                       0x10u
#define SPI_C2_MODFEN_SHIFT                      4
#define SPI_C2_SPMIE_MASK                        0x80u
#define SPI_C2_SPMIE_SHIFT                       7
/* BR Bit Fields */
#define SPI_BR_SPR_MASK                          0xFu
#define SPI_BR_SPR_SHIFT                         0
#define SPI_BR_SPR(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPR_SHIFT))&SPI_BR_SPR_MASK)
#define SPI_BR_SPPR_MASK                         0x70u
#define SPI_BR_SPPR_SHIFT                        4
#define SPI_BR_SPPR(x)                           (((uint8_t)(((uint8_t)(x))<<SPI_BR_SPPR_SHIFT))&SPI_BR_SPPR_MASK)
/* S Bit Fields */
#define SPI_S_MODF_MASK                          0x10u
#define SPI_S_MODF_SHIFT                         4
#define SPI_S_SPTEF_MASK                         0x20u
#define SPI_S_SPTEF_SHIFT                        5
#define SPI_S_SPMF_MASK                          0x40u
#define SPI_S_SPMF_SHIFT                         6
#define SPI_S_SPRF_MASK                          0x80u
#define SPI_S_SPRF_SHIFT                         7
/* D Bit Fields */
#define SPI_D_Bits_MASK                          0xFFu
#define SPI_D_Bits_SHIFT                         0
#define SPI_D_Bits(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_D_Bits_SHIFT))&SPI_D_Bits_MASK)
/* M Bit Fields */
#define SPI_M_Bits_MASK                          0xFFu
#define SPI_M_Bits_SHIFT                         0
#define SPI_M_Bits(x)                            (((uint8_t)(((uint8_t)(x))<<SPI_M_Bits_SHIFT))&SPI_M_Bits_MASK)

/*!
 * @}
 */ /* end of group SPI_Register_Masks */


/* SPI - Peripheral instance base addresses */
/** Peripheral SPI0 base address */
#define SPI0_BASE                                (0x40076000u)
/** Peripheral SPI0 base pointer */
#define SPI0                                     ((SPI_Type *)SPI0_BASE)
/** Peripheral SPI1 base address */
#define SPI1_BASE                                (0x40077000u)
/** Peripheral SPI1 base pointer */
#define SPI1                                     ((SPI_Type *)SPI1_BASE)
/** Array initializer of SPI peripheral base pointers */
#define SPI_BASES                                { SPI0, SPI1 }

/*!
 * @}
 */ /* end of group SPI_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- UART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Peripheral_Access_Layer UART Peripheral Access Layer
 * @{
 */

/** UART - Register Layout Typedef */
typedef struct {
  __IO uint8_t BDH;                                /**< UART Baud Rate Register: High, offset: 0x0 */
  __IO uint8_t BDL;                                /**< UART Baud Rate Register: Low, offset: 0x1 */
  __IO uint8_t C1;                                 /**< UART Control Register 1, offset: 0x2 */
  __IO uint8_t C2;                                 /**< UART Control Register 2, offset: 0x3 */
  __I  uint8_t S1;                                 /**< UART Status Register 1, offset: 0x4 */
  __IO uint8_t S2;                                 /**< UART Status Register 2, offset: 0x5 */
  __IO uint8_t C3;                                 /**< UART Control Register 3, offset: 0x6 */
  __IO uint8_t D;                                  /**< UART Data Register, offset: 0x7 */
} UART_Type;

/* ----------------------------------------------------------------------------
   -- UART Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Register_Masks UART Register Masks
 * @{
 */

/* BDH Bit Fields */
#define UART_BDH_SBR_MASK                        0x1Fu
#define UART_BDH_SBR_SHIFT                       0
#define UART_BDH_SBR(x)                          (((uint8_t)(((uint8_t)(x))<<UART_BDH_SBR_SHIFT))&UART_BDH_SBR_MASK)
#define UART_BDH_SBNS_MASK                       0x20u
#define UART_BDH_SBNS_SHIFT                      5
#define UART_BDH_RXEDGIE_MASK                    0x40u
#define UART_BDH_RXEDGIE_SHIFT                   6
#define UART_BDH_LBKDIE_MASK                     0x80u
#define UART_BDH_LBKDIE_SHIFT                    7
/* BDL Bit Fields */
#define UART_BDL_SBR_MASK                        0xFFu
#define UART_BDL_SBR_SHIFT                       0
#define UART_BDL_SBR(x)                          (((uint8_t)(((uint8_t)(x))<<UART_BDL_SBR_SHIFT))&UART_BDL_SBR_MASK)
/* C1 Bit Fields */
#define UART_C1_PT_MASK                          0x1u
#define UART_C1_PT_SHIFT                         0
#define UART_C1_PE_MASK                          0x2u
#define UART_C1_PE_SHIFT                         1
#define UART_C1_ILT_MASK                         0x4u
#define UART_C1_ILT_SHIFT                        2
#define UART_C1_WAKE_MASK                        0x8u
#define UART_C1_WAKE_SHIFT                       3
#define UART_C1_M_MASK                           0x10u
#define UART_C1_M_SHIFT                          4
#define UART_C1_RSRC_MASK                        0x20u
#define UART_C1_RSRC_SHIFT                       5
#define UART_C1_UARTSWAI_MASK                    0x40u
#define UART_C1_UARTSWAI_SHIFT                   6
#define UART_C1_LOOPS_MASK                       0x80u
#define UART_C1_LOOPS_SHIFT                      7
/* C2 Bit Fields */
#define UART_C2_SBK_MASK                         0x1u
#define UART_C2_SBK_SHIFT                        0
#define UART_C2_RWU_MASK                         0x2u
#define UART_C2_RWU_SHIFT                        1
#define UART_C2_RE_MASK                          0x4u
#define UART_C2_RE_SHIFT                         2
#define UART_C2_TE_MASK                          0x8u
#define UART_C2_TE_SHIFT                         3
#define UART_C2_ILIE_MASK                        0x10u
#define UART_C2_ILIE_SHIFT                       4
#define UART_C2_RIE_MASK                         0x20u
#define UART_C2_RIE_SHIFT                        5
#define UART_C2_TCIE_MASK                        0x40u
#define UART_C2_TCIE_SHIFT                       6
#define UART_C2_TIE_MASK                         0x80u
#define UART_C2_TIE_SHIFT                        7
/* S1 Bit Fields */
#define UART_S1_PF_MASK                          0x1u
#define UART_S1_PF_SHIFT                         0
#define UART_S1_FE_MASK                          0x2u
#define UART_S1_FE_SHIFT                         1
#define UART_S1_NF_MASK                          0x4u
#define UART_S1_NF_SHIFT                         2
#define UART_S1_OR_MASK                          0x8u
#define UART_S1_OR_SHIFT                         3
#define UART_S1_IDLE_MASK                        0x10u
#define UART_S1_IDLE_SHIFT                       4
#define UART_S1_RDRF_MASK                        0x20u
#define UART_S1_RDRF_SHIFT                       5
#define UART_S1_TC_MASK                          0x40u
#define UART_S1_TC_SHIFT                         6
#define UART_S1_TDRE_MASK                        0x80u
#define UART_S1_TDRE_SHIFT                       7
/* S2 Bit Fields */
#define UART_S2_RAF_MASK                         0x1u
#define UART_S2_RAF_SHIFT                        0
#define UART_S2_LBKDE_MASK                       0x2u
#define UART_S2_LBKDE_SHIFT                      1
#define UART_S2_BRK13_MASK                       0x4u
#define UART_S2_BRK13_SHIFT                      2
#define UART_S2_RWUID_MASK                       0x8u
#define UART_S2_RWUID_SHIFT                      3
#define UART_S2_RXINV_MASK                       0x10u
#define UART_S2_RXINV_SHIFT                      4
#define UART_S2_RXEDGIF_MASK                     0x40u
#define UART_S2_RXEDGIF_SHIFT                    6
#define UART_S2_LBKDIF_MASK                      0x80u
#define UART_S2_LBKDIF_SHIFT                     7
/* C3 Bit Fields */
#define UART_C3_PEIE_MASK                        0x1u
#define UART_C3_PEIE_SHIFT                       0
#define UART_C3_FEIE_MASK                        0x2u
#define UART_C3_FEIE_SHIFT                       1
#define UART_C3_NEIE_MASK                        0x4u
#define UART_C3_NEIE_SHIFT                       2
#define UART_C3_ORIE_MASK                        0x8u
#define UART_C3_ORIE_SHIFT                       3
#define UART_C3_TXINV_MASK                       0x10u
#define UART_C3_TXINV_SHIFT                      4
#define UART_C3_TXDIR_MASK                       0x20u
#define UART_C3_TXDIR_SHIFT                      5
#define UART_C3_T8_MASK                          0x40u
#define UART_C3_T8_SHIFT                         6
#define UART_C3_R8_MASK                          0x80u
#define UART_C3_R8_SHIFT                         7
/* D Bit Fields */
#define UART_D_R0T0_MASK                         0x1u
#define UART_D_R0T0_SHIFT                        0
#define UART_D_R1T1_MASK                         0x2u
#define UART_D_R1T1_SHIFT                        1
#define UART_D_R2T2_MASK                         0x4u
#define UART_D_R2T2_SHIFT                        2
#define UART_D_R3T3_MASK                         0x8u
#define UART_D_R3T3_SHIFT                        3
#define UART_D_R4T4_MASK                         0x10u
#define UART_D_R4T4_SHIFT                        4
#define UART_D_R5T5_MASK                         0x20u
#define UART_D_R5T5_SHIFT                        5
#define UART_D_R6T6_MASK                         0x40u
#define UART_D_R6T6_SHIFT                        6
#define UART_D_R7T7_MASK                         0x80u
#define UART_D_R7T7_SHIFT                        7

/*!
 * @}
 */ /* end of group UART_Register_Masks */


/* UART - Peripheral instance base addresses */
/** Peripheral UART0 base address */
#define UART0_BASE                               (0x4006A000u)
/** Peripheral UART0 base pointer */
#define UART0                                    ((UART_Type *)UART0_BASE)
/** Peripheral UART1 base address */
#define UART1_BASE                               (0x4006B000u)
/** Peripheral UART1 base pointer */
#define UART1                                    ((UART_Type *)UART1_BASE)
/** Peripheral UART2 base address */
#define UART2_BASE                               (0x4006C000u)
/** Peripheral UART2 base pointer */
#define UART2                                    ((UART_Type *)UART2_BASE)
/** Array initializer of UART peripheral base pointers */
#define UART_BASES                               { UART0, UART1, UART2 }

/*!
 * @}
 */ /* end of group UART_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- WDOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Peripheral_Access_Layer WDOG Peripheral Access Layer
 * @{
 */

/** WDOG - Register Layout Typedef */
typedef struct {
  __IO uint8_t CS1;                                /**< Watchdog Control and Status Register 1, offset: 0x0 */
  __IO uint8_t CS2;                                /**< Watchdog Control and Status Register 2, offset: 0x1 */
  union {                                          /* offset: 0x2 */
    __IO uint16_t CNT;                               /**< WDOG_CNT register., offset: 0x2 */
    struct {                                         /* offset: 0x2 */
      __I  uint8_t CNTH;                               /**< Watchdog Counter Register: High, offset: 0x2 */
      __I  uint8_t CNTL;                               /**< Watchdog Counter Register: Low, offset: 0x3 */
    } CNT8B;
  };
  union {                                          /* offset: 0x4 */
    __IO uint16_t TOVAL;                             /**< WDOG_TOVAL register., offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint8_t TOVALH;                             /**< Watchdog Timeout Value Register: High, offset: 0x4 */
      __IO uint8_t TOVALL;                             /**< Watchdog Timeout Value Register: Low, offset: 0x5 */
    } TOVAL8B;
  };
  union {                                          /* offset: 0x6 */
    __IO uint16_t WIN;                               /**< WDOG_WIN register., offset: 0x6 */
    struct {                                         /* offset: 0x6 */
      __IO uint8_t WINH;                               /**< Watchdog Window Register: High, offset: 0x6 */
      __IO uint8_t WINL;                               /**< Watchdog Window Register: Low, offset: 0x7 */
    } WIN8B;
  };
} WDOG_Type;

/* ----------------------------------------------------------------------------
   -- WDOG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Register_Masks WDOG Register Masks
 * @{
 */

/* CS1 Bit Fields */
#define WDOG_CS1_STOP_MASK                       0x1u
#define WDOG_CS1_STOP_SHIFT                      0
#define WDOG_CS1_WAIT_MASK                       0x2u
#define WDOG_CS1_WAIT_SHIFT                      1
#define WDOG_CS1_DBG_MASK                        0x4u
#define WDOG_CS1_DBG_SHIFT                       2
#define WDOG_CS1_TST_MASK                        0x18u
#define WDOG_CS1_TST_SHIFT                       3
#define WDOG_CS1_TST(x)                          (((uint8_t)(((uint8_t)(x))<<WDOG_CS1_TST_SHIFT))&WDOG_CS1_TST_MASK)
#define WDOG_CS1_UPDATE_MASK                     0x20u
#define WDOG_CS1_UPDATE_SHIFT                    5
#define WDOG_CS1_INT_MASK                        0x40u
#define WDOG_CS1_INT_SHIFT                       6
#define WDOG_CS1_EN_MASK                         0x80u
#define WDOG_CS1_EN_SHIFT                        7
/* CS2 Bit Fields */
#define WDOG_CS2_CLK_MASK                        0x3u
#define WDOG_CS2_CLK_SHIFT                       0
#define WDOG_CS2_CLK(x)                          (((uint8_t)(((uint8_t)(x))<<WDOG_CS2_CLK_SHIFT))&WDOG_CS2_CLK_MASK)
#define WDOG_CS2_PRES_MASK                       0x10u
#define WDOG_CS2_PRES_SHIFT                      4
#define WDOG_CS2_FLG_MASK                        0x40u
#define WDOG_CS2_FLG_SHIFT                       6
#define WDOG_CS2_WIN_MASK                        0x80u
#define WDOG_CS2_WIN_SHIFT                       7
/* CNT Bit Fields */
#define WDOG_CNT_CNT_MASK                        0xFFFFu
#define WDOG_CNT_CNT_SHIFT                       0
#define WDOG_CNT_CNT(x)                          (((uint16_t)(((uint16_t)(x))<<WDOG_CNT_CNT_SHIFT))&WDOG_CNT_CNT_MASK)
/* CNTH Bit Fields */
#define WDOG_CNTH_CNTHIGH_MASK                   0xFFu
#define WDOG_CNTH_CNTHIGH_SHIFT                  0
#define WDOG_CNTH_CNTHIGH(x)                     (((uint8_t)(((uint8_t)(x))<<WDOG_CNTH_CNTHIGH_SHIFT))&WDOG_CNTH_CNTHIGH_MASK)
/* CNTL Bit Fields */
#define WDOG_CNTL_CNTLOW_MASK                    0xFFu
#define WDOG_CNTL_CNTLOW_SHIFT                   0
#define WDOG_CNTL_CNTLOW(x)                      (((uint8_t)(((uint8_t)(x))<<WDOG_CNTL_CNTLOW_SHIFT))&WDOG_CNTL_CNTLOW_MASK)
/* TOVAL Bit Fields */
#define WDOG_TOVAL_TOVAL_MASK                    0xFFFFu
#define WDOG_TOVAL_TOVAL_SHIFT                   0
#define WDOG_TOVAL_TOVAL(x)                      (((uint16_t)(((uint16_t)(x))<<WDOG_TOVAL_TOVAL_SHIFT))&WDOG_TOVAL_TOVAL_MASK)
/* TOVALH Bit Fields */
#define WDOG_TOVALH_TOVALHIGH_MASK               0xFFu
#define WDOG_TOVALH_TOVALHIGH_SHIFT              0
#define WDOG_TOVALH_TOVALHIGH(x)                 (((uint8_t)(((uint8_t)(x))<<WDOG_TOVALH_TOVALHIGH_SHIFT))&WDOG_TOVALH_TOVALHIGH_MASK)
/* TOVALL Bit Fields */
#define WDOG_TOVALL_TOVALLOW_MASK                0xFFu
#define WDOG_TOVALL_TOVALLOW_SHIFT               0
#define WDOG_TOVALL_TOVALLOW(x)                  (((uint8_t)(((uint8_t)(x))<<WDOG_TOVALL_TOVALLOW_SHIFT))&WDOG_TOVALL_TOVALLOW_MASK)
/* WIN Bit Fields */
#define WDOG_WIN_WIN_MASK                        0xFFFFu
#define WDOG_WIN_WIN_SHIFT                       0
#define WDOG_WIN_WIN(x)                          (((uint16_t)(((uint16_t)(x))<<WDOG_WIN_WIN_SHIFT))&WDOG_WIN_WIN_MASK)
/* WINH Bit Fields */
#define WDOG_WINH_WINHIGH_MASK                   0xFFu
#define WDOG_WINH_WINHIGH_SHIFT                  0
#define WDOG_WINH_WINHIGH(x)                     (((uint8_t)(((uint8_t)(x))<<WDOG_WINH_WINHIGH_SHIFT))&WDOG_WINH_WINHIGH_MASK)
/* WINL Bit Fields */
#define WDOG_WINL_WINLOW_MASK                    0xFFu
#define WDOG_WINL_WINLOW_SHIFT                   0
#define WDOG_WINL_WINLOW(x)                      (((uint8_t)(((uint8_t)(x))<<WDOG_WINL_WINLOW_SHIFT))&WDOG_WINL_WINLOW_MASK)

/*!
 * @}
 */ /* end of group WDOG_Register_Masks */


/* WDOG - Peripheral instance base addresses */
/** Peripheral WDOG base address */
#define WDOG_BASE                                (0x40052000u)
/** Peripheral WDOG base pointer */
#define WDOG                                     ((WDOG_Type *)WDOG_BASE)
/** Array initializer of WDOG peripheral base pointers */
#define WDOG_BASES                               { WDOG }

/*!
 * @}
 */ /* end of group WDOG_Peripheral_Access_Layer */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma pop
#elif defined(__CWCC__)
  #pragma pop
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_access_layer */


/* ----------------------------------------------------------------------------
   -- Backward Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Backward_Compatibility_Symbols Backward Compatibility
 * @{
 */

/* No backward compatibility issues. */

/*!
 * @}
 */ /* end of group Backward_Compatibility_Symbols */


#endif  /* #if !defined(MKE06Z4_H_) */

/* MKE06Z4.h, eof. */
