/* Light sensor pins */
#define LEFT_LIGHT_SENSOR A0
#define CENTER_LIGHT_SENSOR A1
#define RIGHT_LIGHT_SENSOR A2

/* Motors PWM pins */
#define LEVITATION_FAN 3
#define LEFT_PROP_FANS 5
#define CENTER_PROP_FAN 9
#define RIGHT_PROP_FANS 10

#define LIGHT_SENS_THRESHOLD 60

/* Thrust percentages */
#define _PERCENT_100 255
#define _PERCENT_90 230
#define _PERCENT_80 204
#define _PERCENT_70 179
#define _PERCENT_60 153
#define _PERCENT_50 128
#define _PERCENT_40 102
#define _PERCENT_30 77
#define _PERCENT_20 51
#define _PERCENT_10 26
#define _PERCENT_0 0

void setup() {
  Serial.begin(9600);
  setPWMPins();
}

void loop() {
  static int previousState = 0;
  int state = checkState();
  switch (state) { 
    case 2:
    // left sensor detecting line
    // a more severe case, rotate CCW cosiderably
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_20);
    centerPropeller(_PERCENT_50);
    rightPropeller(_PERCENT_100);
    previousState = state;
    Serial.println("left");
      break;
      
    case 3:
    // center sensor detecting line
    // go forward
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_100);
    centerPropeller(_PERCENT_100);
    rightPropeller(_PERCENT_100);
    Serial.println("center");
      break;
      
    case 5:
    // right sensor detetcting line
    // a more severe case, rotate CW cosiderably
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_100);
    centerPropeller(_PERCENT_50);
    rightPropeller(_PERCENT_20);
    previousState = state;
    Serial.println("right");
      break;
      
    case 6:
    // left + center sensors detecting line
    // rotate CCW slightly
    previousState = state;
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_50);
    centerPropeller(_PERCENT_100);
    rightPropeller(_PERCENT_100);
    Serial.println("left + center");
      break;
      
    case 10:
    // left + right sensors detecting line
    // unlikely to happen, but go forward slowly
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_80);
    centerPropeller(_PERCENT_80);
    rightPropeller(_PERCENT_80);
    Serial.println("left + right");
      break;
      
    case 15:
    // center + right sensors detecting line
    // rotate CW slightly
    previousState = state;
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_100);
    centerPropeller(_PERCENT_100);
    rightPropeller(_PERCENT_50);
    Serial.println("center + right");
      break;
      
    case 30:
    // left + center + right sensor detecting line
    // less likely to happen, go forward slowly
    levitate(_PERCENT_100);
    leftPropeller(_PERCENT_80);
    centerPropeller(_PERCENT_80);
    rightPropeller(_PERCENT_80);
    previousState = state;
    Serial.println("left + center + right");
      break;
      
    default:
    // line not detected by any sensor at this state
    if (previousState == 0) {
      // hovercraft hasn't started the course
      // start levitating and go forward slowly
      levitate(_PERCENT_100);
      leftPropeller(_PERCENT_80);
      centerPropeller(_PERCENT_80);
      rightPropeller(_PERCENT_80);
    } else {
      // hovercraft already started the course and is lost
      // so go back to previous state
      // TO BE IMPLEMENTED
    }
    Serial.print("none, previous state:");
    Serial.println(previousState);
      break;
  }
  delay(50);
}

int checkState() {
  int result = 1;
  
  int left = analogRead(LEFT_LIGHT_SENSOR);
  int center = analogRead(CENTER_LIGHT_SENSOR);
  int right = analogRead(RIGHT_LIGHT_SENSOR);
  
  left = map(left, 0, 1023, 0, 255);
  center = map(center, 0, 1023, 0, 255);
  right = map(right, 0, 1023, 0, 255);
  
  Serial.print("L: ");
  Serial.print(left);
  Serial.print("     C: ");
  Serial.print(center);
  Serial.print("     R: ");
  Serial.print(right);
  Serial.println();
  
  if (left < LIGHT_SENS_THRESHOLD) result *= 2;
  if (center < LIGHT_SENS_THRESHOLD) result *= 3;
  if (right < LIGHT_SENS_THRESHOLD) result *= 5;
  
  return result;
}

void levitate(int powLevel) {
  // @param powLevel is a value between 0 - 255
  analogWrite(LEVITATION_FAN, powLevel);
}

void leftPropeller(int powLevel) {
  analogWrite(LEFT_PROP_FANS, powLevel);
}

void centerPropeller(int powLevel) {
  analogWrite(CENTER_PROP_FAN, powLevel);
}

void rightPropeller(int powLevel) {
  analogWrite(RIGHT_PROP_FANS, powLevel);
}

void setPWMPins() {
  pinMode(LEVITATION_FAN, OUTPUT);
  pinMode(LEFT_PROP_FANS, OUTPUT);
  pinMode(CENTER_PROP_FAN, OUTPUT);
  pinMode(RIGHT_PROP_FANS, OUTPUT);
}
