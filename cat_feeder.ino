#define REV_TIME 7000       // Average time to rotate the valve
#define NUM_OF_CATS 1       // Number of cats that you want to feed

const int stop_sensor_pin = 18;  // Indicates when valve is closed
const int motor_relay = 4;

bool startup_flag;

void setup() {
  pinMode(stop_sensor_pin, INPUT);
  pinMode(motor_relay, OUTPUT);

  Serial.begin(9600);

  digitalWrite(motor_relay, LOW);

  startup_flag = true;
}

/**
 * @brief Main loop
 * 
 */
void loop() {
  if(startup_flag){
    feed(NUM_OF_CATS);
  }
  startup_flag = false;
}

/**
 * @brief Turns on the valve and feed the cats with almost 100gr 
 * 
 * @param cat_number 
 * @return true fro
 */
bool feed(int cat_number){
  int stop_sensor_val = 0;

  digitalWrite(motor_relay, HIGH);
  Serial.println("Open valve");

  delay(REV_TIME*cat_number);
  Serial.println("End of delay");

  stop_sensor_val = digitalRead(stop_sensor_pin);

  while(!stop_sensor_val){
    stop_sensor_val = digitalRead(stop_sensor_pin);
  }
  Serial.println("Sensor triggered");
  
  digitalWrite(motor_relay, LOW);
  return true;
}


