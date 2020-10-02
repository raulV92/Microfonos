// Control de microfonos SV:
int modo = 0; // 0 -> mic, 1 -> mp3

// pin de audio auxiliar
int out_mp3= 13;

// pines de control de microfonos
int mic1[] = {12,11,10,9};
int mic2[]={8,7,6,5};
int mic_por_linea=4;

int estados_mic1[]= {0,0,0,0};
int estados_mic2[] = {0,0,0,0};
int tolerancia=7;

// entradas de boton
int entrada1 = A1;
int entrada2 = A2;

int in_mp3 = A0;

int in_botones=A4;
int estados_botones[]={0,0,0};
int botones[]={4,3,2};

void setup() {

  pinMode(in_mp3,INPUT);
  pinMode(out_mp3,OUTPUT);
  digitalWrite(out_mp3,LOW);

  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);

  for(int k=0;k<mic_por_linea;k++){
    pinMode(mic1[k],OUTPUT);
    pinMode(mic2[k],OUTPUT);

    digitalWrite(mic1[k],LOW);
    digitalWrite(mic2[k],LOW);
    
    }

  Serial.begin(9600);
}

void botones_independientes(){

  int boton=-1; // valores 0,1,2
  int entrada_boton=0;
  //cambia_estado(estados_botones, estados, int mic_elegido)
  
  entrada_boton=analogRead(in_botones);
  if (entrada_boton>50){
    boton=elige_boton(entrada_boton);
    
    if(boton==0 || boton==1 || boton ==2){  
    estados_botones[boton]=cambia_estado(botones,
                                        estados_botones[boton],
                                        boton);
    }
    }
    
    //Serial.print("\n Estado igual: ");
    //Serial.print(estados_botones[0]);
    //Serial.print(estados_botones[1]);
    //Serial.print(estados_botones[2]);
    //delay(1000);
  }

int elige_boton(int valor_analogico){
  // valores calibrados con r = 330 y 1000 a GND
  int resistencias[] = {771,616,514,440}; 
  int tolerancia=9;
  int  mic_elegido;
  int cont;

  Serial.println("funcion elige_boton");
  for(cont=0; cont <mic_por_linea ;cont++){  // HARCODED SIZE !!
    
    if(resistencias[cont]-tolerancia <valor_analogico &&
      valor_analogico < resistencias[cont] + tolerancia){
        mic_elegido = cont;
        //Serial.println("microfono en for :");
        //Serial.println(mic_elegido);
        //delay(2000);
        break;
      }
  }
  Serial.println("termina FOR");
  Serial.println(mic_elegido);
  return mic_elegido;
}

int cambia_estado(int pin[], int estado_previo, int mic_elegido){
  //
  if (estado_previo == 0){
    digitalWrite(pin[mic_elegido],HIGH);
    //Serial.print("pin prendido: ");
    //Serial.print(pin[mic_elegido]);
    delay(700);
    return 1;
  }

  if (estado_previo == 1){
    digitalWrite(pin[mic_elegido],LOW);
    //Serial.print("pin apagado: ");
    //Serial.print(pin[mic_elegido]);
    delay(700);
    return 0;
  } 
}


void modo_microfono_0(int mic[], int estados_mic[], int entrada){

  int mic_elegido = -1;  // valores validos, 0,1,2,3
  int valor_analogico=0;

  valor_analogico=analogRead(entrada);
  //Serial.print("\nvalor de analog: ");
  //Serial.println(valor_analogico);

  if (valor_analogico > 30){
    mic_elegido=elige_boton(valor_analogico);
    //Serial.println("microfono de regreso: ");
    //Serial.print(mic_elegido);
    if (mic_elegido>-1){
      estados_mic[mic_elegido]=cambia_estado(mic,
                                            estados_mic[mic_elegido],
                                            mic_elegido);
    
    }
    
  }//else{
   // Serial.print("\n Estado igual: ");
   // Serial.print(estados_mic2[0]);
   // Serial.print(estados_mic2[1]);
   // Serial.print(estados_mic2[2]);
   // Serial.print(estados_mic2[3]);
  //}

}

void apaga_microfonos(int mic[],int estados_mic[]){

  for(int k=0;k<mic_por_linea;k++){
      digitalWrite(mic[k],LOW);
      estados_mic[k]=0; 
    }
  }


void espera_cambio_a0(){
  if(analogRead(in_mp3)>50){
    if (modo==1){
        digitalWrite(out_mp3,LOW);
        delay(700);
      
        modo=0;
      }else if(modo==0){
        
        apaga_microfonos(mic1,estados_mic1);
        apaga_microfonos(mic2,estados_mic2);
        digitalWrite(out_mp3,HIGH);
        delay(700);
        modo=1;
        }
    }
  
  }

void loop() {

  
  espera_cambio_a0();

  //if(modo==0){ 
    modo_microfono_0(mic2, estados_mic2, entrada2);
    modo_microfono_0(mic1, estados_mic1, entrada1);
  //}

  botones_independientes();
}
