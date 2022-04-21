#include <M5Core2.h>
#include <FastLED.h>
#include "neuton.h"

//10 individually programmable RGB LEDs via SK6812 to create customized and animated light effects,
#define NUM_LEDS 10

#define DATA_PIN 25
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

#define INPUT_IMAGE_WIDTH            28
#define INPUT_IMAGE_HEIGHT            28
#define LCD_INPUT_IMAGE_WIDTH         240
#define LCD_INPUT_IMAGE_HEIGHT          240
#define NUM_CLASSES               10
#define LCD_INPUT_IMAGE_SHIFT         210
#define DRAW_IMAGE_X1             0  +10
#define DRAW_IMAGE_X2             240-10
#define DRAW_IMAGE_Y1             0  +10
#define DRAW_IMAGE_Y2             240-10
#define CLEAR_BUTTON_X1              250
#define CLEAR_BUTTON_Y1             190
#define CLEAR_BUTTON_X2               CLEAR_BUTTON_X1 + 60
#define CLEAR_BUTTON_Y2             CLEAR_BUTTON_Y1 + 40
#define PREDICT_BUTTON_X1              250
#define PREDICT_BUTTON_Y1             140
#define PREDICT_BUTTON_X2               PREDICT_BUTTON_X1 + 60
#define PREDICT_BUTTON_Y2             PREDICT_BUTTON_Y1 + 40

float in_data[INPUT_IMAGE_WIDTH][INPUT_IMAGE_HEIGHT];
float* targetBuff = (float*) calloc(784, sizeof(float));   // allocate memory for pixel buffer with 0s
int counter = 0;

void Draw_Menu()
{
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;
  leds[8] = CRGB::Black;
  leds[9] = CRGB::Black;
  FastLED.show();

  M5.Lcd.clear();
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLUE);

  M5.Lcd.drawRect(250, 2, 28, 28, PURPLE);
  M5.Lcd.fillRoundRect(250, 190, 60, 40, 10, GREEN);  // Clear button box

  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextDatum(ML_DATUM);    //Set text alignment to center-aligned
  M5.Lcd.drawString("Predicted?", 250, 65, 2);

  M5.Lcd.fillRoundRect(250, 140, 60, 40, 10, RED);   // Predict button box

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextDatum(ML_DATUM);    //Set text alignment to center-aligned
  M5.Lcd.drawString("PREDICT", 255, 160, 2);

  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setTextDatum(ML_DATUM);    //Set text alignment to center-aligned
  M5.Lcd.drawString("CLEAR", 260, 210, 2);

  M5.Lcd.drawRect(2, 2, 237, 237, BLUE);
}

