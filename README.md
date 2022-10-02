# VideoTrigger
My oscilloscope wouldn't trigger relibly on the video signal from my TMS9128 VDP, so I made this to help me work on my VDPtoTFT project. 

# Why?
Builders (and possessors) of retro-computers have many options for generating video. One of the chips used in computers in the early 80's was the TMS9918 family of Video Display Processors.

Because I had a semi-rational excuse for wanting to make my chip drive a small TFT display (the rabbit-hole starts [here](https://github.com/ukmaker/VDPtoTFT)), I needed to be able to view the video outputs on my oscilloscope. My 'scope has a "Video Trigger" mode, which I gleefully enabled in anticipation of a nice, rock-solid display. No dice. No triggering of any sort was happening. This turns out not to be (totally) the fault of my 'scope - a Hantek DSO1502P. I think this is because the output from the chip isn't really a conformant video signal. I'm not entirely sure why, unless the scope expects "normal" voltage levels which take quite some work to produce from what the chip outputs.

So, in order to view the signals properly, I needed an external trigger signal. Hence this project. It uses the well known LM1881 sync separator IC to derive horizontal and vertical sync from the chip's "Y" output. These in turn feed an Atemga328P - programmed using the Arduino framework - which allows user selection of 
  - which edge of hsync to trigger off
  - which frame to trigger off
These are selectable by pushing and turning a rotary encoder, and the setting is display on a small OLED display.

Here is the circuit built on a bit of protoboard. It is powered from the scope's USB port. Input signal is on the left, trigger output is on the right.
[The Video Trigger](./images/VideoTrigger.JPG)

This picture shows the video signal and the trigger signal. Note that the trigger output from my circuit is the falling edge of the blue signal - the rising edge occurs an arbitrary time after the horizontal trigger signal, and is a very slow edge.
[Triggering on the falling edge](./images/NegativeTrigger.png)

And in this image you can see the slow rising edge of the trigger signal
[Triggering on the rising edge](./images/PositiveTrigger.png)

# Is It Any Good?
Well, it's good enough for me :-)

The input has no buffering, so the impedance is a bit low and interferes with the input signal. I'll add a buffer of some sort when I get some time. This migh be a bit tricky due to the output levels of the VDP.