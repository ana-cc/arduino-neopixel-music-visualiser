This is a proof-of-concept music visualizer for an Arduino that has a neopixel shield.
Can be adapted for other type of individually addressable LED matrices.
To use:
1. ```apt install gstreamer1.0-pulseaudio libgstreamer1.0-dev python3-serial```
2. Modify the 'src' element in the C script to point to your soundcard output
3. Compile the script:
``` gcc `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gstreamer-1.0` pulse_freq.c -o pulse_freq.out ```
4. Upload the sketch to your Arduino Uno
5. Start the python script and play some music

