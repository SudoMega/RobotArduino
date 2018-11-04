int PWMA = 2; //Speed control 
int PWMB = 3; //Speed control

int STBY = 10;
int AIN1 = 9; //Direction1
int AIN2 = 8; //Direction1
int BIN1 = 12; //Direction2
int BIN2 = 11; //Direction2 

//SENSORES INFRAROJOS
int SEN_INF_FL = A0; //(Adelante Izquierda)
int SEN_INF_FR = A1; //(Adelante Derecha)
int SEN_INF_BL = A2; //(Atras Izquierda)
int SEN_INF_BR = A3; //(Atras Derecha)


//SENSORES ULTRASONICOS
int ULTRA_TRIG_F = 4;    // Trigger Ultra adelante
int ULTRA_ECHO_F = 5;    // Echo
int ULTRA_TRIG_R = 6;    // Trigger Ultra Derecha
int ULTRA_ECHO_R = 7;    // Echo
//int ULTRA_TRIG_L = x;    // Trigger Ultra Izquierda  //DEFINIR PIN
//int ULTRA_ECHO_L = y;    // Echo                     //DEFINIR PIN

//NOMBRE DE LOS VALORES A UTILIZAR
bool VALOR_INF_FL; //(Adelante Izquierda)
bool VALOR_INF_FR; //(Adelante Derecha)
bool VALOR_INF_BL; //(Atras Izquierda)
bool VALOR_INF_BR; //(Atras Derecha)
int VALOR_ULTRA_F; //(Ultra adelante)
int VALOR_ULTRA_R; //(Ultra adelante)
int VALOR_ULTRA_L; //(Ultra adelante)

int cont = 1;
int orientacion = 0;

void setup(){
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  pinMode(SEN_INF_FL , INPUT);
  pinMode(SEN_INF_FR , INPUT);
  pinMode(SEN_INF_BL , INPUT);
  pinMode(SEN_INF_BR , INPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

String Enemigo = "";

void loop(){
/*
  delay(5000);
 for(int i=0;i<200;i++){
  delay(50);
  if(!lectura()){
    adelante(255,0);
    delay(10);
    }
  else{
    adelante(255,2);
    delay(800);
    }
  }
  adelante(0,0);
  delay(2500);
  */
////FASE DE LECTURA DE SENSORES////

  VALOR_INF_FL = Lectura_INF(SEN_INF_FL);
  VALOR_INF_FR = Lectura_INF(SEN_INF_FR);
  VALOR_INF_BL = Lectura_INF(SEN_INF_BL);
  VALOR_INF_BR = Lectura_INF(SEN_INF_BR);

  VALOR_ULTRA_F = Lectura_ULTRA(ULTRA_TRIG_F,ULTRA_ECHO_F);
  VALOR_ULTRA_R = Lectura_ULTRA(ULTRA_TRIG_R,ULTRA_ECHO_R);
  VALOR_ULTRA_L = Lectura_ULTRA(ULTRA_TRIG_L,ULTRA_ECHO_L);

////FASE DE TOMA DE DECISIONES////

  if (CheckAllInf() == true){

    if(VALOR_INF_FL == true){
      movimiento(255,"derecha");
    }
    if(VALOR_INF_FR == true){
      movimiento(255,"izquierda");
    }
    if(VALOR_INF_BL == true){
      movimiento(255,"derecha");
    }
    if(VALOR_INF_BR == true){
      movimiento(255,"izquierda");
    }     
    
  }
  else{
    if (VALOR_ULTRA_F < 30){
      movimiento(255,"adelante");
    
    }
    else if (VALOR_ULTRA_R < 30){
      movimiento(255,"derecha");
    }
     else if (VALOR_ULTRA_L < 30){
     movimiento(255,"izquierda");
    }
    else (
      movimiento(255,"adelante");
  }
}

//Lee los sensores infrarojos que se le entregan, con un treshold de 750.
bool Lectura_INF(int INF){
  
  float value;
  value = analogRead(INF);  //lectura digital de pin
  if(value < 750  ){
    Serial.println(value);
    return true;
  }else{
    Serial.println(value);
    return false;
  } 
}

//Lee los sensores Ultrasonicos y retorna la distancia que detecta en cm.
int Lectura_ULTRA(int Trigger, int Echo){
  
  digitalWrite(Trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  int cm = (duration/2) / 29.1;
  return cm;
}


void circuito1(){
  while(!lectura()){
    adelante(225,0);
  }    
  orientacion++;
}

void movimiento(int speed, String direction){
  
}

bool CheckAllInf(){
  if (VALOR_INF_FL == true || VALOR_INF_FR == true || VALOR_INF_BL == true || VALOR_INF_BR == true){
    return true;
  }
  else {return false}
}




















void adelante(int speed, int direction){
  digitalWrite(STBY, HIGH);
  boolean inPinA1;
  boolean inPinA2;
  boolean inPinB1;
  boolean inPinB2;
   if(direction ==0){
    inPinA1 = LOW;
    inPinA2 = HIGH;
    inPinB1 = LOW;
    inPinB2 = HIGH;
   }
   if(direction == 1){
     inPinA1 = HIGH;
     inPinA2 = LOW;
     inPinB1 = HIGH;
     inPinB2 = LOW;
    }
   if(direction == 2){
    // inPinA1 = LOW;
   // inPinA2 = LOW;
     inPinB1 = HIGH;
     inPinB2 = LOW;
   }
   if(direction == 3){
     inPinA1 = HIGH;
     inPinA2 = LOW;
    // inPinB1 = LOW;
    // inPinB2 = HIGH;
   }
    digitalWrite(AIN1, inPinA1);
    digitalWrite(AIN2, inPinB2);
    analogWrite(PWMA, speed);
    
    digitalWrite(BIN1, inPinB1);
    digitalWrite(BIN2, inPinB2);
    analogWrite(PWMB, speed);
 }

void giroD(){
   
 
}





void move(int speed, int direction){
  
  digitalWrite(STBY, HIGH);

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
}

void stop(){ 
  digitalWrite(STBY, LOW); 
}
