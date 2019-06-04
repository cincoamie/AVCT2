#include <iostream>
#include "E101.h"
using namespace std;

char address[15] = {'1','3','0','.','1','9','5','.','6','.','1','9','6'};
char request[24] = {'P','l','e','a','s','e'};
int port = 1024; 
char pass[24];
bool quad4 = false;

class Robot{
private:
	int m_left, m_right;
	int dv; // dv = difference in speed of left and right motors
	double line_error; 
	const int cam_width = 320;
	const int cam_height = 240;
	const int m_left_go = 52;
	const int m_right_go = 38;
	double kp = 0.05;
	//double kd = 0;
	int line_present = 1;
	int colour = 0;
public:
	void SetMotors();
	void OpenGate();
	int MeasureLine();
	int FollowLine();
	int FollowColour();
	};

// Quadrant 1, open the gate
void Robot::OpenGate(){
	//opens gate
connect_to_server(address,port);
send_to_server(request);
receive_from_server(pass);
send_to_server(pass);
}

//Quadrant 2
int Robot::MeasureLine(){
	hardware_exchange();
	take_picture();
	update_screen();
	int line[26];
	for(int i=0;i<26;i++){
		int pixel = get_pixel(cam_height/2,i*(cam_width/26),3);		//gets pixel intensity along middle row
		if(pixel>80){
			line[i]=0;				//pixel is white
		}else{
			line[i] = 1;			//pixel is black
		}		
		cout<<line[i]<<endl;		//colour of pixel determined to be black or white
	}
	//checks for presence of line
	int check = 0;					
	for(int a : line){
		check = check + a;
	}	
	if(check<1){
		line_present = 0;
		
		//checks if the robot has reached quadrant 4
		int red = get_pixel(cam_height/2,cam_width/2,0);
		int notRed = get_pixel(cam_height/2,cam_width/2,1) + get_pixel(cam_height/2,cam_width/2,2);
		if(red>notRed){
			quad4 = true;
		}
		
	}else{
		line_present = 1;
	}
	cout<<"Line Present: "<<line_present<<endl;
	
	//calculates error
	int error = 0;
	for(int i=0;i<26;i++){
		line[i] = line[i]*(i-13);
		error += line[i];
	}

	cout<<"Error: "<<error<<endl;
	return error;
}
int Robot::FollowLine(){
	line_error = MeasureLine();
	double l_last;
	double r_last;
	
	//go forward
	if(line_present==1){
		dv = (int)(line_error*kp);
		m_left = m_left_go + dv;
		l_last = m_left_go + dv;
		m_right = m_right_go + dv;
		r_last = m_right_go + dv;
		SetMotors();
		
	//go back
	}else{
		cout<<"Line Missing"<<endl;
		m_left = 35;
		m_right = 55;
		SetMotors();
		sleep1(100);
		if((l_last-45)-(l_last-r_last)<5){
			m_left = 55;
			m_right = 44;
			SetMotors();
			sleep1(100);
		}else if((r_last-45)-(r_last-l_last)>-5){
			m_left = 46;
			m_right = 35;
			SetMotors();
			sleep1(100);
		}
	}
return 0;
}

//Sets speed of left and right motors
void Robot::SetMotors(){
	set_motors(5,m_left);
	set_motors(3,m_right);
}

//Quadrant 4
int Robot::FollowColour(){
	//int pillar[26];
	//if(colour=0){
		
	//}else if(colour=1){
	
	//}else if(colour=2){
	
	//}
return 0;
}

//MAIN
int main(){	
	int err;
	err = init(0);
	cout<<"After init() error="<<err<<endl;
Robot robot;

open_screen_stream();
int count = 0;

while(count<5000){
	//robot.OpenGate();
	
	//if(quad4==false){
		robot.FollowLine();
	//}else if(quad4==true){
	//	robot.FollowColour();
	//}
	count++;
	sleep1(100);
}

close_screen_stream();
stoph();
return 0;
}
