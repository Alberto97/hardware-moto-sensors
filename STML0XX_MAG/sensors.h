/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_SENSORS_H
#define ANDROID_SENSORS_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <linux/input.h>
#include <linux/limits.h>

#include <hardware/hardware.h>
#include <hardware/sensors.h>
#include <hardware/mot_sensorhub_stml0xx.h>

__BEGIN_DECLS

int init_sensors(hw_module_t const* module, hw_device_t** device);

/*****************************************************************************/

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define MIN_SENSOR_ID (0 + SENSORS_HANDLE_BASE)
#define ID_A  (0 + SENSORS_HANDLE_BASE)  /* Accelerometer */
#define ID_M  (1 + SENSORS_HANDLE_BASE)  /* Magnetometer */
#define ID_OR (2 + SENSORS_HANDLE_BASE)  /* Orientation */
#define ID_RV (3 + SENSORS_HANDLE_BASE)  /* Rotation Vector */
#define ID_L  (4 + SENSORS_HANDLE_BASE)  /* Light */
#define ID_P  (5 + SENSORS_HANDLE_BASE)  /* Proximity */
#define ID_DR (6 + SENSORS_HANDLE_BASE)  /* Display Rotate */
#define ID_FU (7 + SENSORS_HANDLE_BASE)  /* Flat Up */
#define ID_FD (8 + SENSORS_HANDLE_BASE)  /* Flat Down */
#define ID_S  (9 + SENSORS_HANDLE_BASE)  /* Stowed */
#define ID_CA (10 + SENSORS_HANDLE_BASE) /* Camera Activate */
#define ID_A2 (11 + SENSORS_HANDLE_BASE) /* Secondary Accel */
#define MAX_SENSOR_ID (11 + SENSORS_HANDLE_BASE)

/*****************************************************************************/
/* Sensor Hub definitions */

/* KXTJ2 configured to +/-8G 14-bit mode
 * 14-bit Register Data range -8192 ~ +8191
 * 1024 LSG = 1G
*/
#define RANGE_G                     (8.0f)
#define LSG                         (1024.0f)

/* Conversion of acceleration data to SI units (m/s^2) */
#define CONVERT_A                   (GRAVITY_EARTH / LSG)
#define CONVERT_A_X                 (CONVERT_A)
#define CONVERT_A_Y                 (CONVERT_A)
#define CONVERT_A_Z                 (CONVERT_A)

/* Proximity uncovered and covered values */
#define PROX_UNCOVERED              (100.0f)
#define PROX_COVERED                (3.0f)
#define PROX_SATURATED              (1.0f)

/* Display rotate values */
#define DISP_FLAT                   (0x10)
#define DISP_UNKNOWN                (-1.0f)

/* Flat up / down values */
#define FLAT_NOTDETECTED            (0.0f)
#define FLAT_DETECTED               (1.0f)

/*****************************************************************************/

/* AKM Definitions */

/* For Accelerometer */
#define EVENT_TYPE_ACCEL_X          ABS_X
#define EVENT_TYPE_ACCEL_Y          ABS_Y
#define EVENT_TYPE_ACCEL_Z          ABS_Z
#define EVENT_TYPE_ACCEL_STATUS     ABS_RX

/* For Magnetometer */
#define EVENT_TYPE_MAGV_X           ABS_RY
#define EVENT_TYPE_MAGV_Y           ABS_RZ
#define EVENT_TYPE_MAGV_Z           ABS_THROTTLE
#define EVENT_TYPE_MAGV_STATUS      ABS_RUDDER

/* Fusion Orientaion */
#define EVENT_TYPE_YAW              ABS_HAT0Y
#define EVENT_TYPE_PITCH            ABS_HAT1X
#define EVENT_TYPE_ROLL             ABS_HAT1Y

/* Fusion Rotaion Vector */
#define EVENT_TYPE_ROTVEC_X         ABS_TILT_X
#define EVENT_TYPE_ROTVEC_Y         ABS_TILT_Y
#define EVENT_TYPE_ROTVEC_Z         ABS_TOOL_WIDTH
#define EVENT_TYPE_ROTVEC_W			ABS_VOLUME

#define CONVERT_Q14					(1.0f / 16384.0f)
#define CONVERT_AKM_G				(GRAVITY_EARTH / 720.0f)

/* conversion of acceleration data to SI units (m/s^2) */
/* 720 LSB = 1G */
#define AKSC_LSG					(720.0f)
#define CONVERT_A_AKM					CONVERT_AKM_G
/* conversion of magnetic data to uT units */
#define CONVERT_M					(0.06f)

/* conversion of orientation data (Q6) to degree units */
#define CONVERT_OR					(1.0f / 64.0f)
/* conversion of rotation vector (Q14) data to float */
#define CONVERT_RV					CONVERT_Q14

#define SENSOR_STATE_MASK           (0x7FFF)

/*****************************************************************************/

__END_DECLS

#endif  // ANDROID_SENSORS_H