#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10

MCP_CAN CAN(CAN_CS);

void setup()
{
  Serial.begin(115200);

  Serial.println("CAN TX TEST");

  // MCP2515 초기화
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
  {
    Serial.println("MCP2515 INIT OK");
  }
  else
  {
    Serial.println("MCP2515 INIT FAIL");

    while (1);
  }

  // CAN 정상 통신 모드
  CAN.setMode(MCP_NORMAL);

  Serial.println("CAN NORMAL MODE");
}

void loop()
{
  byte data[1];

  data[0] = 0x01;

  byte result = CAN.sendMsgBuf(
    0x100,    // CAN ID
    0,        // Standard CAN
    1,        // 데이터 길이
    data
  );

  if (result == CAN_OK)
  {
    Serial.println("TX -> ID:0x100 DATA:0x01");
  }
  else
  {
    Serial.println("CAN TX ERROR");
  }

  delay(1000);
}
