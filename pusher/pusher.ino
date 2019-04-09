#include <Servo.h>
#include <stdint.h>

#define SHARP   // версия датчика расстояния

#define SERVO_CH    9 // канал сервы
#define DIST_SENSOR_CH  A0  // канал датчика дистанции

#define SEND_DELAY  500  // время задержки отправки данных
#define PUSH_BYTE   0xAA  // байт от которого будут толкаться кубики
#define SERVO_PUSH_DELAY  800  // время задержки сервы при толчке

#define VOLTAGE   5

Servo mservo;  // экземпляр сервы

void distanceSensorInit()   // инициализация датчика расстояния
{
  pinMode(DIST_SENSOR_CH, INPUT); 
}

uint8_t distanceSensorGetData()
{
  static uint8_t result;
  static float temp;  // вспомогательная переменная  
  result = 0xFF;  // при каждом запуске ф-ии выставляем максимальную дистанцию
  #ifdef SHARP
    temp = 29.988f * pow(((float)analogRead(DIST_SENSOR_CH)*VOLTAGE/1024.f), -1.173f); // https://github.com/guillaume-rico/SharpIR
    if (temp > 255)
    {
      result = 255;      
    }
    else
    {
      result = (uint8_t)temp;
    }
  #endif
  return result;
}

void push()   // отталкиваем кубик
{
  mservo.write(20);  // 20 градусов
  delay(SERVO_PUSH_DELAY);   // задержка
  mservo.write(0);  // 0 градусов  
  delay(SERVO_PUSH_DELAY);   // задержка после выкидывания кубика
}

void setup() 
{
  mservo.attach(SERVO_CH, 830, 2200);  // привязка к выводу
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
