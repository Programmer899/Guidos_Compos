# Guidos_Compos
### - What is this project
My idea for this project is a little by the time I had left, meaning that it could not be too complex.
Therefore I ended up choosing a compass because I were interested in knowing how the magnetometer works.

### - What purpose does it have
We all know that you do not want to get stuck somewhere with a map but lost the compass along the way.
Therefore I have made this simple compass that simply points to north.

In addition to be a helping hand when reading the map, it is a sort of project that gives you perspective and learn stuff along the way.
Some of this is done in educational purpose.

### - Description:
It is a simple compas using a MMC5983MA-Magnetometer to detirmine the direction of the earth's magnetic field.
To read the values my "Guidos Compos" is using a bare minimum "ATtiny45-20SU SO-8" as it's cpu.
As for the power source, I am inputing power from a 3.7V 500mAh battery that is beeing lowered down to 3.3V for the "ATtiny" and the "Magnetometer". The leds are then powered directly from the battery and beeing controlled by the Attiny.
At some places there are also some capacitors placed out to take in fact of the small noise in the power, which can be an outcome from the battery. What the meaning of the capacitor is to balance out the noice giving you: noise -> mostly clear.

### - Problems along the way:
I have had a few problems along the way, as an example the wiring. I had to redo the entire wiring after noticing that my led lights were rotated incorrectly.
I have also had a little to much going on during this PCB week on [Half Life](https://halflife.hackclub.com/) meaning that I have not had much time to work on my project.
Though, in the end I got something to hopefully work.
### - Firmware:
[Firmware - Built](https://github.com/Programmer899/Guidos_Compos/blob/main/firmware.hex)
---
## Schematic:
![Schematic](Schematic.png)
## PCB
![PCB](PCB.png)
## 3D View
![Rendered 3D Biew](Rendered.png)

### Build Of Materials
**#** | **Item**                 | **Qty** | **Supplier** | **Cost/item** | **Total Cost** | **link**                                                                                                                     
-------|--------------------------|---------|--------------|---------------|----------------|------------------------------------------------------------------------------------------------------------------------------
 **1** | Voltage Regulator 3\.3V  | 1       | Electrokit   | $0\.13        | $0\.13         | https://www\.electrokit\.com/en/l7833\-sot\-223\-voltage\-regulator\-3\.3v\-1a                                               
 **2** | Attiny45                 | 1       | Electrokit   | $2\.90        | $2\.90         | https://www\.electrokit\.com/en/attiny45\-20su\-so\-8\-8\-bit\-mcu\-flash\-4k                                                
 **3** | Battery charger          | 1       | Electrokit   | $5\.50        | $5\.50         | https://www\.electrokit\.com/en/tp4056\-batteriladdare\-lipo\-microusb                                                       
 **4** | SK6812\-E                | 25      | Amazon       | $0\.04        | $1\.00         | https://www\.amazon\.se/gp/product/B0GGJFR5QX/ref=sw\_img\_1?smid=A2WIE6SW95T3HY&psc=1                                       
 **5** | Battery                  | 1       | Conrad       | $8\.90        | $8\.90         | https://www\.conrad\.se/sv/p/amewi\-rc\-batteripack\-lipo\-3\-7\-v\-500\-mah\-antal\-celler\-1\-softcase\-jst\-3436344\.html 
 **6** | MMC5983MA                | 1       | Digikey      | $3\.32        | $3\.32         | https://www\.digikey\.com/en/products/detail/memsic\-inc/MMC5983MA/10452795                                                  
 **TOTAL** | Estimated total shipping |         |              |               | $8\.23         |
 
 **NOTE:**
 The shipping is not fully accurate due to me having insuffisient time for the project - Time = money.  
 Meaning that the **shipping** is for Electrokit and Amazon. Conrad and Digikey are outside the money-range.
