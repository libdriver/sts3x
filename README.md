[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver STS3X

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sts3x/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The STS3x-DIS is Sensirion’s new high accuracy digital temperature sensor. It relies on the industry proven CMOSens® technology, providing for increased intelligence, reliability and improved accuracy specifications compared to its predecessors. Its functionality includes enhanced signal processing, two distinctive and user selectable I2C addresses and communication speeds of up to 1 MHz. The DFN package has a footprint of 2.5 x 2.5 mm2 while keeping a height of 0.9 mm. This allows for integration of the STS3x-DIS into a great variety of applications.Additionally, the wide supply voltage range of 2.15 V to 5.5 V guarantees compatibility with a wide range of applications. All in all, the STS3x-DIS incorporates more than 15 years of Sensirion’s digital sensor know-how.

LibDriver STS3X is the full function driver of STS3X launched by LibDriver.It provides functions of temperature continuous mode reading and temperature single mode reading. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver STS3X source files.

/interface includes LibDriver STS3X IIC platform independent template.

/test includes LibDriver STS3X driver test code and this code can test the chip necessary function simply.

/example includes LibDriver STS3X sample code.

/doc includes LibDriver STS3X offline document.

/datasheet includes STS3X datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_sts3x_basic.h"

uint8_t res;
uint8_t i;
float temperature;

res = sts3x_basic_init(STS3X_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sts3x_interface_delay_ms(1000);
    res = sts3x_basic_read((float *)&temperature);
    if (res != 0)
    {
        (void)sts3x_basic_deinit();

        return 1;
    }
    sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)sts3x_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_sts3x_shot.h"

uint8_t res;
uint8_t i;
float temperature;

res = sts3x_shot_init(STS3X_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    sts3x_interface_delay_ms(1000);
    res = sts3x_shot_read((float *)&temperature);
    if (res != 0)
    {
        (void)sts3x_shot_deinit();

        return 1;
    }
    sts3x_interface_debug_print("sts3x: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)sts3x_shot_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/sts3x/index.html](https://www.libdriver.com/docs/sts3x/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.