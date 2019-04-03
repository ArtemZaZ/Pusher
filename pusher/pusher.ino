#include <Servo.h>
#include <stdint.h>

#define SHARP   // версия датчика расстояния

#define SERVO_CH    9 // канал сервы
#define DIST_SENSOR_CH  A0  // канал датчика дистанции

#define SEND_DELAY  400  // время задержки отправки данных
#define PUSH_BYTE   0x3C  // байт от которого будут толкаться кубики
#define SERVO_PUSH_DELAY  1000  // время задержки сервы при толчке

#define VOLTAGE   5

Servo mservo;  // экземпляр сервы

void distanceSensorInit()   // инициализация датчика расстояния
{
  pinMode(DIST_SENSOR_CH, INPUT); 
}

uint8_t distanceSensorGetData()
{
  static uint8_t result;  
  result = 0xFF;  // при каждом запуске ф-ии выставляем максимальную дистанцию
  #ifdef SHARP
    result = (uint8_t)(29.988f * pow(((float)analogRead(DIST_SENSOR_CH)*VOLTAGE/1024.f), -1.173f)); // https://github.com/guillaume-rico/SharpIR
  #endif
  return result;
}

void push()   // отталкиваем кубик
{
  mservo.write(180);  // 180 градусов
  delay(SERVO_PUSH_DELAY);   // задержка
  mservo.write(0);  // 0 градусов  
}

void setup() 
{
  mservo.attach(SERVO_CH, 800, 2200);  // привязка к выводу
  distanceSensorInit();   // инициализация датчика расстояния
  mservo.write(0); // ноль градусов
  Serial.begin(9600); // инициализация уарта
}

void loop() 
{
  static uint32_t lastSendTime = 0; // время последней отправки данных
  if(millis() - lastSendTime > SEND_DELAY)  // если время превышает порог задержки на отправку
  {
    Serial.println(distanceSensorGetData());  // отправляем байт данных
    lastSendTime = millis();
  }
  
  if(Serial.available() > 0)  // пришли какие-то данные
  {
    if(Serial.read() == PUSH_BYTE)  // пришел нужный байт
    {
      push();
    }
  }
  
}
