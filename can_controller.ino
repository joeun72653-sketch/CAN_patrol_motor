#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

#define DEFAULT_PWM 150

void sendMotorCommand(byte enable, byte direction, byte pwm)
{
  byte data[3];

  data[0] = enable;
  data[1] = direction;
  data[2] = pwm;

  byte result = CAN.sendMsgBuf(
    0x100,
    0,
    3,
    data
  );

  if (result == CAN_OK)
  {
    Serial.print("[CAN TX] ");

    Serial.print("ENABLE=");
    Serial.print(enable);

    Serial.print(" DIR=");
    Serial.print(direction);

    Serial.print(" PWM=");
    Serial.println(pwm);
  }
  else
  {
    Serial.println("[CAN] TX ERROR");
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.println("CAN CONTROLLER");

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
  {
    Serial.println("[CAN] MCP2515 INIT OK");
  }
  else
  {
    Serial.println("[CAN] MCP2515 INIT FAIL");
    while (1);
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("[CAN] NORMAL MODE");
  Serial.println("Commands: start / stop / forward / reverse");
}

void loop()
{
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command == "start")
    {
      sendMotorCommand(1, 0, DEFAULT_PWM);
      Serial.println("[CMD] START");
    }

    else if (command == "stop")
    {
      sendMotorCommand(0, 0, 0);
      Serial.println("[CMD] STOP");
    }

    else if (command == "forward")
    {
      sendMotorCommand(1, 0, DEFAULT_PWM);
      Serial.println("[CMD] FORWARD");
    }

    else if (command == "reverse")
    {
      sendMotorCommand(1, 1, DEFAULT_PWM);
      Serial.println("[CMD] REVERSE");
    }

    else
    {
      Serial.println("[CMD] UNKNOWN");
    }
  }
}
