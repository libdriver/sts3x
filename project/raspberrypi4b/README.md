### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(sts3x REQUIRED)
```


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
./sts3x -i

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
./sts3x -p

sts3x: SCL connected to GPIO3(BCM).
sts3x: SDA connected to GPIO2(BCM).
```

```shell
./sts3x -t reg --addr=0

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
./sts3x -t read --addr=0 --times=3

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
sts3x: temperature is 30.27C.
sts3x: temperature is 30.22C.
sts3x: temperature is 30.20C.
sts3x: set rate 1Hz.
sts3x: temperature is 30.41C.
sts3x: temperature is 30.44C.
sts3x: temperature is 30.51C.
sts3x: set rate 2Hz.
sts3x: temperature is 30.46C.
sts3x: temperature is 30.41C.
sts3x: temperature is 30.43C.
sts3x: set rate 4Hz.
sts3x: temperature is 30.38C.
sts3x: temperature is 30.40C.
sts3x: temperature is 30.54C.
sts3x: set rate 10Hz.
sts3x: temperature is 30.54C.
sts3x: temperature is 30.53C.
sts3x: temperature is 30.57C.
sts3x: set medium repeatability.
sts3x: set rate 0.5Hz.
sts3x: temperature is 30.58C.
sts3x: temperature is 30.51C.
sts3x: temperature is 30.47C.
sts3x: set rate 1Hz.
sts3x: temperature is 30.51C.
sts3x: temperature is 30.54C.
sts3x: temperature is 30.68C.
sts3x: set rate 2Hz.
sts3x: temperature is 30.71C.
sts3x: temperature is 30.65C.
sts3x: temperature is 30.67C.
sts3x: set rate 4Hz.
sts3x: temperature is 30.63C.
sts3x: temperature is 30.58C.
sts3x: temperature is 30.60C.
sts3x: set rate 10Hz.
sts3x: temperature is 30.65C.
sts3x: temperature is 30.65C.
sts3x: temperature is 30.65C.
sts3x: set high repeatability.
sts3x: set rate 0.5Hz.
sts3x: temperature is 30.78C.
sts3x: temperature is 30.86C.
sts3x: temperature is 30.77C.
sts3x: set rate 1Hz.
sts3x: temperature is 30.64C.
sts3x: temperature is 30.70C.
sts3x: temperature is 30.67C.
sts3x: set rate 2Hz.
sts3x: temperature is 30.74C.
sts3x: temperature is 30.78C.
sts3x: temperature is 30.78C.
sts3x: set rate 4Hz.
sts3x: temperature is 30.71C.
sts3x: temperature is 30.72C.
sts3x: temperature is 30.71C.
sts3x: set rate 10Hz.
sts3x: temperature is 30.68C.
sts3x: temperature is 30.68C.
sts3x: temperature is 30.68C.
sts3x: single read.
sts3x: set low repeatability.
sts3x: temperature is 30.65C.
sts3x: temperature is 30.77C.
sts3x: temperature is 30.63C.
sts3x: set medium repeatability.
sts3x: temperature is 30.60C.
sts3x: temperature is 30.71C.
sts3x: temperature is 30.86C.
sts3x: set high repeatability.
sts3x: temperature is 30.86C.
sts3x: temperature is 30.81C.
sts3x: temperature is 30.78C.
sts3x: disable clock stretching.
sts3x: temperature is 30.74C.
sts3x: temperature is 30.75C.
sts3x: temperature is 30.74C.
sts3x: finish read test.
```

```shell
./sts3x -e read --addr=0 --times=3

sts3x: 1/3.
sts3x: temperature is 30.64C.
sts3x: 2/3.
sts3x: temperature is 30.70C.
sts3x: 3/3.
sts3x: temperature is 30.77C.
```

```shell
./sts3x -e shot --addr=0 --times=3

sts3x: 1/3.
sts3x: temperature is 30.91C.
sts3x: 2/3.
sts3x: temperature is 30.82C.
sts3x: 3/3.
sts3x: temperature is 30.81C.
```

```shell
./sts3x -h

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

