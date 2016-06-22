// read inputs and update ControllerState
void readState()
{
  // --- read main control byte ---
  
  if (digitalRead(SASPIN))
    MainControls(SAS, HIGH);
  else
    MainControls(SAS, LOW);
    
  if (digitalRead(RCSPIN))
    MainControls(RCS, HIGH);
  else
    MainControls(RCS, LOW);
  
  if (digitalRead(GEARPIN))
    MainControls(GEAR, HIGH);
  else
    MainControls(GEAR, LOW);
  
  if (digitalRead(BRAKESPIN))
    MainControls(BRAKES, HIGH);
  else
    MainControls(BRAKES, LOW);
  
  if (digitalRead(LIGHTSPIN))
    MainControls(LIGHTS, HIGH);
  else
    MainControls(LIGHTS, LOW);
  
  if (digitalRead(PRECPIN))
    MainControls(PRECISION, HIGH);
  else
    MainControls(PRECISION, LOW);
  
  if (digitalRead(STAGEPIN))
    MainControls(STAGE, HIGH);
  else
    MainControls(STAGE, LOW);
  
  if (digitalRead(ABORTPIN)) // over 3 V is logically HIGH
    MainControls(ABORT, HIGH);
  else
    MainControls(ABORT, LOW);
  
  
  // --- read digital joystick input ---
  
  if (digitalRead(TXPPIN))
    ControllerState.TX = 1000;
  else if (digitalRead(TXNPIN))
    ControllerState.TX = -1000;
  else
    ControllerState.TX = 0;
    
  if (digitalRead(TYPPIN))
    ControllerState.TY = 1000;
  else if (digitalRead(TYNPIN))
    ControllerState.TY = -1000;
  else
    ControllerState.TY = 0;

  if (digitalRead(TZPPIN))
    ControllerState.TZ = 1000;
  else if (digitalRead(TZNPIN))
    ControllerState.TZ = -1000;
  else
    ControllerState.TZ = 0;
  
  
  // --- read analog joystick input ---
  
  ControllerState.Pitch = constrain(map(analogRead(PITCHPIN),JXMIN+JDB,JXMAX-JDB,-1000,1000),-1000, 1000);
  ControllerState.Roll = constrain(map(analogRead(ROLLPIN),JYMIN+JDB,JYMAX-JDB,-1000,1000),-1000, 1000);
  ControllerState.Yaw = constrain(map(analogRead(YAWPIN),JZMIN+JDB,JZMAX-JDB,-1000,1000),-1000, 1000);
  
  
  // --- read throttle potentiometer ---
  
  ControllerState.Throttle = constrain(map(analogRead(THROTTLEPIN),0,1023,0,1000),0, 1000);
  
}

// for debugging
void printState()
{
  Serial.print("Main Control Byte: ");
  Serial.println(ControllerState.MainControls);
  
  Serial.print("Pitch: "); Serial.println(ControllerState.Pitch);
  Serial.print("Yaw: "); Serial.println(ControllerState.Yaw);
  Serial.print("Roll: "); Serial.println(ControllerState.Roll);
  
  Serial.print("TX: "); Serial.println(ControllerState.TX);
  Serial.print("TY: "); Serial.println(ControllerState.TY);
  Serial.print("TZ: "); Serial.println(ControllerState.TZ);
  
  Serial.print("Throttle: "); Serial.println(ControllerState.Throttle);
}

// utility function for setting input flags
// copied from KSP SerialIO demo
void MainControls(byte n, boolean s) 
{
  if (s)
    ControllerState.MainControls |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
  else
    ControllerState.MainControls &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
}
