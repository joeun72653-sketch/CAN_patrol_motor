#include <SPI.h>
#include <mcp_can.h>

// ===============================
// MCP2515 CAN 설정
// ===============================
#define CAN_CS 10

MCP_CAN CAN(CAN_CS);


// ===============================
// BTS7960 핀 설정
// ===============================
#define RPWM 5
#define LPWM 6
#define R_EN 7
#define L_EN 8


// ===============================
// 모터 정지
// ===============================
void motorStop()
{
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 0);

  Serial.println("[MOTOR] STOP");
}


// ===============================
// 모터 정회전
// ===============================
void motorForward(byte pwm)
{
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  analogWrite(RPWM, pwm);
  analogWrite(LPWM, 0);

  Serial.print("[MOTOR] FORWARD PWM=");
  Serial.println(pwm);
}


// ===============================
// 모터 역회전
// ===============================
void motorReverse(byte pwm)
{
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  analogWrite(RPWM, 0);
  analogWrite(LPWM, pwm);

  Serial.print("[MOTOR] REVERSE PWM=");
  Serial.println(pwm);
}


// ===============================
// SETUP
// ===============================
void setup()
{
  Serial.begin(115200);

  // BTS7960 핀 설정
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);

  // BTS7960 Enable
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // 시작 시 안전하게 모터 정지
  motorStop();

  Serial.println("==============================");
  Serial.println("CAN MOTOR CONTROLLER");
  Serial.println("==============================");

  // MCP2515 초기화
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
  {
    Serial.println("[CAN] MCP2515 INIT OK");
  }
  else
  {
    Serial.println("[CAN] MCP2515 INIT FAIL");

    motorStop();

    while (1);
  }

  // CAN Normal Mode
  CAN.setMode(MCP_NORMAL);

  Serial.println("[CAN] NORMAL MODE");
  Serial.println("[CAN] Waiting for motor command...");
}


// ===============================
// LOOP
// ===============================
void loop()
{
  // CAN 메시지가 들어왔는지 확인
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    unsigned long canId;
    byte len = 0;
    byte buf[8];

    // CAN 메시지 읽기
    CAN.readMsgBuf(&canId, &len, buf);

    // Motor Command ID = 0x100
    if (canId == 0x100 && len >= 3)
    {
      // CAN 데이터 해석
      byte enable    = buf[0];
      byte direction = buf[1];
      byte pwm       = buf[2];

      // 수신 데이터 출력
      Serial.print("[CAN RX] ID=0x");
      Serial.print(canId, HEX);

      Serial.print(" ENABLE=");
      Serial.print(enable);

      Serial.print(" DIR=");
      Serial.print(direction);

      Serial.print(" PWM=");
      Serial.println(pwm);


      // ===============================
      // 모터 제어
      // ===============================

      // Enable = 0 → STOP
      if (enable == 0)
      {
        motorStop();
      }

      // Enable = 1 → 모터 구동
      else
      {
        // Direction = 0 → 정회전
        if (direction == 0)
        {
          motorForward(pwm);
        }

        // Direction = 1 → 역회전
        else if (direction == 1)
        {
          motorReverse(pwm);
        }

        // 잘못된 Direction
        else
        {
          motorStop();
          Serial.println("[ERROR] Invalid direction");
        }
      }
    }
  }
}
