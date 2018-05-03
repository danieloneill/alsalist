# alsalist
Very basic tool to scan ALSA sequencer devices and list clients/sources in an easily parseable way.

I use this (instead of a BASH mess of "aconnect -l" and a pile of pipes) to automate connecting my DTX400k kit USB MIDI data to the output of a USB MIDI adapter.

## Build
You'll need make, gcc, libasound2-dev, and pkg-config

```
$ cd alsalist
$ make
```

## Usage
I mean... you just run it, and it lists your alsa sequencer clients/ports in a line-by-line fashion.

The lines are in a "&lt;client id&gt;:&lt;source id&gt; &lt;client name&gt; : &lt;source name&gt;" format.

```
$ ./alsalist
0:0 System : Timer
0:1 System : Announce
14:0 Midi Through : Midi Through Port-0
$
```

## Example
You could do fun things with the help of a few basic Unix utilites and udev to automate connections. I do it by first creating a udev rules file to trigger my script to run whenever one of two devices are plugged in. (Well, also when they're unplugged, but whatever):


### /etc/udev/rules.d/10-midi.rules
```
# The DTX USB device stuff:
ATTRS{idVendor}=="0499", ATTRS{idProduct}=="7000", RUN="/usr/local/bin/midi.sh"

# The USB cable stuff:
ATTRS{idVendor}=="15ca", ATTRS{idProduct}=="1806", RUN="/usr/local/bin/midi.sh"
```

The udev rules have that "RUN" thing that makes it try to run a script. Here's the script:

### /usr/local/bin/midi.sh 
```
#!/bin/bash

aconnect -x

SOURCE=`/home/pi/alsalist/alsalist | grep DTX | awk '{print $1;}'`
DEST=`/home/pi/alsalist/alsalist | grep "MIDI Interface cable" | awk '{print $1;}'`

aconnect $SOURCE $DEST
```

In the above, the sequencer connections are flushed with "aconnect -x", then alsalist is called to fish out the "client:source" IDs of the appropriate things. In this case, my DTX input is being piped to my USB MIDI adapter's output.

You can find the idVendor and idProduct values for your hardware by running "lsusb" in a terminal, and you need to change what "grep" looks for to suit your device names (as seen in the output of alsalist when you run it by itself).

The script and udev rules are very basic and contain no error checking, but any errors that could happen would be harmless and when the conditions are appropriate (both devices connected) it should work as expected nonetheless.
