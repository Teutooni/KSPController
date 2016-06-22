// joystick calibration values
#define JXMIN 0			// minimum value obtained from the ADC when joystick is -X
#define JXMAX 1023		// similarly max value
#define JYMIN 0
#define JYMAX 1023
#define JZMIN 0
#define JZMAX 1023
#define JDB 0


// digital pins
#define SASPIN 0
#define RCSPIN 1
#define GEARPIN 2
#define BRAKESPIN 3
#define LIGHTSPIN 4
#define PRECPIN 5

#define STAGEPIN 6
#define ABORTPIN 7

#define TXPPIN 8
#define TXNPIN 9
#define TYPPIN 10
#define TYNPIN 11
#define TZPPIN 12
#define TZNPIN 13

// analog pins
#define PITCHPIN 0
#define ROLLPIN 1
#define YAWPIN 2
#define THROTTLEPIN 3

//Input enums (used in MainControls byte)
#define SAS 7
#define RCS 6
#define LIGHTS 5
#define GEAR 4
#define BRAKES 3
#define PRECISION 2
#define ABORT 1
#define STAGE 0

//if no message received from KSP for more than 2s, go idle
#define IDLETIMER 2000
#define CONTROLREFRESH 25

//macro
#define details(name) (uint8_t*)&name,sizeof(name)

struct VesselData
{
    byte id;                //1
    float AP;               //2
    float PE;               //3
    float SemiMajorAxis;    //4
    float SemiMinorAxis;    //5
    float VVI;              //6
    float e;                //7
    float inc;              //8
    float G;                //9
    long TAp;               //10
    long TPe;               //11
    float TrueAnomaly;      //12
    float Density;          //13
    long period;            //14
    float RAlt;             //15
    float Alt;              //16
    float Vsurf;            //17
    float Lat;              //18
    float Lon;              //19
    float LiquidFuelTot;    //20
    float LiquidFuel;       //21
    float OxidizerTot;      //22
    float Oxidizer;         //23
    float EChargeTot;       //24
    float ECharge;          //25
    float MonoPropTot;      //26
    float MonoProp;         //27
    float IntakeAirTot;     //28
    float IntakeAir;        //29
    float SolidFuelTot;     //30
    float SolidFuel;        //31
    float XenonGasTot;      //32
    float XenonGas;         //33
    float LiquidFuelTotS;   //34
    float LiquidFuelS;      //35
    float OxidizerTotS;     //36
    float OxidizerS;        //37
    uint32_t MissionTime;   //38
    float deltaTime;        //39
    float VOrbit;           //40
    uint32_t MNTime;        //41
    float MNDeltaV;         //42
    float Pitch;            //43
    float Roll;             //44
    float Heading;          //45
    uint16_t ActionGroups;  //46 status bit order:SAS, RCS, Light, Gear, Brakes, Abort, Custom01 - 10
    byte SOINumber;         //47 SOI Number (decimal format: sun-planet-moon e.g. 130 = kerbin, 131 = mun)
    byte MaxOverHeat;       //48  Max part overheat (% percent)
    float MachNumber;       //49
    float IAS;              //50  Indicated Air Speed
    byte CurrentStage;      //51  Current stage number
    byte TotalStage;        //52  TotalNumber of stages
};

VesselData VData;

struct HandShakePacket
{
  byte id;
  byte M1;
  byte M2;
  byte M3;
};

HandShakePacket HPacket;

struct ControlPacket
{
	byte id;
	byte MainControls;                  //SAS RCS Lights Gear Brakes Precision Abort Stage
	byte Mode;                          //0 = stage, 1 = docking, 2 = map
	unsigned int ControlGroup;          //control groups 1-10 in 2 bytes
	byte AdditionalControlByte1;        //other stuff
	byte AdditionalControlByte2;
	int Pitch;                          //-1000 -> 1000
	int Roll;                           //-1000 -> 1000
	int Yaw;                            //-1000 -> 1000
	int TX;                             //-1000 -> 1000
	int TY;                             //-1000 -> 1000
	int TZ;                             //-1000 -> 1000
	int WheelSteer;                     //-1000 -> 1000
	int Throttle;                       //    0 -> 1000
	int WheelThrottle;                  //    0 -> 1000
};

ControlPacket ControllerState;

unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld;
unsigned long now;

boolean Connected = false;

byte id;

void setup()
{
	Serial.begin(9600);          //  setup serial
  
	pinMode(SASPIN, INPUT);
	pinMode(RCSPIN, INPUT);
	pinMode(GEARPIN, INPUT);
	pinMode(LIGHTSPIN, INPUT);
	pinMode(BRAKESPIN, INPUT);
	pinMode(PRECPIN, INPUT);
	
	pinMode(STAGEPIN, INPUT);
	pinMode(ABORTPIN, INPUT);
	
	pinMode(TXPPIN, INPUT);
	pinMode(TXNPIN, INPUT);
	pinMode(TYPPIN, INPUT);
	pinMode(TYNPIN, INPUT);
	pinMode(TZPPIN, INPUT);
	pinMode(TZNPIN, INPUT);
  
	pinMode(PITCHPIN, INPUT);
	pinMode(ROLLPIN, INPUT);
	pinMode(YAWPIN, INPUT);
  
	pinMode(THROTTLEPIN, INPUT);
	
	//pinMode(LED_BUILTIN, OUTPUT); // for debugging 
}

bool toggle = false;

void loop()
{
  // connect and update vessel state
  if (waitForConnection() != -1)
  {
    // we have a connection
    // check if CONTROLREFRESH time has passed
    now = millis();
    controlTime = now - controlTimeOld;
    if (controlTime > CONTROLREFRESH)
    {
      // time for control update
      controlTimeOld = now;
      readState();
      KSPBoardSendData(details(ControllerState));
    }
  }
  else
  {
    delay(200); // KSP not in flight, sleep
  }
}
