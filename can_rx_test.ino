#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

void setup()
{
  Serial.begin(115200);

  Serial.println("CAN RX TEST");

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
  {
    Serial.println("MCP2515 INIT OK");
  }
  else
  {
    Serial.println("MCP2515 INIT FAIL");

    while (1);
  }

  CAN.setMode(MCP_NORMAL);

  Serial.println("CAN NORMAL MODE");
}

void loop()
{
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    unsigned long canId;
    byte len = 0;
    byte buf[8];

    CAN.readMsgBuf(
      &canId,
      &len,
      buf
    );

    Serial.print("RX ID   : 0x");
    Serial.println(canId, HEX);

    Serial.print("RX DATA : ");

    for (byte i = 0; i < len; i++)
    {
      Serial.print("0x");

      if (buf[i] < 0x10)
      {
        Serial.print("0");
      }

      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }

    Serial.println();
    Serial.println("----------------");
  }
}
