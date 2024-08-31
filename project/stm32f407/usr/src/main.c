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
 * @file      main.c
 * @brief     main source file
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

#include "driver_sts3x_register_test.h"
#include "driver_sts3x_read_test.h"
#include "driver_sts3x_basic.h"
#include "driver_sts3x_shot.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     sts3x full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t sts3x(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"times", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    sts3x_address_t addr = STS3X_ADDRESS_0;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = STS3X_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = STS3X_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 2 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (sts3x_register_test(addr) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (sts3x_read_test(addr, times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        
        /* basic init */
        res = sts3x_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sts3x_interface_delay_ms(1000);
            
            /* read data */
            res = sts3x_basic_read((float *)&temperature);
            if (res != 0)
            {
                (void)sts3x_basic_deinit();
                
                return 1;
            }
            
            /* output */
            sts3x_interface_debug_print("sts3x: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
        }
        
        /* basic deinit */
        (void)sts3x_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        
        /* shot init */
        res = sts3x_shot_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sts3x_interface_delay_ms(1000);
            
            /* read data */
            res = sts3x_shot_read((float *)&temperature);
            if (res != 0)
            {
                (void)sts3x_shot_deinit();
                
                return 1;
            }
            
            /* output */
            sts3x_interface_debug_print("sts3x: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
        }
        
        /* shot deinit */
        (void)sts3x_shot_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        sts3x_interface_debug_print("Usage:\n");
        sts3x_interface_debug_print("  sts3x (-i | --information)\n");
        sts3x_interface_debug_print("  sts3x (-h | --help)\n");
        sts3x_interface_debug_print("  sts3x (-p | --port)\n");
        sts3x_interface_debug_print("  sts3x (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        sts3x_interface_debug_print("  sts3x (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        sts3x_interface_debug_print("  sts3x (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        sts3x_interface_debug_print("  sts3x (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]\n");
        sts3x_interface_debug_print("\n");
        sts3x_interface_debug_print("Options:\n");
        sts3x_interface_debug_print("      --addr=<0 | 1>    Set the addr pin.([default: 0])\n");
        sts3x_interface_debug_print("  -e <read | shot>, --example=<read | shot>\n");
        sts3x_interface_debug_print("                        Run the driver example.\n");
        sts3x_interface_debug_print("  -h, --help            Show the help.\n");
        sts3x_interface_debug_print("  -i, --information     Show the chip information.\n");
        sts3x_interface_debug_print("  -p, --port            Display the pin connections of the current board.\n");
        sts3x_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        sts3x_interface_debug_print("                        Run the driver test.\n");
        sts3x_interface_debug_print("      --times=<num>     Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        sts3x_info_t info;
        
        /* print sts3x info */
        sts3x_info(&info);
        sts3x_interface_debug_print("sts3x: chip is %s.\n", info.chip_name);
        sts3x_interface_debug_print("sts3x: manufacturer is %s.\n", info.manufacturer_name);
        sts3x_interface_debug_print("sts3x: interface is %s.\n", info.interface);
        sts3x_interface_debug_print("sts3x: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sts3x_interface_debug_print("sts3x: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sts3x_interface_debug_print("sts3x: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sts3x_interface_debug_print("sts3x: max current is %0.2fmA.\n", info.max_current_ma);
        sts3x_interface_debug_print("sts3x: max temperature is %0.1fC.\n", info.temperature_max);
        sts3x_interface_debug_print("sts3x: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        sts3x_interface_debug_print("sts3x: SCL connected to GPIOB PIN8.\n");
        sts3x_interface_debug_print("sts3x: SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register sts3x function */
    shell_init();
    shell_register("sts3x", sts3x);
    uart_print("sts3x: welcome to libdriver sts3x.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("sts3x: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("sts3x: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("sts3x: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("sts3x: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("sts3x: param is invalid.\n");
            }
            else
            {
                uart_print("sts3x: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
