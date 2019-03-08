#include <Wire.h>
#include <JY901.h>
#include <MsTimer2.h>
//#include <TimerOne.h>
int motorPWM = 6;
float Balance_Kp = 40, Balance_Ki = 50, Balance_Kd = 50;
float Angle_X = 0, Acc_X = 0;
float G_rpm = 0;
void Write_IMU() {
	//print received data. Data was received in serialEvent;
	//Serial.print("Acc:"); Serial.print((float)JY901.stcAcc.a[0] / 32768 * 16); Serial.print(" "); Serial.print((float)JY901.stcAcc.a[1] / 32768 * 16); Serial.print(" "); Serial.println((float)JY901.stcAcc.a[2] / 32768 * 16);
	 //Serial.print("Gyro:"); Serial.print((float)JY901.stcGyro.w[0] / 32768 * 2000); Serial.print(" "); Serial.print((float)JY901.stcGyro.w[1] / 32768 * 2000); Serial.print(" "); Serial.println((float)JY901.stcGyro.w[2] / 32768 * 2000);
	//Serial.print("Angle:"); Serial.print((float)JY901.stcAngle.Angle[0] / 32768 * 180); Serial.print(" "); Serial.print((float)JY901.stcAngle.Angle[1] / 32768 * 180); Serial.print(" "); Serial.println((float)JY901.stcAngle.Angle[2] / 32768 * 180);
	//Serial.print("Mag:"); Serial.print(JY901.stcMag.h[0]); Serial.print(" "); Serial.print(JY901.stcMag.h[1]); Serial.print(" "); Serial.println(JY901.stcMag.h[2]);
	//Serial.println("");
	//delay(1000);
	Serial.print((float)JY901.stcAngle.Angle[1] / 32768 * 180); Serial.print(" "); Serial.print(" "); Serial.print((float)JY901.stcAcc.a[1] / 32768 * 16);
	Serial.println("");
	Angle_X = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
	Acc_X = (float)JY901.stcAcc.a[1] / 32768 * 16;
	while (Serial1.available()){
		JY901.CopeSerialData(Serial1.read()); //Call JY901 data cope function
	}

}

int On_Ground() {
	if (Angle_X > -5 && Angle_X < 5) {
		analogWrite(6, 20);
		return 1;
	}
	else if (Angle_X > 84 && Angle_X < 96) {
		analogWrite(6, 20);
		return 1;
	}
	else
		return 0;
}

int Balance_Range() {
	if (Angle_X > 35 && Angle_X < 55)
		return 1;
	else
		return 0;
}

/*ת��->pwm*/
/*��������ƫ-2000 �ٶȷ�Χ-2000~2000*/
int RPM_To_PWM(int n) {
	int pwm = 0;
	pwm = n / 4000 * 255;
	return pwm;
}


/*PI*/
float PI_control() {
	float pwm = 0;

	return pwm;
}
/*PD����
*��������,��������
*�뵱ǰ�ٶȱȽϱ�֤ÿ�μ��ٶȷ�����ȷ
*�����һ���ж�ʱ����û��Խ��ƽ��� �������ٻ������ٶȵ��ۼ�
*���Էſ�ʱ�������λ�ó��� Ȼ��������ظ�λ��
*��������ҿ������д���ϣ��������������
*Email address��herenzhie@gmail.com
**/
float PD_control() {
	float dd = 0,rpm=0;
	float bias = Angle_X - 44;
	dd = Balance_Kp * (bias) + Balance_Kd * Acc_X;
	Serial.print(dd);
	G_rpm += rpm;
	float pwm = RPM_To_PWM(G_rpm);
	return pwm;
}
/*PID*/
float PID_control() {
	float pwm = 0;

	return pwm;
}

/*
*5ms �жϺ���
*���cubheλ��״̬  ������ƽ��λ�� ��ʼƽ��
*���ڵ��������ת��
*/
void control() {
	if (Balance_Range()) {
		//digitalWrite(4, HIGH);
		analogWrite(6, PD_control());
		//delay(200);
		//digitalWrite(4, LOW);
	}
	else if (On_Ground()) {
		analogWrite(6, 20);
	}
}

void setup()
{
	Serial.begin(9600);
	Serial1.begin(9600);
	digitalWrite(4, HIGH);
	digitalWrite(13, HIGH);
	pinMode(motorPWM, OUTPUT);
	digitalWrite(4, HIGH);
	analogWrite(motorPWM, 150);
	//Timer1.initialize(10);
	//Timer1.attachInterrupt(control);
	MsTimer2::set(5, control);
	MsTimer2::start();

}

void loop()
{
	Write_IMU();
	delay(1000);
}


