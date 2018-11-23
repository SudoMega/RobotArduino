//CODIGO EJEMPLO MOTORES CON ENCODER
//https://www.dfrobot.com/wiki/index.php/12V_DC_Motor_251rpm_w/Encoder_(SKU:_FIT0186)#Specification

/*RobotSumo.apk DEBERIA ESTAR ADJUNTO CON ESTE CODIGO, ES LA APLICACION PARA EL CONTROL REMOTO*/

/*Este codigo es para hacer funcionar un robot sumo
 *no se le an realizado todas las pruebas pertinentes, asi que no se debe utilizar sin antes probar como funciona.
 *Probablemente sea mejor crear un metodo con las estrategias que se quieran utilizar
 *
 *Se escribio este codigo pensando en un robot con 3 sensores Ultrasonicos (adelante, izquierda y derecha) 
 *y 4 sensores infrarojos (uno en cada esquina, apuntando hacia el suelo), pero es escalable usando los metodos
 * 
 * El robot primero inicializa todos sus Pins, apaga sus motores y comienza el loop
 * si es la primera vez que se enciende, la variable firstBoot lo detiene para esperar un encendido desde el bluetooth
 * cuando llega la señal, el robot ejecuta una de las 3 estrategias programadas,
 * luego el robot verifiica si  hay infrarojos activos, si los hay, trata de salir de la linea, 
 * si no hay, prosigue a revisar sus ultrasonicos,
 * si detecta algo, gira acorde a la posicion enemiga, si esta enfrente, se mantiene persiguiendolo en linea recta,
 * finalmente si no encuentra nada, esta por default que gire a la derecha.
 * 
 *CAMBIAR LOS PINS DEPENDIENDO DE COMO LO ORDENEN EN LA PLACA, SE RECOMIENDA PLACAS CON MUCHOS PINS DIGITALES COMO EL ARDUINO MEGA 
 */

#include <SoftwareSerial.h>
                                  //Variable para utilizar bluetooth sin perder las puertas Serial
SoftwareSerial Bluetooth(10, 11); // CONECTAR RX Y TX EN PINS CONTRARIOS, SINO NO FUNCIONA EL BLUETOOTH
                                  
bool robotOn = false;  //switch para encender al robot
int estrategia = 0;    //Variable para elegir la estrategia inicial
bool firstBoot = false;//Revisa si es la primera vez que se enciende

int PWMA = 2; //Speed control, motor 1, rueda izquierda
int PWMB = 3; //Speed control, motor 2, rueda derecha

int STBY = 13; //nos permite poner en standby a los motores
int AIN1 = 9;  //Motor 1, Rueda Izquierda, polo positivo
int AIN2 = 8;  //Motor 1, Rueda Izquierda, polo negativo
int BIN1 = 12; //Motor 2, Rueda derecha, polo positivo
int BIN2 = 20; //Motor 2, Rueda derecha, polo negativo 

//SENSORES INFRAROJOS
int SEN_INF_FL = 21; //(Adelante Izquierda)
int SEN_INF_FR = 20; //(Adelante Derecha)
int SEN_INF_BL = 19; //(Atras Izquierda)
int SEN_INF_BR = 18; //(Atras Derecha)


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

  Bluetooth.begin(9600);
  Serial.begin(9600);
  apagado(); //Se apagan los motores, para que el sumo no se mueva
}

void loop(){
////FASE DE ENCENDIDO Y ESTRATEGIA INICIAL////
while (firstBoot == false){
revisarEncendido();
}
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

  revisarEncendido(); //Revisa si hay que apagar el robot
  checkAllSensors(); // Lo primero que hace es actualizar el valor de los sensores

  //Ciclo While que nos mantiene dentro si el sensor ultrasonico detecto al enemigo, cosa de no dejar nunca de avanzar
     while (VALOR_ULTRA_F < 20){

        revisarEncendido(); //Revisa si hay que apagar el robot
        checkAllSensors(); // Lo primero que hace es actualizar el valor de los sensores
        movimiento(255, 255,"adelante");
    
    }
     if (VALOR_ULTRA_R < 20){
       movimiento(255, 255,"derecha");
    }
     else if (VALOR_ULTRA_L < 20){
       movimiento(255, 255,"izquierda");
    }
    else {
       movimiento(255, 255,"derecha"); //Por default gira hacia la izquierda
    }
  
  }
  //Estos movimientos cuentan con pequeños delays porque estan diseñados para sacar al robot de la linea blanca
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

//Lee los sensores infrarojos que se le entregan, si hay problemas con los colores, invertir la logica (variable 'diff')
bool Lectura_INF(int INF){

  pinMode(INF, OUTPUT );
  digitalWrite( INF, HIGH );  
  delayMicroseconds(10);
  pinMode(INF, INPUT );

  long time = micros();

  while (digitalRead(INF) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;

  //Este if dice si es negro o blanco
  if (diff < 1000){
    return true;
    } 

  else {return false;}
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

  //Retorna la medicion en centimetros
  int cm = (duration/2) / 29.1;
  return cm;
}

//Revisa todos los sensores infrarojos, si hay alguno encendido, retorna true
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
   //Aqui se ingresan las velocidades y direcciones
    digitalWrite(AIN1, inPinA1);
    digitalWrite(AIN2, inPinA2);
    analogWrite(PWMA, speedI);
    
    digitalWrite(BIN1, inPinB1);
    digitalWrite(BIN2, inPinB2);
    analogWrite(PWMB, speedD);
 }


//Funciones para comenzar o detener los motores, encendido tiene un delay de 5 segundos (reglas del sumo)
void apagado(){ 
  digitalWrite(STBY, LOW); 
}

void encendido(){ 
  delay (5000);
  firstBoot=true;
  digitalWrite(STBY, HIGH); 
}

//Revisa si hay algun mensaje desde el bluetooth, si lo hay, ve que numero se recibio y selecciona una estrategia
void revisarEncendido(){
  if(Bluetooth.available() > 0){
    int dataBT = Bluetooth.read();
    Serial.write(Bluetooth.read());
    if (dataBT == 1){robotOn = true; estrategia = 1;}
    if (dataBT == 2){robotOn = true; estrategia = 2;}
    if (dataBT == 3){robotOn = true; estrategia = 3;}
    if (dataBT == 0){robotOn = false;}
    if (robotOn == true){encendido();}
    else{apagado();}
  }
}

//Refresca los valores de todos los sensores, para ser utilizados en el loop
  void checkAllSensors(){
 
    VALOR_INF_FL = Lectura_INF(SEN_INF_FL);
    VALOR_INF_FR = Lectura_INF(SEN_INF_FR);
    VALOR_INF_BL = Lectura_INF(SEN_INF_BL);
    VALOR_INF_BR = Lectura_INF(SEN_INF_BR);

    VALOR_ULTRA_F = Lectura_ULTRA(ULTRA_TRIG_F,ULTRA_ECHO_F);
    VALOR_ULTRA_R = Lectura_ULTRA(ULTRA_TRIG_R,ULTRA_ECHO_R);
    VALOR_ULTRA_L = Lectura_ULTRA(ULTRA_TRIG_L,ULTRA_ECHO_L);
  
  }

