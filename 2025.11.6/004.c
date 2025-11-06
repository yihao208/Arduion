// 定义引脚
const int buttonPin = 2;    // 按钮引脚
const int rgbRPin = 3;      // R
const int rgbBPin = 5;      // B
const int rgbGPin = 6;      // G

// 全局变量
int count = 0;  // 计数变量，记录按钮按压次数

void setup() {
  pinMode(buttonPin, INPUT); 
  pinMode(rgbRPin, OUTPUT);
  pinMode(rgbGPin, OUTPUT);
  pinMode(rgbBPin, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // 检测按钮是否按下
  if (digitalRead(buttonPin) == HIGH) {
    count++; // 计数加1
    // 串口显示当前次数
    Serial.println(count); 
    //防止多次计数
    while(digitalRead(buttonPin) == HIGH)
      delay(20); 
     
      // 计数达到8次时额外输出"good!"
      if (count == 8) { 
        Serial.println("good!");
    }
  }   
     // 根据计数控制RGB小灯颜色
   if (count <= 7) {
     // 粉色 
     analogWrite(rgbRPin, 255);
     analogWrite(rgbGPin, 105);
     analogWrite(rgbBPin, 180);
   } else {
     // 橙色 
     analogWrite(rgbRPin, 255);
     analogWrite(rgbGPin, 165);
     analogWrite(rgbBPin, 0);
   }   
}
