#include <Servo.h>

// 硬件引脚定义
// 电源/状态
const int POWER_SWITCH = A4;
const int GREEN_LED = 2;
const int YELLOW_LED = 4;
const int RED_LED = 7;
const int BUZZER = 8;

// 双超声波传感器
const int TRIG1 = A0, ECHO1 = A1; // 传感器1
const int TRIG2 = A2, ECHO2 = A3; // 传感器2

// 舵机（腿部+机械臂）
Servo leg1;  // 左前腿部
Servo leg2;  // 右后腿部
Servo leg3;  // 左后腿部
Servo leg4;  // 右前腿部
Servo arm1;  // 机械臂1
Servo arm2;  // 机械臂2

// 电机引脚
const int MOTOR_IN1 = 12, MOTOR_IN2 = 13;

// 状态变量
bool isPowerOn = false;
float dist1, dist2; // 两个超声波距离


// 电源检测函数
void checkPower() {
  bool switchState = digitalRead(POWER_SWITCH);
  // 开机：开关闭合（LOW）且当前未开机
  if (switchState == LOW && !isPowerOn) {
    isPowerOn = true;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    Serial.println("Select Mode: ");
    delay(1000);
  }
  // 关机：开关断开（HIGH）且当前已开机
  else if (switchState == HIGH && isPowerOn) {
    isPowerOn = false;
    // 关机状态：所有硬件复位
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    noTone(BUZZER);
    stopAll();
  }
}


// 超声波测距函数
float measureDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH) / 58.0; // 转换为厘米
}


// 地面步行模式
void groundWalkMode() {
  // 玩具电机正转5s
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  delay(5000);
  stopAll(); // 动作完成后停止所有硬件
}


// 月面漫步模式
void moonWalkMode() {
  digitalWrite(YELLOW_LED, HIGH);
  
  // 左前+右后腿部舵机转90度，延时1s
  leg1.write(90);
  leg2.write(90);
  delay(1000);
  
  // 左后+右前腿部舵机转90度（重复2次）
  for (int i = 0; i < 2; i++) {
    leg3.write(90);
    leg4.write(90);
    delay(1000);
    leg3.write(0);
    leg4.write(0);
    delay(1000);
  }
  
  // 舵机复位+黄色LED灭
  leg1.write(0);
  leg2.write(0);
  digitalWrite(YELLOW_LED, LOW);
}


// 平衡+机械臂模式
void balanceAndArmMode() {
  // 四个腿部舵机转45度（保持平衡）
  leg1.write(45);
  leg2.write(45);
  leg3.write(45);
  leg4.write(45);
  
  // 机械臂动作
  arm1.write(30);
  arm2.write(60);
  
  // 红色LED闪烁2s（4次闪烁）
  for (int i = 0; i < 4; i++) {
    digitalWrite(RED_LED, HIGH);
    delay(250);
    digitalWrite(RED_LED, LOW);
    delay(250);
  }
  
  // 蜂鸣器滴1声
  tone(BUZZER, 1000);
  delay(200);
  noTone(BUZZER);
  
}


// 机械臂问候模式
void armGreetMode() {
  // 机械臂转45度→复位（重复2次）
  for (int i = 0; i < 2; i++) {
    arm1.write(45);
    arm2.write(45);
    delay(1000);
    arm1.write(0);
    arm2.write(0);
    delay(500);
  }
  
  // 蜂鸣器滴2声
  tone(BUZZER, 1000);
  delay(200);
  noTone(BUZZER);
  delay(200);
  tone(BUZZER, 1000);
  delay(200);
  noTone(BUZZER);
}


// 停止所有硬件函数
void stopAll() {
  // 电机停止
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  
  // 所有舵机复位
  leg1.write(0);
  leg2.write(0);
  leg3.write(0);
  leg4.write(0);
  arm1.write(0);
  arm2.write(0);
}

// 故障模式（进阶任务）
void faultMode() {
  // 四个腿部舵机旋转90度→机器人趴下
  leg1.write(90);
  leg2.write(90);
  leg3.write(90);
  leg4.write(90);
  // 红色LED亮起，黄色LED闪烁，蜂鸣器报警
  digitalWrite(RED_LED, HIGH);
  tone(BUZZER, 1500); // 报警声频率
  digitalWrite(YELLOW_LED, HIGH);
  delay(500);
  digitalWrite(YELLOW_LED, LOW);
  delay(500);
  noTone(BUZZER);
  // 故障后保持趴下状态，直到关机/退出
}


// 串口输入处理函数（进阶任务）
void handleSerialInput() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case '1':
        balanceAndArmMode(); 
        groundWalkMode(); // 地面步行
        break;
      case '2':
        balanceAndArmMode();
        moonWalkMode();  // 月面漫步
        break;
      case '3':
        while(isPowerOn){
          faultMode();     // 故障模式
          checkPower();
        }  
        break;
      default:
        Serial.println("Invalid input! Please enter 1/2/3");
        break;
    }
  }
}

void setup() {
  // 引脚模式初始化
  pinMode(POWER_SWITCH, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // 舵机绑定引脚+初始复位
  leg1.attach(3);  leg1.write(0);
  leg2.attach(5);  leg2.write(0);
  leg3.attach(6);  leg3.write(0);
  leg4.attach(9);  leg4.write(0);
  arm1.attach(10); arm1.write(0);
  arm2.attach(11); arm2.write(0);

  // 关机状态：红灯常亮
  digitalWrite(RED_LED, HIGH);
  Serial.begin(9600);
}


void loop() {
  // 电源状态检测
  checkPower();

  // 仅开机状态执行后续逻辑
  if (isPowerOn) {
    // 读取双超声波距离
    dist1 = measureDistance(TRIG1, ECHO1);
    dist2 = measureDistance(TRIG2, ECHO2);
    
    // 串口实时输出距离
    Serial.print("dist1: "); Serial.print(dist1);
    Serial.print("cm | dist2: "); Serial.println(dist2);

    // 地面步行模式
    if (dist1 > 100 && dist2 > 30) {
      groundWalkMode();
      while(dist1 > 100 && dist2 > 30 && isPowerOn){
        dist1 = measureDistance(TRIG1, ECHO1);
        dist2 = measureDistance(TRIG2, ECHO2);
        checkPower();
        delay(20);
      }
    }
    // 月面漫步模式
    else if (dist1 < 50 && dist2 > 30) {
      moonWalkMode();
      while(dist1 < 50 && dist2 > 30 && isPowerOn){
        dist1 = measureDistance(TRIG1, ECHO1);
        dist2 = measureDistance(TRIG2, ECHO2);
        checkPower();
        delay(20);
      }
    }
    // 平衡+机械臂模式
    else if (dist1 < 50 && dist2 < 30) {
      balanceAndArmMode();
      while(dist1 < 50 && dist2 < 30 && isPowerOn){
        dist1 = measureDistance(TRIG1, ECHO1);
        dist2 = measureDistance(TRIG2, ECHO2);
        checkPower();
        delay(20);
      }
    }
    // 机械臂问候模式
    else if (dist1 > 100 && dist2 < 30) {
      armGreetMode();
      while(dist1 > 100 && dist2 < 30 && isPowerOn){
        dist1 = measureDistance(TRIG1, ECHO1);
        dist2 = measureDistance(TRIG2, ECHO2);
        checkPower();
        delay(20);
      }
    }
    // 进阶任务：超声波1距离50-100cm→串口输入控制
    if (dist1 > 50 && dist1 < 100) {
      handleSerialInput(); // 处理串口输入
    }

    delay(300);
  }
}
