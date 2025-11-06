// 定义引脚
const int pot1Pin = A0;    // 第一个电位计（控制小灯泡亮度）
const int pot2Pin = A1;    // 第二个电位计（控制RGB颜色）
const int rgbRPin = 3;     // RGB小灯红色引脚
const int rgbGPin = 6;     // RGB小灯绿色引脚
const int rgbBPin = 5;     // RGB小灯蓝色引脚
const int ledPin = 9;      // 小灯泡的PWM引脚

void setup() {
  // 初始化引脚模式
  pinMode(ledPin, OUTPUT);
  pinMode(rgbRPin, OUTPUT);
  pinMode(rgbGPin, OUTPUT);
  pinMode(rgbBPin, OUTPUT);
  Serial.begin(9600); // 波特率9600
}

void loop() {
  // 读取第一个电位计的模拟值
  int pot1Value = analogRead(pot1Pin);
  //数据转换
  int led_light = pot1Value / 102.4 * 25.6; 
  //点亮灯泡
  analogWrite(ledPin, led_light); 

  // 读取第二个电位计的模拟值，划分区间控制RGB颜色切换
  int pot2Value = analogRead(pot2Pin);
  if (pot2Value < 341) {    // 0-340：红色
    analogWrite(rgbRPin, 255);
    analogWrite(rgbGPin, 0);
    analogWrite(rgbBPin, 0);
  } else if (pot2Value < 682) { // 341-681：绿色
    analogWrite(rgbRPin, 0);
    analogWrite(rgbGPin, 255);
    analogWrite(rgbBPin, 0);
  } else {                  // 682-1023：蓝色
    analogWrite(rgbRPin, 0);
    analogWrite(rgbGPin, 0);
    analogWrite(rgbBPin, 255);
  }
}
