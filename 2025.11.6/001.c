// 定义LED引脚
int ledPins[] = {2, 3, 4}; // 使用数字引脚2,3,4连接三个LED
int ledCount = 3; // LED数量

void setup() {
  // 设置所有LED引脚为输出模式
  for(int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // 流水灯效果
  for(int i = 0; i < ledCount; i++) {
    digitalWrite(ledPins[i], HIGH); // 点亮当前LED
    delay(500); // 等待0.5秒
    digitalWrite(ledPins[i], LOW); // 熄灭当前LED
  }
}
