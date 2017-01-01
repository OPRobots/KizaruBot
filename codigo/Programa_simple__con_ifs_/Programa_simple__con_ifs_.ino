
int MotorIalante = 5;
int MotorIatras = 3;
int MotorDalante = 6;
int MotorDatras = 9;

int Sensor1 = A4;
int Sensor2 = A3;
int Sensor3 = A2;
int Sensor4 = A1;
int Sensor5 = A0;

int pato1 = 0;
int pato2 = 0;
int pato3 = 0;
int pato4 = 0;
int pato5 = 0;

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


  digitalWrite(MotorIalante, HIGH);
  digitalWrite(MotorIatras, HIGH);
  digitalWrite(MotorDalante, HIGH);
  digitalWrite(MotorDatras, HIGH);

 // Serial.begin(9600);
  delay(500);
}

void loop() {

    pato1 = digitalRead(Sensor1);
    pato2 = digitalRead(Sensor2);
    pato3 = digitalRead(Sensor3);
    pato4 = digitalRead(Sensor4);
    pato5 = digitalRead(Sensor5);


    if (pato1 == 1 && pato2 == 0 && pato3 == 0 && pato4 == 0 && pato5 == 1) {
      velD = vel;
      velI = vel;
    } if (pato1 == 1 && pato2 == 0 && pato3 == 0 && pato4 == 1 && pato5 == 1) {
      velD = vel - 55;
      velI = vel;
    } if (pato1 == 0 && pato2 == 0 && pato3 == 1 && pato4 == 1 && pato5 == 1) {
      velD = vel - 85;
      velI = vel;
    } if (pato1 == 0 && pato2 == 1 && pato3 == 1 && pato4 == 1 && pato5 == 1) {
      velD = 0;
      velI = vel+15;
    } if (pato1 == 1 && pato2 == 1 && pato3 == 0 && pato4 == 0 && pato5 == 1) {
      velD = vel;
      velI = vel - 55;
    } if (pato1 == 1 && pato2 == 1 && pato3 == 1 && pato4 == 0 && pato5 == 0) {
      velD = vel;
      velI = vel - 85;
    } if (pato1 == 1 && pato2 == 1 && pato3 == 1 && pato4 == 1 && pato5 == 0) {
      velD = vel+15;
      velI = 0;
    }
    analogWrite(MotorDalante, velD);
    analogWrite(MotorIalante, velI);


}
