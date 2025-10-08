// #define BLYNK_TEMPLATE_ID "TMPL6FG3Uti_B"
// #define BLYNK_TEMPLATE_NAME "SMART HOME"
// #define BLYNK_AUTH_TOKEN "9mYX_XzDif1rCXVbNYIHINWAZ_jAIVKR"
/* Comment this out to disable prints and save space */
#define BLYNK_TEMPLATE_ID           "TMPL6szvd0yhQ"
#define BLYNK_TEMPLATE_NAME         "SMART DUSTBIN"
#define BLYNK_AUTH_TOKEN            "KlQuGyx8XdkLNwt6efCFt8hEC3IEraz2"
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SMART DUSTBIN";
char pass[] = "DUSTBIN1234";
const char* host = "www.google.com";
int k1 = D0;
int k2 = D2;
int k3 = D3;
int k4 = D4;
int speedPin = D1;
int sw1 = D5; 
int sw2 = D6;
int sw3 = D7; 
int sw4 = D8;
int led = A0;
int currentSpeed = 0;
int state1, state2, state3, state4 = 1;
int btn_state, btn_state1, btn_state2, btn_state3 = 0;
int preState, preState1, preState2, preState3 = 0;
bool isConnected = Blynk.connected();

void setup() {
  Serial.begin(115200);// put your setup code here, to run once:
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT);
  pinMode(sw4,INPUT);

  pinMode(k1, OUTPUT);
  pinMode(k2, OUTPUT);
  pinMode(k3, OUTPUT);
  pinMode(k4, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(k1, HIGH);
  digitalWrite(k2, HIGH);
  digitalWrite(k3, HIGH);
  digitalWrite(k4, HIGH);
  pinMode(led, HIGH); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

BLYNK_WRITE(V0){
  int state = param.asInt();
  if(state == 1){
    state1 = 0;
  }else if(state == 0){
    state1 = 1;

  }

}
BLYNK_WRITE(V1){
  int state = param.asInt();
  if(state == 1){
    state2 = 0;
  }else if(state == 0){
    state2 = 1;

  }

}
BLYNK_WRITE(V2){
  int state = param.asInt();
  if(state == 1){
    state3 = 0;
  }else if(state == 0){
    state3 = 1;

  }

}
BLYNK_WRITE(V3){
  int state = param.asInt();
  if(state == 1){
    state4 = 0;
  }else if(state == 0){
    state4 = 1;

  }

}



BLYNK_WRITE(V4){
  int state = param.asInt();
  currentSpeed = map(state, 0,100,0,255);
analogWrite(speedPin, currentSpeed );
Serial.println(map(state, 0,100,0,255));

}
////////////////////////////////////////////////////

BLYNK_CONNECTED() {
    Blynk.syncAll();
}
/////////////////////////////////////////////////////////////

 
///////////////////////////////////////////////////////////////////////

void relay1(){

  if(digitalRead(sw1)==HIGH){
    if(btn_state = !btn_state){
       Blynk.virtualWrite(V0, btn_state);
       btn_state =!btn_state;
       preState = 1;
       state1 = 0;
    }
   btn_state = !btn_state;
  }else if(digitalRead(sw1)==LOW && preState == 1){
    Blynk.virtualWrite(V0, 0);
    btn_state = 0;
    preState = 2;
    state1 = 1;

  }if(digitalRead(sw1)==LOW && preState==2){
    Blynk.virtualWrite(V0,0);
    preState = 0;
    state1 = 1;
  }


  digitalWrite(k4, state1);
}
void relay2(){
    if(digitalRead(sw2)==HIGH){
    if(btn_state1 = !btn_state1){
       Blynk.virtualWrite(V1, btn_state1);
       btn_state1 =!btn_state1;
       preState1 = 1;
       state2 = 0;
    }
   btn_state1 = !btn_state1;
  }else if(digitalRead(sw2)==LOW && preState1 == 1){
    Blynk.virtualWrite(V1, 0);
    btn_state1 = 0;
    preState1 = 2;
    state2 = 1;

  }if(digitalRead(sw2)==LOW && preState1==2){
    Blynk.virtualWrite(V1,0);
    preState1 = 0;
    state2 = 1;
  }

  digitalWrite(k3, state2);
}
void relay3(){
    if(digitalRead(sw3)==HIGH){
    if(btn_state2 = !btn_state2){
       Blynk.virtualWrite(V2, btn_state2);
       btn_state2 =!btn_state2;
       preState2 = 1;
       state3= 0;
    }
   btn_state2 = !btn_state2;
  }else if(digitalRead(sw3)==LOW && preState2 == 1){
    Blynk.virtualWrite(V2, 0);
    btn_state2 = 0;
    preState2 = 2;
    state3 = 1;

  }if(digitalRead(sw3)==LOW && preState2==2){
    Blynk.virtualWrite(V2,0);
    preState2 = 0;
    state2 = 1;
  }

  digitalWrite(k2, state3);
}
void relay4(){
    if(digitalRead(sw4)==HIGH){
    if(btn_state3 = !btn_state3){
       Blynk.virtualWrite(V3, btn_state3);
       btn_state3 =!btn_state3;
       preState3 = 1;
       state4 = 0;
    }
   btn_state3 = !btn_state3;
  }else if(digitalRead(sw4)==LOW && preState3 == 1){
    Blynk.virtualWrite(V3, 0);
    btn_state3 = 0;
    preState3 = 2;
    state4 = 1;

  }if(digitalRead(sw4)==LOW && preState3==2){
    Blynk.virtualWrite(V3,0);
    preState3 = 0;
    state4 = 1;
  }

  digitalWrite(k1, state4);
}
void offlineMode(){
  if(digitalRead(sw1)==1){digitalWrite(k4, LOW);}else if (digitalRead(sw1)==LOW){digitalWrite(k4, HIGH);}
  if(digitalRead(sw2)==1){digitalWrite(k3, LOW);}else if (digitalRead(sw2)==LOW){digitalWrite(k3, HIGH);}
  if(digitalRead(sw3)==1){digitalWrite(k2, LOW);}else if (digitalRead(sw3)==LOW){digitalWrite(k2, HIGH);}
  if(digitalRead(sw4)==1){digitalWrite(k1, LOW);}else if (digitalRead(sw4)==LOW){digitalWrite(k1, HIGH);}
  analogWrite(speedPin, currentSpeed);

}
void loop() {
  // digitalWrite(led, HIGH);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    

    if (client.connect(host, 80)) {
       Blynk.run();
      Serial.println("Connected to the internet");
      client.stop();
        relay1();
        relay2();
        relay3();
        relay4();
      
    } else {
      offlineMode();
          Serial.println("Internet connection lost");
      //analogWrite(led, 0);
    }
 


}
}
