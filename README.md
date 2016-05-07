This is a repost of the early beta firmware for the glowdeck to try to get community help in improving the glowdeck
i did not write this initial code vut want to get it out to the public so we glowdeck users can have a funcioning product









Glowdeck Beta Firmware

======================
by Justin Kaufman | PLSCO, Inc.

This is the place where you can track and contribute to the latest source code for the Glowdeck Beta development (dev) board. If you are just joining us, here is what you should do to get your bearings:

2. Download/install the Glowduino IDE from <http://j.mp/glowdev>
3. Clone the Glowdeck_Beta source code repository to your computer
4. Read the Wiki for upload instructions*, build information, and issue log.

*Here are the basic steps for uploading code to your board: 

A: Your board must be in DFU (or bootloader) mode in order to upload code to it.

B: You will know your board is in DFU mode because when you plug in power, the display will be blank and the LEDs will all be red.

C: There are several ways to enter DFU mode, but the simplest method is to (1) completely unplug your board; (2) hold down the front button; and (3) plug in 12V power to your board. Then you can release the front button and you should be in DFU mode (if the lights are not all on in red, then try again).

D: Next, connect the board to your computer via the micro-USB jack in the back (next to the DC barrel jack). 

E: Never leave the micro-USB connected to your computer when your board is not plugged into your 12V power supply (i.e. unplug micro-USB before disconnecting power, and likewise, plug in power before connecting micro-USB).

F: Because the board implements an MSD bootloader, your computer will interpret that a new volume has been attached called Glowdeck V1 (i.e. just as if you connected a USB thumb drive). Advanced users can upload compiled .bin files directly to this virtual volume simply by dragging them (i.e. copying them) to the disk, but it is strongly recommended that you start by uploading your code from the Glowduino IDE.

G: Launch the Glowduino IDE, make sure the board selected is Glowdeck V1 (it should be selected by default).

H: Load the code from this repo - or one of the sample sketches included in the IDE program package - and click compile.

I: If the code successfully compiles (even if there are warnings along the way), then go ahead and click upload.

J: If you are a Windows user, your code will upload and your board should reboot, running your newly uploaded code.

K: If you are a Mac user, a Terminal window will launch. Simply enter your password and follow the instructions to complete the upload of your code (we are working on removing this step for Mac uploads, but for now, it is necessary to ensure that the hidden files OSX creates in new volumes are not added to the virtual Glowdeck instance). Then reset your board (either by pressing the reset switch or disconnecting and reconnecting power), and your code will run.


IMPORTANT NOTE: THIS CODE WILL NOT COMPILE OR UPLOAD PROPERLY IF YOU DO NOT HAVE THE MOST RECENT GLOWDUINO IDE APPLICATION PACKAGE INSTALLED.

Download the latest Glowduino IDE build here: <http://j.mp/glowdev>

