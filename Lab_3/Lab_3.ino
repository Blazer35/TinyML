/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

//#include <TensorFlowLite.h>
#include <Chirale_TensorFlowLite.h>
#include <Arduino_LSM9DS1.h>


#include "model.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 14 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

// Gesture labels

} 

// //should quantize the accelerometer values before feeding them into the model:
// int8_t quantized_value = (int8_t)((value / scale) + zero_point);
// TfLiteTensor* input = interpreter->input(0);
// float scale = input->params.scale;
// int zero_point = input->params.zero_point;

// float scale = input->params.scale;
// int zero_point = input->params.zero_point;



 // namespace
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized.");

    // Load model from array
  model = tflite::GetModel(gesture_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model version mismatch.");
    while (1);
  }
  static tflite::AllOpsResolver resolver;

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Tensor allocation failed!");
    while (1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  // Serial.println("Setup complete.");
  // Serial.print("Number of dimensions: ");
  // Serial.println(input->dims->size);
  // Serial.print("Dim 1 size: ");
  // Serial.println(input->dims->data[0]);
  // Serial.print("Dim 2 size: ");
  // Serial.println(input->dims->data[1]);
  // Serial.print("Input type: ");
  // Serial.println(input->type);
  // Serial.println("--- Setup Finished ---");


}



void loop() {
  // put your main code here, to run repeatedly:

   float ax, ay, az;
   Serial.println("--- Loop Iteration ---");

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    // Serial.print("Raw Accel: X="); Serial.print(ax, 3); // Print with 3 decimal places
    // Serial.print(", Y="); Serial.print(ay, 3);
    // Serial.print(", Z="); Serial.println(az, 3);

    // Get quantization params
    float scale = input->params.scale;
    int zero_point = input->params.zero_point;

    //Scaling
    float multiplier = 10.0;  // Adjust this to match training data scale
    float ax_scaled = ax * multiplier;
    float ay_scaled = ay * multiplier;
    float az_scaled = az * multiplier;


    // --- Debug Print: Scaled Accelerometer Values ---
    // Serial.print("Scaled Accel: X="); Serial.print(ax_scaled, 2); // Print with 2 decimal places
    // Serial.print(", Y="); Serial.print(ay_scaled, 2);
    // Serial.print(", Z="); Serial.println(az_scaled, 2);

    // Quantize input
    input->data.int8[0] = (int8_t)((ax_scaled / scale) + zero_point);
    input->data.int8[1] = (int8_t)((ay_scaled / scale) + zero_point);
    input->data.int8[2] = (int8_t)((az_scaled / scale) + zero_point);

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
      Serial.println("Inference failed!");
      return;
    }

    int best_index = -1;
    float best_score = 0.0;

    Serial.println("Scores:"); 

    for (int i = 0; i < available_classes_num; ++i) { // Correctly uses available_classes_num
      // Get the raw quantized score for the current class
      int8_t raw_score = output->data.int8[i];

      // Dequantize the score: (raw_score - zero_point) * scale
      float score = (raw_score - output->params.zero_point) * output->params.scale;

      // Serial.print("  "); // Indent for readability
      // Serial.print((const char*)available_classes[i]); // Print class label
      // Serial.print(": ");
      // Serial.println(score, 4);

      // Find the class with the highest score
      if (score > best_score) {
        best_score = score;
        best_index = i;
      }
    }

    if (best_index >= 0 && best_score > 0.7) {
      Serial.print((const char*)available_classes[i])
    } else {
      // If no strong prediction is made (score not above threshold),
      // we can consider printing a default "Idle" or "No clear gesture".
      // Use strcmp to robustly check if the highest score was for the "idle" class.
      //bool idle_is_highest = (best_index != -1 && strcmp(available_classes[best_index], "idle") == 0); // <-- Explicit cast added here

      if (idle_is_highest) {
        // If "idle" was the highest score but below confidence threshold
        //Serial.println("Prediction: idle (low confidence)");
      } else {
        // If any other gesture was highest, but below confidence threshold, or no gesture was clear
        Serial.println("uncertain");
      }
    }

    delay(1000);
  }

}
