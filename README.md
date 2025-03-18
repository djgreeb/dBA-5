# dBA 5 
 Digital Rotary mixer dBA 5

The dBA 5 is a 2-channel digital rotary DJ mixer. It is based on the body and knobs of an old 1957 meter.
The mixer is based on the DevBox STM32H743 Ver2.0 board, 2 PCM1808 ADCs and 2 CS4344 DACs. The sampling frequency is 48 kHz.
Each of the input channels can operate in line in or Phono mode; 2 phono correctors are installed.

There is a Trim control on the back panel. But the volume level is controlled using the LEVEL knobs during the mix. These knobs have a zero position not in the extreme right position. The listening mode is designed as a switch. Allows you to listen to channel 1, channel 1 + master, master, channel 2 + master, channel 2.

Each channel has a filter knob. In the middle position, the filter is not active. To the left is the low-pass filter, to the right is the high-pass filter. Filter operation is indicated by LEDs, the color of which matches the color of the backlight of the pointer indicators.

The blinking speed of the indicator reflects the depth of the filter. Listening in headphones allows you to listen to the channel before the filter, even if the filter is activated.
The level meters display the level of the selected listening channel.

https://youtube.com/shorts/7Eyr_4_-lbI?si=8GjPAUgvpXiqGfXN

https://youtube.com/shorts/V9p8OeJIuLw?si=TA9aBOhravzZ_spN

https://youtube.com/shorts/Ao05V8m91X4?si=t5FqrDS9XYP-LJhe

The connection diagram of the DAC, ADC and peripherals can be determined from the ROTARY_MIXER.pdf document, according to the corresponding names of the microcontroller signals. The diagram of the analog part is not yet documented, it may be published later or any other analog mixer circuitry may be used.

Some photos:


![IMG_5485](https://github.com/user-attachments/assets/c0187300-9487-4375-b4b3-9160a46f238e)
![IMG_5484](https://github.com/user-attachments/assets/1cc52c5c-462f-43cc-a9ac-b00fe452af80)
![IMG_5483](https://github.com/user-attachments/assets/792e7a94-4ec1-4c08-9f8e-9f7dce05ae30)
![IMG_5299](https://github.com/user-attachments/assets/80cd1492-dd1c-4cbf-a584-017d0b3b4e11)

