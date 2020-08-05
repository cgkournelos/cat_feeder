
#define MAX_REVOLUTIONS 2   // Max rotations for feeding one cat
#define NUM_OF_CATS 1       // Number of cats that you want to feed
#define FEED_CYCLE_TIME 100 // Reading of stop sensor

#include <RTClib.h>
#include <Wire.h>

const int motor_relay = 2;
const int stop_sensor_pin = 3; // Indicates when valve is closed

RTC_DS1307 rtc;

char days_of_week[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool feed_on_startup;

void setup() {
  pinMode(stop_sensor_pin, INPUT);
  pinMode(motor_relay, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Serial.begin(115200);

  digitalWrite(motor_relay, LOW);

  if (!rtc.begin()) {
    // Serial.println("Error on RTC begin. Could not find RTC module");
    // Serial.flush();
    // abort();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (!rtc.isrunning()) {
    // Serial.println("Error RTC is not running");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // August 01, 2020 at 3am you would call:
    // rtc.adjust(DateTime(2020, 08, 01, 3, 0, 0));
  }

  feed_on_startup = true;

  // Serial.println("[setup] Complete");
  delay(5000); // Startup delay
}

/**
 * @brief Main loop
 */
void loop() {

  if (feed_on_startup || timeToFeed()) {
    feed();
  }

  feed_on_startup = false;
  delay(1000);
}

/**
 * @brief Turns on the valve and feed the cats with almost 40gr
 *
 */
void feed() {
  int stop_sensor_val = 0;
  int prev_val = 0;

  // Start motor
  digitalWrite(motor_relay, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  // Serial.println("[feed] Open valve");

  stop_sensor_val = digitalRead(stop_sensor_pin);
  prev_val = stop_sensor_val;
  int revs_counter = 0;

  while (revs_counter < NUM_OF_CATS * MAX_REVOLUTIONS) {
    stop_sensor_val = digitalRead(stop_sensor_pin);
    if ((stop_sensor_val == 1) && (prev_val == 0)) {
      revs_counter++;
    }
    prev_val = stop_sensor_val;
    delay(FEED_CYCLE_TIME);
    // Serial.print("[feed] Revolutions : ");
    // Serial.println(revs_counter);
  }
  // Serial.println("[feed] End of feed loop");

  while (!stop_sensor_val) {
    stop_sensor_val = digitalRead(stop_sensor_pin);
  }

  // Serial.println("[feed] Fully close valve");

  // Stop motor
  digitalWrite(motor_relay, LOW);
  delay(60000); // delay 1 minute before it can run again
  // Serial.println("[feed] End of feed function");
  digitalWrite(LED_BUILTIN, LOW);
  return true;
}

/**
 * @brief Checks if it is time to feed
 *
 * @return true
 * @return false
 */
bool timeToFeed() {
  DateTime now = rtc.now();

  // printTime(now, "timeToFeed");

  if (((now.hour() == 9) || (now.hour() == 21)) && (now.minute() == 5)) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Print a DateTime from RTC in Serial
 *
 * @param time
 * @param print_tag
 */
void printTime(DateTime time, String print_tag) {
  Serial.print("[");
  Serial.print(print_tag);
  Serial.print("] ");
  Serial.print(time.year(), DEC);
  Serial.print('/');
  Serial.print(time.month(), DEC);
  Serial.print('/');
  Serial.print(time.day(), DEC);
  Serial.print(" (");
  Serial.print(days_of_week[time.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(time.hour(), DEC);
  Serial.print(':');
  Serial.print(time.minute(), DEC);
  Serial.print(':');
  Serial.print(time.second(), DEC);
  Serial.println();
}