// 定义交通灯引脚
int redPin = 11;    // 红灯连接数字引脚11
int yellowPin = 10; // 黄灯连接数字引脚10
int greenPin = 9;   // 绿灯连接数字引脚9

void setup() {
  // 设置所有LED引脚为输出模式
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  // 绿灯亮5秒
  digitalWrite(greenPin, HIGH);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
  delay(5000);
  
  // 绿灯熄灭
  digitalWrite(greenPin, LOW);
  
  // 黄灯闪烁3次，每次间隔1秒
  for(int i = 0; i < 3; i++) {
    digitalWrite(yellowPin, HIGH);
    delay(1000);
    digitalWrite(yellowPin, LOW);
    delay(1000);
  }
  
  // 红灯亮5秒
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowPin, LOW);
  delay(5000);
  
  // 红灯熄灭，准备下一轮循环
  digitalWrite(redPin, LOW);
}
