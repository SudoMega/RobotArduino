//CODIGO EJEMPLO MOTORES CON ENCODER
//https://www.dfrobot.com/wiki/index.php/12V_DC_Motor_251rpm_w/Encoder_(SKU:_FIT0186)#Specification

bool robotOn = false; //switch para encender al robot
int dataBT = 0;
int estrategia = 0;

int PWMA = 2; //Speed control, motor 1, rueda izquierda
int PWMB = 3; //Speed control, motor 2, rueda derecha

int STBY = 10;
int AIN1 = 9;  //Motor 1, Rueda Izquierda, polo positivo
int AIN2 = 8;  //Motor 1, Rueda Izquierda, polo negativo
int BIN1 = 12; //Motor 2, Rueda derecha, polo positivo
int BIN2 = 11; //Motor 2, Rueda derecha, polo negativo 

//SENSORES INFRAROJOS
int SEN_INF_FL = A0; //(Adelante Izquierda)
int SEN_INF_FR = A1; //(Adelante Derecha)
int SEN_INF_BL = A2; //(Atras Izquierda)
int SEN_INF_BR = A3; //(Atras Derecha)


//SENSORES ULTRASONICOS
int ULTRA_TRIG_F = 4;     // Trigger Ultra adelante
int ULTRA_ECHO_F = 5;     // Echo
int ULTRA_TRIG_R = 6;     // Trigger Ultra Derecha
int ULTRA_ECHO_R = 7;     // Echo
int ULTRA_TRIG_L = 30;    // Trigger Ultra Izquierda 
int ULTRA_ECHO_L = 31;    // Echo                    

//NOMBRE DE LOS VALORES A UTILIZAR
bool VALOR_INF_FL; //(infrarojo Adelante Izquierda)
bool VALOR_INF_FR; //(infrarojo Adelante Derecha)
bool VALOR_INF_BL; //(infrarojo Atras Izquierda)
bool VALOR_INF_BR; //(infrarojo Atras Derecha)
int VALOR_ULTRA_F; //(Ultra adelante)
int VALOR_ULTRA_R; //(Ultra derecha)
int VALOR_ULTRA_L; //(Ultra izquierda)

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
  
  pinMode(ULTRA_TRIG_F, OUTPUT);
  pinMode(ULTRA_ECHO_F, INPUT);
  pinMode(ULTRA_TRIG_R, OUTPUT);
  pinMode(ULTRA_ECHO_R, INPUT);
  pinMode(ULTRA_TRIG_L, OUTPUT);
  pinMode(ULTRA_ECHO_L, INPUT);

  Serial.begin(9600);
  apagado();
}

