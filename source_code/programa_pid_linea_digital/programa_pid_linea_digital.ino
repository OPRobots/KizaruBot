0333
//Pines de los motores
int MotorIalante = 5;
int MotorIatras = 3;
int MotorDalante = 6;
int MotorDatras = 9;

//Pines de los Sensores leyendolos de izquierda a derecha viendo el robot de la bateria a los sensores
int Sensor1 = A4;
int Sensor2 = A3;
int Sensor3 = A2;
int Sensor4 = A1;
int Sensor5 = A0;

//Arrays de valor de sensores en negro en blanco y el umbral (utilizados en la calibracion)
int negros[5];
int blancos[5];
int umbrales[5];
// este array es un array intermediario para hacer operaciones no tiene relevancia en el programa
int lectura[5];

//Variables PID
float kp = 0;
float kd = 0;
float ki = 0;
int posicion = 0;
int posicion_anterior = 0;
int velocidad = 0;
int tiempo = 0;
int correccion = 0;

//Variables barias
bool calibracion = true;
int i = 0;
int recivido = 0;
long millisAnterior = 0;

int velD = 100;
int velI = 100;

int vel = 140;
void setup() {
  pinMode(MotorIalante, OUTPUT);
  pinMode(MotorIatras, OUTPUT);
  pinMode(MotorDalante, OUTPUT);
  pinMode(MotorDatras, OUTPUT);


  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  pinMode(Sensor4, INPUT);
  pinMode(Sensor5, INPUT);

  pinMode(13, OUTPUT);


  digitalWrite(MotorIalante, HIGH);
  digitalWrite(MotorIatras, HIGH);
  digitalWrite(MotorDalante, HIGH);
  digitalWrite(MotorDatras, HIGH);

  Serial.begin(9600);
  delay(500);
}

void loop() {


  ///////////////////////////////////////////////////
  /*Comunicacion del bluethooth para calibracion del PID*/
  ///////////////////////////////////////////////////

  if (Serial.available() > 0) {
    recivido = Serial.read();
  }
  else {
    recivido = 0;
  }

  switch (recivido) {
    case 97:
      kp += 0.1;
      Serial.print("Kp: ");
      Serial.println(kp);
      break;
    case 115:
      kd += 0.1;
      Serial.print("Kd: ");
      Serial.println(kd);
      break;
    case 100:
      ki += 0.1;
      Serial.print("Ki: ");
      Serial.println(ki);
      break;
    case 122:
      kp -= 0.1;
      Serial.print("Kp: ");
      Serial.println(kp);
      break;
    case 120:
      kd -= 0.1;
      Serial.print("Kd: ");
      Serial.println(kd);
      break;
    case 99:
      ki -= 0.1;
      Serial.print("Ki: ");
      Serial.println(ki);
      break;
    case 105:
      vel += 1;
      Serial.print("Vel: ");
      Serial.println(vel);
      break;
    case 107:
      vel -= 1;
      Serial.print("Vel: ");
      Serial.println(vel);
      break;
  }


  ///////////////////////////////////////////////////
  /*Calculo del analogico de desplazamiento del array de sensores*/
  ///////////////////////////////////////////////////

  //cada 20 milisegundos lee los sensores y llama a la funcion que ejecuta el calculo para hacerlo de manera periodica

  //  if (millis() % 20 == 0) {
  lectura[0] = digitalRead(Sensor1);
  lectura[1] = digitalRead(Sensor2);
  lectura[2] = digitalRead(Sensor3);
  lectura[3] = digitalRead(Sensor4);
  lectura[4] = digitalRead(Sensor5);

//una secuencia de ifs para determinar cada posible situacion del robot y crear un psudoanalogico escalonado para meter al PID

  if (lectura[0] == 1 && lectura[1] == 0 && lectura[2] == 0 && lectura[3] == 0 && lectura[4] == 1) {

    posicion = 0;

  } else if (lectura[0] == 1 && lectura[1] == 0 && lectura[2] == 0 && lectura[3] == 1 && lectura[4] == 1) {

    posicion = -25;

  } else if (lectura[0] == 0 && lectura[1] == 0 && lectura[2] == 1 && lectura[3] == 1 && lectura[4] == 1) {

    posicion = -50;

  } else if (lectura[0] == 0 && lectura[1] == 1 && lectura[2] == 1 && lectura[3] == 1 && lectura[4] == 1 && posicion < 85) { // salio del to por uno de los laos

    posicion = -92;

  } else if (lectura[0] == 1 && lectura[1] == 1 && lectura[2] == 0 && lectura[3] == 0 && lectura[4] == 1) {

    posicion = 25;

  } else if (lectura[0] == 1 && lectura[1] == 1 && lectura[2] == 1 && lectura[3] == 0 && lectura[4] == 0) {

    posicion = 50;

  } else if (lectura[0] == 1 && lectura[1] == 1 && lectura[2] == 1 && lectura[3] == 1 && lectura[4] == 0 && posicion > -85) {

    posicion = 92;

  } else if (lectura[0] == 1 && lectura[1] == 1 && lectura[2] == 1 && lectura[3] == 1 && lectura[4] == 1) {
    // 
    if (posicion >= 0) {
      posicion = 130;
    } else {
      posicion = -130;
    }
  }


  ///////////////////////////////////////////////////
               /*Calculo del PID*/
  ///////////////////////////////////////////////////


  velocidad = posicion - posicion_anterior;
  correccion = (kp * posicion) + (kd * velocidad) + (ki * tiempo);
  correccion = map(correccion, -500, 500, -200, 200);
  posicion_anterior = posicion;
  
 /* Serial.print(correccion);
if(velocidad>0){
  Serial.print("   ");
  Serial.println(velocidad);
}
  Serial.println("   ");
  Serial.println(posicion);
  Serial.print("   ");
  Serial.println(correccion);
   }*/


  ///////////////////////////////////////////////////
  /*Asignacion de velocidades a los motores*/
  ///////////////////////////////////////////////////

  //aqui aplicamos la correccion del pid a las velocidades de los motores
  velD = vel + correccion;
  velI = vel - correccion;

  //limitamos desbordamientos
  velD = constrain(velD, -255, 255);
  velI = constrain(velI, -255, 255);

  //asiganmos valores a la rueda derecha teniendo en cuenta de que si el valor es negativo va hacia atras
  if (velD >= 0) {
    analogWrite(MotorDalante, velD);
    digitalWrite(MotorDatras, HIGH);
  } else {
    analogWrite(MotorDatras, (velD +255));
    digitalWrite(MotorDalante, HIGH);
  }

  //asiganmos valores a la rueda izquierda teniendo en cuenta de que si el valor es negativo va hacia atras
  if (velI >= 0) {
    analogWrite(MotorIalante, velI);
    digitalWrite(MotorIatras, HIGH);
  } else {
    analogWrite(MotorIatras, (velI +255));
    digitalWrite(MotorIalante, HIGH);
  }
  /*Serial.print(velD);
  Serial.print("   ");
  Serial.println(velI);*/
}


/*
 y esto es to...
 esto es to...
 esto es todo amigos
 */

