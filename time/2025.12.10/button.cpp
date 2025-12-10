// C++ code
//
void setup()
{
  pinMode(7, INPUT);
  Serial.begin(9600);
}

void loop()
{
  bool a = digitalRead(7);
  if(a == 1){
    Serial.println("kaishi");
    while(a == 1){
      a = digitalRead(7);
      //Serial.println(a);
    }
  }
  else if(a == 0){
    Serial.println("guanbi");
    while(a == 0){
      a = digitalRead(7);
      //Serial.println(a);
    }
  }
}



==========================================================================
// C++ code
//
void setup()
{
  pinMode(7, INPUT);
  Serial.begin(9600);
}

bool a = 0;
bool b = 0;

void loop()
{
  a = digitalRead(7);
  if(a == 1 && b == 0){
    Serial.println("kaishi");
    b = 1;   
    }
  else if(a == 0 && b == 1){
    Serial.println("guanbi");
    b = 0;
  }
}