void loop(){

////FASE DE ENCENDIDO Y ESTRATEGIA INICIAL////

//revisarEncendido();
encendido(); //Con esto se enciende el robot pormientras, aun no esta terminado el bluetooth
  if(estrategia == 1){
    
    movimiento(255, 255,"adelante");
    delay(500);

    estrategia = 0;
  }
  if(estrategia == 2){
    
    movimiento(255, 255,"derecha");
    delay(300);
    movimiento(255, 255,"adelante");
    delay(400);
    movimiento(255, 255,"izquierda");
    delay (200);
    
    estrategia = 0;
  }
  if(estrategia == 3){


  estrategia = 0;
  }

////FASE DE LECTURA DE SENSORES////

checkAllSensors();

////FASE DE TOMA DE DECISIONES////

while (CheckAllInf() == false){

  //revisarEncendido(); //Revisa si hay que apagar el robot
  checkAllSensors(); // Lo primero que hace es actualizar el valor de los sensores
  
  if (VALOR_ULTRA_F < 20){
      movimiento(255, 255,"adelante");
    
    }
    else if (VALOR_ULTRA_R < 20){
      movimiento(255, 255,"derecha");
    }
     else if (VALOR_ULTRA_L < 20){
      movimiento(255, 255,"izquierda");
    }
    else {
      movimiento(255, 255,"derecha");
    }
  
  }
  
  checkAllSensors();
    if(VALOR_INF_FL == true){
      movimiento(255, 255,"derecha");
      delay(400);
    }
    if(VALOR_INF_FR == true){
      movimiento(255, 255,"izquierda");
      delay(400);
    }
    if(VALOR_INF_BL == true){
      movimiento(255, 255,"derecha");
      delay(400);
    }
    if(VALOR_INF_BR == true){
      movimiento(255, 255,"izquierda");
      delay(400);
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
int Lectura_ULTRA(int trigger, int echo){
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  pinMode(echo, INPUT);
  int duration = pulseIn(echo, HIGH);

  int cm = (duration/2) / 29.1;
  return cm;
}

bool CheckAllInf(){
  if (VALOR_INF_FL == true || VALOR_INF_FR == true || VALOR_INF_BL == true || VALOR_INF_BR == true){
    return true;
  }
  else {return false;}
}

//Funcion para controlar el movimiento, se piden las velocidades de cada rueda (izquierda y derecha) y un String con la direccion
void movimiento (int speedI, int speedD, String direction){
  boolean inPinA1;
  boolean inPinA2;
  boolean inPinB1;
  boolean inPinB2;
   if(direction =="adelante"){
    //SI SE MUEVE HACIA ATRAS, INVERTIR CABLES +/- O CAMBIAR LOW CON HIGH
    inPinA1 = HIGH;
    inPinA2 = LOW;
    inPinB1 = HIGH;
    inPinB2 = LOW;
   }
   if(direction == "derecha"){
     //SI GIRA HACIA LA IZQUIERDA, CAMBIAR inPin1 CON inPinB
     inPinA1 = HIGH;
     inPinA2 = LOW;
     inPinB1 = LOW;
     inPinB2 = HIGH;
    }
   if(direction == "izquierda"){
     inPinA1 = LOW;
     inPinA2 = HIGH;
     inPinB1 = HIGH;
     inPinB2 = LOW;
   }
   if(direction == "atras"){
     inPinA1 = LOW;
     inPinA2 = HIGH;
     inPinB1 = LOW;
     inPinB2 = HIGH;
   }
    digitalWrite(AIN1, inPinA1);
    digitalWrite(AIN2, inPinA2);
    analogWrite(PWMA, speedI);
    
    digitalWrite(BIN1, inPinB1);
    digitalWrite(BIN2, inPinB2);
    analogWrite(PWMB, speedD);
 }


//Funciones para comenzar o detener la placa
void apagado(){ 
  digitalWrite(STBY, LOW); 
}

void encendido(){ 
  delay (5000);
  digitalWrite(STBY, HIGH); 
}

void revisarEncendido(){
  if(Serial.available() > 0){
    dataBT = Serial.read();
    if (dataBT == 1){robotOn = true; estrategia = 1;}
    if (dataBT == 2){robotOn = true; estrategia = 2;}
    if (dataBT == 3){robotOn = true; estrategia = 3;}
    if (dataBT == 0){robotOn = false;}
    if (robotOn == true){digitalWrite(STBY, HIGH);}
    else{digitalWrite(STBY, LOW);}
  }
}

  void checkAllSensors(){
 
    VALOR_INF_FL = Lectura_INF(SEN_INF_FL);
    VALOR_INF_FR = Lectura_INF(SEN_INF_FR);
    VALOR_INF_BL = Lectura_INF(SEN_INF_BL);
    VALOR_INF_BR = Lectura_INF(SEN_INF_BR);

    VALOR_ULTRA_F = Lectura_ULTRA(ULTRA_TRIG_F,ULTRA_ECHO_F);
    VALOR_ULTRA_R = Lectura_ULTRA(ULTRA_TRIG_R,ULTRA_ECHO_R);
    VALOR_ULTRA_L = Lectura_ULTRA(ULTRA_TRIG_L,ULTRA_ECHO_L);
  
  }

