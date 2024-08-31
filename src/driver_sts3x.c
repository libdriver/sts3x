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
 * @file      driver_sts3x.c
 * @brief     driver sts3x source file
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

#include "driver_sts3x.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Sensirion STS3X"        /**< chip name */
#define MANUFACTURER_NAME         "Sensirion"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.4f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                     /**< chip max supply voltage */
#define MAX_CURRENT               1.5f                     /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip command definition
 */
#define STS3X_COMMAND_FETCH_DATA            0xE000U        /**< fetch data command */
#define STS3X_COMMAND_BREAK                 0x3093U        /**< break command */
#define STS3X_COMMAND_SOFT_RESET            0x30A2U        /**< soft reset command */
#define STS3X_COMMAND_HEATER_ENABLE         0x306DU        /**< heater enable command */
#define STS3X_COMMAND_HEATER_DISABLE        0x3066U        /**< heater disable command */
#define STS3X_COMMAND_READ_STATUS           0xF32DU        /**< read status command */
#define STS3X_COMMAND_CLEAR_STATUS          0x3041U        /**< clear status command */

/**
 * @brief     write the command
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] cmd is the write command
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_sts3x_write(sts3x_handle_t *handle, uint16_t cmd)
{
    if (handle->iic_write_address16(handle->iic_addr, cmd, NULL, 0) != 0)        /* iic write */
    {
        return 1;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an sts3x handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_sts3x_read(sts3x_handle_t *handle, uint16_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_read_address16(handle->iic_addr, reg, data, len) != 0)        /* iic read */
    {
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief     calculate the crc
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    crc
 * @note      none
 */
static uint8_t a_sts3x_crc(uint8_t *data, uint16_t len)
{
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i, j;
  
    for (j = len; j != 0; --j)                                              /* length-- */
    {
        crc ^= *data++;                                                     /* xor */
        for (i = 8; i != 0; --i)                                            /* 8 times */
        {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc<<1);        /* calculate crc */
        }
    }
  
    return crc;                                                             /* return crc */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an sts3x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t sts3x_init(sts3x_handle_t *handle)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                            /* check iic_init */
    {
        handle->debug_print("sts3x: iic_init is null.\n");                   /* iic_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                          /* check iic_deinit */
    {
        handle->debug_print("sts3x: iic_deinit is null.\n");                 /* iic_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_read_address16 == NULL)                                  /* check iic_read_address16 */
    {
        handle->debug_print("sts3x: iic_read_address16 is null.\n");         /* iic_read_address16 is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->iic_write_address16 == NULL)                                 /* check iic_write_address16 */
    {
        handle->debug_print("sts3x: iic_write_address16 is null.\n");        /* iic_write_address16 is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("sts3x: delay_ms is null.\n");                   /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                             /* iic init */
    {
        handle->debug_print("sts3x: iic init failed.\n");                    /* iic init failed */
       
        return 1;                                                            /* return error */
    }
    command = STS3X_COMMAND_SOFT_RESET;                                      /* set command */
    res = a_sts3x_write(handle, command);                                    /* write command */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");               /* write command failed */
        (void)handle->iic_deinit();                                          /* close iic */
        
        return 1;                                                            /* return error */
    }
    handle->delay_ms(100);                                                   /* delay 100ms */
    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an sts3x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_deinit(sts3x_handle_t *handle)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    command = STS3X_COMMAND_BREAK;                                    /* set command */
    res = a_sts3x_write(handle, command);                             /* write command */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");        /* write command failed */
        
        return 1;                                                     /* return error */
    }
    if (handle->iic_deinit() != 0)                                    /* iic deinit */
    {
        handle->debug_print("sts3x: iic deinit failed.\n");           /* iic deinit failed */
       
        return 1;                                                     /* return error */
    }
    handle->inited = 0;                                               /* flag close */
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] addr_pin is the chip iic address pin
 * @return    status code
 *            - 0 success
 *            - 1 set addr pin failed
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sts3x_set_addr_pin(sts3x_handle_t *handle, sts3x_address_t addr_pin) 
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set address pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an sts3x handle structure
 * @param[out] *addr_pin points to a chip iic address pin buffer
 * @return      status code
 *              - 0 success
 *              - 1 get addr pin failed
 *              - 2 handle is NULL
 * @note        none
 */
uint8_t sts3x_get_addr_pin(sts3x_handle_t *handle, sts3x_address_t *addr_pin) 
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    
    *addr_pin = (sts3x_address_t)(handle->iic_addr);        /* get address pin */
    
    return 0;                                               /* success return 0 */
}

/**
 * @brief      get the current status
 * @param[in]  *handle points to an sts3x handle structure
 * @param[out] *status points to a status buffer
 * @return      status code
 *              - 0 success
 *              - 1 get status failed
 *              - 2 handle is NULL
 * @note        none
 */
uint8_t sts3x_get_status(sts3x_handle_t *handle, uint16_t *status) 
{
    uint8_t res;
    uint8_t data[3];
    uint16_t command;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    memset(data, 0, sizeof(uint8_t) * 3);                                  /* clear the buffer */
    command = STS3X_COMMAND_READ_STATUS;                                   /* set command */
    res = a_sts3x_read(handle, command, (uint8_t *)data, 3);               /* read status */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("sts3x: read status failed.\n");               /* read status failed */
       
        return 1;                                                          /* return error */
    }
    if (a_sts3x_crc((uint8_t *)data, 2) == data[2])                        /* check crc */
    {
        *status = (uint16_t)((((uint16_t)data[0]) << 8) | data[1]);        /* get status */
        
        return 0;                                                          /* success return 0 */
    }
    else
    {
        handle->debug_print("sts3x: crc check failed.\n");                 /* crc check failed */
       
        return 1;                                                          /* return error */
    }
}

/**
 * @brief     clear the current status
 * @param[in] *handle points to an sts3x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear status failed
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sts3x_clear_status(sts3x_handle_t *handle)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    command = STS3X_COMMAND_CLEAR_STATUS;                             /* set command */
    res = a_sts3x_write(handle, command);                             /* write command */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");        /* write command failed */
           
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     set the measurement repeatability
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] repeatability is the measurement repeatability
 * @return    status code
 *            - 0 success
 *            - 1 set repeatability failed
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t sts3x_set_repeatability(sts3x_handle_t *handle, sts3x_repeatability_t repeatability) 
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    if (handle->inited != 1)                               /* check handle initialization */
    {
        return 3;                                          /* return error */
    }
    
    handle->repeatability = (uint8_t)repeatability;        /* set repeatability */
    
    return 0;                                              /* success return 0 */
}

