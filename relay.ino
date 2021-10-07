bool flagRelayToggled = false;
const int relayBlockSize = 16, relayTypeId = 3;
IODevice relayToToggle;
IODevice relayArray[relayBlockSize];


void createRelayBlock() {
  int pinNrStart = 22;
  // Relay id's start at 30 in database, this is the first block
  const int idIndexStart = 30;

  for (int i = 0; i < relayBlockSize; ++i) {
    IODevice ioDevice = IODevice();
    // map i to id so that it matches ID in database
    int id = idIndexStart + i;
    ioDevice.id = id;
    // Set pin nr
    ioDevice.pinNr = pinNrStart;

    // Set pin mode
    pinMode(pinNrStart, INPUT_PULLUP);
    pinMode(pinNrStart, OUTPUT);
    // add to device array
    relayArray[i] = ioDevice;

    // Increment pin nr for next relay
    pinNrStart++;
  }
}

void setupRelay() {
  createRelayBlock();
}

void setFlagRelayToggled(bool flag) {
  flagRelayToggled = flag;
}

void setRelayIdToToggle(int relayId) {
  for (int i = 0; i < relayBlockSize ; ++i) {
    if (relayArray[i].id == relayId) {
      relayToToggle = relayArray[i];

      break;
    }
  }
}

void serializeToggledRelay(JsonArray items) {
  JsonObject obj = items.createNestedObject();
  obj["id"] = relayToToggle.id;

  if (digitalRead(relayToToggle.pinNr) == LOW) {
    obj["low"] = 1;
  }
  else if (digitalRead(relayToToggle.pinNr) == HIGH) {
    obj["low"] = 0;
  }
}

void addRelayArrayToJsonArray(JsonArray items) {
  for (int i = 0; i < relayBlockSize; i++) {
    JsonObject obj = items.createNestedObject();
    obj["id"] = relayArray[i].id;

    if (digitalRead(relayArray[i].pinNr) == LOW) {
      obj["low"] = 1;
    }
    else if (digitalRead(relayArray[i].pinNr) == HIGH) {
      obj["low"] = 0;
    }
  }
}

void toggleRelay() {
  if (!digitalRead(relayToToggle.pinNr) == LOW) {
    digitalWrite(relayToToggle.pinNr, LOW);
  } else {
    digitalWrite(relayToToggle.pinNr, HIGH);
  }
}

void relayLoop() {
  if (flagRelayToggled) {
    toggleRelay();
    publishMessageRelayStates();

    flagRelayToggled = false;
  }
}
