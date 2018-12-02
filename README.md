# Bin Laden in a box
An old project from 2002 - It's a tamagotchi, only this time the creature is bin laden.<br/>

The code is so bad, but the nostalgia was too much, I had to share this :)

<center>
<img src="__2018__/screenshot.png" alt="screenshot">
</center>

## Instructions

Step-by-step... Because it's annoying to Google stuff.

### Preperation
1. `mkdir ~/dosbox && cd "$_"`
2. `git clone https://github.com/thedp/laden_in_box.git`
3. Download [Turbo C 2.01](http://cc.embarcadero.com/item/25636) to `~/dosbox/`
4. Extract TC Disks to a single directory: `mkdir tc201/full ; cp -R tc201/Disk*/ tc201/full/`
5. Install [DOSBox](https://www.dosbox.com)

### Inside DOSBox
1. Open DOSBox, and execute `mount c ~/dosbox`, then `c:`
2. Execute `TC201\FULL\INSTALL.EXE`
3. Run `TC\TC.EXE`
4. Change `Options -> Model` from `Small` to `Large`
5. `File -> Load` the file `LADEN\LADEN.C`
6. `Compile -> Make EXE file`
7. ?
8. Profit

## Troubleshooting
- Making in-game mouse to work on MacOS: Use `fn + Ctrl + F10` to switch mouse to DOSBox, and back.

- If the reaction is laggy, [increase DOSBox memory](https://www.dosbox.com/wiki/DOSBox_FAQ#Increasing_memory_size).
