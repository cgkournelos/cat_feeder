#define REV_TIME 2000

const int pos_sensor = 18;
const int motor_relay = 4;

bool init_flag;

void setup() {
  pinMode(pos_sensor, INPUT);
  pinMode(motor_relay, OUTPUT);
  Serial.begin(9600);
  digitalWrite(motor_relay, LOW);
  init_flag = true;
}

void loop() {
  if(init_flag){
    feed();
    feed();
    feed();
    feed();
    feed();
  }
  init_flag = false;
}

void feed(){
  digitalWrite(motor_relay, HIGH);
  delay(REV_TIME);
  while(!digitalRead(pos_sensor)){}
  digitalWrite(motor_relay, LOW);
}