void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
  FastLED.setBrightness(255);
  // put your setup code here, to run once:
  M5.begin();  //Initialize M5Stack
  M5.Lcd.fillScreen(WHITE);
  delay(100);
  M5.Lcd.clear();    //Clear the content displayed on the screen.

  Draw_Menu();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (M5.Touch.ispressed())
  {
    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Blue;
    leds[3] = CRGB::Blue;
    leds[4] = CRGB::Blue;
    leds[5] = CRGB::Blue;
    leds[6] = CRGB::Blue;
    leds[7] = CRGB::Blue;
    leds[8] = CRGB::Blue;
    leds[9] = CRGB::Blue;
    FastLED.show();

    TouchPoint_t coordinate;
    coordinate = M5.Touch.getPressPoint();

    if ((coordinate.x > DRAW_IMAGE_X1 && coordinate.x < DRAW_IMAGE_X2) && (coordinate.y > DRAW_IMAGE_Y1 && coordinate.y < DRAW_IMAGE_Y2))
    {
      counter++;

      int x = coordinate.y * ((float)28 / 240);
      int y = coordinate.x * ((float)28 / 240);
      
      printf("in data at X= %d Y =%d state x=%d y=%d\r\n", x , y , coordinate.x, coordinate.y);

      in_data[x + 1][y + 1] = 1.0;
      in_data[x - 1][y - 1] = 1.0;
      in_data[x + 1][y] = 1.0;
      in_data[x - 1][y] = 1.0;
      in_data[x][y + 1] = 1.0;
      in_data[x][y - 1] = 1.0;
      in_data[x + 1][y - 1] = 1.0;
      in_data[x - 1][y + 1] = 1.0;
      in_data[x][y] = 1.0;

      M5.Lcd.drawPixel(y + 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x + 1, 255);
      M5.Lcd.drawPixel(y + 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x + 1, 255);
      M5.Lcd.drawPixel(y - 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x - 1, 255);
      M5.Lcd.drawPixel(y + 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x, 255);
      M5.Lcd.drawPixel(y - 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x, 255);
      M5.Lcd.drawPixel(y + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x, 255);
      M5.Lcd.drawPixel(y + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x + 1, 255);
      M5.Lcd.drawPixel(y + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x - 1, 255);
      M5.Lcd.drawPixel(y + 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x - 1, 255);
      M5.Lcd.drawPixel(y - 1 + LCD_INPUT_IMAGE_SHIFT, LCD_INPUT_IMAGE_HEIGHT + x + 1, 255);
      M5.Lcd.fillCircle(coordinate.x, coordinate.y, 5, 255);
    }

    else if ((coordinate.x > PREDICT_BUTTON_X1 && coordinate.x < PREDICT_BUTTON_X2) && (coordinate.y > PREDICT_BUTTON_Y1 && coordinate.y < PREDICT_BUTTON_Y2 ))
    {
      for (int i = 0; i < INPUT_IMAGE_WIDTH; ++i)
      {
        for (int j = 0; j < INPUT_IMAGE_HEIGHT;  ++j)
        {
          if (in_data[i][j] > 0.0)
          {
            M5.Lcd.drawPixel(j + 250, i, 255);
            //Serial.print(int(in_data[i][j]));
          }
          Serial.print(int(in_data[i][j]));
          targetBuff[i * 28 + j] = in_data[i][j];
        }
        printf("\n");
      }

      Serial.println(neuton_model_ram_usage());

      if (neuton_model_set_inputs(targetBuff) == 0 && counter > 50)
      {
        uint16_t predictedClass;
        float* probabilities;

        if (neuton_model_run_inference(&predictedClass, &probabilities) == 0)
        {
          RedTail();
          // code for handling prediction result
          if (predictedClass == 1)
          {
            M5.Lcd.drawChar('1', 265, 80, 6);
            Serial.println("Output 1");
          }
          if (predictedClass == 2)
          {
            M5.Lcd.drawChar('2', 265, 80, 6);
            Serial.println("Output 2");
          }
          if (predictedClass == 3)
          {
            M5.Lcd.drawChar('3', 265, 80, 6);
            Serial.println("Output 3");
          }
          if (predictedClass == 4)
          {
            M5.Lcd.drawChar('4', 265, 80, 6);
            Serial.println("Output 4");
          }
          if (predictedClass == 5)
          {
            M5.Lcd.drawChar('5', 265, 80, 6);
            Serial.println("Output 5");
          }
          if (predictedClass == 6)
          {
            M5.Lcd.drawChar('6', 265, 80, 6);
            Serial.println("Output 6");
          }
          if (predictedClass == 7)
          {
            M5.Lcd.drawChar('7', 265, 80, 6);
            Serial.println("Output 7");
          }
          if (predictedClass == 8)
          {
            M5.Lcd.drawChar('8', 265, 80, 6);
            Serial.println("Output 8");
          }
          if (predictedClass == 9)
          {
            M5.Lcd.drawChar('9', 265, 80, 6);
            Serial.println("Output 9");
          }
          if (predictedClass == 0)
          {
            M5.Lcd.drawChar('0', 265, 80, 6);
            Serial.println("Output 0");
          }
        }
      }

    }

    else if ((coordinate.x > CLEAR_BUTTON_X1 && coordinate.x < CLEAR_BUTTON_X2) && (coordinate.y > CLEAR_BUTTON_Y1 && coordinate.y < CLEAR_BUTTON_Y2 ))
    {
      GreenTail();
      Draw_Menu();

      /** use this while collecting 28 X 28 pixels data
        //          for(int i = 0; i < 784; ++i)
        //          {
        //            Serial.print(targetBuff[i]);
        //            Serial.print(",");
        //          }
      **/


      counter = 0;

      free(targetBuff); // free the memory
      targetBuff = (float*) calloc(784, sizeof(float));     // reallocate memory for pixel buffer with 0.0s

      for (int i = 0; i < INPUT_IMAGE_WIDTH; ++i)
      {
        for (int j = 0; j < INPUT_IMAGE_HEIGHT;  ++j)
        {
          in_data[i][j] = 0.00;
        }
      }
    }
  }

  vTaskDelay(1);
}

void GreenTail()
{
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Green;
  leds[5] = CRGB::Green;
  leds[6] = CRGB::Green;
  leds[7] = CRGB::Green;
  leds[8] = CRGB::Green;
  leds[9] = CRGB::Green;
  FastLED.show();

  vTaskDelay(500);

  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;
  leds[8] = CRGB::Black;
  leds[9] = CRGB::Black;
  FastLED.show();
}


void RedTail()
{
  M5.Axp.SetLDOEnable(3, true);  //Open the vibration.

  leds[0] = CRGB::Red;
  leds[1] = CRGB::Red;
  leds[2] = CRGB::Red;
  leds[3] = CRGB::Red;
  leds[4] = CRGB::Red;
  leds[5] = CRGB::Red;
  leds[6] = CRGB::Red;
  leds[7] = CRGB::Red;
  leds[8] = CRGB::Red;
  leds[9] = CRGB::Red;
  FastLED.show();

  vTaskDelay(500);

  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;
  leds[8] = CRGB::Black;
  leds[9] = CRGB::Black;
  FastLED.show();

  M5.Axp.SetLDOEnable(3, false);
}
