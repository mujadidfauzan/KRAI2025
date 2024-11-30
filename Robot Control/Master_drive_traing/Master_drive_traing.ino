// Pin Rs485
#include <Ps3Controller.h>


#define enPin 4 // RS pin


int spd = 50;
int lX, lY, rX, rY;
float arah, arah1, amp;

char slaveAdr1 = 'A';
char slaveAdr2 = 'B';
char slaveAdr3 = 'C';


void analog() {
  // Analog position to pwm spd
  lX = map(Ps3.data.analog.stick.lx, -128, 127, -spd, spd);  // Left stick X-axis
  lY = map(Ps3.data.analog.stick.ly, -128, 127, spd, -spd);  // Left stick Y-axis (Y inverted)
  rX = map(Ps3.data.analog.stick.rx, -128, 127, -spd, spd);  // Right stick X-axis
  rY = map(Ps3.data.analog.stick.ry, -128, 127, spd, -spd);  // Right stick Y-axis (Y inverted) 

  // Normalization
  amp = sqrt(lX*lX+lY*lY+1);
  lX *= abs(lX)/amp;
  lY *= abs(lY)/amp;
  amp = sqrt(rX*rX+rY*rY+1);
  rX *= abs(rX)/amp;
  rY *= abs(rY)/amp;

  // Monitoring
  Serial.print("LX: "); Serial.println(lX);
  Serial.print("LY: "); Serial.println(lY);
  Serial.print("RX: "); Serial.println(rX);
  Serial.print("RY: "); Serial.println(rY);
  // Calculate the direction of the right stick in radians

  // Get degree value
  arah1 = atan2(rX, rY);
  arah = rX;
}

void tombol() {
  // R1
  if (Ps3.event.button_down.r1) {
    spd += 10;
    spd = min(spd, 100);
  }

  // L1
  if (Ps3.event.button_down.l1) {
    spd -= 10;
    spd = max(spd, 0);
  }
  
}

// int theta, vel_x, vel_y, magnitude;
int v1, v2, v3;
void kinematics(int vel_x, int vel_y, int rot) {
  vel_y *= -1;
  // theta = theta * 0.0174533;         //convert from deg angle to radian
  // vel_x = magnitude * cos(theta);
  // vel_y = magnitude * sin(theta);
  Serial.print("vel x: "); Serial.println(vel_x);
  Serial.print("vel y: "); Serial.println(vel_y);
  Serial.print("rot -: "); Serial.println(rot);

  v1 = -vel_x + rot;                              // belakang
  v2 = 0.5 * vel_x + (sqrt(3)/2) * vel_y + rot;   // depan kanan
  v3 = 0.5 * vel_x - (sqrt(3)/2) * vel_y + rot;   // depan kiri
}

// CLASS SLAVE
class Slave {
public:
  Slave(char address, int data1) {
    Serial.println(address);
    Serial.println(data1);
    delay(15);
  }

  Slave(char address, int data1, int data2) {
    Serial.println(address);
    Serial.println(data1);
    Serial.println(data2);
    delay(15);
  }

  Slave(char address, int data1, int data2, int data3) {
    Serial.println(address);
    Serial.println(data1);
    Serial.println(data2);
    Serial.println(data3);
    delay(15);
  }
};

void onConnect() {
  Serial.println("Connected.");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(enPin, OUTPUT);

  digitalWrite(enPin, HIGH);

  Ps3.attachOnConnect(onConnect);
  Ps3.begin("e8:6b:ea:c3:e4:3a");
}

void loop() {
  analog();
  tombol();
  kinematics(rX, lY, lX);

  Slave slave1(slaveAdr1, v1);
  Slave slave2(slaveAdr2, v2);
  Slave slave3(slaveAdr3, v3);
}





