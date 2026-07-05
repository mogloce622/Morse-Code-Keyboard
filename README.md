# Morse-Code-Keyboard
## Hardware
Build this circuit on a piece of perf board or make your own pcb
![Schematic](Images/MorseCodeSchematic.png)
## Loading Code
#### Installing drivers for programming
To program attiny with micronucleus you will need to download Digistump.Drivers.zip from link:

https://github.com/digistump/digistumparduino/releases

After downloading un-zip it and run DPinst64.exe
#### Burning bootloader
To load a code on attiny85, first you need to burn micronucleus bootloader.
Add this to prefrences and in Boards Manager install ATTinyCore by Spence Konde:
```http
https://raw.githubusercontent.com/SpenceKonde/ReleaseScripts/refs/heads/master/package_drazzy.com_index.json
```
Now select ATtiny85(Micronucleus/DigiSPark) board and change Burn Bootloader Methode to Fresh Install(via ISP).
![BoardSetup](Images/BoardSetup.png)

Also you need to select your ISP programmer, for that you can use arduino as isp. Connect you arduino with attiny like on circuit below, but before that remove 10uf cap and upload Arduino as ISP sketch that you can find in examples.
![attiny85](Images/Attiny85withArduino.png)
#### Installing Digistump
Because now you have a micronucleus bootloader you can put attiny on your usb board. To use digistump libraries you need to instal DigiStump board for that add this in prefrences and install it in board manager:
```http
https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json
```
Now select Digispark(default - 16.5mhz) board, also chose micronucleus as bootloader and click upload. Don't plug in your usb board before you see you need to do that in output.
![BoardSetup2](Images/BoardSetup2.png)
## App
I also built this windows app that's uselles for learning morse code but it looks nice. I saw something like this on instagram so I wanted to build it.
[![Download ZIP](https://img.shields.io/badge/Download-ZIP-blue)](https://github.com/mogloce622/Morse-Code-Keyboard/raw/main/Morse%20Code.zip)
![App](Images/AppScreenshot.png)
