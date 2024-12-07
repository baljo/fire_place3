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
- Connect 

- Connect the cellular antenna to the Eval Board (the Bluetooth antenna can be left unconnected for this project).
- Connect the Person Sensor to the Eval Board, ensuring that you connect it to the I2C Grove port, not the analog port!

![](/images/Grove_connector_cropped.jpg)


- Optional: 
  - Connect a battery 
  - If using a 6V servo or actuator like in this project, it can be powered temporarily from the Eval Board. For long-term use, a separate power source is recommended.
    - In following images following wire colors are used: white = signal, black = ground, red = power
    - Connect the servo signal wire to **PWM0** (**D4** in the program)
    - Connect the ground wire to **GND** on the board


![](/images/PWM_GND_cropped.jpg)


  - Connect the power wire to **VCC** on the board

![](/images/Power_cropped.jpg)


- Power the B524 using the SOM power switch
- Connect the Eval board to your computer via an USB-cable
- If using a battery, charge it using the BAT POWER switch

![](/images/Switches_cropped_2.jpg)



## Software apps and online services

- Visual Studio Code with the Particle Workbench extension installed
- Optional if you want to get an electronical Santa notification in addition to the mechanical
  - Particle’s Webhook Integration for sending notifications to your mobile
  - Twilio, Pushover, or similar services for sending SMS or notifications
- The complete program can be found [here](/src/Person_sensor.cpp)

### Set up a Webhook integration to Pushover

This assumes you are using the Pushover service, but the overall principle is similar, regardless of chosen service.

#### Pushover service ####
- Create an account at Pushover (or similar service, e.g. Twilio)
  - Create an application in Pushover
  - Take a note of the User Key and API token
- Also install the Pushover app on your mobile device to get notifications


**User key field in Pushover** 

![](/images/Pushover_user_key.jpg)

**API Token/key in Pushover**

![](/images/Pushover_API_key.jpg)



#### Particle Webhook ####

- In the Particle console, Go to `Integrations`
- Add a new integration
- Scroll down and select `Custom Webhook`
- Select ´Custom template`
- Paste in the code below
- Replace the `token` and `user` in the code with the ones from your chosen service 
- `event` - in this case *"face_detected"* - should be same event as you are publishing from your code
- Test the integration, if everything is set up correctly, you should get a notification on your mobile device
  - If you use Pushover, the notification on your mobile is received through the Pushover service, not as a SMS.




```
{
    "name": "Identified as person: ",
    "event": "face_detected",
    "responseTopic": "{{PARTICLE_DEVICE_ID}}/hook-response/{{PARTICLE_EVENT_NAME}}",
    "disabled": false,
    "template": "webhook",
    "url": "https://api.pushover.net/1/messages.json",
    "requestType": "POST",
    "noDefaults": true,
    "rejectUnauthorized": true,
    "unchunked": false,
    "dataUriResponseEvent": false,
    "form": {
        "token": "token to selected service",
        "user": "user id at selected service",
        "title": "Door opened by person ",
        "message": "{{{PARTICLE_EVENT_VALUE}}}"
    }
}
```

![](/images/webhook.jpg)



### Calibrate the Person Sensor

The idea with the Person Sensor is that it should behave similarly as any other sensor, i.e. send data  back to the calling device. It is pre-programmed with algorithms that detect nearby faces, and return information over a simple  I2C interface. It is designed to be used as an input to a larger system and be treated like any other sensor where power and information are provided through the I2C interface.

All this means that apart from a few configurations options, the sensor can't be programmed, which also makes it extremely tamperproof (100 %?). But how do we then get it to not only **recognize if a face is seen**, but also to **identify the face**? The solution here is called calibration.

#### Calibration

While calibration can technically be done stand-alone, it’s much more practical to connect a computer. This way you can verify the functionality via the terminal window: 
- Position the person whose face you want to store in front of the sensor. When the face is detected, the green LED will light up.
- Within ~200 ms, press the `Mode` button on the Eval Board to store the face's features and assign it an ID in the sensor's EEPROM. Up to eight faces can be stored, with IDs ranging from 0 to 7.
- Repeat this for each face you'd like to store.

Below the function which is reacting on the button event. The "magic" happens in this code line - `person_sensor_write_reg(NEXT_ID_REGISTER, nextFaceID);` - which stores the face's features and corresponding ID into EEPROM.

```
void button_clicked(system_event_t event, int param)
{
    int times = system_button_clicks(param);

    faceIDs[nextFaceID] = nextFaceID;                   // Assign new ID to face
    person_sensor_write_reg(NEXT_ID_REGISTER, nextFaceID);
    Serial.printlnf("******** %d ********", nextFaceID);
    nextFaceID++;
}
```


**Mode button**

![](/images/Mode_button_cropped.jpg)



### Verify facial identification ###

Both during the calibration process and under normal use you can follow what the program is doing via the terminal window. To open this, open the Command Palette (Ctrl+Shift+P) and search for `Particle: Serial Monitor`. If you want to redo the calibration, just recycle power to the device and start the calibration again.

Apart from verification and troubleshooting purposes, you should also take a note of which face is having which ID. E.g. if your face were registered with ID 1, and Santa's with ID 4, you need to remember this for next step.

### Set up the Santa detection alarm ###

If you want the servo to react to specific faces, you need to change this code (around line 156):
`(face-> id == 4)` to include the face or faces you want to be able to unlock the door. In this case Santa has id #4, and while other faces like the president of Finland and Mona Lisa are identified by the Person Sensor, we don't take any actions on them.

Depending on the mechanical alarm device you've created, you might want to change how many times the servo is moving back and forth, in this case with the variable `jingles`, or the end positions and speed of the servo in the `santa`-function.

```
#ifdef santa
        if (face-> id == 4)  {
          int jingles = 6;
          
          for (int jingle = 0; jingle < jingles; jingle++)
            santa();
        }
#else
```



```
void santa()  {
  int speed = 5;

  for(servo_pos = 0; servo_pos < 175; servo_pos += speed)   // goes from 0 degrees to xxx degrees
  {                                                     // in steps of 1 degree
    myservo.write(servo_pos);                           // tell servo to go to position in variable 'pos'
    delay(5);                                           // waits for the servo to reach the position
  }

  delay(200);

  for(servo_pos = 175; servo_pos>=1; servo_pos -= speed)      // goes from xxx degrees to 0 degrees
  {                                                     // in steps of 1 degree
    myservo.write(servo_pos);                           // tell servo to go to position in variable 'pos'
    delay(5);                                           // waits for the servo to reach the position
  }
}
```



# CONCLUSION #

![](/images/All_persons.png)


The Santa Detector worked even better than I expected—especially given the three layers of glass in the windows.

As you've seen from the video and still pictures, the Person Sensor can distinguish between different faces and react accordingly. A further improvement to the program would be to utilize the confidence score reported by the sensor, e.g. only taking actions if the confidence is > 80 %. Other practical improvements is to use longer wires to the sensor and the servo so they can easier be placed further away from the Particle board.


Feel free to clone [my Github repo](https://github.com/baljo/santa_detector) to build your own Santa Detector!