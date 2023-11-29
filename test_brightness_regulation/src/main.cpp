#include <Arduino.h>
#include <BH1750.h>

const int ledPinPWM = 2;  // Chọn chân GPIO tương ứng với LED
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

BH1750 brightnessMeasure;

uint8_t ledPower = 0;
const uint16_t targetLux = 200;  // Mức độ rọi mong muốn

// Function prototype
void adjustBrightness(uint16_t targetLux, uint16_t currentLux);

void setup() {
  Serial.begin(115200);
  delay(500);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPinPWM, ledChannel);

  Wire.begin();
  brightnessMeasure.begin();
}

void loop() {
  uint16_t currentLux = brightnessMeasure.readLightLevel();  // Đọc giá trị độ sáng từ cảm biến

  // Điều chỉnh độ sáng LED để duy trì mức độ rọi mong muốn
  adjustBrightness(targetLux, currentLux);

  Serial.print("Current Lux: ");
  Serial.print(currentLux);
  Serial.print(" | LED Brightness: ");
  Serial.println(ledPower);

  delay(500);  // Đợi 2 giây giữa các lần đo
}

void adjustBrightness(uint16_t targetLux, uint16_t currentLux) {
  const int step = 5;  // Bước điều chỉnh độ sáng

  if (currentLux < targetLux) {
    ledPower = min(255, ledPower + step);  // Tăng độ sáng nếu độ sáng hiện tại < độ sáng mong muốn
  } else if (currentLux > targetLux) {
    ledPower = max(0, ledPower - step);  // Giảm độ sáng nếu độ sáng hiện tại > độ sáng mong muốn
  }

  ledcWrite(ledChannel, ledPower);
}
