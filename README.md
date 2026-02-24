
# 🪄 TinyML Magic Wand

Real-Time Gesture Recognition on Arduino Nano 33 BLE

An end-to-end TinyML project that performs real-time gesture recognition entirely on-device using TensorFlow Lite for Microcontrollers.

This project demonstrates custom dataset collection, model training (Edge Impulse + TensorFlow), model quantization, and deployment to an embedded microcontroller.

---

## 🚀 Overview

This system uses IMU (accelerometer + gyroscope) data from the Arduino Nano 33 BLE to classify hand gestures in real time.

The entire inference pipeline runs locally on the nRF52840 microcontroller — no cloud required.

### Key Features

* Custom time-series dataset collection
* Model training using:

  * Edge Impulse
  * TensorFlow (Google Colab)
* Post-training quantization (int8)
* Real-time embedded inference
* Memory-optimized deployment
* Fully offline operation

---

## 🧠 System Pipeline

```
IMU Data → Windowing → Normalization → Quantized TFLite Model → Gesture Prediction
```

The model processes buffered time-series sensor data and outputs a probability distribution across gesture classes.

---

## 📊 Gesture Classes

Example classes:

* Circle
* Shake
* Up-Down
* Left-Right
* Idle

(Modify this section to match your actual classes.)

---

## 🏗️ Hardware

* **Board:** Arduino Nano 33 BLE
* **MCU:** nRF52840
* **Sensor:** Onboard LSM9DS1 IMU

No external sensors required.

---

## 🧪 Data Collection

* Sampling Rate: 100 Hz
* Window Size: 1 second
* Window Overlap: 50%
* Features Used:

  * Accelerometer (Ax, Ay, Az)
  * Gyroscope (Gx, Gy, Gz)
* Preprocessing:

  * Window buffering
  * Normalization / scaling

Gestures were recorded multiple times to ensure dataset variability and robustness.

---

## 🧠 Model Development

Two models were explored:

### 1️⃣ Edge Impulse Model

* Dense neural network
* Automatically optimized for embedded deployment
* Exported as int8 quantized TFLite model

### 2️⃣ Custom TensorFlow Model (Google Colab)

* Custom-built neural network
* Converted to TensorFlow Lite
* Post-training quantization applied
* Deployed using TensorFlow Lite for Microcontrollers

---

## 📈 Performance Metrics

(Replace with your actual results.)

| Model           | Accuracy | Flash Usage | RAM Usage | Inference Time |
| --------------- | -------- | ----------- | --------- | -------------- |
| Edge Impulse    | 92%      | 48 KB       | 12 KB     | 12 ms          |
| Custom TF Model | 89%      | 52 KB       | 14 KB     | 15 ms          |

* Inference runs fully on-device
* No external communication required
* Real-time classification performance

---

## ⚡ Deployment Details

* Framework: TensorFlow Lite for Microcontrollers
* Quantization: int8
* IDE: Arduino IDE
* Target: nRF52840 (Nano 33 BLE)

The model is compiled into firmware and executed using TFLite Micro inference engine.

---

## 🔧 System Architecture

```
[IMU Sensor]
      ↓
[Window Buffer]
      ↓
[Preprocessing]
      ↓
[TFLite Micro Model]
      ↓
[Gesture Classification]
      ↓
[LED / Serial Output / BLE Command]
```

---

## 📂 Repository Structure

```
magic-wand-tinyml/
├── data/
├── training/
│   ├── edge_impulse_project/
│   └── colab_training.ipynb
├── model/
│   └── model.tflite
├── firmware/
│   └── magic_wand.ino
└── README.md
```

---

## 🎯 Key Learnings

* Deploying ML models under strict memory constraints
* Quantization trade-offs (accuracy vs. model size)
* Real-time embedded inference
* TinyML debugging and optimization
* Bridging machine learning and embedded systems

---


## 🔮 Future Improvements

* Continuous rolling-window detection
* False-trigger reduction logic
* BLE integration with mobile dashboard
* Model pruning experiments
* CNN-based architecture comparison
* Latency optimization benchmarking

---

## 📌 Summary

This project demonstrates practical Edge AI deployment by taking a trained ML model and running it efficiently on a constrained embedded device.

It highlights skills in:

* Machine Learning
* Embedded Systems
* Model Optimization
* Real-Time Systems
* TinyML Deployment

