// ISR flags
volatile bool flagBinAtDrop = false;
volatile bool flagBinAtLoad = false;
volatile bool flagLiftAtBottom = false;
volatile bool flagLiftAtTop = false;

// id's correspond to table in database
const int detectionSensorTypeId = 2;
const int sensorDropId = 10, sensorLoadId = 11, sensorLiftTopId = 12, sensorLiftBottomId = 13;
const int sensorBinLoadPinNr = 2, sensorBinDropPinNr = 3, sensorLiftTopPinNr = 18, sensorLiftBottomPinNr = 19;

void setupDetectionSensors() {
  /** !! INPUT proximity switches IF LOW detection = TRUE */
  // set pin mode
  pinMode(sensorBinDropPinNr, INPUT_PULLUP);
  pinMode(sensorBinLoadPinNr, INPUT_PULLUP);
  pinMode(sensorLiftTopPinNr, INPUT_PULLUP);
  pinMode(sensorLiftBottomPinNr, INPUT_PULLUP);

  // ATTACH INTERRUPT EVENTS
  attachInterrupt(digitalPinToInterrupt(sensorBinLoadPinNr), onChangeSensorLiftLoad, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorBinDropPinNr), onChangeSensorLiftDrop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorLiftTopPinNr), onChangeSensorLiftTop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorLiftBottomPinNr), onChangeSensorLiftBottom, CHANGE);
}

/** SENSOR STATE CHANGE (ATTACHED INTERRUPTS) */
// ISR CALL, called when sensor BOTTOM flipped
void onChangeSensorLiftLoad() {
  flagBinAtLoad = true;
}

// ISR CALL, called when sensor TOP flipped
void onChangeSensorLiftDrop() {
  flagBinAtDrop = true;
}

// ISR CALL, called when lift top is flipped
void onChangeSensorLiftTop() {
  flagLiftAtBottom = true;
}

// ISR CALL, called when lift top is flipped
void onChangeSensorLiftBottom() {
  flagLiftAtTop = true;
}

void addBinDropToJsonArray(JsonArray items) {
  JsonObject obj = items.createNestedObject();
  obj["id"] = sensorDropId;

  if (digitalRead(sensorBinDropPinNr) == LOW) {
    obj["low"] = 1;
  }
  else if (digitalRead(sensorBinDropPinNr) == HIGH) {
    obj["low"] = 0;
  }
}

void addBinLoadToJsonArray(JsonArray items) {
  JsonObject obj = items.createNestedObject();
  obj["id"] = sensorLoadId;

  if (digitalRead(sensorBinLoadPinNr) == LOW) {
    obj["low"] = 1;
  }
  else if (digitalRead(sensorBinLoadPinNr) == HIGH) {
    obj["low"] = 0;
  }
}

void addLiftBottomToJsonArray(JsonArray items) {
  JsonObject obj = items.createNestedObject();
  obj["id"] = sensorLiftBottomId;

  if (digitalRead(sensorLiftBottomPinNr) == LOW) {
    obj["low"] = 1;
  }
  else if (digitalRead(sensorLiftBottomPinNr) == HIGH) {
    obj["low"] = 0;
  }
}

void addLiftTopToJsonArray(JsonArray items) {
  JsonObject obj = items.createNestedObject();
  obj["id"] = sensorLiftTopId;

  if (digitalRead(sensorLiftTopPinNr) == LOW) {
    obj["low"] = 1;
  }
  else if (digitalRead(sensorLiftTopPinNr) == HIGH) {
    obj["low"] = 0;
  }
}

void sensorLoop() {
  if (flagBinAtDrop) {
    Serial.println("Flag bin at drop true");

    publishMessageProximityChanged();
    flagBinAtDrop = false;
  }
  if (flagBinAtLoad) {
    Serial.println("Flag bin at load true");

    publishMessageProximityChanged();
    flagBinAtLoad = false;
  }
  if (flagLiftAtBottom) {
    Serial.println("Flag lift at bottom true");

    publishMessageProximityChanged();
    flagLiftAtBottom = false;
  }
  if (flagLiftAtTop) {
    Serial.println("Flag lift at top true");

    publishMessageProximityChanged();
    flagLiftAtTop = false;
  }
}
