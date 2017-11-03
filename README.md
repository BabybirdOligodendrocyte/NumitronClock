# Arduino Numitron Clock
This is my first electronics project with or without an arduino. The Clock uses an Arduino Uno with TPIC6B595N shift registers in order to control the IV-9 Numitron tubes. This github includes the parts list, arduino code, gerber file, svg file and instructions for assembly. This clock does not use an RTC in order to cutdown on parts, but one could be easily added to the arduino. The only known problems are cosmetic and as follows:
1. The USB hole in the back of the wooden enclosure is smaller than most standard USB by a few mm.
2. The DHT11 needs to be soldered flushly in order for the PCB to sit flat on the box.

[![clock420.jpg](https://s1.postimg.org/3qeyrjngnz/clock420.jpg)](https://postimg.org/image/5zxzb1874r/)

Parts required in no particular order:

1. 8 x 20 pin IC sockets  ($3.50 for 50)
https://www.ebay.com/itm/50PCS-20-Pin-DIL-DIP-IC-Socket-PCB-Mount-Connector-NEW-GOOD-QUALITY/351492891851?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649

2. 4 x TPIC6B595N shift registers ($3.50 for 10)
https://www.ebay.com/itm/10PCS-TPIC6B595N-DIP-IC-TI-NEW-GOOD-QUALITY-Z5/291548967760?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649

3. 4 x IV-9 Numitron tubes ($15.00 for 6)
https://www.ebay.com/itm/IV-9-numitron-nixie-tubes-lot-of-6-tested/322791591551?hash=item4b27e0ce7f:g:haIAAOxyJX1TF4Wl

4. 2 x 7mm Tactile Push Buttons ($1.75 for 50)
https://www.ebay.com/itm/50pcs-6-6-7mm-Tactile-Push-Button-Switch-4pin-Tact-Switch-4P-DIP-for-Arduino/162212052435?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649

5. 1 x 10 pin female header for pcb ($1.14 for a giant strip)
https://www.ebay.com/itm/10PCS-40Pin-2-54mm-Single-Row-Straight-Female-Pin-Header-Strip-PBC-Arduino/201674778320?epid=1781023094&hash=item2ef4c0e6d0:g:n7kAAOSwOyJX5O~4

6. 1 x DHT11 temperature and humidity sensor ($1.11)
https://www.ebay.com/itm/1PCS-Arduino-DHT11-Temperature-and-Relative-Humidity-Sensor-Module-NEW/181846902071?epid=1279768341&hash=item2a56eb8537:g:uXsAAOSwzrxUu0El

7. 3 x 10k ohm resistor
8. 32 x 100 ohm resistors
9. 1 x .1uf capacitor

10. 1 x Clock PCB  ($27.00 for 10 plus shipping from seeed fusion)
https://www.seeedstudio.com/fusion_pcb.html
(use the gerber file attached)

11. 1 x Arduino Uno ($3.00-$5.00)

12. 1 x Wooden enclosure (Free from ponoko for first project + $7.99 shipping)
https://www.ponoko.com/
(use the svg file)

# In The Beginning

[![Clock_Image1.jpg](https://s1.postimg.org/1u2no1nyrz/Clock_Image1.jpg)](https://postimg.org/image/5dolduqojv/)

The clock was pretty easy to get working after some trial and error. Arduino pins 6, 8 and 10 are attached to the shift registers as shown in the image below. Then the drains are attached to the individual leads of the IV-9 tube. The image shows a 7 segment display being used, but the IV-9 is attached similarly with 100 ohm resistors except for the lead attached directly to power. The tubes are not soldered to the PCB and are instead inserted into the 20 pin ic sockets making it easy to replace them in the future if they burn out. You can also clip the leads down to whatever height you want before inserting them. 

[![TPIC6_B595_N_Circuit.png](https://s1.postimg.org/5qrh0dywrj/TPIC6_B595_N_Circuit.png)](https://postimg.org/image/2e8r60icez/)

Below is an image of the PCB board included in the gerber file. 

[![Updated_PCB.png](https://s1.postimg.org/1hziceinu7/Updated_PCB.png)](https://postimg.org/image/1w5y39qyp7/)

# Soldering the PCB

1) Solder all of the resistors (except for R47, R45 and R44) with 100 ohm resistors
2) Solder the 10k ohm resistors at R47, R45 and R44.
3) Solder the 8 IC sockets onto the board.
4) Solder the 10 pin female header on the opposite side of the board (so you don't have cables reach over the top later).
5) Solder the tactile buttons (B1, B2) and DHT11.
6. Solder the .1uf capacitor at C1.

Notice the orientation of the TPIC6B595N on the board before inserting into the IC socket
The lights are inserted into the IC sockets with the main cathode being connected to the front right socket. 

# Arduino Code

Once you have finished soldering you can upload the arduino code and connect the arduino to the female pin headers on the PCB. 

You should now have a working clock. In order to change the time hold B2 for 1 second and release to change hours. Simply press B2 to change minutes. Pressing B1 will bring up the temperature and humidity. 



