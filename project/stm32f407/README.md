### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. STS3X

#### 3.1 Command Instruction

1. Show sts3x chip and driver information.

   ```shell
   sts3x (-i | --information)
   ```

2. Show sts3x help.

   ```shell
   sts3x (-h | --help)
   ```

3. Show sts3x pin connections of the current board.

   ```shell
   sts3x (-p | --port)
   ```

4. Run sts3x register test.

   ```shell
   sts3x (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run sts3x read test, num is test times.

   ```shell
   sts3x (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run sts3x read function, num is read times.

   ```shell
   sts3x (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

7. Run sts3x shot function, num is read times.

   ```shell
   sts3x (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
sts3x -i

sts3x: chip is Sensirion STS3X.
sts3x: manufacturer is Sensirion.
sts3x: interface is IIC.
sts3x: driver version is 1.0.
sts3x: min supply voltage is 2.4V.
sts3x: max supply voltage is 5.5V.
sts3x: max current is 1.50mA.
sts3x: max temperature is 125.0C.
sts3x: min temperature is -40.0C.
```

```shell
sts3x -p

sts3x: SCL connected to GPIOB PIN8.
sts3x: SDA connected to GPIOB PIN9.
```

```shell
sts3x -t reg --addr=0

sts3x: chip is Sensirion STS3X.
sts3x: manufacturer is Sensirion.
sts3x: interface is IIC.
sts3x: driver version is 1.0.
sts3x: min supply voltage is 2.4V.
sts3x: max supply voltage is 5.5V.
sts3x: max current is 1.50mA.
sts3x: max temperature is 125.0C.
sts3x: min temperature is -40.0C.
sts3x: start register test.
sts3x: sts3x_set_addr_pin/sts3x_get_addr_pin test.
sts3x: set address pin 0.
sts3x: check addr pin ok.
sts3x: set address pin 1.
sts3x: check addr pin ok.
sts3x: sts3x_set_repeatability/sts3x_get_repeatability test.
sts3x: set repeatability high.
sts3x: check repeatability pin ok.
sts3x: set repeatability medium.
sts3x: check repeatability pin ok.
sts3x: set repeatability low.
sts3x: check repeatability pin ok.
sts3x: set heater test.
sts3x: enable heater.
sts3x: check heater ok.
sts3x: disable heater.
sts3x: check heater ok.
sts3x: get status test.
sts3x: check status 0x8010.
sts3x: clear status test.
sts3x: check clear status ok.
sts3x: finish register test.
```

```shell
sts3x -t read --addr=0 --times=3

sts3x: chip is Sensirion STS3X.
sts3x: manufacturer is Sensirion.
sts3x: interface is IIC.
sts3x: driver version is 1.0.
sts3x: min supply voltage is 2.4V.
sts3x: max supply voltage is 5.5V.
sts3x: max current is 1.50mA.
sts3x: max temperature is 125.0C.
sts3x: min temperature is -40.0C.
sts3x: start read test.
sts3x: continuous read.
sts3x: set low repeatability.
sts3x: set rate 0.5Hz.
sts3x: temperature is 28.54C.
sts3x: temperature is 28.51C.
sts3x: temperature is 28.54C.
sts3x: set rate 1Hz.
sts3x: temperature is 28.54C.
sts3x: temperature is 28.45C.
sts3x: temperature is 28.61C.
sts3x: set rate 2Hz.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.55C.
sts3x: set rate 4Hz.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.54C.
sts3x: set rate 10Hz.
sts3x: temperature is 28.50C.
sts3x: temperature is 28.50C.
sts3x: temperature is 28.44C.
sts3x: set medium repeatability.
sts3x: set rate 0.5Hz.
sts3x: temperature is 28.51C.
sts3x: temperature is 28.50C.
sts3x: temperature is 28.47C.
sts3x: set rate 1Hz.
sts3x: temperature is 28.50C.
sts3x: temperature is 28.44C.
sts3x: temperature is 28.42C.
sts3x: set rate 2Hz.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.44C.
sts3x: temperature is 28.45C.
sts3x: set rate 4Hz.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.42C.
sts3x: set rate 10Hz.
sts3x: temperature is 28.45C.
sts3x: temperature is 28.44C.
sts3x: temperature is 28.47C.
sts3x: set high repeatability.
sts3x: set rate 0.5Hz.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.45C.
sts3x: temperature is 28.45C.
sts3x: set rate 1Hz.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.45C.
sts3x: temperature is 28.41C.
sts3x: set rate 2Hz.
sts3x: temperature is 28.40C.
sts3x: temperature is 28.44C.
sts3x: temperature is 28.42C.
sts3x: set rate 4Hz.
sts3x: temperature is 28.52C.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.44C.
sts3x: set rate 10Hz.
sts3x: temperature is 28.45C.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.44C.
sts3x: single read.
sts3x: set low repeatability.
sts3x: temperature is 28.47C.
sts3x: temperature is 28.37C.
sts3x: temperature is 28.42C.
sts3x: set medium repeatability.
sts3x: temperature is 28.41C.
sts3x: temperature is 28.40C.
sts3x: temperature is 28.37C.
sts3x: set high repeatability.
sts3x: temperature is 28.42C.
sts3x: temperature is 28.41C.
sts3x: temperature is 28.40C.
sts3x: disable clock stretching.
sts3x: temperature is 28.35C.
sts3x: temperature is 28.37C.
sts3x: temperature is 28.37C.
sts3x: finish read test.
```

```shell
sts3x -e read --addr=0 --times=3

sts3x: 1/3.
sts3x: temperature is 28.31C.
sts3x: 2/3.
sts3x: temperature is 28.33C.
sts3x: 3/3.
sts3x: temperature is 28.33C.
```

```shell
sts3x -e shot --addr=0 --times=3

sts3x: 1/3.
sts3x: temperature is 28.31C.
sts3x: 2/3.
sts3x: temperature is 28.33C.
sts3x: 3/3.
sts3x: temperature is 28.31C.
```

```shell
sts3x -h

Usage:
  sts3x (-i | --information)
  sts3x (-h | --help)
  sts3x (-p | --port)
  sts3x (-t reg | --test=reg) [--addr=<0 | 1>]
  sts3x (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  sts3x (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  sts3x (-e shot | --example=shot) [--addr=<0 | 1>] [--times=<num>]

Options:
      --addr=<0 | 1>    Set the addr pin.([default: 0])
  -e <read | shot>, --example=<read | shot>
                        Run the driver example.
  -h, --help            Show the help.
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                        Run the driver test.
      --times=<num>     Set the running times.([default: 3])
```

