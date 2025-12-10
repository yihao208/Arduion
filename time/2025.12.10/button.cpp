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


====================================================================================
// C++ code
//
void setup()
{
  pinMode(7, INPUT);
  Serial.begin(9600);
}

int last_mode = 0;
int new_mode = 0;
bool a;

void loop()
{
  a = digitalRead(7);
  //Serial.println(a);
  if(a == 1){
    new_mode = 1;
    //Serial.println("1");
  }
  else if(a == 0){
    new_mode = 2;
    //Serial.println("2");
  }
  
  if (new_mode != last_mode){
    last_mode = new_mode;
    //Serial.println(last_mode);
    switch(new_mode){
      case 1:
        Serial.println("11111");
        break;
      case 2:
        Serial.println("22222");
        break;
    
    
    }
  
  }
}
