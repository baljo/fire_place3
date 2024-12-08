# Santa's Hot or Not?

Every year, Santa Claus embarks on his magical journey, delivering gifts to good boys and girls around the world. But have you ever wondered about the challenges he faces? Sure, Santa has the sleigh, the reindeer, and a bag full of gifts, but there's one hurdle he encounters every single time—**chimneys**!

Imagine Santa's concern as he peers down a chimney, only to find a roaring fire blazing below. Enter the **Santa Chimney Safety System**, a cutting-edge fusion of holiday cheer and modern technology. This project ensures that Santa can safely slide down the chimney without encountering any "fiery surprises."

![](/images/Hot_Unsafe.png)

## Story

It all started on a cold winter night when Santa's Chief Elf of Safety, Elfred, declared, "Santa, we need a solution! The reindeer are complaining about smoke, and you're tired of your boots getting singed." Thus, Santa turned to the best minds in tech (that’s you!) to craft a foolproof system for chimney inspections.

The Santa Safety Game was born—a whimsical yet practical solution to verify chimney conditions before Santa makes his descent. Combining the powers of a thermal camera, an OLED display, and a sprinkle of holiday magic, this system determines whether the chimney is:

- **Santa-Safe:** The fire is out, and Santa can land with a big "Ho Ho Ho!"
- **Santa-Unsafe:** The fireplace is too hot, and Santa must move on to the next house.

But why stop at functionality? To bring the holiday spirit to life, this project adds animations showing a Santa when he can land safely, and another when he's afraid he will get scorched and thus move on to the next chimney, everything accompanied with sound and music.



## Solution

While Santa could survive with just a temperature sensor and simple electronics, where’s the fun in that? Instead, I challenged myself to integrate an IR thermal imaging camera, a color OLED display, and a tiny piezo buzzer, all connected to a Photon 2.

**Here’s how it works:**
- The thermal camera measures temperature at a resolution of 32x24 (768 pixels).
  - Additionally, average, minimum, and maximum temperatures are calculated.
- The measured temperature is shown on a color OLED display, interpolated to fill the whole screen.
  - If the temperature is cozy enough, an animation informs that conditions are safe for Santa.
  - If it's too hot (e.g., due to a recent fire), the display shows a sad Santa leaving for the next chimney, accompanied by a melancholy melody.

The thermal camera's wide-angle field of view (110°x75°) is evident in the images and videos, visible as small red blotches indicating hot water or a fire in the fireplace.

![](/images/Hot_water_compr.jpg)

Here's a link to a [YouTube video](https://studio.youtube.com/video/2iiJ5c-oN7o/edit) with sound, below the same video as a GIF-animation.
 

![](/videos/Santa_safe.and_unsafe-ezgif.com-optimize.gif)



## Bill of materials

