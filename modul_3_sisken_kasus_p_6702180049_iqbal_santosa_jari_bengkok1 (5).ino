/**************************************************/

//Modul Praktikum 3 - Sistem kendali Kasus P
//Nama          : IQBAL SANTOSA
//Kelompok      : 3
//Nama Tim      : Jari Bengkok
//Anggota 1     : Iqbal Santosa
//Versi Program : 1.0

/**************************************************/
//Deklarasi sensor
int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int sensor5 = A4;
int sensor6 = A5;
int button1 = 8;
int button2 = 9;
int baca_sensor[6];

//Variabel Motor
int pinEnable = 4; //Harus di set high jika ingin mengaktifkan
int pinEnable2 = 2; //Harus di set high jika ingin mengaktifkan
//Variabel Motor kiri
int motor_kiri1 = 5; //input motor driver 1 L293D
int motor_kiri2 = 6; //input motor driver 2 L293D
//Variabel motor kanan
int motor_kanan1 = 3; //input motor driver 3 L293D
int motor_kanan2 = 11;//input motor driver 4 L293D

int maxpwm = 255;
int x;

//Variabel dan konstanta untuk persamaan pid
int kp = 5, ki = 0, kd = 0;
int lastError = 0;
int error = 0;
int rate_i, rate_d;
int kecepatanMotorKanan, kecepatanMotorKiri, kecepatanSetPoint = 150;

void setup()
{
  //Pin Mode input sensor
  pinMode(sensor1, INPUT); //set pin A0 sebagai input
  pinMode(sensor2, INPUT); //set pin A1 sebagai input
  pinMode(sensor3, INPUT); //set pin A2 sebagai input
  pinMode(sensor4, INPUT); //set pin A3 sebagai input
  pinMode(sensor5, INPUT); //set pin A4 sebagai input
  pinMode(sensor6, INPUT); //set pin A5 sebagai input
  
  //pin mode output motor
  pinMode(pinEnable, OUTPUT);
  pinMode(pinEnable2, OUTPUT);
  pinMode(motor_kiri1, OUTPUT);
  pinMode(motor_kiri2, OUTPUT);
  pinMode(motor_kanan1, OUTPUT);
  pinMode(motor_kanan2, OUTPUT);
  
  //Insialisasi komunikasi serial
  Serial.begin(9600); //default 9600
}

void readsensor(){ //fungsi untuk membaca sensor dan menyimpan nilai
baca_sensor[0] = analogRead(sensor1);
baca_sensor[1] = analogRead(sensor2);
baca_sensor[2] = analogRead(sensor3);
baca_sensor[3] = analogRead(sensor4);
baca_sensor[4] = analogRead(sensor5);
baca_sensor[5] = analogRead(sensor6);
  
  delay(50);

  for(x=0; x<=5; x++){
  }  
}

void loop()
{
  readsensor();
  //Serial.print("Nilai last error:");
  //Serial.println(lastError);
  //catatan saat semua sensor mendeteksi gelap kedua motor jalan
  if(baca_sensor[0] < 34 && baca_sensor[1] > 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] > 34 && baca_sensor[5] > 34) {
    error = -3;
  }
  if(baca_sensor[0] < 34 && baca_sensor[1] < 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] > 34 && baca_sensor[5] > 34) {
    error = -2;
  }
   if(baca_sensor[0] > 34 && baca_sensor[1] < 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] > 34 && baca_sensor[5] > 34) {
    error = -1;
  }
  if(baca_sensor[0] > 34 && baca_sensor[1] > 34 &&
   baca_sensor[2] < 34 && baca_sensor[3] < 34 ||  
   baca_sensor[2] < 34 || baca_sensor[3] < 34 &&
     baca_sensor[4] > 34 && baca_sensor[5] > 34){
     error = 0;
  }
  if(baca_sensor[0] > 34 && baca_sensor[1] > 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] < 34 && baca_sensor[5] > 34) {
    error = 1;
  }
  if(baca_sensor[0] > 34 && baca_sensor[1] > 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] < 34 && baca_sensor[5] < 34) {
    error = 2;
  }
  if(baca_sensor[0] > 34 && baca_sensor[1] > 34 &&
   baca_sensor[2] > 34 && baca_sensor[3] > 34 &&
   baca_sensor[4] > 34 && baca_sensor[5] < 34) {
    error = 3;
  }
  //nilai terendah photodioda 33 paling gelap
  //nilai tertinggi = 687 paling terang
  //aktifkan motor
  digitalWrite(pinEnable, HIGH);
  digitalWrite(pinEnable2, HIGH);
  //fungsi deteksi garis
  follow_line();
}

void follow_line(){
readsensor();
  
//fungsi MV
  rate_d = error - lastError;
  rate_i = error + lastError;
  lastError = error;
  int moveControl = (kp * error) + (ki * rate_i) + (kd * rate_d);

  kecepatanMotorKanan = kecepatanSetPoint - moveControl;
  kecepatanMotorKiri = kecepatanSetPoint + moveControl;

  setMotor(kecepatanMotorKiri, kecepatanMotorKanan);
  Serial.print("Kecepatan Motor Kiri : ");
  Serial.println(kecepatanMotorKiri);
  Serial.print("Kecepatan Motor Kanan : ");
  Serial.println(kecepatanMotorKanan);
  Serial.print("Error : ");
  Serial.println(error);
  Serial.print("MoveControl : ");
  Serial.println(moveControl);
  Serial.print("Set Point : ");
  Serial.println(kecepatanSetPoint);
  
}

void setMotor(int pwmLeft, int pwmRight){
  if (pwmLeft > maxpwm){
   pwmLeft = maxpwm;
  }
  else if (pwmLeft < -maxpwm){
   pwmLeft = -maxpwm;
  }
  
  if (pwmLeft < 0){
    pwmLeft *= -1;
    analogWrite(motor_kiri2, pwmLeft);
    analogWrite(motor_kiri1, 0);
    }
  else{
    analogWrite(motor_kiri2, 0);
    analogWrite(motor_kiri1, pwmLeft);
  }
  if (pwmRight > maxpwm){
    pwmRight = maxpwm;
  }
  else if (pwmRight < -maxpwm){
    pwmRight = -maxpwm;
  }
  if (pwmRight < 0){
    pwmRight *= -1;
    analogWrite(motor_kanan2, pwmRight);
    analogWrite(motor_kanan1, 0);
    }
  else{
    analogWrite(motor_kanan2, 0);
    analogWrite(motor_kanan1, pwmRight);
  }
}  