# Bartender Device

This device simulates a bartender who always serves your drink to the correct glass.

## Components

| Component         | Qty | Notes                          |
| ----------------- | --- | ------------------------------ |
| Microcontroller   | 1   | Arduino Nano                   |
| Servo Motor       | 1   | Controls the tap               |
| Water Pump        | 1   | Starts pouring the drink       |
| OLED Display      | 1   | Shows the current state        |
| Encoder           | 1   | Selects the volume and starts the process |
| Switches          | 7   | Switch per glass               |
| LEDs              | 7   | Underglow for the glass        |

## How It Works

1. **Glass State Detection**:
   - The device checks the state of each glass using the limit switches.
   - Based on the switch input and pouring log, it determines if the glass is in state:
   
     `NO_GLASS`, `EMPTY`, `FILLED`, or `HALF`.

2. **Servo Motor Operation**:
   - The servo motor moves to position the tap over the selected glass.

3. **Pouring Process**:
   - The pump is activated to pour the drink into the glass.
   - The amount of liquid poured can be set in milliliters using the encoder.

4. **OLED Display**:
   - Displays the current state and volume to be poured.
   - Provides progress bar while pouring

## Features

- **Encoder Volume**:
   - Allows you to choose the amount of liquid to be poured based on the calculated pump flow rate.

> [!NOTE]
> Flow rate has been hard coded as milliliters per second

- **Glass State**:
   - If a glass is not moved after pouring and the volume is changed, the state is set to `HALF`, indicating it can be topped up.

- **Pouring drink**:
  - `FILLED` and `NO_GLASS` - Skip these glasses
  - `HALF` - Pour only the difference if the volume changed. 
  - `EMPTY` - Pour the selected volume.

- **Encoder Button**:
  - Press - start pouring process
  - Hold - no function assigned,  but defined in program

- **Oled Animation**
  - While pouring the drink show little progress bar for each glass
  - Value on the screen will show the amount of drink to pour. For `HALF` glass it will only show reminding difference to pour


## Usage

> [!WARNING]
> If you move a filled glass and place it on the switch, it will overflow as there is no weight sensor to prevent spilling.

1. **Setup**:
   - Connect the Arduino Nano, limit switches, servo motor, pump, OLED display, and encoder as per the schematic provided in the project documentation.

2. **Operation**:
   - Use the encoder to select the desired volume in milliliters.
   - Place the glasses on the limit switches.
   - Press the encoder button to start.
   - The device will detect the state of each glass.
   - The servo will move the tap over the first available glass.
   - The pump will pour the selected volume into the glass.
   - The device will then move to the next glass and repeat the process.

3. **Drink**:
   - Empty the glass c:

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/tuptusstokrotka/Bartender.git
   ```

2. **Upload the Code**:
   - Open the project in VS Code.
   - Get the PlatformIO extension.
   - Select the appropriate COM port.
   - Upload the code to the Arduino Nano.

## Contribution

Feel free to open issues or submit pull requests.

### Enjoy!