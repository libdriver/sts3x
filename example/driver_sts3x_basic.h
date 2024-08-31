/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sts3x_basic.h
 * @brief     driver sts3x basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-09-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/09/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_STS3X_BASIC_H
#define DRIVER_STS3X_BASIC_H

#include "driver_sts3x_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sts3x_example_driver sts3x example driver function
 * @brief    sts3x example driver modules
 * @ingroup  sts3x_driver
 * @{
 */

/**
 * @brief sts3x basic example default definition
 */
#define STS3X_BASIC_DEFAULT_RATE                 STS3X_RATE_10HZ                 /**< rate 100Hz */
#define STS3X_BASIC_DEFAULT_REPEATABILITY        STS3X_REPEATABILITY_HIGH        /**< set high */
#define STS3X_BASIC_DEFAULT_HEATER               STS3X_BOOL_FALSE                /**< disable heater */

/**
 * @brief     basic example init
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t sts3x_basic_init(sts3x_address_t addr_pin);

/**
 * @brief      basic example read
 * @param[out] *temperature points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sts3x_basic_read(float *temperature);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sts3x_basic_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
