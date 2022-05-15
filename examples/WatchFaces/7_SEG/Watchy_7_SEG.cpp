#include <stdlib.h> //srand, rand
#include "Watchy_7_SEG.h"

#define DARKMODE true

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void Watchy7SEG::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    //Hide wifi logo when WIFI is active (currently a low res image, looks bad)
    //display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(!WIFI_CONFIGURED)
	display.drawBitmap(120, 77, wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawWatchFaceChicken(){
    const int epd_allArray_LEN = 16;
    const unsigned char* epd_allArray[epd_allArray_LEN] = {
        epd_bitmap_Anacona_Screenshot_2022_05_14_10_59_31,
        epd_bitmap_Andalusian_Screenshot_2022_05_14_11_00_20,
        epd_bitmap_Barnevelder_Screenshot_2022_05_14_11_00_52,
        epd_bitmap_Campine_Screenshot_2022_05_14_11_01_16,
        epd_bitmap_Cornish_Screenshot_2022_05_14_22_39_23,
        epd_bitmap_Ixworth_Screenshot_2022_05_14_22_39_52,
        epd_bitmap_Silkie_Screenshot_2022_05_14_22_40_29,
        epd_bitmap_AmericanGame_Screenshot_2022_05_14_22_49_18,
        epd_bitmap_Cochin_Screenshot_2022_05_14_22_46_00,
        epd_bitmap_Derbyshire_Screenshot_2022_05_14_22_48_46,
        epd_bitmap_NewHampshire_Screenshot_2022_05_14_22_47_31,
        epd_bitmap_RhodeIslandRed_Screenshot_2022_05_14_22_47_57,
        epd_bitmap_Bresse_Screenshot_2022_05_14_22_37_59,
        epd_bitmap_Sussex_Screenshot_2022_05_14_22_49_58,
        epd_bitmap_Australorp_Screenshot_2022_05_14_22_56_21,
        epd_bitmap_JerseyGiant_Screenshot_2022_05_14_22_55_37
    };

    display.fillScreen(GxEPD_BLACK);
    srand(currentTime.Minute * currentTime.Hour * currentTime.Wday); //seeds rand() with the current minute causing watchy to display a new random image
    //change the % after rand() to the size of *art_sheet []
    display.drawBitmap(0, 0, epd_allArray[(rand() % epd_allArray_LEN)], DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_WHITE); //draws a random image from art_sheet.h full size

}

void Watchy7SEG::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    // Print minute to the nearest five
    int currentMinute = currentTime.Minute;
    display.print(int(currentMinute/10));
    int nearestFiveMins = int((currentMinute % 10) / 5) ? 5 : 0;
    display.println(nearestFiveMins);
}

void Watchy7SEG::drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if(currentTime.Wday == 4){
        w = w - 5;
    }
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.Day < 10){
    display.print("0");
    }
    display.println(currentTime.Day);
    display.setCursor(5, 150);
    display.println(tmYearToCalendar(currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void Watchy7SEG::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}
void Watchy7SEG::drawBattery(){
    display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.0){ //Changed battery levels from defaults to better match linearity for my watchy
        batteryLevel = 3;
    }
    else if(VBAT > 3.85 && VBAT <= 4.0){
        batteryLevel = 2;
    }
    else if(VBAT > 3.70 && VBAT <= 3.85){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.70){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(159 - w - x1, 150);
    }else{
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }else
    return;
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
