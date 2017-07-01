/*
   control car with IR controller
  DC motor: 1,2,3,4
*/
#include <AFMotor.h>//use TIMER2A, TIMER2B, TIMER0A, TIMER0B
#include <IRremote.h>//use TIMER2, i changed the timer to timer1
void SwitchParser();

#define CMD_1        0x90//控制up的命令ID
#define CMD_2        0x91//控制down的命令ID
#define CMD_3        0x92//控制left的命令ID
#define CMD_4        0x93//控制right的命令ID
#define CMD_5        0x94//控制stop的命令ID

// DC motor on M1,M2,M3,M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

IRrecv irrecv(9);
decode_results results;
int cmd = 4;

void setup()
{
  Serial.begin(115200);           // set up Serial library at 115200 bps
  Serial.println("start");
  irrecv.enableIRIn(); // Start the receiver

  // turn on motor
  motor1.setSpeed(250);
  motor2.setSpeed(250);
  motor3.setSpeed(250);
  motor4.setSpeed(250);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(200);
}

void loop()
{
  SwitchParser();
  switch (cmd)
  {
    case CMD_1://forward
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case CMD_2://backward
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;
    case CMD_3://left
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case CMD_4://right
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;
    case CMD_5://release motor
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
      break;
    case 5:
      break;
    default:
      //do nothing
      break;
  }

}
void SwitchParser()
{
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
      case 0x80ffb04f://up
        cmd = CMD_1;//forward
        break;
      case 0x80ff708f://down
        cmd = CMD_2;//backward
        break;
      case 0x80ff50af://left
        cmd = CMD_3;//left
        break;
      case 0x80ffd02f://right
        cmd = CMD_4;//right
        break;
      case 0x80ff30cf://on/off
      case 0x80fff00f://enter
        cmd = CMD_5;//release
        break;
      default:
        //DO NOTHING
        break;
    }
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

  if (Serial.available() > 0) {
    cmd = Serial.read();
    Serial.print(cmd);
  }
}
