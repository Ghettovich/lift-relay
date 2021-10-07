// Update these with values suitable for your network.
byte mac[] = {0xe4, 0x3a, 0xb1, 0xf1, 0x4e, 0x6c};
IPAddress ip(192, 168, 178, 20);
IPAddress server(192, 168, 178, 251);

const char proximityLiftTopic[] = "proximity/lift";
const char relayStatesTopic[] = "relay/states";
const char toggleRelaySub[] = "toggle/relay";

EthernetClient ethClient;
PubSubClient client(ethClient);


void setupMqttClient() {
  client.setBufferSize(512);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void mqttLoop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

void callback(char* topic, byte* payload, unsigned int len) {
  if (strcmp(toggleRelaySub, topic) == 0) {
    Serial.println("toggle relay");

    DynamicJsonDocument doc(len);
    deserializeJson(doc, payload);

    int toggleRelayId = doc["toggle"];

    if (toggleRelayId) {
      setRelayIdToToggle(toggleRelayId);
      setFlagRelayToggled(true);
    } else {
      Serial.println("converting relay id failed");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      publishMessageRelayStates();
      publishMessageProximityChanged();

      // ... and resubscribe
      client.subscribe(toggleRelaySub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishMessageProximityChanged() {
  char payload[512];
  DynamicJsonDocument doc(512);
  JsonArray sensors = doc.createNestedArray("proximities");

  addBinDropToJsonArray(sensors);
  addBinLoadToJsonArray(sensors);
  addLiftBottomToJsonArray(sensors);
  addLiftTopToJsonArray(sensors);

  serializeJson(doc, payload);

  if (client.publish(proximityLiftTopic, reinterpret_cast<const unsigned char *>(payload), strlen(payload), true)) {
    Serial.println("Success");
  } else {
    Serial.print("Failure with code ");
    Serial.println(client.state());
  }
}

void publishMessageRelayStates() {
  char payload[512];
  DynamicJsonDocument doc(512);
  JsonArray items = doc.createNestedArray("relays");

  addRelayArrayToJsonArray(items);

  serializeJson(doc, payload);

  if (client.publish(relayStatesTopic, reinterpret_cast<const unsigned char *>(payload), strlen(payload), true)) {
    Serial.println("Success");
  } else {
    Serial.print("Failure with code ");
    Serial.println(client.state());
  }
}
