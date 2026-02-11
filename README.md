# ESP32 Online Flappybird &middot; [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com) [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/ainh01/esp32OnlineFlappybird/blob/master/LICENSE)  
> A two-player Flappybird game implemented on an ESP32, playable online via Firebase.  

This project implements a two-player Flappybird game on an ESP32 microcontroller, featuring real-time online multiplayer capabilities powered by Firebase.  

## Installing / Getting started  

To get started, you will need an ESP32 development board and the Arduino IDE with ESP32 board support installed.  

```cpp  
// This is the main program file for the ESP32 Flappybird game.  
// It includes WiFi connectivity, Firebase integration, and game logic.  

#include <Arduino.h>  
#include <WiFi.h>  
#include <FirebaseClient.h>  
#include <WiFiClientSecure.h>  

// Game state variables and hardware pin configurations are defined here.  
// The main loop continuously updates game state, draws to an LED matrix,  
// and communicates with Firebase for player inputs and game settings.  
```  

The code above initializes the ESP32, connects to Wi-Fi, and sets up communication with a Firebase Realtime Database. It then enters a continuous loop where it refreshes the LED matrix display, processes game logic, handles player controls (received from Firebase), and updates game settings from Firebase.  

## Developing  

### Built With  
* Arduino IDE  
* ESP32 Board Support Package  
* FirebaseClient Library (for Arduino)  

### Prerequisites  
* **Arduino IDE**: Download from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)  
* **ESP32 Board Support Package**: Follow instructions at [https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)  
* **FirebaseClient Library**: Install via Arduino IDE's Library Manager (search for "FirebaseClient").  
* **A Firebase Project**: Set up a new project at [https://console.firebase.google.com/](https://console.firebase.google.com/) and create a Realtime Database.  
* **ESP32 Development Board**: Any ESP32 DevKit board should work.  
* **LED Matrix (e.g., 8x8 LED matrix)**: Connected to the ESP32 according to the `initDoc` and `initNgang` functions.  

### Setting up Dev  

To set up the development environment, follow these steps:  

```shell  
git clone https://github.com/ainh01/esp32OnlineFlappybird.git  
cd esp32OnlineFlappybird/  
```  
After cloning, open the `main.cpp` file in the Arduino IDE. You will need to update the `WIFI_SSID`, `WIFI_PASSWORD`, `DATABASE_SECRET`, and `DATABASE_URL` macros in the `WifConnect` namespace with your Wi-Fi credentials and Firebase project details. Ensure the Firebase Realtime Database has the correct structure for `GameConChim/Settings`, `GameConChim/Player1`, and `GameConChim/Player2` for the game to function correctly.  

### Building  

To build the project:  

1. Open `main.cpp` in the Arduino IDE.  
2. Select your ESP32 board from `Tools > Board > ESP32 Arduino`.  
3. Select the correct COM port for your ESP32 from `Tools > Port`.  
4. Click `Sketch > Upload` to compile and upload the code to your ESP32.  

This process compiles the C++ code for the ESP32 and flashes it onto the device.  

### Deploying / Publishing  
The deployment process involves uploading the compiled firmware to the ESP32. No external publishing server is involved.  

```shell  
// Within Arduino IDE  
// Select your ESP32 board and port.  
// Click the "Upload" button (right arrow icon).  
```  
This action compiles the `main.cpp` code and flashes the resulting binary to the connected ESP32 board, making the game executable on the hardware.  

## Versioning  

We can use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/ainh01/esp32OnlineFlappybird/tags).  

## Configuration  

The following configurations can be adjusted in the `main.cpp` file:  

*   **Wi-Fi Credentials**:  
    *   `WIFI_SSID`: Your Wi-Fi network name.  
    *   `WIFI_PASSWORD`: Your Wi-Fi network password.  
*   **Firebase Database Credentials**:  
    *   `DATABASE_SECRET`: Your Firebase project secret (though `noAuth` is currently used, this might be a placeholder or for future authentication).  
    *   `DATABASE_URL`: The URL of your Firebase Realtime Database.  
