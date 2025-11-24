//v3 updates
// From 5 speed mode. It turns into 4 since the motor will start at around 102 value of analog
// Changing direction of motor where added

const int l1 = 0;  // right direction indicator
const int l2 = 1;  // left direction indicator
// speed control
const int sw1 = 2;
static int sw1_current_state = 0;
int sw1_new_state = 1;
static int speed_counter = 0;  //Counter for motor speed through PWM
const int sw2 = 3;
static int sw2_current_state = 0;
int sw2_new_state = 1;
int speed_state = 0;
int speed_cont;
static int new_speed;
int speed_val [5] = {0,102,153,204,255}; // Value for the PWM
// direction control
const int sw3 = 4;
int sw3_new_state = 1;
static int sw3_current_state = 0;
int direction_counter = 0;  // 0 right, 1 left
int direction_state = 0;
//initializer
int speed_initial = 0;
int direction_initial = 0;

const int a_1 = 6;  // Input 1 for motor direction
const int a_2 = 7;  // Input 2 for motor direction
const int en_12 = 5;
// 74HC595
const int clr = 8;
const int srclk = 9;
const int rclk = 10;
const int oe = 11;
const int ser = 12;
//for loop is used to control the data rate instead of shiftout. This lead for more solid lights in the seven segment
// q0 is not used
int pinsegment[10][8] = {
  //G,F,E,D,C,B,A,NONE
  { 0, 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 0, 0, 0, 1, 1, 0, 0 },  //1
  { 1, 0, 1, 1, 0, 1, 1, 0 },  //2
  { 1, 0, 0, 1, 1, 1, 1, 0 },  //3
  { 1, 1, 0, 0, 1, 1, 0, 0 },  //4
  { 1, 1, 0, 1, 1, 0, 1, 0 },  //5
  { 1, 1, 1, 1, 1, 0, 1, 0 },  //6
  { 0, 0, 0, 0, 1, 1, 1, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1, 0 },  //8
  { 1, 1, 0, 0, 1, 1, 1, 0 },  //9
};

void number_lookup(int number);
int speed_function(bool button1, bool button2);
int direction_function(bool button3);
void motor_function(int speed);
void motor_direction(int direction, int speed, bool input);

void setup() {
  // put your setup code here, to run once:
  //LED
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  // Input module
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  //Motor module
  pinMode(a_1, OUTPUT);    // Motor direction control
  pinMode(a_2, OUTPUT);     // Motor direction control
  pinMode(en_12, OUTPUT);  //Motor speed control
  digitalWrite(a_1, LOW);
  digitalWrite(a_2, LOW);
  // Display module
  pinMode(clr, OUTPUT);
  pinMode(srclk, OUTPUT);
  pinMode(rclk, OUTPUT);
  pinMode(oe, OUTPUT);
  pinMode(ser, OUTPUT);
  digitalWrite(oe, LOW);
  digitalWrite(clr, HIGH);
  digitalWrite(l1, HIGH);
  number_lookup(0); 
}

void loop() {
  // put your main code here, to run repeatedly:
  //Input module
  bool sw1_state = digitalRead(sw1);
  bool sw2_state = digitalRead(sw2);
  bool sw3_state = digitalRead(sw3);
   
  //Make sure that the inital value = 0
  if (sw1_state == HIGH) {
    speed_initial = 1;
  }
  if (sw3_state == HIGH) {
    direction_initial = 1;
  }
  //Speed control
  speed_state = speed_function(sw1_state, sw2_state);
  // speed value
  int speed_cont = speed_val[speed_state];
  //Direction control
  direction_state = direction_function(sw3_state);
  if (speed_initial == 0) {
    speed_state = 0;
    speed_cont = 0;
  }
  if (direction_initial == 0) {
    direction_state = 0;
    digitalWrite(a_1, HIGH);
    digitalWrite(a_2, LOW);
  }

  number_lookup(speed_state);
  motor_direction(direction_state,speed_cont,sw3_state);
  motor_function(speed_cont);
  delay(10);
}

void number_lookup(int number) {
  for (int j = 0; j < 8; j++) {
    digitalWrite(ser, pinsegment[number][j]);
    digitalWrite(srclk, HIGH);
    digitalWrite(srclk, LOW);
  }
  digitalWrite(rclk, HIGH);
  digitalWrite(rclk, LOW);
}

int speed_function(bool button1, bool button2) {
  if (button1 == HIGH && speed_counter < 4) {
    if (sw1_current_state < sw1_new_state) {
      sw1_current_state = 1;
      speed_counter++;
      return speed_counter;
    }
  }

  if (button2 == HIGH && speed_counter > 0) {
    if (sw2_current_state < sw2_new_state) {
      sw2_current_state = 1;
      speed_counter--;
      return speed_counter;
    }
  }
  if (button1 == LOW && button2 == LOW) {
    sw1_current_state = 0;
    sw2_current_state = 0;
  }
  if (button1 == HIGH && button2 == HIGH) {
    sw1_current_state = 1;
    sw2_current_state = 1;
  }
}
// For direction indiciator
int direction_function(bool button3) {
  if (button3 == HIGH) {
    if (sw3_current_state < sw3_new_state) {
      sw3_current_state = 1;
      //left
      if (direction_counter == 1) {
        direction_counter = 0;
        digitalWrite(l1, HIGH);
        digitalWrite(l2, LOW);
        return direction_counter;
      }
      //right
      if (direction_counter == 0) {
        direction_counter = 1;
        digitalWrite(l2, HIGH);
        digitalWrite(l1, LOW);
        return direction_counter;
      }
    }
  }
  if (button3 == LOW) {
    sw3_current_state = 0;
  }
}

void motor_function(int speed) {
  if (speed == 0) {
    
    new_speed = 0;
  }
  while(speed>0)
  {
    
    if(speed == speed_val[1])
    {
      if(new_speed<speed)
      {
        new_speed+=2;
      }
      if(new_speed>speed)
      {
        new_speed--;
      }
    }
    if(new_speed<speed)
    {
      new_speed++;
    }
    if(new_speed>speed)
    {
      new_speed--;
    }
    analogWrite(en_12,new_speed);
    delay(10);
    if(new_speed==speed)
    {
      break;
    }
  }
  analogWrite(en_12,new_speed); 
}

void motor_direction(int direction, int speed, bool input)
{
  //This function is used to make sure a smooth change of direction
  // It has to make sure the motor will stop smoothly then change direction
  int current_speed;
  int speed_count;
if(input==HIGH)
{ 
    //Record the inital speed first
    current_speed = speed;
    while(true)
    {
      speed=0;
      analogWrite(en_12, speed);
    if(speed<=0)
    {
      if(direction ==1)
      {
        digitalWrite(a_1, LOW);
        digitalWrite(a_2, HIGH);
      }
      if(direction ==0)
      {
        digitalWrite(a_1, HIGH);
        digitalWrite(a_2, LOW);
      }
    }
    break;
  }
   while(speed!=current_speed)
    {
      speed++;
      analogWrite(en_12,speed);
      if(speed==current_speed)
      {
        break;
      }
    }
}
  
}
