/* *****************************************************************
 *
 * Smart Light Strip
 * 
 * *****************************************************************/

/*******************************CONFIG******************************/

#define PIN D8       // arduino control pin
#define NUMPIXELS 30 // light numbers

char auth[] = "a6f0f4ac1eee";
char ssid[] = "Redmi_4ECD";    // wifi ssid/name
char pswd[] = "tanzhongzheng"; // wifi password

/* *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <Blinker.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define ON "on"
#define OFF "off"

#define SWITCH "switch"
#define COLORWHEEL "colorWheel"
#define BRIGHTNESS "brightness"

#define RGB_1 "RGBKey"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

BlinkerButton Button1("btn-switch");
BlinkerNumber Number1("num-abc");


int counter = 0;

int color[4] = {255, 255, 255, 255};   // RGBW note: W was not used in ws2812B.
int brightness = 255;                  // 亮度
bool wsState = true;                   // ligth state
uint8_t wsMode = BLINKER_CMD_MIOT_DAY; // light mode

uint8_t colorR = 255, colorG = 255, colorB = 255, colorW = 255;
uint32_t color32;

BlinkerRGB WS2812(RGB_1);

void button1_callback(const String &state)
{
    BLINKER_LOG("----------------------------------------------------------------");
    BLINKER_LOG("get button state: ", state);

    if (state == ON)
    {
        Button1.print(ON);

        digitalWrite(LED_BUILTIN, LOW);

        lightSwitch(ON);

        digitalWrite(5, HIGH);
    }
    else if (state == OFF)
    {
        Button1.print(OFF);

        digitalWrite(LED_BUILTIN, HIGH);

        lightSwitch(OFF);

        digitalWrite(5, LOW);
    }
}

/**
 * Blinker数据解析
 */
void parseJson(const String &json, String &key, String &value)
{
    DynamicJsonDocument doc(200);
    DeserializationError err = deserializeJson(doc, json);
    if (err)
    {
        BLINKER_LOG("parse json error.");
        BLINKER_LOG(err.f_str());
        key = "error";
        return;
    }

    JsonObject obj = doc.as<JsonObject>();
    for (JsonPair p : obj)
    {
        key = p.key().c_str();

        if (key == SWITCH)
        {

            value = p.value().as<char *>();
        }
        else if (key == COLORWHEEL)
        {
            JsonArray arr = p.value().as<JsonArray>();

            int i = 0;
            for (JsonVariant v : arr)
            {
                color[i++] = v.as<int>();
            }

            colorR = color[0];
            colorG = color[1];
            colorB = color[2];
            colorW = color[3];
            brightness = color[3];
        }
        else if (key == BRIGHTNESS)
        {
            brightness = p.value().as<int>();
            BLINKER_LOG("brightness:", brightness);
        }

        return;
    }
}

/**
 * 开关控制
 */
void lightSwitch(const String &state)
{
    BLINKER_LOG("----------------------------------------------------------------");
    BLINKER_LOG("light stat:", state);
    if (state == ON)
    {
        pixelShow();
    }
    else if (state == OFF)
    {
        pixels.clear();

        // after emptying the lights, you need to use the show function to update
        pixels.show();
    }
}

/**
 * 颜色更新
 */
void pixelShow()
{
    BLINKER_LOG("COLOR:", colorR, " ", colorG, " ", colorB, " BRIGHTNESS:", colorW);
    pixels.setBrightness(colorW);
    for (int i = 0; i < NUMPIXELS; i++)
    {
        // GRB
        pixels.setPixelColor(i, pixels.Color(colorR, colorG, colorB));
    }
    pixels.show();
}

/**
 * 灯带回调函数
 */
void ws2812_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    BLINKER_LOG("----------------------------------------------------------------");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);

    colorR = r_value;
    colorG = g_value;
    colorB = b_value;
    colorW = bright_value;

    pixelShow();
}

uint32_t getColor()
{
    return colorR << 16 | colorG << 8 | colorB;
}

