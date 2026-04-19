<h1 align="center">
  <img src="./docs/assets/logos/Lumvia.png" width="32" vertical-align="middle" alt="Lumvia Logo"> Lumvia
</h1>

<p align="center">
  <strong>A truly serverless ecosystem for global smart lighting control.</strong><br>
  Manage addressable LED strips from anywhere via a seamless Telegram Mini App interface.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP8266-orange.svg?style=flat-square" alt="Platform ESP8266">
  <img src="https://img.shields.io/badge/LEDs-WS2812B%20%2F%20Generic-brightgreen.svg?style=flat-square" alt="Supported LEDs">
  <img src="https://img.shields.io/badge/Interface-Telegram_Mini_App-blue.svg?style=flat-square" alt="Telegram Mini App">
  <img src="https://img.shields.io/badge/Hosted-GitHub_Pages-lightgrey.svg?style=flat-square" alt="GitHub Pages">
</p>

---

<p align="center">
  <video src="./docs/assets/demo/uiDemo.mp4" width="320" autoplay loop muted playsinline></video>
</p>

### 💡 The Concept

Lumvia removes the complexity of DIY smart lighting. Forget about port forwarding, setting up a Raspberry Pi for Home Assistant, or paying for proprietary cloud subscriptions.

* **Zero-Configuration:** A true "Plug & Play" approach. Flash your device once, and it's ready to communicate via the Telegram API cloud.
* **Global Access:** If you have an internet connection, you have control. Manage your lights from home, the office, or abroad.
* **Native Interface:** The control panel is a modern **Telegram Mini App**. It lives right inside your messenger—no extra apps to install.

---

###  Features

* **Intuitive UI:** Go beyond text commands. Control color, brightness, and effect speed through a graphical interface.
* **Multi-Device Support:** Add unlimited modules (e.g., Kitchen, Bedroom) and switch between them instantly in the app.
* **Shared Access:** Grant control to family members or roommates seamlessly.
* **Independent:** The project is designed to have zero reliance on personal servers or third-party paid backends.

---

###  How It Works (Architecture)

Lumvia utilizes a unique communication method to bypass standard network restrictions (CGNAT, dynamic IPs) and standard Telegram bot limitations (direct bot-to-bot communication is forbidden).

1.  **The Interface (App Bot):** The Telegram Mini App acts as your controller. It sends commands as messages to a **Private Telegram Channel**.
2.  **The Bridge (Telegram Channel):** This private channel serves as a secure, real-time data buffer.
3.  **The Device (LED Bot):** The firmware running on the ESP8266 polls this specific channel, reads the commands posted by the App Bot, and applies changes to the LED strip.

This "serverless magic" ensures privacy, stability, and zero hosting costs.

---

###  Technical Stack

**Hardware:**
* **MCU:** ESP8266 (NodeMCU, Wemos D1 Mini, etc.).
* **LEDs:** WS2812B (default). Supported via the flexible [NeoPixelBus](https://github.com/Makuna/NeoPixelBus) library, allowing compatibility with various other addressable LED types.

**Software:**
* **Firmware:** C++ developed in [PlatformIO](https://platformio.org/) (Arduino framework). Highly optimized for efficient Telegram API interaction.
* **Frontend:** Modern Web Application integrated as a Telegram Mini App.

---

###  Getting Started

1.  **Hardware:** Connect your addressable LED strip to the ESP8266 (ensure proper power injection for long strips).
2.  **Telegram Setup:**
    * Create two Telegram Bots via @BotFather.
    * Create a Private Telegram Channel.
    * Add both bots as administrators to that channel.
3.  **Firmware:** Open the source code in PlatformIO, enter your WiFi credentials, the Token for the **LED Bot**, and the Private Channel ID. Flash the ESP8266.
4.  **Launch App:** Configure the **App Bot** to launch the Lumvia Web App.

Access the ready-to-use interface here: **[Lumvia Web App](https://arsenizar.github.io/Lumvia/)**