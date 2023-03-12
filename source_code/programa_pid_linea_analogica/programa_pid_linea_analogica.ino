
//Pines de los motores
int MotorIalante = 5;
int MotorIatras = 3;
int MotorDalante = 6;
int MotorDatras = 9;

//Pines de los Sensores lellendolos de izquierda a derecha viendo el robot de la bateria a los sensores
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

int vel = 180;
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
  /*realizamos la calibracion inicial obteniendo los minimos y maximos leidos por cada sensor durante 5 seg y calculamos los umbrales*/
                                  ///////////////////////////////////////////////////

  if (calibracion == true) {
    digitalWrite(13,HIGH);
    millisAnterior = millis();
    do {

      // lectura de los sensores
      lectura[0] = analogRead(Sensor1);
      lectura[1] = analogRead(Sensor2);
      lectura[2] = analogRead(Sensor3);
      lectura[3] = analogRead(Sensor4);
      lectura[4] = analogRead(Sensor5);

      // registramos los valores inimos y maximos en negros y blancos
      for (i = 0; i < 5; i++) {
        if (lectura[i] < negros[i]) {
          negros[i] = lectura[i];
        }
        if (lectura[i] > blancos[i]) {
          blancos[i] = lectura[i];
        }
      }

    } while ((millis() - millisAnterior) < 5000);

    // Calculamos los umbrales para cada sensor
    for (i = 0; i < 5; i++) {
      umbrales[i] = (negros[i] + blancos[i]) / 2;
    }

    calibracion = false;
    digitalWrite(13,LOW);
  }

        ///////////////////////////////////////////////////
      /*Comunicacion del bluethooth para calibracion del PID*/
        ///////////////////////////////////////////////////

if (Serial.available() > 0) {
                recivido = Serial.read();
        }
        else{
          recivido = 0;
        }

 switch (recivido) {
    case 97:
      kp += 0.2;
      Serial.print("Kp: ");
      Serial.println(kp);
      break;
    case 115:
      kd += 0.2;
      Serial.print("Kd: ");
      Serial.println(kd);
      break;
    case 100:
      ki += 0.1;
      Serial.print("Ki: ");
      Serial.println(ki);
      break;
    case 122:
      kp -= 0.2;
      Serial.print("Kp: ");
      Serial.println(kp);
      break;
    case 120:
      kd -= 0.2;
      Serial.print("Kd: ");
      Serial.println(kd);
      break;
    case 99:
      ki -= 0.1;
      Serial.print("Ki: ");
      Serial.println(ki);
      break;
  }
 

         ///////////////////////////////////////////////////
  /*Calculo del analogico de desplazamiento del array de sensores*/
        ///////////////////////////////////////////////////

  //cada 20 milisegundos lee los sensores y llama a la funcion que ejecuta el calculo para hacerlo de manera periodica

  if (millis() % 20 == 0) {
    lectura[0] = analogRead(Sensor1);
    lectura[1] = analogRead(Sensor2);
    lectura[2] = analogRead(Sensor3);
    lectura[3] = analogRead(Sensor4);
    lectura[4] = analogRead(Sensor5);

    //llamada a la funcion que calcula el valor analogico en funcion de la posicion del robot sobre la linea negra.
    //debemos darle el valor que nos dio la ultima vez, un array con las lecturas de los analogicos, y un array con los umbrales de cada sensor
    posicion = array_sensores_analogico(posicion, lectura, umbrales);
  }


  ///////////////////////////////////////////////////
              /*Calculo del PID*/
  ///////////////////////////////////////////////////


  velocidad = posicion - posicion_anterior;

  correccion = (kp * posicion) + (kd * velocidad) + (ki * tiempo);

  posicion_anterior = posicion;
//Serial.print(posicion);
//Serial.println(correccion);

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
  if (velD > 0) {
    analogWrite(MotorDalante, velD);
    digitalWrite(MotorDatras, HIGH);
  } else {
    analogWrite(MotorDatras, (velD*-1));
    digitalWrite(MotorDalante, HIGH);
  }

//asiganmos valores a la rueda izquierda teniendo en cuenta de que si el valor es negativo va hacia atras
  if (velI > 0) {
    analogWrite(MotorIalante, velI);
    digitalWrite(MotorIatras, HIGH);
  } else {
    analogWrite(MotorIatras, (velI*-1));
    digitalWrite(MotorIalante, HIGH);
  }

}


/*
 y esto es to...
 esto es to...
 esto es todo amigos
 */
 
