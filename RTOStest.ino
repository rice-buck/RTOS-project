#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_PIN 4
#define BUZZER_PIN 15

QueueHandle_t buttonQueue;

void ledTask(void *parameter) {
  pinMode(LED_PIN, OUTPUT);

  while (true) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    digitalWrite(LED_PIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void buttonTask(void *parameter) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  int buttonState;

  while (true) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      buttonState = 1;
      xQueueSend(buttonQueue, &buttonState, portMAX_DELAY);
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void buzzerTask(void *parameter) {
  pinMode(BUZZER_PIN, OUTPUT);
  int received;

  while (true) {
    if (xQueueReceive(buttonQueue, &received, portMAX_DELAY)) {
      digitalWrite(BUZZER_PIN, HIGH);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void telemetryTask(void *parameter) {
  int received;

  while (true) {
    if (xQueueReceive(buttonQueue, &received, portMAX_DELAY)) {
      Serial.println("Button pressed!");
    }
  }
}

void setup() {

  Serial.begin(115200);

  buttonQueue = xQueueCreate(5, sizeof(int));

  xTaskCreate(
    ledTask,
    "LED Task",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    buttonTask,
    "Button Task",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    buzzerTask,
    "Buzzer Task",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    telemetryTask,
    "Telemetry Task",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop() {
}