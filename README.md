# Sangam Suraksha: A Real-Time Water Quality Monitoring & Disinfection System

## Introduction

Sangam Suraksha is an innovative, static, floating IoT-based device designed to combat water pollution at large religious gatherings. Developed to address the critical public health risks associated with microbial contamination, our system provides real-time water quality monitoring and automated, human-safe disinfection. The project's initial focus is on the holy rivers during events like the Kumbh Mela, ensuring that ritual bathing can be performed safely.

Our mission is to safeguard public health and preserve the sanctity of water without disrupting traditions.

## The Problem

Large-scale religious gatherings, while spiritually significant, place immense environmental stress on rivers. The concentration of millions of pilgrims for ritual bathing leads to heavy microbial contamination, particularly from **faecal coliform bacteria**. This poses a severe public health risk, leaving millions vulnerable to waterborne diseases such as Hepatitis, dysentery, and other infections. The lack of real-time data and targeted disinfection is a growing crisis.

## The Solution

Sangam Suraksha tackles this problem with a proactive, multi-layered approach. Our system is built on a simple yet effective five-step cycle:

1. **Detection:** A suite of sensors continuously monitors key water quality parameters.

2. **Testing:** Amperometric biosensors conduct real-time bacterial testing.

3. **Purification:** The disinfection system activates, purifying the water with a combination of chlorine and UV-C sterilization.

4. **Reporting:** Data is sent to an IoT dashboard, providing immediate alerts to authorities and health officials.

## Key Features

* **Real-Time Monitoring:** Continuous monitoring of water quality parameters like pH, TDS, Turbidity, and faecal coliform.

* **Human-Safe Disinfection Logic:** Integrated IR/PIR sensors pause the disinfection process when human presence is detected, ensuring safety during active bathing hours.

* **AI/ML Integration:** The system employs **scikit-learn** with **LazyPredict** for rapid model benchmarking, enabling selection of the best classification/regression model.

* **Wifi Communication:** The system uses **Wifi** for data transfer to a central IoT dashboard, allowing officials to monitor conditions remotely.

* **Rugged & Portable Design:** The device is housed in an **IP67-rated waterproof enclosure** and powered by a rechargeable battery pack, allowing for easy and flexible deployment.

## Technology Stack

### **Sensing & Detection**

* Amperometric Biosensors

* pH, TDS, Turbidity Sensors

* Microcontroller 

### **AI/ML Layer**

* scikit-learn

* LazyPredict 

### **Disinfection System**

* Chlorine Dispenser Module

* UV-C Sterilization Unit

* Human Presence Sensor (IR/PIR)

### **Communication & IoT**

* Wifi

* IoT Dashboard

* GPS Module

### **Hardware & Power**

* Rechargeable Battery Pack

* Waterproof Enclosure (IP67)

## Hardware Implementation
<img width="1248" height="832" alt="Group 1" src="https://github.com/user-attachments/assets/4e164524-b216-4402-ba27-3bb19b9afd5a" />

## Prototype
<img width="1344" height="768" alt="Gemini_Generated_Image_9z2hro9z2hro9z2h" src="https://github.com/user-attachments/assets/2a6f65d1-965c-41a1-bc76-3ca4a3ebefc7" />




## Future Scope

This project is not just a prototype; it's a foundation. The scalable technology stack allows for the deployment of multiple units to cover larger areas, creating a robust network for comprehensive water quality management. This system can be adapted for use in other high-risk areas, such as municipal water bodies and industrial water management.
