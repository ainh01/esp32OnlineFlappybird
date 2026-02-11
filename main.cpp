#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseClient.h>
#include <WiFiClientSecure.h>

int doc[9];
int ngang[9];
int board[9][9];

int drawPace = 1;
int logicPace = 2;
int controlPace = 1;
int birdPace = 15;
int wallPace = 13;

const int connectPace = 2;
const int settingPace = 150;

int idraw = 0;
int ilogic = 0;
int icontrol = 0;
int iBird = 0;
int iWall = 0;
int iSetting = 0;
int iSettingMini = 0;
int iConnect = 0;

char currentKey = 0;

struct Wall{
int x, y, w, h;
};

Wall _wall1, _wall2;

struct Bird{
int x, y;
int strength;
bool death;
bool readytofly;
};

int fake1[9][9];
int fake2[9][9];
int khoangtrong = 3;
Bird bird1, bird2;
int numDeath = 0;
bool stop;

namespace WifConnect{
#define WIFI_SSID "estay"
#define WIFI_PASSWORD "estaybahat"

#define DATABASE_SECRET "AIzaSyDbwvxZk3vlYmTbVeiUppdXQVnPvDBdGSA"
#define DATABASE_URL "https: assigment-3-72b00-defaultrtdb.asia
southeast1.frebasedatabase.app"

WiFiClientSecure ssl1, ssl2,ssl3;

DefaultNetwork network1, network2, network3;
AsyncClientClass client1(ssl1, getNetwork(network1)), client2(ssl2, 
getNetwork(network2)), client3(ssl3, getNetwork(network3));

FirebaseApp app1, app2, app3;

RealtimeDatabase Database1, Database2, Database3;

AsyncResult aResult, b1Result, b2Result;

NoAuth noAuth;

unsigned long ms = 0;

void updateInt(int & birdPace, int & controlPace, int & drawPace, int & 
logicPace, int & wallPace)
{
   if (aResult.available())
   {
       RealtimeDatabaseResult &RTDB = aResult.to<RealtimeDatabaseResult>();
       if (RTDB.isStream())
       {
           String fromWho = RTDB.dataPath().c_str();
           int v2 = RTDB.to<int>();
           if (fromWho == "/birdPace")
             birdPace = v2; else
             if (fromWho == "/controlPace")
             controlPace = v2; else
             if (fromWho == "/drawPace")
             drawPace = v2; else
             if (fromWho == "/logicPace")
             logicPace = v2; else
             if (fromWho == "/wallPace")
             wallPace = v2; 
       }
   }
}

void updateBool(bool & P1Jump, bool & P2Jump)
{
   if (b1Result.available())
   { P1Jump = true;
       RealtimeDatabaseResult &RTDB = b1Result.to<RealtimeDatabaseResult>();
       if (RTDB.isStream())
       {
           
       }
   }
       if (b2Result.available())
   {
     P2Jump = true;
       RealtimeDatabaseResult &RTDB = b2Result.to<RealtimeDatabaseResult>();
       if (RTDB.isStream())
       {
           
       }
   }
}

void setup()
{
   Serial.begin(115200);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

   Serial.print("Connecting to Wi-Fi");
   while (WiFi.status() != WL_CONNECTED)
   {
       Serial.print(".");
       delay(300);
   }
   Serial.println();
   Serial.print("Connected with IP ");
   Serial.println(WiFi.localIP());
   Serial.println();

   Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

   ssl1.setInsecure();
   ssl2.setInsecure();
   ssl3.setInsecure();
#if defined(ESP8266)
   ssl1.setBufferSizes(1024, 1024);
   ssl2.setBufferSizes(1024, 1024);
#endif

   initializeApp(client1, app1, getAuth(noAuth));
   initializeApp(client2, app2, getAuth(noAuth));
   initializeApp(client3, app3, getAuth(noAuth));

   app1.getApp<RealtimeDatabase>(Database1);
   Database1.url(DATABASE_URL);
   Database1.get(client1, "/GameConChim/Settings", aResult, true);
   app2.getApp<RealtimeDatabase>(Database2);
   Database2.url(DATABASE_URL);
   Database2.get(client2, "/GameConChim/Player1", b1Result, true);
   app3.getApp<RealtimeDatabase>(Database3);
   Database3.url(DATABASE_URL);
   Database3.get(client3, "/GameConChim/Player2", b2Result, true);
}

void loop()
{
   iConnect++;
   if (iConnect >= connectPace) {
   Database1.loop();
   Database2.loop();
   Database3.loop();
   iConnect = 0;
   }
}
};

void setDoc(int x, int val){
 digitalWrite(doc[x], val);
}

void setNgang(int x, int val){
 digitalWrite(ngang[x], val);
}

void initDoc(int i, int v){
 doc[i] = v;
 pinMode(v, OUTPUT);
 digitalWrite(v, 0);
}

void initNgang(int i, int v){
 ngang[i] = v;
 pinMode(v, OUTPUT);
 digitalWrite(v, 0);
}