/* *****************************************************************
 * 
 * MIOT 回调函数
 * 
 * *****************************************************************/
void miotPowerState(const String &state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON)
    {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerMIOT.powerState(ON);
        BlinkerMIOT.print();

        wsState = true;

        if (colorW == 0)
            colorW = 255;

        lightSwitch(ON);

        Button1.print(ON);
    }
    else if (state == BLINKER_CMD_OFF)
    {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerMIOT.powerState(OFF);
        BlinkerMIOT.print();

        wsState = false;

        colorW = 0;

        lightSwitch(OFF);

        Button1.print(OFF);
    }
}

/**
 * 颜色回调函数
 */
void miotColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);

    colorR = color >> 16 & 0xFF;
    colorG = color >> 8 & 0xFF;
    colorB = color & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);

    pixelShow();

    BlinkerMIOT.color(color);
    BlinkerMIOT.print();
}

/**
 * 模式回调函数
 */
void miotMode(uint8_t mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_MIOT_DAY)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COLOR)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_TV)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_READING)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER)
    {
        // Your mode function
    }

    wsMode = mode;

    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();
}

/**
 * 亮度回调函数  1-100
 */
void miotBright(const String &bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    colorW = bright.toInt();

    BLINKER_LOG("now set brightness: ", colorW);

    BlinkerMIOT.brightness(colorW);
    BlinkerMIOT.print();
}

/**
 * 色温控制回调函数  1000-10000
 */
void miotColorTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set colorTemperature: ", colorTemp);

    int32_t colorT = colorTemp;

    BlinkerMIOT.colorTemp(colorTemp);
    BlinkerMIOT.print();
}

/**
 * 设备查询接口
 */
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER:
        BlinkerMIOT.powerState(wsState ? ON : OFF);
        break;
    case BLINKER_CMD_QUERY_COLOR_NUMBER:
        BlinkerMIOT.color(getColor());
        break;
    case BLINKER_CMD_QUERY_MODE_NUMBER:
        BlinkerMIOT.mode(wsMode);
        break;
    case BLINKER_CMD_QUERY_COLORTEMP_NUMBER:
        break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER:
        BlinkerMIOT.brightness(colorW);
        break;
    default:
        BLINKER_LOG("queryCode error: ", queryCode);
        break;
    }

    BlinkerMIOT.print();
}

/**
 * APP数据读取
 */
void dataRead(const String &data)
{
    String key;
    String value;

    BLINKER_LOG("----------------------------------------------------------------");
    BLINKER_LOG("Blinker readString: ", data);

    parseJson(data, key, value);
    BLINKER_LOG("key  : ", key);
    BLINKER_LOG("value: ", value);

    if (key == "error")
    {
        BLINKER_LOG("processing error.");
        return;
    }

    if (key == COLORWHEEL)
    {
        lightSwitch(ON);
    }
    else if (key == BRIGHTNESS)
    {
        BLINKER_LOG("brightness2: ", brightness);
        pixels.setBrightness(brightness);
        pixels.show();
    }

    counter++;
    Number1.print(counter);
}

void setup()
{
    /* 默认端口 BLE RX-3 TX-2 */
    Serial.begin(9600);
    BLINKER_DEBUG.stream(Serial);
    // BLINKER_DEBUG.debugAll();

    BLINKER_LOG("pixels config");
    pixels.begin();
    // pixels.setBrightness(colorW);
    WS2812.attach(ws2812_callback);
    // pixels.show();
    // pixelShow();

    BLINKER_LOG("build in led config");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // pinMode(5, OUTPUT);
    // digitalWrite(5, HIGH);

    // config wifi
    BLINKER_LOG("config wifi");
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);
    BlinkerMIOT.attachMode(miotMode);
    BlinkerMIOT.attachBrightness(miotBright);
    BlinkerMIOT.attachColorTemperature(miotColorTemp);
    BlinkerMIOT.attachQuery(miotQuery);

    Button1.attach(button1_callback);
}

void loop()
{
    Blinker.run();

#if defined(__AVR__ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
}
