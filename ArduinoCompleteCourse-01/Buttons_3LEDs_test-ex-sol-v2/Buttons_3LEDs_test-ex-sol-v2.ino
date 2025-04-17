
// Pin numbers
// Buttons
const int button1_pin = 5;
const int button2_pin = 6;

int led1_pin = 2;
int led2_pin = 3;
int led3_pin = 4;

bool on_off_state = HIGH;
bool led_activation_states[3] = { true, false, false };

int flash_frequency = 1500;
int selected_pattern = 1;

// Timers
unsigned long last_time_button1_pressed = 0;
unsigned long last_time_button2_pressed = 0;
unsigned long flash_timer = 0;
unsigned int debounce_delay = 50;

void setup() {
  pinMode(button1_pin, INPUT);
  pinMode(button2_pin, INPUT);

  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  pinMode(led3_pin, OUTPUT);
}

void flash_LEDs() {

  if (millis() > (flash_timer + flash_frequency)) {
    on_off_state = !on_off_state;
    flash_timer = millis();
  }

  if (led_activation_states[0]) {
    digitalWrite(led1_pin, on_off_state);
  } else {
    digitalWrite(led1_pin, LOW);
  }

  if (led_activation_states[1]) {
    digitalWrite(led2_pin, on_off_state);
  } else {
    digitalWrite(led2_pin, LOW);
  }

  if (led_activation_states[2]) {
    digitalWrite(led3_pin, on_off_state);
  } else {
    digitalWrite(led3_pin, LOW);
  }
}

void loop() {

  bool button1_read = digitalRead(button1_pin);
  bool button2_read = digitalRead(button2_pin);

  // ------------------------------------------------------------------- //
  // BUTTON #1 ACTIONS
  // ------------------------------------------------------------------- //
  if (button1_read == HIGH) {
    // Debounce button #1
    if ((millis() - last_time_button1_pressed) > debounce_delay) {
      // Halve the flash frequency
      flash_frequency = flash_frequency / 2;
      if (flash_frequency < 40) {
        flash_frequency = 1500;
      }
    }
    // store millis for button 2
    last_time_button1_pressed = millis();
  }

  // ------------------------------------------------------------------- //
  // BUTTON #2 ACTIONS
  // ------------------------------------------------------------------- //
  if (button2_read == HIGH) {
    // Debounce button #2
    if ((millis() - last_time_button2_pressed) > debounce_delay) {

      selected_pattern = selected_pattern + 1;
      if (selected_pattern > 7) {
        selected_pattern = 0;
      }

      switch (selected_pattern) {
        case 0:
          led_activation_states[0] = false;
          led_activation_states[1] = false;
          led_activation_states[2] = false;
          break;
        case 1:
          led_activation_states[0] = true;
          led_activation_states[1] = false;
          led_activation_states[2] = false;
          break;
        case 2:
          led_activation_states[0] = false;
          led_activation_states[1] = true;
          led_activation_states[2] = false;
          break;
        case 3:
          led_activation_states[0] = false;
          led_activation_states[1] = false;
          led_activation_states[2] = true;
          break;
        case 4:
          led_activation_states[0] = true;
          led_activation_states[1] = true;
          led_activation_states[2] = false;
          break;
        case 5:
          led_activation_states[0] = true;
          led_activation_states[1] = false;
          led_activation_states[2] = true;
          break;
        case 6:
          led_activation_states[0] = false;
          led_activation_states[1] = true;
          led_activation_states[2] = true;
          break;
        case 7:
          led_activation_states[0] = true;
          led_activation_states[1] = true;
          led_activation_states[2] = true;
          break;
      }
    }
    // store millis for button 2
    last_time_button2_pressed = millis();
  }

  flash_LEDs();
}