- MCU: [Photon 2](https://store.particle.io/products/photon-2) or another Particle MCU

![](/images/Photon%202_compr.jpg)

- [Grove Thermal Imaging Camera 32x24 ](https://www.seeedstudio.com/Grove-Thermal-Imaging-Camera-IR-Array-MLX90640-110-degree-p-4334.html)
  - Seeed Studio offers similar cameras with different features and prices. If you use a different resolution, only a few lines of code need adjustment.

![](/images/Thermal_camera.jpg)

- Display: [Color OLED Module 96x64](https://www.parallax.com/product/color-oled-display-module-96-x-64/)
  - Easily swapped for another SSD1331-based display with the same resolution or other SPI displays with the right libraries.

![](/images/OLED_compr.jpg)

- [Grove Shield FeatherWing for Particle Mesh](https://www.adafruit.com/product/4309) or a solderless breadboard

![](/images/4309-00_compr.jpg)

- Assorted jumper wires (female-to-male recommended for the display)
- [Piezo buzzer](https://www.adafruit.com/product/160) (optional)

![](/images/Piezo_buzzer.jpg)

## Skills & tools required

For this project, you’ll need:
- Particle-flavored, Arduino-style, C++ development IDE
- No soldering required
- Optional: A 3D-printed case for the camera, display, and FeatherWing for a polished finish. I designed prototypes, though they might benefit from further tweaks:
  - [3d-printed Featherwing case](/images/Grove%20Shield%20Feather%20Case.stl) with engraved port labels for easy use (*VAMK* is the university where I teach)
  - [3D-printed case](/images/Thermal%20camera%20+%20display.stl)
  - [3D-printed lid](/images/Thermal%20imaging%20LID.stl)
  - I tend to use TPU for these type of scenarios as a little flexibility makes it easy to assemble and disassemble. I learned the hard way that printing TPU with a speed of 140 mm/s (direct drive) might jam the extruder completely, leading to complete disassembly of it... 

![](/images/Case%20and%20lid.jpg)
![](/images/Featherwing_case.jpg)
  
# Assembly

Only a few steps are needed to assemble the system:
1. Attach the Photon 2 to the Featherwing (or breadboard)
2. Connect the camera with a Grove cable to port I2C2 on the Featherwing
3. Wire the display as follows: *(nc = not connected = no wire needed)*

| Display   | Photon 2  |
| --------  | -------   |
| vcc       | 3V3       |
| gnd       | gnd       |
| nc        | -         |
| mosi/DIN  | S0        |
| sclk/clk  | S2        |
| cs        | S3        |
| d/c       | D5        |
| rst/res   | D4        |


![](/images/OLED_wired_compr.jpg)
![](/images/Photon%202%20wired_compr.jpg)


4. Optionally, connect the piezo buzzer (A2 for positive pin, GND for negative).


# Code Explanations

Once the devices are physically connected and working, it's relatively simple to display **text data** from the thermal camera to the OLED display. The main challenge lies in converting temperature data to a color scale, interpolating it while converting between different resolutions and aspect ratios (32x24 vs 96x64). Once that is accomplished, the code might need to be optimized and sped up to achieve better FPS than 0.5. Another challenge is to convert and optimize bitmaps for the animations.

Below the first thermal images of me, first in ASCII, second in colors. The quality in real life is much better than the photo indicates:

![](/images/ASCII_temps.png)
![](/images/First_thermal_image.jpg)

## Thermal Image Frame Rate 
While showing thermal images the frame rate is around 1 - 2 FPS which is sufficient in many cases, especially as temperatures generally do not change very quickly. Still, it should be possible to double or even triple the frame rate by using higher SPI and I2C frequencies, though these might need shorter and/or soldered wires. The code can likely be optimized even more, particularly by using integers instead of floats in the processor-heavy loops, and decreasing color accuracy.

## Bitmap Frame Rate
The original idea was to show GIF-animations on the display, but due to the slowness I needed to stick to showing static images with blinking text. Again, this could probably be optimized by using other methods for writing to the display

The Santa bitmaps were created with a free online tool, and then I used the magick-program to convert them to correct resolution (using this command line prompt: `magick convert magick convert tenor3-ezgif.com-optimize.gif -resize 64x96! -depth 8 frame_%03d.bmp`. To make the bitmaps usable, I converted them to .h-files using [this Python-program](/src/convert_bmp_to_h.py) I created.

![](/src/frames/frame_000.bmp)
![](/src/frames/frame_034.bmp)


## Camera, Display, and Piezo Buzzer Settings

From line 12 or so in the [main program](/src/fire_place3.cpp) you'll find connection and resolution settings for the camera, display, and piezo buzzer. Adjust these as needed based on your devices and connections.


```
// MLX90640 thermal imaging camera
Adafruit_MLX90640 mlx;

// OLED display
#define mosi S0
#define sclk S2
#define cs   S3
#define dc   D5
#define rst  D4

// Pin for the piezo buzzer
const int buzzerPin = A2;

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);
float frame[32 * 24];               // Buffer for MLX90640 data
uint16_t oledBuffer[96 * 64];       // Back buffer for OLED

// Define screen resolution and color range
#define SCREEN_WIDTH 96
#define SCREEN_HEIGHT 64

```

## Temperature Threshold and Bitmap Initializations

Here you'll find the warning threshold `hot_temp`, i.e. when should it warn that the chimney is too hot for Santa. In this program the average temperature of all the pixels is compared to this threshold, but you can of course use the maximum temp instead if you want. Especially with the wide-angle camera I used, it took a long time until the fire was so hot that the average temperature reached 58°C.
`show_cozy` is used to determine how often a satisfied Santa should be shown on the display, here it's once a minute.
The `#include "frame...`-lines includes the four bitmap .h-files that will be used later.  

```
float hot_temp = 58.0;  // In Celsius
uint8_t show_cozy = 60; // How frequently in seconds should cozy Santa be shown 

// This file contains the RGB565 array for the static image
#include "frame_frame_000.h" 
#include "frame_frame_999.h" 
#include "frame_frame_034.h" 
#include "frame_frame_034_1.h" 
```

## Melodies

Generating sound on a piezo buzzer is done through PWM (Pulse Width Modulation). As all PWM pins on the Photon 2 are assigned to the same timer, you cannot play different notes with different buzzers at the same time. If you want to learn a bit more about sound on the Photon 2, you can e.g. check this [tutorial](https://reparke.github.io/ITP348-Physical-Computing/lectures/week08/lecture_audio.html#optional-controlling-volume-1).

To play notes, it's easier to use note names instead of raw frequencies, these notes are defined in `notes.h`. In addition to the note, you also need to define how long it should be played. So, with "Jingle Bells" below, you see that the three first notes are E4, and further down you see the durations for the notes are quarter notes for the first two, and a half note for the third one. There is online software to convert melodies to a format suitable for piezo buzzers, and you can also try generative AI, although I had mixed experiences with it in this case.

The notes are then played with the function `playMelody`.

```
// Function to play a melody with a tempo adjustment
void playMelody(const int *melody, const int *noteDurations, int length, float tempoMultiplier) {
```

```
// Melody for "Jingle Bells"
const int jingle[] = {
    NOTE_E4, NOTE_E4, NOTE_E4, 
    NOTE_E4, NOTE_E4, NOTE_E4, 
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4, 
    NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, 
    NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4
};
// Note durations for "Jingle Bells"
const int jingleBellsDurations[] = {
    4, 4, 2, 
    4, 4, 2, 
    4, 4, 4, 4, 2, 
    4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 4, 4, 4, 2
};

```

## Thermal and Bitmap Image Functions

The function `analyzeFrameData` loops through the thermal data to derive min, max, and average temperatures. As the resolution of the thermal camera is ±1.5°C, it would actually make sense to use integers instead of floats to speed up the operations.

```
// Function to analyze the frame data and calculate min, max, and average temperatures
void analyzeFrameData(float* data, int size, float& tempMin, float& tempMax, float& tempAvg) {
```

This function maps the temperature data to color gradients (Blue -> Cyan -> Green -> Yellow -> Red). The coldest pixels in the image become blue, while the hottest ones become red. In some cases you might want to fix the lower and upper limits, or calibrate against the initially measured temperature data. 
```
uint16_t mapTemperatureToColor(float temp, float tempMin, float tempMax) {
```

Finally, the function `drawStaticImage` copies the requested bitmap to the backbuffer and displays it.
```
void drawStaticImage(const uint16_t* image) {
```

## Main Loop

In the `loop()`-function the main functionality is the "heavy" loop which is color-mapping and interpolating between the thermal camera and the display.

```
    // Fill the OLED buffer with interpolated and color-mapped data
    for (uint8_t screenY = 0; screenY < SCREEN_HEIGHT; screenY++) {
        for (uint8_t screenX = 0; screenX < SCREEN_WIDTH; screenX++) {
            float mlxX = mlxMapX[screenX];
            float mlxY = mlxMapY[screenY];
            float temp = interpolate(mlxX, mlxY, frame, 32, 24);

            oledBuffer[screenY * SCREEN_WIDTH + screenX] = mapTemperatureToColor(temp, tempMin, tempMax);
        }
    }

```

In addition, you'll also find the functions displaying numeric data on the display, as well as the conditional settings when to react on the temperature.

```
    // Display min and max temperatures
    display.setCursor(10, 5);
    display.print(tempMin, 1);
    display.setCursor(60, 5);
    display.print(tempMax, 1);

  
    if(tempAvg >= hot_temp) {
.......................

    // Perform an action once per minute
    unsigned long currentTime = millis(); // Get current time in milliseconds
    if (currentTime - lastActionTime >= show_cozy * 1000) { 
        lastActionTime = currentTime; // Update the timestamp

        // Add your code here for actions to be performed once per minute
        if(tempAvg < hot_temp) {
```

# CONCLUSION #

While this system might seem unnecessary for a once-a-year task, it demonstrates how you can combine simple components with affordable MCUs to develop innovative solutions. This project doubles as a festive tool for Santa and a prototype for year-round thermal imaging applications!

Feel free to clone my [GitHub repo](https://github.com/baljo/fire_place3) to build your own **Santa Chimney Safety System**!

![](/images/A_cheerful_Santa_Claus_standing_near_a_fireplace.jpg)