#include "TimeScreen.h"

#include <stdlib.h>
#include <time.h>
#include "GetWeather.h"

#include "Foofingers/fonts/MotorolaScreenType_w_degree8pt7b.h"
#include "Foofingers/fonts/Bebas_Regular60pt7b.h"
#include "Foofingers/fonts/Bebas_Regular30pt7b.h"
#include "Foofingers/FooFingers_sprites.h"
#include "GetLocation.h"

using namespace Watchy;

void TimeScreen::show() {
  //setenv("TZ", Watchy_GetLocation::currentLocation.timezone, 1);
  //tzset();
  tm t;
  time_t tt = now();
  localtime_r(&tt, &t);

  Watchy::display.fillScreen(bgColor);

  String s_currentHour = "";
  String s_currentMin = "";

  // hours
  if(t.tm_hour < 10){
      s_currentHour.concat("0");
  }
  s_currentHour.concat(t.tm_hour);

  display.setFont(&Bebas_Regular60pt7b);
  display.setCursor(20, 145);
  display.print(s_currentHour);

  // minutes
  if(t.tm_min < 10){
      s_currentMin.concat("0");
  }
  s_currentMin.concat(t.tm_min);

  display.setFont(&Bebas_Regular30pt7b);
  display.setCursor(130, 102);
  display.print(s_currentMin);

  // date
  String s_currentDateDay = "";
    switch (t.tm_mday){
      case 1:
      case 21:
      case 31:
        s_currentDateDay.concat("st");
      break;
      case 2:
      case 22:
        s_currentDateDay.concat("nd");
      break;
      case 3:
      case 23:
        s_currentDateDay.concat("rd");
      break;
      default:
        s_currentDateDay.concat("th");
      break;
    }
  
  display.setCursor(5, 22);
  display.setFont(&MotorolaScreenType_w_degree8pt7b);
  display.print(&t, "%a, %B %d");
  display.print(s_currentDateDay);

  // Show battery top right
  int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 4;
        display.drawBitmap(167, 8, foofingers_img_bat100, 28, 13, GxEPD_WHITE);
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 3;
        display.drawBitmap(167, 8, foofingers_img_bat75, 28, 13, GxEPD_WHITE);
    }
    else if(VBAT > 3.88 && VBAT <= 3.95){
        batteryLevel = 2;
        display.drawBitmap(167, 8, foofingers_img_bat50, 28, 13, GxEPD_WHITE);
    } 
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
        display.drawBitmap(167, 8, foofingers_img_bat25, 28, 13, GxEPD_WHITE);
    }    
    else if(VBAT <= 3.80){
        batteryLevel = 0;
        display.drawBitmap(167, 8, foofingers_img_bat0, 28, 13, GxEPD_WHITE);
    }

    // Show steps
    uint32_t stepCount = sensor.getCounter();

    const unsigned char* stepIcon;

    if(stepCount > 0 && stepCount <= 3999){
        stepIcon = foofingers_img_step_goal_1;
    }
    else if(stepCount > 4000 && stepCount <= 8000){
        stepIcon = foofingers_img_step_goal_2;
    }
    else if(stepCount > 8001){
        stepIcon = foofingers_img_step_goal_3;
    }
    
    display.drawBitmap(10, 168, stepIcon, 38, 23, GxEPD_WHITE);
    display.setFont(&MotorolaScreenType_w_degree8pt7b);
    display.setCursor(55, 190);
    display.println(stepCount);

    // Show Weather
    const unsigned char* weatherIcon;
    auto wd = Watchy_GetWeather::getWeather();
    uint32_t weatherCode = wd.weatherConditionCode;

    if(weatherCode > 801){//Cloudy
    weatherIcon = foofingers_img_cloudy;
    }else if(weatherCode == 801){//Few Clouds
    weatherIcon = foofingers_img_cloudsun;  
    }else if(weatherCode == 800){//Clear
    weatherIcon = foofingers_img_sunny;  
    }else if(weatherCode >=700){//Atmosphere
    weatherIcon = foofingers_img_cloudy; 
    }else if(weatherCode >=600){//Snow
    weatherIcon = foofingers_img_snow;
    }else if(weatherCode >=500){//Rain
    weatherIcon = foofingers_img_rain;  
    }else if(weatherCode >=300){//Drizzle
    weatherIcon = foofingers_img_drizzle;
    }else if(weatherCode >=200){//Thunderstorm
    weatherIcon = foofingers_img_thunderstorm; 
    }else
    return;
    display.drawBitmap(128, 110, weatherIcon, 48, 48, GxEPD_WHITE);

    // Show temp
    String tempText = "";
    tempText.concat(wd.temperature);
    tempText.concat("°C");
    
    display.setFont(&MotorolaScreenType_w_degree8pt7b);
    display.setCursor(170, 190);
    display.println(tempText);

}