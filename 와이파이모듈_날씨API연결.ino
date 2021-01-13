#include "WiFiEsp.h"
#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 2
SoftwareSerial esp8266(txPin, rxPin);

char ssid[]="U+Net9C63";
char pass[]="32F4002054"; //우리집 와이파이 비밀번호
int status=WL_IDLE_STATUS;

const char* host="www.kma.go.kr";
const String url="/wid/queryDFSRSS.jsp?zone=1138055100"; //우리 동네 날씨 rss
WiFiEspClient client; //객체
String line0="";
String line1="";
uint8_t cnt=0;
unsigned long int cnt_time=0;
bool cnt_start=false;
int cnt_val=0;

void get_weather(){
  Serial.println("Starting connection to server...");
  if(client.connect(host, 80)){ //날씨 서버에 연결
    Serial.println("Connected to server");
    client.print("GET"+url+"HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
    //서버에 날씨 rss 데이터 요청하고 TCP 연결 종료
    /*client.print("GET ");
    client.print(url);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");*/
    cnt_start=true;
  }
  else{
    Serial.println("Connect Failed!");
  }
}
void setup(){
  Serial.begin(9600);
  esp8266.begin(9600); //wifi 시리얼
  WiFi.init(&esp8266); //wifi 모듈 초기화
  while(status!=WL_CONNECTED){ //연결 안돼있으면
    Serial.print("Attempting to connet to WPA SSID: ");
    Serial.println(ssid);
    status=WiFi.begin(ssid, pass); //연결해준다
  }
  Serial.println("You're connected to the network");
  Serial.println();
  delay(1000);
  get_weather();
}

void check_client_available(){
  if(cnt_start==true){
    if(millis()-cnt_time>=100){
      cnt_time=millis();
      cnt_val++;
      Serial.print(".");
      if(cnt_val==30){
        cnt_start=false;
      }
    }
  }
}
void loop(){
  while(client.available()){ //날씨 데이터 수신하면 while문 진입
    char c=client.read();
    if(c=='<'){
      cnt++;
    }
    if(cnt>28&&cnt<=78){
      line0+=c;
    }
    else if(cnt>78&&cnt<=118){
      line1+=c;
    }
    else if(cnt>118){
      break;
    }
    //Serial.write(c); //데이터 받고 시리얼에 출력
  } //client 종료됨
  if(cnt!=0){
    while(client.available()){
      char c=client.read();
    }
    if(!client.connected()){
      Serial.println();
      Serial.println("Disconnecting from server...");
      client.stop();
    }
    Serial.println(cnt);
    Serial.println("weather data for parsing");
    Serial.println(line0);
    Serial.println(line1);
    cnt=0;
  }
}
