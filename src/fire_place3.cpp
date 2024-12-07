// Display thermal image and a single static image on OLED using Adafruit SSD1331
// Date: 2024-12-02, Author: Thomas Vikström

#include "Particle.h"
#include <Wire.h>
#include <Adafruit_MLX90640.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <notes.h>

// Define the MLX90640 and OLED display
Adafruit_MLX90640 mlx;

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

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

float mlxMapX[SCREEN_WIDTH], mlxMapY[SCREEN_HEIGHT]; // Precomputed mapping
uint16_t gradient[256]; // Precomputed gradient array

unsigned long lastActionTime = 0;
float hot_temp = 58.0;  // In Celsius
uint8_t show_cozy = 60; // How frequently in seconds should cozy Santa be shown 

// This file contains the RGB565 array for the static image
#include "frame_frame_000.h" 
#include "frame_frame_999.h" 
#include "frame_frame_034.h" 
#include "frame_frame_034_1.h" 

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

// Extended Beethoven Symphony No. 5 melody
const int symphony5[] = {
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_Eb4, 
    NOTE_F4, NOTE_F4, NOTE_F4, NOTE_D4, 
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, 
    NOTE_Eb4, NOTE_D4, NOTE_C4, NOTE_Bb3
};
// Note durations for Symphony No. 5
const int symphony5Durations[] = {
    4, 4, 4, 1, 
    4, 4, 4, 1, 
    4, 4, 4, 1, 
    4, 4, 4, 1, 
};

const int down[] = {
    NOTE_E4, NOTE_D4, NOTE_C4, 
};
const int downDurations[] = {
    4, 4, 2,
};
const int up[] = {
    NOTE_C4, NOTE_D4, NOTE_E4, 
};



// Function to play a melody with a tempo adjustment
void playMelody(const int *melody, const int *noteDurations, int length, float tempoMultiplier) {
    for (int i = 0; i < length; i++) {
        // Adjust the note duration based on tempoMultiplier
        int noteDuration = (1000 / noteDurations[i]) * tempoMultiplier; 
        tone(A2, melody[i], noteDuration); // Play the note
        
        // Add a delay for better separation between notes
        delay(noteDuration * 1.2);
        
        noTone(A2); // Stop the tone before the next note
    }
}


// Function to analyze the frame data and calculate min, max, and average temperatures
void analyzeFrameData(float* data, int size, float& tempMin, float& tempMax, float& tempAvg) {
    tempMin = data[0];
    tempMax = data[0];
    float sum = 0;

    for (int i = 0; i < size; i++) {
        if (data[i] < tempMin) tempMin = data[i];
        if (data[i] > tempMax) tempMax = data[i];
        sum += data[i];
    }

    tempAvg = sum / size; // Calculate the average
}


uint16_t mapTemperatureToColor(float temp, float tempMin, float tempMax) {
    float normalized = (temp - tempMin) / (tempMax - tempMin);
    normalized = max(0.0, min(1.0, normalized)); // Clamp to 0-1

    uint8_t red = 0, green = 0, blue = 0;

    // Color gradient: Blue -> Cyan -> Green -> Yellow -> Red
    if (normalized <= 0.25) {
        blue = 255;
        green = (uint8_t)(normalized * 4 * 255);          // 0 to 0.25 -> 0 to 255
    } else if (normalized <= 0.5) {
        blue = (uint8_t)((0.5 - normalized) * 4 * 255);   // 0.25 to 0.5 -> 255 to 0
        green = 255;
    } else if (normalized <= 0.75) {
        green = 255;
        red = (uint8_t)((normalized - 0.5) * 4 * 255);    // 0.5 to 0.75 -> 0 to 255
    } else {
        red = 255;
        green = (uint8_t)((1.0 - normalized) * 4 * 255);  // 0.75 to 1.0 -> 255 to 0
    }

    return display.color565(red, green, blue);
}

// Function to interpolate temperature data for OLED
float interpolate(float x, float y, float* data, int width, int height) {
    int x0 = (int)x;
    int x1 = min(x0 + 1, width - 1);
    int y0 = (int)y;
    int y1 = min(y0 + 1, height - 1);

    float dx = x - x0;
    float dy = y - y0;

    float value = (1 - dx) * (1 - dy) * data[y0 * width + x0] +
                  dx * (1 - dy) * data[y0 * width + x1] +
                  (1 - dx) * dy * data[y1 * width + x0] +
                  dx * dy * data[y1 * width + x1];

    return value;
}

