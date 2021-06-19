/* *****************************************************************
 *
 * Smart Light Strip
 * 
 * *****************************************************************/

#define BLINKER_BLE
#define BLINKER_MIOT_LIGHT

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <Blinker.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

// define
#define PIN 8        // arduino control pin
#define NUMPIXELS 30 // light numbers

#define ON "on"
#define OFF "off"
#define COLORWHEEL "colorWheel"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

BlinkerButton Button1("btn-switch");
BlinkerNumber Number1("num-abc");

int counter = 0;

int color[4] = {255, 255, 255, 255}; // RGBW

void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == ON)
    {
        Button1.print(ON);

        digitalWrite(LED_BUILTIN, LOW);

        lightSwitch(ON);
    }
    else if (state == OFF)
    {
        Button1.print(OFF);

        digitalWrite(LED_BUILTIN, HIGH);

        lightSwitch(OFF);
    }
}

void dataRead(const String &data)
{
    String key;
    String value;

    BLINKER_LOG("Blinker readString: ", data);

    parseJson(data, key, value);
    BLINKER_LOG("key  : ", key);
    BLINKER_LOG("value: ", value);

    if (key == COLORWHEEL)
    {
        lightSwitch(ON);
    }

    counter++;
    Number1.print(counter);
}

void setup()
{
    /* 默认端口 BLE RX-3 TX-2 */
    Serial.begin(9600);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Blinker.begin();
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);
}

void loop()
{
    Blinker.run();

#if defined(__AVR__ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    pixels.begin();
}

void parseJson(const String &json, String &key, String &value)
{
    DynamicJsonDocument doc(200);
    DeserializationError err = deserializeJson(doc, json);
    if (err)
    {
        BLINKER_LOG("parse json error.");
        BLINKER_LOG(err.f_str());
        return;
    }

    JsonObject obj = doc.as<JsonObject>();
    for (JsonPair p : obj)
    {
        key = p.key().c_str();
        value = p.value().as<char *>();

        if (value == "")
        {
            JsonArray arr = p.value().as<JsonArray>();
            BLINKER_LOG(arr);

            if (key == COLORWHEEL)
            {
                int i = 0;
                for (JsonVariant v : arr)
                {
                    color[i++] = v.as<int>();
                }
            }

            // color[0] = arr[0];
            // color[1] = arr[1];
            // color[2] = arr[2];
            // color[3] = arr[3];
        }
        return;
    }
}

void lightSwitch(const String &state)
{
    BLINKER_LOG("light stat:", state);
    if (state == ON)
    {
        BLINKER_LOG("COLOR:", color[0], " ", color[1], " ", color[2], " ", color[3]);
        for (int i = 0; i < NUMPIXELS; i++)
        {
            // GRB
            pixels.setPixelColor(i, pixels.Color(color[0], color[1], color[2], color[3]));
            pixels.show();
        }
    }
    else if (state == OFF)
    {
        pixels.clear();

        // after emptying the lights, you need to use the show function to update
        pixels.show();
    }
}
