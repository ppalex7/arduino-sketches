#include <ArduinoECCX08.h>
#include <utility/ECCX08DefaultTLSConfig.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  byte cur[128] = {};
  ECCX08.readConfiguration(cur);
  Serial.println("Current configuration");
  printConfiguration(cur);
  Serial.println();

  Serial.print("The ECCX08 on your board is");
  Serial.print(ECCX08.locked() == 1 ? " " : " NOT ");
  Serial.println("locked");

  Serial.println("Default configuration");
  printConfiguration(ECCX08_DEFAULT_TLS_CONFIG);
  Serial.println();
}

void loop() {
  // do nothing
}

void printConfiguration(const byte* conf) {
  Serial.print("Serial number: ");
  printBytes(conf, 0, 3);
  printBytes(conf, 8, 12);
  Serial.println();

  Serial.print("Device revision number: ");
  printBytes(conf, 4, 7);
  Serial.println();

  Serial.print("I2C_Enable: ");
  printBits(conf[14]);
  Serial.println();

  Serial.print("I2C_Address: ");
  printBits(conf[16]);
  Serial.println();

  Serial.print("OTPmode: ");
  printByte(conf[18]);
  Serial.println();

  Serial.print("ChipMode: ");
  printBits(conf[19]);
  Serial.println();

  printSlotConfigHeader();
  for (int i = 0; i < 16; i++) {
    printSlotConfig(i, conf, 20 + 2*i);
  }
  printSlotLine();

  Serial.print("Counter<0>: ");
  printBytes(conf, 52, 59);
  Serial.println();
  Serial.print("Counter<1>: ");
  printBytes(conf, 60, 67);
  Serial.println();

  Serial.print("LockValue: ");
  printByte(conf[86]);
  Serial.println();

  Serial.print("LockConfig: ");
  printByte(conf[87]);
  Serial.println();

  Serial.print("SlotLocked: ");
  printBits(conf[88]);
  Serial.print("  ");
  printBits(conf[89]);
  Serial.println();

}

void printByte(byte b) {
  if (b < 16) {
    Serial.print("0");
  }
  String r = String(b, HEX);
  r.toUpperCase();
  Serial.print(r);
}

void printBits(byte b) {
  for (int i = 7; i >= 0; i--) {
    byte bit = (b >> i) & 0x01;
    Serial.print(bit);
    if (i % 4 == 0) {
      Serial.print(" ");
    }
  }
}

void printBytes(const byte* arr, int fromIdx, int toIdx) {
  for (int i = fromIdx; i <= toIdx; i++) {
    printByte(arr[i]);
  }
}

void printSlotLine() {
  Serial.println("+------+-------------+----------+----------+-------------+------------+-------+---------+--------+------------------|---------|-----------|----------|---------|---------|---------|");
}

void printSlotConfigHeader() {
  printSlotLine();
  Serial.println("| Slot | WriteConfig | WriteKey | IsSecret | EncryptRead | LimitedUse | NoMac | ReadKey | X509id | IntrusionDisable | ReqAuth | ReqRandom | Lockable | KeyType | PubInfo | Private |");
  printSlotLine();
}

void printSlotConfig(int num, const byte* conf, int start) {
  Serial.print("|   ");
  if (num < 10) {
    Serial.print("0");
  }
  Serial.print(num);
  Serial.print(" |    ");

  // least (!) significant byte first
  for (int i = 7; i >= 4; i--) {
    byte bit = (conf[start + 1] >> i) & 0x01;
    Serial.print(bit);
    Serial.print(" ");
  }
  Serial.print(" |        ");
  Serial.print(String(conf[start + 1] & 0x0F, HEX));
  Serial.print(" |        ");
  Serial.print((conf[start] >> 7) & 0x01);
  Serial.print(" |           ");
  Serial.print((conf[start] >> 6) & 0x01);
  Serial.print(" |          ");
  Serial.print((conf[start] >> 5) & 0x01);
  Serial.print(" |     ");
  Serial.print((conf[start] >> 4) & 0x01);
  Serial.print(" |       ");
  Serial.print(String(conf[start] & 0x0F, HEX));
  // KeyConfig
  Serial.print(" |      ");
  Serial.print(String(conf[start + 77] >> 6, HEX));
  Serial.print(" |                ");
  Serial.print((conf[start + 77] >> 4) & 0x01);
  Serial.print(" |       ");
  Serial.print((conf[start + 76] >> 7) & 0x01);
  Serial.print(" |         ");
  Serial.print((conf[start + 76] >> 6) & 0x01);
  Serial.print(" |        ");
  Serial.print((conf[start + 76] >> 5) & 0x01);
  Serial.print(" |     ");
  Serial.print((conf[start + 76] >> 4) & 0x01);
  Serial.print((conf[start + 76] >> 3) & 0x01);
  Serial.print((conf[start + 76] >> 2) & 0x01);
  Serial.print(" |       ");
  Serial.print((conf[start + 76] >> 1) & 0x01);
  Serial.print(" |       ");
  Serial.print((conf[start + 76] >> 0) & 0x01);
  Serial.println(" |");
}
