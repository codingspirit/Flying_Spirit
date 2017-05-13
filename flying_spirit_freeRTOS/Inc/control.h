
typedef struct
{
    float P;
    float I;
    float D;
    float Desired;
    float Error;
    float PreError;
    float PrePreError;
    float Increment;
    float Integ;
		float iLimit;
    float Deriv;
    float Output;
 
}PID_Typedef;

extern PID_Typedef pitch_angle_PID;	  //pitch PID
extern PID_Typedef pitch_rate_PID;		//pitch PID

extern PID_Typedef roll_angle_PID;    //roll PID
extern PID_Typedef roll_rate_PID;     //roll PID

extern PID_Typedef yaw_angle_PID;     //yaw PID  
extern PID_Typedef yaw_rate_PID;      //yaw PID  

extern PID_Typedef	alt_PID;
extern PID_Typedef alt_vel_PID;
