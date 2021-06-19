/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.7.4 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.5 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.5 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://diandeng.tech/doc
 *       
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

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

BlinkerButton Button1("btn-switch");
BlinkerNumber Number1("num-abc");

int counter = 0;

void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);

    String on = "on";
    String off = "off";
    if (state == "on")
    {
        Button1.print("on");

        digitalWrite(LED_BUILTIN, LOW);

        lightSwitch(on);
    }
    else if (state == "off")
    {
        Button1.print("off");

        digitalWrite(LED_BUILTIN, HIGH);

        lightSwitch(off);
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

    // if (key.compare("btn-switch") == 0){

    // }

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
        return;
    }
}

void lightSwitch(const String &state)
{
    BLINKER_LOG("light stat:", state);
    if (state == "on")
    {
        for (int i = 0; i < NUMPIXELS; i++)
        {

            pixels.setPixelColor(i, pixels.Color(0, 150, 0));
            pixels.show();
        }
    }
    else if (state == "off")
    {
        pixels.clear();

        // after emptying the lights, you need to use the show function to update
        pixels.show();
    }
}
