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
 * @file      driver_sts3x_read_test.c
 * @brief     driver sts3x read test source file
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

#include "driver_sts3x_read_test.h"

static sts3x_handle_t gs_handle;        /**< sts3x handle */

/**
 * @brief     read test
 * @param[in] addr_pin is the iic device address
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sts3x_read_test(sts3x_address_t addr_pin, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    sts3x_info_t info;
    
    /* link functions */
    DRIVER_STS3X_LINK_INIT(&gs_handle, sts3x_handle_t);
    DRIVER_STS3X_LINK_IIC_INIT(&gs_handle, sts3x_interface_iic_init);
    DRIVER_STS3X_LINK_IIC_DEINIT(&gs_handle, sts3x_interface_iic_deinit);
    DRIVER_STS3X_LINK_IIC_READ_ADDRESS16(&gs_handle, sts3x_interface_iic_read_address16);
    DRIVER_STS3X_LINK_IIC_WRITE_ADDRESS16(&gs_handle, sts3x_interface_iic_write_address16);
    DRIVER_STS3X_LINK_DELAY_MS(&gs_handle, sts3x_interface_delay_ms);
    DRIVER_STS3X_LINK_DEBUG_PRINT(&gs_handle, sts3x_interface_debug_print);
    
    /* sts3x info */
    res = sts3x_info(&info);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        sts3x_interface_debug_print("sts3x: chip is %s.\n", info.chip_name);
        sts3x_interface_debug_print("sts3x: manufacturer is %s.\n", info.manufacturer_name);
        sts3x_interface_debug_print("sts3x: interface is %s.\n", info.interface);
        sts3x_interface_debug_print("sts3x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sts3x_interface_debug_print("sts3x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sts3x_interface_debug_print("sts3x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sts3x_interface_debug_print("sts3x: max current is %0.2fmA.\n", info.max_current_ma);
        sts3x_interface_debug_print("sts3x: max temperature is %0.1fC.\n", info.temperature_max);
        sts3x_interface_debug_print("sts3x: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    sts3x_interface_debug_print("sts3x: start read test.\n");
    
    /* set address pin */
    res = sts3x_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set addr pin failed.\n");
       
        return 1;
    }
    
    /* sts3x init */
    res = sts3x_init(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: init failed.\n");
       
        return 1;
    }
    
    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    
    /* disable heater */
    res = sts3x_set_heater(&gs_handle, STS3X_BOOL_FALSE);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set heater failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    sts3x_interface_debug_print("sts3x: continuous read.\n");
    
    /* set low repeatability */
    sts3x_interface_debug_print("sts3x: set low repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_LOW);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rate 0.5Hz */
    sts3x_interface_debug_print("sts3x: set rate 0.5Hz.\n");
    
    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_0P5HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);
    
        /* wait 2500 ms */
        sts3x_interface_delay_ms(2500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 1Hz */
    sts3x_interface_debug_print("sts3x: set rate 1Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_1HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1500 ms */
        sts3x_interface_delay_ms(1500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 2Hz */
    sts3x_interface_debug_print("sts3x: set rate 2Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_2HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 4Hz */
    sts3x_interface_debug_print("sts3x: set rate 4Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_4HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 10Hz */
    sts3x_interface_debug_print("sts3x: set rate 10Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_10HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set medium repeatability */
    sts3x_interface_debug_print("sts3x: set medium repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_MEDIUM);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rate 0.5Hz */
    sts3x_interface_debug_print("sts3x: set rate 0.5Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_0P5HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 2500 ms */
        sts3x_interface_delay_ms(2500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 1Hz */
    sts3x_interface_debug_print("sts3x: set rate 1Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_1HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1500 ms */
        sts3x_interface_delay_ms(1500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 2Hz */
    sts3x_interface_debug_print("sts3x: set rate 2Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_2HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 4Hz */
    sts3x_interface_debug_print("sts3x: set rate 4Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_4HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 10Hz */
    sts3x_interface_debug_print("sts3x: set rate 10Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_10HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set high repeatability */
    sts3x_interface_debug_print("sts3x: set high repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_HIGH);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rate 0.5Hz */
    sts3x_interface_debug_print("sts3x: set rate 0.5Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_0P5HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 2500 ms */
        sts3x_interface_delay_ms(2500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 1Hz */
    sts3x_interface_debug_print("sts3x: set rate 1Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_1HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1500 ms */
        sts3x_interface_delay_ms(1500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 2Hz */
    sts3x_interface_debug_print("sts3x: set rate 2Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_2HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 4Hz */
    sts3x_interface_debug_print("sts3x: set rate 4Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_4HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* set rate 10Hz */
    sts3x_interface_debug_print("sts3x: set rate 10Hz.\n");

    /* start continuous read */
    res = sts3x_start_continuous_read(&gs_handle, STS3X_RATE_10HZ);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: start continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_continuous_read(&gs_handle, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 500 ms */
        sts3x_interface_delay_ms(500);
    }

    /* stop continuous read */
    res = sts3x_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: stop continuous failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }

    /* wait 10 ms */
    sts3x_interface_delay_ms(10);

    /* link iic read address16 with scl */
    DRIVER_STS3X_LINK_IIC_READ_ADDRESS16(&gs_handle, sts3x_interface_iic_scl_read_address16);
    sts3x_interface_debug_print("sts3x: single read.\n");

    /* set low repeatability */
    sts3x_interface_debug_print("sts3x: set low repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_LOW);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_single_read(&gs_handle, STS3X_BOOL_TRUE, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* set medium repeatability */
    sts3x_interface_debug_print("sts3x: set medium repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_MEDIUM);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_single_read(&gs_handle, STS3X_BOOL_TRUE, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* set high repeatability */
    sts3x_interface_debug_print("sts3x: set high repeatability.\n");
    res = sts3x_set_repeatability(&gs_handle, STS3X_REPEATABILITY_HIGH);
    if (res != 0)
    {
        sts3x_interface_debug_print("sts3x: set repeatability failed.\n");
        (void)sts3x_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_single_read(&gs_handle, STS3X_BOOL_TRUE, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* disable clock stretching */
    sts3x_interface_debug_print("sts3x: disable clock stretching.\n");
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        
        res = sts3x_single_read(&gs_handle, STS3X_BOOL_FALSE, (uint16_t *)&temperature_raw, (float *)&temperature_s);
        if (res != 0)
        {
            sts3x_interface_debug_print("sts3x: continuous read failed.\n");
            (void)sts3x_deinit(&gs_handle);
            
            return 1;
        }
        sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature_s);

        /* wait 1000 ms */
        sts3x_interface_delay_ms(1000);
    }

    /* finish read test */
    sts3x_interface_debug_print("sts3x: finish read test.\n");
    (void)sts3x_deinit(&gs_handle);

    return 0;
}
