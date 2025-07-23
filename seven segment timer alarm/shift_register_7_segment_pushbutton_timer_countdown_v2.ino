//v2 updates
// added an led alarm
// added a new counter for set button. This prevents the increment of the counter after a reset
const int ds = 3;
const int oe = 4;
const int stcp = 5;
const int shcp = 6;
const int mr = 7;
const int start_button = 8;
const int set_button = 12;
const int alarm = 10;
const int led = 11;
// for start button function
int start_current_state = 0;
int start_new_state = 1;
int start_counter = 0;
//for set button function
int set_new_state = 1;
int set_current_state = 0;
int set_counter = 0;
static int set_time = 0;
static int set_timer_counter=0;
//for loop is used to control the data rate instead of shiftout. This lead for more solid lights in the seven segment
// q0 is not used
int pinsegment[10][8] = 
{
  //G,F,E,D,C,B,A,NONE
  {0,1,1,1,1,1,1,0}, //0
  {0,0,0,0,1,1,0,0}, //1
  {1,0,1,1,0,1,1,0}, //2
  {1,0,0,1,1,1,1,0}, //3
  {1,1,0,0,1,1,0,0}, //4
  {1,1,0,1,1,0,1,0}, //5
  {1,1,1,1,1,0,1,0}, //6
  {0,0,0,0,1,1,1,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,0,0,1,1,1,0}, //9
};

int start_timer (bool button);
void number_lookup (int number);
int set_timer (bool button);

void setup() {
  // put your setup code here, to run once:
  pinMode(ds, OUTPUT);
  pinMode(oe, OUTPUT);
  pinMode(stcp, OUTPUT);
  pinMode(shcp, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(led, OUTPUT);

  //start button
  pinMode(start_button, INPUT);
  // set button
  pinMode(set_button, INPUT);
  // buzzer
  pinMode(alarm, OUTPUT);;

  digitalWrite(oe, LOW);
  digitalWrite(mr, HIGH);

  //default timer
  //default start timer
  for(int j=0;j<8;j++)
    {
      digitalWrite(ds, pinsegment[8][j]);
      digitalWrite(shcp, HIGH);
      digitalWrite(shcp, LOW);
    }
    digitalWrite(stcp, HIGH);
    digitalWrite(stcp, LOW);  

  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // Check the button state for starting timer
  bool button_state = digitalRead(start_button);
  // to stop the timer from repeating by changing the timer_counter condition
  int start_timer_counter = start_timer(button_state);
  
// This function is used to prevent the counter increase at the momennt when the set button is pressed for reseting after the counter =;
if(set_time!=0)
{
  number_lookup(set_timer_counter);
  delay(500);
  if(digitalRead(set_button)==HIGH);
  {
    set_time=0;
  }
}

if(set_time == 0)
{
  bool set_button_state = digitalRead(set_button);
  set_timer_counter = set_timer(set_button_state);
  number_lookup(set_timer_counter);
  Serial.print(set_timer_counter);
  Serial.println();
}
  // Time will start when start_timer_counter = 1
  // The start timer cannot change nor reset when the timer starts countdown due to the loops.
  if(start_timer_counter == 1)
 {
  for(int i = set_timer_counter; i>-1;i--)
  {
    number_lookup(i);
    delay(1000);
  }
    while(true)
    {
      bool set_state = digitalRead(set_button);
      digitalWrite(alarm, HIGH);
      digitalWrite(led, HIGH);
      number_lookup(0);

      if(set_state ==HIGH)
      {
        set_time=1;
        digitalWrite(alarm, LOW);
        digitalWrite(led, LOW);
        break;
      }
    }  
  }
  }

// Reseting and set the time
int set_timer(bool button)
{
  if(button == HIGH)
  {
    if(set_current_state < set_new_state)
    {
      set_current_state = 1;
      set_counter++;
      return set_counter;
    }
    if(set_counter>9)
    {
      set_counter =0;
      return set_counter;
    }
  }
  else
  {
    set_current_state = 0;
  }
}

// function for starting the timer
int start_timer(bool button)
{
  if(button == HIGH)
  {
    if(start_current_state < start_new_state)
    {
      
      start_counter=1;
      start_current_state = 1;
      return start_counter;
    }

  }
  else
  {
    start_current_state = 0;
    start_counter = 2;
    return start_counter;
  }
}

// function for number lookup
void number_lookup(int number)
{
  for(int j=0;j<8;j++)
    {
      digitalWrite(ds, pinsegment[number][j]);
      digitalWrite(shcp, HIGH);
      digitalWrite(shcp, LOW);
    }
    digitalWrite(stcp, HIGH);
    digitalWrite(stcp, LOW);  
}