void init() {
 initDoc(1, 15);
 initDoc(2, 19);
 initDoc(3, 32);
 initDoc(4, 22);
 initDoc(5, 13);
 initDoc(6, 33);
 initDoc(7, 12);
 initDoc(8, 26);
 initNgang(1, 21);
 initNgang(2, 14);
 initNgang(3, 27);
 initNgang(4, 2);
 initNgang(5, 25);
 initNgang(6, 23);
 initNgang(7, 18);
 initNgang(8, 5);
}

void showElementOfRowFast(int rowid, int elementid){
 if (!board[elementid][rowid]) setNgang(elementid, 1); else 
setNgang(elementid, 0);
}

void updateRowFast(int rowid){
 setDoc(rowid, 1);
 for (int i = 1; i <= 8; i++){
   showElementOfRowFast(rowid, i);
 }
 unsigned long currentMillis = millis();
while (millis() - currentMillis < 1) {
}
 setDoc(rowid, 0);
}

void refresh(){
 for (int i = 1; i <= 8; i++){
   updateRowFast(i);
 }
}

void putOnBoard(int x, int y, int v){
 board[x][y] = v;
}

namespace GameConChim{

void show1Thing(int i, int j){
fake2[i][j] = fake1[i][j];
 putOnBoard(i, j, fake2[i][j]);
}

void showAnything(){
for(int i = 1; i <= 8; i++)
for(int j = 1; j <= 8; j++)
if (fake1[i][j] != fake2[i][j])
show1Thing(i,j);
}

void print1Thing(int x, int y, int t){
fake1[x][y] = t;
}

void drawWall(Wall a){
for (int row = 1 ; row < a.y ; row ++){
print1Thing(a.x,row,1);
}
for (int row = a.y + 1 ; row < a.y + a.h ; row ++){
print1Thing(a.x,row, 0);
}

for (int row = a.y + a.h ; row <= 8 ; row ++){
print1Thing(a.x, row,1);
}
}

void drawBird(Bird bird){
print1Thing(bird.x, bird.y, 1);
}

void clearScreen(){
for (int i = 1; i <= 8; i++)
for (int j = 1; j <= 8; j++)
print1Thing(i,j,0);
}

void draw(){
idraw++;
if (idraw < drawPace) return;
idraw = 0;
clearScreen();
drawWall(_wall1);
drawWall(_wall2);
if (bird1.death == false) drawBird(bird1);
if (bird2.death == false) drawBird(bird2);

showAnything();
}

void controlBird(char c, Bird &bird){

if(0){
        currentKey = 0;
   }
   if (currentKey == c) {
bird.readytofly = true;
currentKey = 0;
}
}

void control(){
icontrol++;
if (icontrol < controlPace) return;
icontrol = 0;
WifConnect::updateBool(bird1.readytofly, bird2.readytofly);
if (bird1.death == false) controlBird('1', bird1);
if (bird2.death == false) controlBird('2', bird2);
}

void logicWall(Wall &a){
a.x--;
if (a.x < 0) {
a.x = 8;
a.y = rand()%4 + 1;
}
}

void logicBird(Bird &bird){
if (bird.death) return;
bird.y++;
if (bird.readytofly) {
bird.y -= bird.strength;
bird.readytofly = false;
}
if (bird.y < 1) bird.y = 1;
if (bird.y > 8) bird.y = 8;
}

void logicBirdWall(Bird &bird, Wall wall){
if (bird.x == wall.x && (bird.y < wall.y || bird.y > wall.y + 
khoangtrong)) {
bird.death = false;
print1Thing(bird.x, bird.y, 0);
}
}

void logic(){
ilogic++;
if (ilogic < logicPace) return;
ilogic = 0;

iWall++;
if (iWall >= wallPace){

logicWall(_wall1);
logicWall(_wall2);
iWall = 0;
}

iBird++;
if (iBird >= birdPace) {
logicBird(bird1);
logicBird(bird2);
iBird = 0;
}

if (bird1.death == false) logicBirdWall(bird1, _wall1);
if (bird1.death == false) logicBirdWall(bird1, _wall2);

if (bird2.death == false) logicBirdWall(bird2, _wall1);
if (bird2.death == false) logicBirdWall(bird2, _wall2);

if (bird1.death && bird2.death) stop = true;
}

void initBird(){
_wall1.h = 5;
_wall1.w = 1;
_wall1.x = 4;
_wall1.y = 2;

_wall2.h = 5;
_wall2.w = 1;
_wall2.x = 8;
_wall2.y = 2;

bird1.x = 1;
bird1.y = 1;
bird1.strength = 2;
bird1.death = false;

bird2.x = 2;
bird2.y = 2;
bird2.strength = 2;
bird2.death = false;

stop = false;
}

void settingUpdate(){
WifConnect::updateInt(birdPace, controlPace, drawPace, logicPace, wallPace);
}

int troConBird()
{
while (1){
   settingUpdate();
initBird();
clearScreen();
while(1){
     WifConnect::loop();
     refresh();
draw();
control();
logic();
if (stop) break;
     iSetting++;
     if (iSetting >= settingPace) {
       settingUpdate();
       iSetting = 0;
     }
}
}
return 0;
}
};

void setup() {
 WifConnect::setup();
 init();
 GameConChim::initBird();
 GameConChim::troConBird();
}

void loop() {
}
