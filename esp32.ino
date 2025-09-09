#include <Arduino.h>
#include "model_data.h"
#include "TensorFlowLite.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
#define PH_SENSOR_PIN      34
#define TURB_SENSOR_PIN    35
#define DO_SENSOR_PIN      32
#define FLOW_SENSOR_PIN    27
#define ONE_WIRE_BUS       4
#define CHLORINE_PUMP_PIN  25
#define UV_LAMP_PIN        26

volatile int pulseCount = 0;
float flowRate = 0.0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// TensorFlow Lite arena
const int kArenaSize = 8 * 1024;
uint8_t tensor_arena[kArenaSize];

const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// --- Calibration Functions ---

float readPH(int pin) {
  int raw = analogRead(pin);
  float voltage = (raw / 4095.0) * 3.3; 
  float phValue = 7 + (2.5 - voltage) * 3.5; // calibrated formula
  return phValue;
}

float readTurbidity(int pin) {
  int raw = analogRead(pin);
  float voltage = (raw / 4095.0) * 3.3;
  float ntu = -1120.4 * pow(voltage, 2) + 5742.3 * voltage - 4352.9; 
  if (ntu < 0) ntu = 0;
  return ntu;
}

float readDO(int pin) {
  int raw = analogRead(pin);
  float voltage = (raw / 4095.0) * 3.3;
  float doValue = voltage * 5.0; // simple scaling, calibrate in real water samples
  return doValue;
}

float readFlow() {
  pulseCount = 0;
  interrupts();
  delay(1000);
  noInterrupts();
  float flowRate = (pulseCount / 7.5); // L/min
  return flowRate;
}

void setup() {
  Serial.begin(115200);

  pinMode(CHLORINE_PUMP_PIN, OUTPUT);
  pinMode(UV_LAMP_PIN, OUTPUT);
  digitalWrite(CHLORINE_PUMP_PIN, LOW);
  digitalWrite(UV_LAMP_PIN, LOW);

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);

  sensors.begin();

  // Load model
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("❌ Model schema mismatch!");
    while (1);
  }

  static tflite::AllOpsResolver resolver;
  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kArenaSize);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("❌ AllocateTensors() failed!");
    while (1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ Setup complete. Running with calibrated sensors.");
}

void loop() {
  // --- Read calibrated sensor values ---
  float phValue = readPH(PH_SENSOR_PIN);
  float turbidity = readTurbidity(TURB_SENSOR_PIN);
  float dissolvedOxygen = readDO(DO_SENSOR_PIN);
  float flowRate = readFlow();

  sensors.requestTemperatures();
  float airTemp = sensors.getTempCByIndex(0);
  float waterTemp = sensors.getTempCByIndex(1);

  // E. coli proxy (from turbidity & DO)
  float ecoli = turbidity * 0.1 - dissolvedOxygen * 0.05; 

  // --- Fill input tensor ---
  input->data.f[0] = ecoli;
  input->data.f[1] = phValue;
  input->data.f[2] = dissolvedOxygen;
  input->data.f[3] = flowRate;
  input->data.f[4] = airTemp;
  input->data.f[5] = waterTemp;
  input->data.f[6] = turbidity;

  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("❌ Inference failed!");
    return;
  }

  float prediction = output->data.f[0];
  Serial.print("Prediction: ");
  Serial.println(prediction);

  if (prediction > 0.5) {
    Serial.println("⚠️ Contamination detected → Chlorine Pump ON, UV OFF");
    digitalWrite(CHLORINE_PUMP_PIN, HIGH);
    digitalWrite(UV_LAMP_PIN, LOW);
  } else {
    Serial.println("✅ Water safe → UV Sterilization ON, Pump OFF");
    digitalWrite(CHLORINE_PUMP_PIN, LOW);
    digitalWrite(UV_LAMP_PIN, HIGH);
  }

  delay(5000);
}
