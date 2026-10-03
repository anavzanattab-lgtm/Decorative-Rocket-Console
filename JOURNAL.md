# Project Journal

## October 2: Switched to an LCD and got the RTC working!!

Today I made a pretty big change to the control panel project. I originally planned to use a 4-digit 7-segment display, but after wiring it up and testing it, I realized that it was going to be much more complicated than I wanted for this project.

So I decided to switch to the 16x2 LCD I had already used in previous Arduino projects.

First, I tested the LCD by itself. I wired it directly to the Arduino Uno and added a 10k potentiometer to control the contrast.

After uploading a simple test program, it actually worked!

The LCD displayed:

```text
HAIL MARY
CONTROL PANEL
```

After that, I connected the DS1302 real-time clock module.

I used three Arduino pins for the communication:

* CLK → D6
* DAT → D7
* RST → D8

The LCD was already using D2, D3, D4, D5, D11 and D12, so this arrangement allowed both modules to work without interfering with each other.

I then wrote a small test program that reads the time from the DS1302 and displays it on the LCD.

At first, something went wrong.

The display showed:

```text
00:63:XX
```

where the seconds were counting normally, but the minutes were completely invalid.

This was a sign that the RTC was not holding a valid time.

I fixed this by explicitly setting an initial date and time in the code. After doing that, the RTC started working correctly and the LCD displayed the time normally.

Now I have two important parts of the project working:

* The 16x2 LCD displays text correctly.
* The DS1302 provides a working real-time clock.
* The LCD continuously displays the current time.
* The Arduino Uno can communicate with both modules at the same time.

The original 4-digit display ended up being more complicated than necessary, especially because it required multiplexing and a lot more wiring. Switching to the LCD should make the rest of the project much easier to build and debug.

The next step is to make sure the RTC keeps its time after restarting the Arduino, and then start adding the three buttons and six LEDs for the 10, 20 and 30 second countdown functions.

**Time spent this session: ~2 hours**
