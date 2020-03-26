//***********************************//
//Modulo des temperatura y humedad   //
//                                   //
//Matias Pierdominici                //
//mpierdominici@itba.edu.ar          //
//***********************************//
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEBUGG


#define PIN_SENSOR D2



#define PIR1_PIN D7
#define PIR2_PIN D6
#define PIR3_PIN D2
#define PIR4_PIN D1
#define RELE_PIN D5

int pir1State=0;
int pir2State=0;
int pir3State=0;
int pir4State=0;

class myTimer
{
  public:
  myTimer(unsigned int seconds=0);// lo canvie a ms
  bool timeOver(void);
  void setNewTime(unsigned long seconds_);
  void showInfo();
  
  unsigned long seconds;
  unsigned long startTime;
  void resetTimer(void);
    
};


myTimer sampleTime(250);





char * ssid ="WIFI Pier";
char * pass ="pagle736pagle";
unsigned int mqttPort=1883;

const char MqttUser[]="movDetection";
const char MqttPassword[]="1234";
const char MqttClientID[]="movDetection";

bool pirSensorState=false;

IPAddress mqttServer(192,168,0,116);

WiFiClient wclient;
PubSubClient mqtt_client(wclient);





void callback(char* topic, byte* payload, unsigned int length);
void  debug_message (char * string, bool newLine)
{
#ifdef DEBUGG
  if(string !=NULL)
  {
    if (!newLine)
    {
      Serial.print(string);
    }else
    {
      Serial.println(string);
    }
  }
  #endif
}

void setUpWifi(char * ssid, char * pass)
{
  String ip;
  debug_message(" ",true);
  debug_message(" ",true);
  debug_message("Conectandose a: ",false);
  debug_message(ssid,true);

  WiFi.begin(ssid,pass);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    debug_message(".",false);
  }
  debug_message(" ",true);
  debug_message("Coneccion realizada",true);
  debug_message("La ip es: ",false);
  ip=WiFi.localIP().toString();
  debug_message((char *)ip.c_str(),true);
}

void setUpMqtt(void)
{
  mqtt_client.setServer(mqttServer,mqttPort);
  mqtt_client.setCallback(callback);
}


void callback(char* topic, byte* payload, unsigned int length)
{
  int tiempo=0;
  payload[length]='\n';
  String message((char *)payload);
  debug_message("Llego un mensage, topic:",false);
  debug_message(topic,false);
  debug_message(", payload : ",false);
  debug_message((char *)payload,true);
  
  if(!strcmp(topic,"movDetection/swOn")){
      digitalWrite(RELE_PIN,HIGH);
  }

  if(!strcmp(topic,"movDetection/swOff")){
      digitalWrite(RELE_PIN,LOW);
  }
  
  
  
  if(!strcmp(topic,"movDetection/requestData"))
  {


    mqtt_client.publish("movDetection/sensor1",(String(pir1State)).c_str());
    mqtt_client.publish("movDetection/sensor2",(String(pir2State)).c_str());
    mqtt_client.publish("movDetection/sensor3",(String(pir3State)).c_str());
    mqtt_client.publish("movDetection/sensor4",(String(pir4State)).c_str());


    
    pir1State=0;
    pir2State=0;
    pir3State=0;
    pir4State=0;   
  }

}

void reconnect()
{
  while(!mqtt_client.connected())
  {
    debug_message("Intentando conectar al servidor MQTT",true);
    if (mqtt_client.connect(MqttClientID,MqttUser,MqttPassword))
      {
            debug_message("conectado",true);
  
  
            // ...suscrivirse a topicos
            mqtt_client.subscribe("movDetection/requestData");
            mqtt_client.subscribe("movDetection/swOn");
            mqtt_client.subscribe("movDetection/swOff");
      }
      else
      {
        debug_message("intentando conetarse al broker",true);
        delay(3000);
      }
  }
}

void setup() {
  Serial.begin(9600);
  setUpWifi(ssid,pass);
  setUpMqtt();
  pinMode(PIR1_PIN,INPUT);
  pinMode(PIR2_PIN,INPUT);
  pinMode(PIR3_PIN,INPUT);
  pinMode(PIR4_PIN,INPUT);
  pinMode(RELE_PIN,OUTPUT);


}



void loop() {

  if (!mqtt_client.connected()) 
  {
      reconnect();
      
 }


 mqtt_client.loop();

 if(sampleTime.timeOver()){
  sampleTime.resetTimer();
//  Serial.println(!digitalRead(PIR1_PIN));
//  Serial.println(!digitalRead(PIR2_PIN));
//  Serial.println(!digitalRead(PIR3_PIN));
//  Serial.println(!digitalRead(PIR4_PIN));
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
//  Serial.println("");
  if(!digitalRead(PIR1_PIN)){
    pir1State=1;
  }
  if(!digitalRead(PIR2_PIN)){
    pir2State=1;
  }
  if(!digitalRead(PIR3_PIN)){
    pir3State=1;
  }
  if(!digitalRead(PIR4_PIN)){
    pir4State=1;
  }
  
 }





 

}





//***********************TIMER**********************************



myTimer::myTimer(unsigned int seconds)
{
  setNewTime(seconds);
}

//timeOver
//devuelve true si ya paso el tiempo seteado,
//caso contrario devuelve false
//
bool myTimer::timeOver(void)
{
  if((millis())>startTime)
  {
    resetTimer();
    return true;
  }
  else
  {
    return false;
  }
}

void myTimer::resetTimer(void)
{
  unsigned long temp=seconds+millis();
 
  startTime=temp;
  //Serial.print("se llamo a rest timer con: ");
  //Serial.println(startTime);
}

void  myTimer::setNewTime(unsigned long seconds_)
{
  unsigned long temp=seconds_;
  //Serial.println(temp);
  seconds=temp;
 
  //Serial.print("s seteo un timer cada: ");
  //Serial.print(seconds_);
  //Serial.print(" se registro un tirmpo de: ");
  //Serial.println(seconds/1000);
  resetTimer();

}

void myTimer::showInfo()
{
  //Serial.println(startTime);
  unsigned long dif=startTime-millis();
  //Serial.print("Remaining time (seconds):");
  //Serial.println(dif/1000);
  //Serial.println(startTime);
  //Serial.println(millis());
  //Serial.println(seconds/1000);
}
