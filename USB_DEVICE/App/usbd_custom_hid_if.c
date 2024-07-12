/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */
#define THROTTLE_ENABLED				1					// 1 for enabled, 0 for disabled
#define RUDDER_ENABLED					1					// 1 for enabled, 0 for disabled
#define JOYSTICK_AXIS_NUM				3
#define SECONDARY_AXIS_NUM			2

#define BUTTONS_NUM							0
#define LEDS_NUM								0
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
enum
{
	REPORT_ID_JOY = 1,
	REPORT_ID_PARAM,
	REPORT_ID_CONFIG_IN,
	REPORT_ID_CONFIG_OUT,
	REPORT_ID_FIRMWARE,
};

#define FIRMWARE_VERSION					0x1713			// v1.7.1b2
#define USED_PINS_NUM							30					// constant for BluePill and BlackPill boards
#define MAX_AXIS_NUM							8						// max 8
#define MAX_BUTTONS_NUM						128					// power of 2, max 128
#define MAX_POVS_NUM							4						// max 4
#define MAX_ENCODERS_NUM					16					// max 64
#define MAX_SHIFT_REG_NUM					4						// max 4
#define MAX_LEDS_NUM							24
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
uint8_t usb_buffer[0x40];
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
   0x06, 0x8c, 0xff, // Usage Page (Vendor Defined Page 8c)
   0x09, 0x01,       // Usage (Vendor Usage 1)
   0xa1, 0x01,       // Collection (Application)

   // Input report
   0x85, 0x01,       // Report ID (1)
   0x09, 0x20,       // Usage (Data in)
   0x15, 0x00,       // Logical Minimum (0)
   0x26, 0xff, 0x00, // Logical Maximum (255)
   0x75, 0x08,       // Report Size (8 bits)
   0x95, 0x01,       // Report Count (1 byte)
   0x81, 0x02,       // Input (Data, Var, Abs)

   // Output report
   0x85, 0x02,       // Report ID (2)
   0x09, 0x21,       // Usage (Data out)
   0x15, 0x00,       // Logical Minimum (0)
   0x26, 0xff, 0x00, // Logical Maximum (255)
   0x75, 0x08,       // Report Size (8 bits)
   0x95, 0x01,       // Report Count (1 byte)
   0x91, 0x02,       // Output (Data, Var, Abs)

//	    0x06, 0x00, 0xff,               //Usage Page(Undefined )
//	    0x09, 0x01,                    // USAGE (Undefined)
//	    0xa1, 0x01,                    // COLLECTION (Application)
//	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//	    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
//	    0x75, 0x08,                    //   REPORT_SIZE (8)
//	    0x95, 0x40,                    //   REPORT_COUNT (64)
//	    0x09, 0x01,                    //   USAGE (Undefined)
//	    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//	    0x95, 0x40,                    //   REPORT_COUNT (64)
//	    0x09, 0x01,                    //   USAGE (Undefined)
//	    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
//	    0x95, 0x01,                    //   REPORT_COUNT (1)
//	    0x09, 0x01,                    //   USAGE (Undefined)
//	    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */

  uint8_t received_data[2];
  USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef*) hUsbDeviceFS.pClassData;
  memcpy(received_data, hhid->Report_buf, 2);
  /* Start next USB packet transfer once data processing is completed */
  if (USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS) != (uint8_t)USBD_OK)
  {
    return -1;
  }

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

