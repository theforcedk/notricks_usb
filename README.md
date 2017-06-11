# notricks_usb
Pentest USB

Order the boards here http://dirtypcbs.com/store/designer/details/8489/1041/atmega32u4-development-board or upload to OSH Park

Download the Sparkfun bootloader (Caterina-promicro16.hex) located at https://github.com/dudanov/sparkfun-promicro-caterina-bootloader/tree/e15665e847b4d9003d69fccb293572aa5e1aae71

```
Burn fuses for external crystal (0 devision)
*Extended*: 0xcb
*High*: 0xd8
*Low*: 0xff
```

```
avrdude -p atmega32u4 -c atmelice_isp -U flash:w:Caterina-promicro16.hex -U efuse:w:0xcb:m -U hfuse:w:0xd8:m -U lfuse:w:0xff:m
```

Open the schematic in EAGLE for actual package sizes, but in general: resistors = 0603, capacitors = 0805

```
1x Atmega32u4 IC
2x 22R resistor
2x 1K resistor
1x 10K resistor
7x 100nF capacitor
1x 1uF capacitor
1x 10uF capacitor
2x LEDs
1x 16MHz crystal
2x 22pF capacitor
1x 2x3 header
Optional: 2x SMD jumper for HWB+VCC
```