void drawStaticImage(const uint16_t* image) {
    // Copy the image to the back buffer
    memcpy(oledBuffer, image, sizeof(oledBuffer));

    // Push the buffer to the display
    display.drawRGBBitmap(0, 0, oledBuffer, SCREEN_HEIGHT, SCREEN_WIDTH);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    // Initialize OLED display
    display.begin();
    display.fillScreen(BLACK);
    display.setTextSize(2);
    display.setTextColor(0xF800); // RED color in RGB565

    display.setCursor(5, 10);
    display.println("Santa's");
    display.setCursor(30, 40);
    display.println("Hot");
    delay(1000);

    display.fillScreen(BLACK);
    drawStaticImage(frame_frame_034);       // Santa too hot
    drawStaticImage(frame_frame_034_1);     // "Too hot"
    display.setTextSize(1);
    display.setTextColor(0xFFE0); // Yellow color in RGB565
    display.setCursor(70, 45);                  
    display.print(97.8, 1);                 // Showing hot temp
    playMelody(down, downDurations, sizeof(down) / sizeof(down[0]), 0.8);

    delay(1000);
    
    display.fillScreen(BLACK);
    display.setTextColor(0xF800); // RED color in RGB565
    display.setTextSize(2);
    display.setCursor(35, 10);
    display.println("or");
    display.setCursor(25, 30);
    display.println("Not?");

    delay(1000);

    display.fillScreen(BLACK);
    drawStaticImage(frame_frame_000);       // Santa cozy
    drawStaticImage(frame_frame_999);       // "Cozy"
    display.setTextColor(0x07E0); // Green color in RGB565
    display.setTextSize(1);
    display.setCursor(70, 45);
    display.print(23.6, 1);                 // Showing cozy temp
    playMelody(up, downDurations, sizeof(up) / sizeof(up[0]), 0.8);

    delay(1000);

    display.setTextSize(1);     // Resetting to normal size
    display.setTextColor(0xFFFF); // White color in RGB565

    // Initialize MLX90640 sensor
    if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
        Serial.println("MLX90640 not found!");
        while (1) delay(10);
    }
    mlx.setMode(MLX90640_CHESS);
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx.setRefreshRate(MLX90640_4_HZ);

    Wire.setClock(700000);                  // Fast I2C

    // Precompute OLED-to-MLX mapping
    for (uint8_t i = 0; i < SCREEN_WIDTH; i++) {
        mlxMapX[i] = i * (32.0 / SCREEN_WIDTH);
    }
    for (uint8_t i = 0; i < SCREEN_HEIGHT; i++) {
        mlxMapY[i] = i * (24.0 / SCREEN_HEIGHT);
    }
}


void loop() {
    // Get the thermal data from the MLX90640
    if (mlx.getFrame(frame) != 0) {
        Serial.println("Failed to read frame!");
        return;
    }

    // Variables to store min, max, and average temperatures
    float tempMin, tempMax, tempAvg;

    // Analyze frame data
    analyzeFrameData(frame, 32 * 24, tempMin, tempMax, tempAvg);

    // Print temperatures to the Serial terminal
    Serial.print("Min Temp: ");
    Serial.print(tempMin, 2);
    Serial.print(" C, Max Temp: ");
    Serial.print(tempMax, 2);
    Serial.print(" C, Avg Temp: ");
    Serial.print(tempAvg, 2);
    Serial.println(" C");


    // Fill the OLED buffer with interpolated and color-mapped data
    for (uint8_t screenY = 0; screenY < SCREEN_HEIGHT; screenY++) {
        for (uint8_t screenX = 0; screenX < SCREEN_WIDTH; screenX++) {
            float mlxX = mlxMapX[screenX];
            float mlxY = mlxMapY[screenY];
            float temp = interpolate(mlxX, mlxY, frame, 32, 24);

            oledBuffer[screenY * SCREEN_WIDTH + screenX] = mapTemperatureToColor(temp, tempMin, tempMax);
        }
    }

    // Push the updated buffer to the display
    display.drawRGBBitmap(0, 0, oledBuffer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Display min and max temperatures
    display.setCursor(10, 5);
    display.print(tempMin, 1);
    display.setCursor(60, 5);
    display.print(tempMax, 1);

    tempAvg = (tempMax + tempMin) / 2;

    if(tempAvg >= hot_temp) {

        display.fillScreen(BLACK);
        display.setTextColor(0xFFE0); // Yellow color in RGB565
        playMelody(down, downDurations, sizeof(down) / sizeof(down[0]), 0.8);

        for(int i=0; i<3;i++){    
            if(i==1)
                playMelody(symphony5, symphony5Durations, sizeof(symphony5) / sizeof(symphony5[0]), 0.9);
            display.setCursor(70, 35);
            display.println("Hot!");
            display.setCursor(70, 45);
            display.print(tempAvg,1);
            drawStaticImage(frame_frame_034);    
            drawStaticImage(frame_frame_034_1);    
        }
        //playMelody(symphony5, symphony5Durations, sizeof(symphony5) / sizeof(symphony5[0]), 1);

        display.setTextColor(0xFFFF); // White color in RGB565
    }


    // tempAvg = (tempMax + tempMin) / 2;
    // Perform an action once per minute
    unsigned long currentTime = millis(); // Get current time in milliseconds
    if (currentTime - lastActionTime >= show_cozy * 1000) { 
        lastActionTime = currentTime; // Update the timestamp

        // Add your code here for actions to be performed once per minute
        if(tempAvg < hot_temp) {

            display.fillScreen(BLACK);
            display.setTextColor(0x07E0); // Green color in RGB565
            for(int i=0; i<3;i++){    
                display.setCursor(70, 35);
                display.println("Cozy!");
                display.setCursor(70, 45);
                display.print(tempAvg,1);
                drawStaticImage(frame_frame_000);    
                drawStaticImage(frame_frame_999);    
            }
        } 
        display.setTextColor(0xFFFF); // White color in RGB565       
        Serial.println("Action triggered after set interval");
    }


}
