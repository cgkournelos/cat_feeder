#define MAX_REVOLUTIONS 2         // Max rotations for feeding one cat
#define NUM_OF_CATS 1             // Number of cats that you want to feed
#define FEED_CYCLE_TIME 1         // Reading of stop sensor   

const int stop_sensor_pin = 18;   // Indicates when valve is closed
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
    feed();
  }
  startup_flag = false;
}

/**
 * @brief Turns on the valve and feed the cats with almost 100gr 
 * 
 * @param cat_number 
 * @return true fro
 */
bool feed(){
  int stop_sensor_val = 0;
  int prev_val = 0;

  digitalWrite(motor_relay, HIGH);
  Serial.println("Open valve");

  stop_sensor_val = digitalRead(stop_sensor_pin);
  prev_val = stop_sensor_val;
  int revs_counter = 0;

  while(revs_counter < NUM_OF_CATS * MAX_REVOLUTIONS){
    stop_sensor_val = digitalRead(stop_sensor_pin);
    if((stop_sensor_val == 1) && (prev_val == 0)){
      revs_counter++;
    }
    prev_val = stop_sensor_val;
    
    Serial.println(revs_counter);
    delay(FEED_CYCLE_TIME);
  }
  Serial.println("End of feed loop");
  
  while (!stop_sensor_val){
      stop_sensor_val = digitalRead(stop_sensor_pin);
  }

  Serial.println("Fully close valve");
  
  digitalWrite(motor_relay, LOW);
  return true;
}


