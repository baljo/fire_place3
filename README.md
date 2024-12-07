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

While Santa could survive by you using only a temperature sensor and simple electronics, what's the fun and challenging in that? Thus I decided to challenge myself and integrate an IR thermal imaging camera, a color OLED display, and a tiny piezo buzzer, all connected to a Photon 2. 
The functionality is as following:
- The thermal camera measures temperature at a resolution of 32x24 (768 pixels).
  - In addition to raw values, an average + min and max temps are derived 
- The measured temperature is shown on a color OLED display, interpolated to fill the whole screen.
  - When the temperature is cozy enough, an animation is informing that the conditions are safe for Santa
  - When it's too hot, e.g. due to a recent fire in the fireplace, an animation is showing a sad Santa leaving for the next chimney, accompanied with a sad melody.

The thermal imaging camera has a wide angle field of view (110°x75°), which is very visible in the picture and video in form of the small red blotches indicating hot water, or a fire in the fireplace.

![](/images/Hot_water_compr.jpg)

Here's a link to a [YouTube video](https://studio.youtube.com/video/2iiJ5c-oN7o/edit) with sound, below same as GIF-animation.
 

![](/videos/Santa_safe.and_unsafe-ezgif.com-optimize.gif)



## Bill of materials

- MCU: [Photon 2](https://store.particle.io/products/photon-2) or another Particle MCU

![](/images/Photon%202_compr.jpg)

- [Grove Thermal Imaging Camera 32x24 ](https://www.seeedstudio.com/Grove-Thermal-Imaging-Camera-IR-Array-MLX90640-110-degree-p-4334.html)
  - Seeed Studio have several other thermal cameras with different features, prices, and availability. If you use another resolution, you only need to change a few code lines. 

![](/images/Thermal_camera.jpg)

- Display: [Color OLED Module 96x64](https://www.parallax.com/product/color-oled-display-module-96-x-64/)
  - You can easily swap this with another display using the SSD1331 driver chip and having same resolution. Other displays using SPI can also be used as long as you find proper libraries.

![](/images/OLED_compr.jpg)

- [Grove Shield FeatherWing for Particle Mesh](https://www.adafruit.com/product/4309) or a solderless breadboard

![](/images/4309-00_compr.jpg)

- Assorted jumper wires, for connecting the display I recommend female to male wires.
- Optional: [Piezo buzzer](https://www.adafruit.com/product/160)

![](/images/Piezo_buzzer.jpg)

## Skills & tools required

For this project, you’ll need:
- Particle-flavored, Arduino-style, C++ development IDE
- No soldering required
- Optional, but recommended to use some type of case for the camera and display, and perhaps a case for the Featherwing. Here are prototypes I designed, for a permanent solution they might benefit from a few improvements:
  - [3d-printed Featherwing case](/images/Grove%20Shield%20Feather%20Case.stl) Note that I engraved port Grove names on the bottom for making it easierto find correct port. 
  - [3D-printed case](/images/Thermal%20camera%20+%20display.stl)
  - [3D-printed lid](/images/Thermal%20imaging%20LID.stl)

![](/images/Case%20and%20lid.jpg)

![](/images/Featherwing_case.jpg)
  
# Assembly

Only a few steps are needed to assemble the system:
- Attach the Photon 2 to the Featherwing (or breadboard)
- Connect the camera with a Grove cable to port A2 on the Featherwing
- Connect the display with jumper wires **************************************
- Optional: Connect the piezo buzzer with two wires *******************************

- Connect the cellular antenna to the Eval Board (the Bluetooth antenna can be left unconnected for this project).
- Connect the Person Sensor to the Eval Board, ensuring that you connect it to the I2C Grove port, not the analog port!

![](/images/Grove_connector_cropped.jpg)



## Software apps and online services

- Visual Studio Code with the Particle Workbench extension installed



# CONCLUSION #




Feel free to clone [my Github repo](https://github.com/baljo/santa_detector) to build your own Santa Detector!