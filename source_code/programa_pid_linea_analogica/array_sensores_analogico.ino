
// funcion encargada de generar el valor analogico de -250 a 250 enn funcion de la posicion del robot sobre la linea
// a partir de la lectura analogica de cada sensor, aplicando una media ponderada entre los valores de lectura

int array_sensores_analogico(int previo, int lectura[], int corte[]) {
  long Val_Analogico = 0;
  long suma = 0;
  int contador = 0;

  //este for hace una suma ponderada aplicandole pesos a cada sensor
  for (int i = 0; i < 5; i++) {

    if (lectura[i] < corte[i]) {
      Val_Analogico += (lectura[i] * (i + 1) * 1000);
      suma += lectura[i];
    }
    else {
      contador++;
    }
  }

  Serial.print(Val_Analogico);
  Serial.print(" - ");

  //este if se activa en caso de que el robot se haya salido de la linea, al haber contado 5 valores superiores al umbral (todos los sensores en blanco)
  //y tomando como referencia el valor que retorno la vez anterior establece si se salio por el lado derecho o por el izquierdo
  if (contador == 5) {
    // Serial.println("pato");
    if (previo > 0) {
      Val_Analogico = 5000;
    }
    if (previo < 0) {
      Val_Analogico = 1000;
    }
  }
  else {
    //si no entro en el anterior if, (sigue sobre la linea) continua con el calculo del analogico
    // esto hace la division entre la suma de los valores de detecion para completar la media ponderada

    contador = 0;
    Val_Analogico = Val_Analogico / suma;

  }

  //Serial.print(Val_Analogico);
  //Serial.print("   ");
  Val_Analogico = map(Val_Analogico, 1000, 5000, -250, 250);
 // Serial.println(Val_Analogico);
  return Val_Analogico;
}
