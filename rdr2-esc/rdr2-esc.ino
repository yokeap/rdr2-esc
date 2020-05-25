class thruster{

  private:
  unsigned int _dirPin, _pwmPin;

  public:
  //constructor
  thruster(unsigned int dirPin, unsigned int pwmPin){
    _dirPin = dirPin;
    _pwmPin = pwmPin;
    pinMode(_dirPin, OUTPUT);  
    pinMode(_pwmPin, OUTPUT); 
  }

  void setDirection(bool dir){
    digitalWrite(_dirPin, dir);    
  }

  void setPWM(unsigned int pwm){
    if ((_dirPin < 5) || (_dirPin > 9)) {
      analogWriteResolution(8);
      analogWrite(_pwmPin, map(pwm, 0, 100, 0, 255));
      //analogWrite(_pwmPin, 127);
    }
    else {
      analogWriteResolution(12);
      analogWrite(_pwmPin, map(pwm, 0, 100, 0, 4095));
    }
  }
  
};

thruster T[4] = {
  thruster(24,7),
  thruster(23,6),       //front left wheel
  thruster(26,9),
  thruster(25,8)
  };

String inString = "";    // string to hold input
bool inStringComplete = false;
int intCount = 0;
bool blFlag = 0;
int value[8];
unsigned long timeCount,last_time;

void serialEvent1() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    inString += inChar;
    
    if ((intCount++ > 30) || (inChar == '\r')) {
      inStringComplete = true;
      intCount = 0;
    }
  }
}

void setup() {
  T[motorNumber].setPWM(100);
  T[motorNumber].setPWM(100);
  T[motorNumber].setPWM(100);
  T[motorNumber].setPWM(100);
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  Serial.println("rdr2-esc");
}

void loop() {
  if(inStringComplete){
    Serial.println(inString);
    motorPwmParser(inString);
    inString = "";
    inStringComplete = false;
  }
}

char motorPwmParser(String strTemp){
  char *strSplit;
  char charArray[strTemp.length() + 1];
  char strBuffer[strTemp.length() + 1];
  uint8_t value = 0;
  strTemp.toCharArray(charArray, strTemp.length() + 1);

  strcpy(strBuffer, charArray);

  strSplit = strtok(strBuffer, ":,\r");
  H_Control(atoi(strSplit),0);
  
  strSplit = strtok(0, ":,\r");
  H_Control(atoi(strSplit),1);

  
  strSplit = strtok(0, ":,\r");
  H_Control(atoi(strSplit),2);
  
  strSplit = strtok(0, ":,\r");
  H_Control(atoi(strSplit),3);

  return 1;
}

void H_Control(int value,uint8_t motorNumber)
{
    if(value <= -100) value = -99;
    if(value >= 100) value = 99;
    
    if(value > 0) 
      T[motorNumber].setDirection(1);
    if(value < 0) 
      T[motorNumber].setDirection(0);

      T[motorNumber].setPWM(100 - abs(value));
}