*   **Game Pace Settings (fetched from Firebase /GameConChim/Settings)**:  
    *   `birdPace`: Controls the bird's movement speed.  
    *   `controlPace`: Controls the responsiveness of player inputs.  
    *   `drawPace`: Controls the refresh rate of the display.  
    *   `logicPace`: Controls the speed of game logic updates.  
    *   `wallPace`: Controls the movement speed of the walls.  

## Tests  

This project does not include dedicated unit tests. Functional testing involves:  

1.  **Uploading the code** to an ESP32 with an attached LED matrix.  
2.  **Monitoring Serial output** in the Arduino IDE for Wi-Fi connection and Firebase communication status.  
3.  **Interacting with the Firebase Realtime Database** (e.g., manually changing player jump values or game settings) to observe game behavior on the LED matrix.  

```cpp  
// The `WifConnect::loop()` and `GameConChim::settingUpdate()` functions  
// continuously check for updates from Firebase, effectively testing the  
// communication and configuration update mechanisms during runtime.  
WifConnect::loop(); // Checks for new data from Firebase (player inputs, settings)  
// ...  
GameConChim::settingUpdate(); // Applies new game pace settings from Firebase  
```  
These runtime checks ensure that the ESP32 can successfully connect to Wi-Fi, establish a secure connection to Firebase, receive real-time updates for game settings and player actions, and update the game state accordingly.  

## Style guide  

The code generally follows a C++ style, common in Arduino projects. It prioritizes clarity and direct hardware interaction.  

## Api Reference  

This project primarily uses the **Firebase Realtime Database API** for its online features.  

*   **Firebase Database URL**: Configured via `DATABASE_URL`.  
*   **Authentication**: Currently uses `NoAuth` for simplicity, meaning public read/write access to the specified database paths. For production, consider implementing more secure authentication rules in Firebase.  
*   **Endpoints**:  
    *   `/GameConChim/Settings`: Stores integer values for `birdPace`, `controlPace`, `drawPace`, `logicPace`, and `wallPace`. The ESP32 subscribes to changes here to dynamically update game speed.  
    *   `/GameConChim/Player1`: Expects boolean values (e.g., `true` when Player 1 "jumps").  
    *   `/GameConChim/Player2`: Expects boolean values (e.g., `true` when Player 2 "jumps").  

Player inputs (jumps) are sent to these `/Player1` and `/Player2` paths, and the ESP32 listens for these changes to trigger bird movements.  

## Database  

The project utilizes **Google Firebase Realtime Database** (latest version available at the time of development).  

**Database Design:**  

The database is structured as a JSON tree:  

```json  
{  
  "GameConChim": {  
    "Settings": {  
      "birdPace": <integer>,    // Example: 15  
      "controlPace": <integer>, // Example: 1  
      "drawPace": <integer>,    // Example: 1  
      "logicPace": <integer>,   // Example: 2  
      "wallPace": <integer>     // Example: 13  
    },  
    "Player1": <boolean>,       // Example: true (for a jump)  
    "Player2": <boolean>        // Example: true (for a jump)  
  }  
}  
```  

*   **`GameConChim` (Root Node)**: Contains all game-related data.  
*   **`Settings` (Child of `GameConChim`)**: Stores global game parameters that can be adjusted in real-time. Each parameter is an integer representing a pace or delay value.  
*   **`Player1` (Child of `GameConChim`)**: A boolean value. When `true`, it signals Player 1's bird to "jump". This value is reset by the ESP32 after processing.  
*   **`Player2` (Child of `GameConChim`)**: Similar to `Player1`, but for Player 2.  

**Download Links:**  
Firebase is a cloud-based service, so there's no direct "download" for the database itself. You manage it through the [Firebase Console](https://console.firebase.google.com/).  

## Licensing  

This project is licensed under the MIT License. See the [LICENSE](https://github.com/ainh01/esp32OnlineFlappybird/blob/master/LICENSE) file for the full license text.
