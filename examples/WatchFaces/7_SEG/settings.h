#ifndef SETTINGS_H
#define SETTINGS_H

//Weather Settings
//#define CITY_ID "5128581" //New York City https://openweathermap.org/current#cityid
//TODO: refactor these parameters, many now unused with onecall API
#define CITY_ID "6141256" //Saskatoon
#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //use your own API key :)
#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/onecall?id=" //open weather api
#define TEMP_UNIT "metric" //metric = Celsius , imperial = Fahrenheit
#define TEMP_LANG "en"
#define WEATHER_UPDATE_INTERVAL 1440 //must be greater than 5, less than 128 (int8_t) measured in minutes - changed to int
//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * -6 //New York is UTC -5
#define DST_OFFSET_SEC 0

watchySettings settings{
    CITY_ID,
    OPENWEATHERMAP_APIKEY,
    OPENWEATHERMAP_URL,
    TEMP_UNIT,
    TEMP_LANG,
    WEATHER_UPDATE_INTERVAL,
    NTP_SERVER,
    GMT_OFFSET_SEC,
    DST_OFFSET_SEC
};

#endif
