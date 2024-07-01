# S6_IoT-final_project-smart_home-wokwi_simulation
Followed what's in the assignement document and added some extra things :
- A buzzer and a whatsapp bot to alarm about CO2 level when it reaches a dangerous level. 
- Instead of defining some thresholds for progressive openings of the window with the servo motor, I used the map method to make both opening and closing of the window very dynamic, depending on the temperature, humidity and CO2 level.
- An option to export collected data as a CSV or SQL file for further data analysis.


## Used Components : 
- ESP32
- Bread Board 
- Photoresistor (LDR) sensor
- DHT-22 sensor 
- Potentiometer slide
- Servo Motor
- Buzzer
- LED RGB 
- Led-Bar-Graph

**Screenshot of the components connected with each other in the Wokwi simulation website:**
![Screenshot of the componenets connected with each other in the wokwi simulation website.](https://github.com/badrhrm/S6_IoT-final_project-smart_home-wokwi_simulation/assets/106437361/c4c42eec-d2c5-4154-9c36-3d22b4d60343)



## What is it and How does it work :
### Scenarios :
There are 3 smart home simulation scenarios, each one of them simulates a different concept:
  * scenario 1 :
    - we use slide-potentiometer to dynamically adjust the CO2 level.
    - the servo motor opens and closes the window dynamically depending on level of humidity, temperature and CO2 level.
    - when CO2 level reaches danger threshold the buzzer is activated and whatsapp bot sends warning messages.
  * scenario 2 :
    - we use DHT-22 sensor to dynamically adjust humidity and temperature level.
    - if temperature level surpasses the climatisation threshold then we activate the climatiseur (LED RGB Blue).
    - if humidity level subceedes (inferior to) the chauffage threshold then we activate the chauffage (LED RGB Red).
  * scenario 3 :
    - we use photoresistor (LDR) sensor to dynamically adjust the light level.
    - we activate certain segments of the LED bar graph depending on the level of light captured by LDR sensor.
### Integrating ThingSpeak with Wokwi :
Using ThingSpeak's visualization tools to show temperature, humidity and CO2 values in real time and creating graphs to show variations over time.

**Screenshot of one of these type of graphs in ThingSpeak :**
![Screenshot of one of these type of graphs in ThingSpeak.](https://github.com/badrhrm/S6_IoT-final_project-smart_home-wokwi_simulation/assets/106437361/3f520bd9-1399-45c0-b0c7-b4584b26e42a)

### HTML PAGE :
A simple HTML page to show some of the ThingSpeak graphs and allow export of data either as a CSV or SQL file for further data analysis.


## Limitations :
- Low performance due to running it in a simulation website. 
- SQL download option does not work.


## Simulation :
[Wokwi Simulation Link ](https://wokwi.com/projects/397986526422680577).