/**
 * @brief      get the measurement repeatability
 * @param[in]  *handle points to an sts3x handle structure
 * @param[out] *repeatability points to a measurement repeatability buffer
 * @return     status code
 *             - 0 success
 *             - 1 get repeatability failed
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sts3x_get_repeatability(sts3x_handle_t *handle, sts3x_repeatability_t *repeatability) 
{
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    *repeatability = (sts3x_repeatability_t)(handle->repeatability);        /* get repeatability */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief      read data once
 * @param[in]  *handle points to an sts3x handle structure
 * @param[in]  clock_stretching_enable is a clock stretching bool value
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sts3x_single_read(sts3x_handle_t *handle, sts3x_bool_t clock_stretching_enable, 
                          uint16_t *temperature_raw, float *temperature_s)
{
    uint8_t res;
    uint16_t command;
    uint8_t data[3];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    if (handle->repeatability == STS3X_REPEATABILITY_HIGH)                         /* repeatability high */
    {
        if (clock_stretching_enable == STS3X_BOOL_FALSE)                           /* if disable clock stretching */
        {
            command = 0x2400U;                                                     /* set disable high */
        }
        else if (clock_stretching_enable == STS3X_BOOL_TRUE)                       /* if enable clock stretching */
        {
            command = 0x2C06U;                                                     /* set enable high */
        }
        else
        {
            handle->debug_print("sts3x: clock stretching is invalid.\n");          /* clock stretching is invalid */
           
            return 1;                                                              /* return error */
        }
    }
    else if (handle->repeatability == STS3X_REPEATABILITY_MEDIUM)                  /* repeatability medium */
    {
        if (clock_stretching_enable == STS3X_BOOL_FALSE)                           /* if disable clock stretching */
        {
            command = 0x240BU;                                                     /* set disable medium */
        }
        else if (clock_stretching_enable == STS3X_BOOL_TRUE)                       /* if enable clock stretching */
        {
            command = 0x2C0DU;                                                     /* set enable medium */
        }
        else
        {
            handle->debug_print("sts3x: clock stretching is invalid.\n");          /* clock stretching is invalid */
           
            return 1;                                                              /* return error */
        }
    }
    else if (handle->repeatability == STS3X_REPEATABILITY_LOW)                     /* repeatability low */
    {
        if (clock_stretching_enable == STS3X_BOOL_FALSE)                           /* if disable clock stretching */
        {
            command = 0x2416U;                                                     /* set disable low */
        }
        else if (clock_stretching_enable == STS3X_BOOL_TRUE)                       /* if enable clock stretching */
        {
            command = 0x2C10U;                                                     /* set enable low */
        }
        else
        {
            handle->debug_print("sts3x: clock stretching is invalid.\n");          /* clock stretching is invalid */
           
            return 1;                                                              /* return error */
        }
    }
    else
    {
        handle->debug_print("sts3x: repeatability is invalid.\n");                 /* repeatability is invalid */
       
        return 1;                                                                  /* return error */
    }
    memset(data, 0, sizeof(uint8_t) * 3);                                          /* clear the buffer */
    res = a_sts3x_read(handle, command, (uint8_t *)data, 3);                       /* read data */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("sts3x: read data failed.\n");                         /* read data failed */
       
        return 1;                                                                  /* return error */
    }
    if (a_sts3x_crc((uint8_t *)data, 2) != data[2])                                /* check crc */
    {
        handle->debug_print("sts3x: crc check failed.\n");                         /* crc check failed */
       
        return 1;                                                                  /* return error */
    }
    *temperature_raw = (uint16_t)((((uint16_t)data[0]) << 8) | data[1]);           /* get raw temperature */
    *temperature_s = (float)(*temperature_raw) / 65535.0f * 175.0f - 45.0f;        /* convert raw temperature */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     start reading
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] rate is the sample rate
 * @return    status code
 *            - 0 success
 *            - 1 start continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_start_continuous_read(sts3x_handle_t *handle, sts3x_rate_t rate)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    if (handle->repeatability == STS3X_REPEATABILITY_HIGH)               /* repeatability high */
    {
        if (rate == STS3X_RATE_0P5HZ)                                    /* 0.5Hz */
        {
            command = 0x2032U;                                           /* set 0.5Hz high */
        }
        else if (rate == STS3X_RATE_1HZ)                                 /* 1Hz */
        {
            command = 0x2130U;                                           /* set 1Hz high */
        }
        else if (rate == STS3X_RATE_2HZ)                                 /* 2Hz */
        {
            command = 0x2236U;                                           /* set 2Hz high */
        }
        else if (rate == STS3X_RATE_4HZ)                                 /* 4Hz */
        {
            command = 0x2334U;                                           /* set 4Hz high */
        }
        else if (rate == STS3X_RATE_10HZ)                                /* 10Hz */
        {
            command = 0x2737U;                                           /* set 10Hz high */
        }
        else
        {
            handle->debug_print("sts3x: rate is invalid.\n");            /* rate is invalid */
           
            return 1;                                                    /* return error */
        }
    }
    else if (handle->repeatability == STS3X_REPEATABILITY_MEDIUM)        /* repeatability medium */
    {
        if (rate == STS3X_RATE_0P5HZ)                                    /* 0.5Hz */
        {
            command = 0x2024U;                                           /* set 0.5Hz medium */
        }
        else if (rate == STS3X_RATE_1HZ)                                 /* 1Hz */
        {
            command = 0x2126U;                                           /* set 1Hz medium */
        }
        else if (rate == STS3X_RATE_2HZ)                                 /* 2Hz */
        {
            command = 0x2220U;                                           /* set 2Hz medium */
        }
        else if (rate == STS3X_RATE_4HZ)                                 /* 4Hz */
        {
            command = 0x2322U;                                           /* set 4Hz medium */
        }
        else if (rate == STS3X_RATE_10HZ)                                /* 10Hz */
        {
            command = 0x2721U;                                           /* set 10Hz medium */
        }
        else
        {
            handle->debug_print("sts3x: rate is invalid.\n");            /* rate is invalid */
           
            return 1;                                                    /* return error */
        }
    }
    else if (handle->repeatability == STS3X_REPEATABILITY_LOW)           /* repeatability low */
    {
        if (rate == STS3X_RATE_0P5HZ)                                    /* 0.5Hz */
        {
            command = 0x202FU;                                           /* set 0.5Hz low */
        }
        else if (rate == STS3X_RATE_1HZ)                                 /* 1Hz */
        {
            command = 0x212DU;                                           /* set 1Hz low */
        }
        else if (rate == STS3X_RATE_2HZ)                                 /* 2Hz */
        {
            command = 0x222BU;                                           /* set 2Hz low */
        }
        else if (rate == STS3X_RATE_4HZ)                                 /* set 4Hz */
        {
            command = 0x2329U;                                           /* set 4Hz low */
        }
        else if (rate == STS3X_RATE_10HZ)                                /* 10Hz */
        {
            command = 0x272AU;                                           /* set 10Hz low */
        }
        else
        {
            handle->debug_print("sts3x: rate is invalid.\n");            /* rate is invalid */
           
            return 1;                                                    /* return error */
        }
    }
    else
    {
        handle->debug_print("sts3x: repeatability is invalid.\n");       /* repeatability is invalid */
       
        return 1;                                                        /* return error */
    }
    res = a_sts3x_write(handle, command);                                /* write command */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");           /* write command failed */
           
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     stop reading
 * @param[in] *handle points to an sts3x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_stop_continuous_read(sts3x_handle_t *handle)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    command = STS3X_COMMAND_BREAK;                                    /* set command */
    res = a_sts3x_write(handle, command);                             /* write command */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");        /* write command failed */
           
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief      read data continuously
 * @param[in]  *handle points to an sts3x handle structure
 * @param[out] *temperature_raw points to a raw temperature buffer
 * @param[out] *temperature_s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sts3x_continuous_read(sts3x_handle_t *handle, uint16_t *temperature_raw, float *temperature_s)
{
    uint8_t res;
    uint16_t command;
    uint8_t data[3];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    command = STS3X_COMMAND_FETCH_DATA;                                            /* set fetch data */
    res = a_sts3x_read(handle, command, (uint8_t *)data, 3);                       /* read data */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("sts3x: read data failed.\n");                         /* read data failed */
       
        return 1;                                                                  /* return error */
    }
    if (a_sts3x_crc((uint8_t *)data, 2) != data[2])                                /* check crc */
    {
        handle->debug_print("sts3x: crc check failed.\n");                         /* crc check failed */
       
        return 1;                                                                  /* return error */
    }
    *temperature_raw = (uint16_t)((((uint16_t)data[0]) << 8) | data[1]);           /* get raw temperature */
    *temperature_s = (float)(*temperature_raw) / 65535.0f * 175.0f - 45.0f;        /* convert raw temperature */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     soft reset the chip
 * @param[in] *handle points to an sts3x handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_soft_reset(sts3x_handle_t *handle)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    command = STS3X_COMMAND_SOFT_RESET;                               /* set command */
    res = a_sts3x_write(handle, command);                             /* write command */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");        /* write command failed */
           
        return 1;                                                     /* return error */
    }
    handle->delay_ms(100);                                            /* delay 100ms */
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     enable or disable the chip heater
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set heater failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_set_heater(sts3x_handle_t *handle, sts3x_bool_t enable)
{
    uint8_t res;
    uint16_t command;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (enable == STS3X_BOOL_TRUE)                                    /* enable heater */
    {
        command = STS3X_COMMAND_HEATER_ENABLE;                        /* set enable */
    }
    else if (enable == STS3X_BOOL_FALSE)                              /* disable heater */
    {
        command = STS3X_COMMAND_HEATER_DISABLE;                       /* set disable */
    }
    else
    {
        handle->debug_print("sts3x: bool is invalid.\n");             /* bool is invalid */
           
        return 1;                                                     /* return error */
    }
    res = a_sts3x_write(handle, command);                             /* write command */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("sts3x: write command failed.\n");        /* write command failed */
           
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to an sts3x handle structure
 * @param[in] command is the chip command
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t sts3x_set_reg(sts3x_handle_t *handle, uint16_t command)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->inited != 1)                     /* check handle initialization */
    {
        return 3;                                /* return error */
    }
    
    return a_sts3x_write(handle, command);       /* write command */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an sts3x handle structure
 * @param[in]  command is the chip command
 * @param[out] *buf points to data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t sts3x_get_reg(sts3x_handle_t *handle, uint16_t command, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }
    
    return a_sts3x_read(handle, command, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an sts3x info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t sts3x_info(sts3x_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(sts3x_info_t));                          /* initialize sts3x info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
