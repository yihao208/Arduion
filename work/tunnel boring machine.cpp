#include <Servo.h>

// 引脚定义
// 电源/状态引脚
const int POWER_SWITCH = A3;    // 滑动开关引脚
const int GREEN_LED = A4;       // 电源绿灯
const int RED_LED = A5;         // 状态红灯
const int BUZZER = 2;           // 蜂鸣器引脚

// 传感器引脚
const int TRIG_PIN = A0;        // 超声波TRIG
const int ECHO_PIN = A1;        // 超声波ECHO
const int TEMP_PIN = A2;        // 温度传感器

// 电机引脚（4个电机，8引脚）
const int M1_IN1 = 6, M1_IN2 = 7;  // 电机1
const int M2_IN1 = 8, M2_IN2 = 9;  // 电机2
const int M3_IN1 = 10, M3_IN2 = 11;// 电机3
const int M4_IN1 = 12, M4_IN2 = 13;// 电机4

// 舵机对象
Servo servo1;  // 舵机1（3脚）
Servo servo2;  // 舵机2（5脚）

// 状态变量
bool isPowerOn = false;         // 开机状态
unsigned long motorRunTime = 0; // 电机运转计时
bool isServoActionDone = true;  // 舵机动作完成标记
bool isFaultMode = false;       // 故障模式标记
bool isMotorRunning = false;    // 电机是否处于运行状态

// 1. 电源状态检测函数
void checkPowerState() {
  bool switchState = digitalRead(POWER_SWITCH);
  //Serial.println(switchState);
  //Serial.println(isPowerOn);
  if (switchState == LOW && !isPowerOn) {
    // 开机：绿灯亮
    isPowerOn = true;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    Serial.println("Please input: ");  // 进阶任务开机提示
    delay(1000);
  } else if (switchState == HIGH && isPowerOn) {
    // 关机：红灯亮1s+蜂鸣器响1s
    isPowerOn = true;
    isFaultMode = false;
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 1000); delay(1000);
    noTone(BUZZER);
    isPowerOn = false;
  }
}

// 2. 超声波测距函数
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(8);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) / 58.0;
}

// 3. 温度读取函数
int readTemperature() {
  int adc = analogRead(TEMP_PIN);
  float voltage = adc * (5.0 / 1024.0);
  return (voltage - 0.5) * 100;
}

// 4. 电机控制工具函数（单个电机）
void runMotor(int in1, int in2, int state1, int state2) {
  digitalWrite(in1, state1);
  digitalWrite(in2, state2);
}
void stopMotor(int in1, int in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// 5. 所有电机停止函数
void stopAllMotors() {
  stopMotor(M1_IN1, M1_IN2);
  stopMotor(M2_IN1, M2_IN2);
  stopMotor(M3_IN1, M3_IN2);
  stopMotor(M4_IN1, M4_IN2);
}

// 6. 舵机动作执行函数
void executeServoAction() {
  // 舵机1：0°→60°→0°→120°→0°→180°
  servo1.write(60); delay(1000);
  servo1.write(0); delay(1000);
  servo1.write(120); delay(1000);
  servo1.write(0); delay(1000);
  servo1.write(180); delay(1500);
  servo1.write(0); delay(1500);

  // 舵机2：180°→120°→180°→60°→180°→0°
  servo2.write(120); delay(1000);
  servo2.write(180); delay(1000);
  servo2.write(60); delay(1000);
  servo2.write(180); delay(1000);
  servo2.write(0); delay(1500);
  servo2.write(180); delay(1500);
}

// 7. 串口输入检测（进阶任务：故障模式）
void checkSerialInput() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '1') {
      isFaultMode = true;
      stopAllMotors();
    }
  }
}

void setup() {
  // 引脚初始化
  pinMode(POWER_SWITCH, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // 电机引脚初始化
  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);
  stopAllMotors();  // 电机初始停止

  // 舵机初始化
  servo1.attach(3); servo1.write(0);
  servo2.attach(5); servo2.write(180);
  
  digitalWrite(RED_LED, HIGH);

  Serial.begin(9600);
}

void loop() {
  // 电源状态检测
  checkPowerState();

  if (isPowerOn) {
    if (isFaultMode) {
      // 故障模式：红灯常亮+蜂鸣器交替响
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      tone(BUZZER, 500); delay(500);
      noTone(BUZZER); delay(500);
      return;
    }

    // 基础任务：传感器读取
    float distance = measureDistance();
    int temp = readTemperature();
    // 串口输出状态
    Serial.print("dist: "); Serial.print(distance); Serial.print("cm | temp: "); Serial.println(temp);

    // 温度超温检测：停机
    if (temp >= 80) {
      stopAllMotors();
      isMotorRunning = false;  // 电机停止，更新状态标记
      servo1.write(0); servo2.write(180);
      return;
    }

    // 电机控制逻辑
    if (distance > 20 && distance < 50) {
      // 20-50cm：电机1/2转动（前进）
      runMotor(M1_IN1, M1_IN2, HIGH, LOW);  // M1正转
      runMotor(M2_IN1, M2_IN2, HIGH, LOW);  // M2正转
      stopMotor(M3_IN1, M3_IN2);            // M3/4停止
      stopMotor(M4_IN1, M4_IN2);
      isServoActionDone = true;
      isMotorRunning = false;  // 标记电机为停止状态
      
    } else if (distance < 20) {
      // <20cm：电机1/2继续，电机3/4转动（刀盘+出土）
      runMotor(M1_IN1, M1_IN2, HIGH, LOW);
      runMotor(M2_IN1, M2_IN2, HIGH, LOW);
      runMotor(M3_IN1, M3_IN2, HIGH, LOW);  // M3正转（刀盘）
      runMotor(M4_IN1, M4_IN2, HIGH, LOW);  // M4同步（出土）
      
      // 仅当电机从停止→启动时，才更新时间
      if (!isMotorRunning) {
        motorRunTime = millis();            // 记录电机启动时间
        isServoActionDone = false;
        isMotorRunning = true;              // 标记电机为运行状态
      }
    } else {
      // >50cm：电机全部停止
      stopAllMotors();
      isServoActionDone = true;
      isMotorRunning = false;  // 标记电机为停止状态
    }

    // 舵机动作：电机运转3s后执行
    if (!isServoActionDone && (millis() - motorRunTime >= 3000)) {
      executeServoAction();
      isServoActionDone = true;
      isMotorRunning = false;  // 标记电机为停止状态
    }

  } else {
    // 关机状态：设备复位
    stopAllMotors();
    isMotorRunning = false;  // 复位电机运行状态
    servo1.write(0); servo2.write(180);
    digitalWrite(GREEN_LED, LOW);
  }

  // 进阶任务：串口输入检测（故障模式）
  checkSerialInput();
}
