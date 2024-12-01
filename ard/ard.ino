// Define pin for the buzzer
const int buzzerPin = 10; // Connect the buzzer to pin 8

void setup() {
    // Initialize the buzzer pin as output
    pinMode(buzzerPin, OUTPUT);

    // Start the serial communication
    Serial.begin(9600); // Match the baud rate used in Qt
    Serial.println("Arduino ready to receive messages.");
}

void loop() {
    // Check if data is available on the serial port
    if (Serial.available() > 0) {
        // Read the incoming message
        String receivedMessage = Serial.readStringUntil('\n'); // Read until newline
        receivedMessage.trim(); // Remove any extra whitespace or newline characters

        // Log the received message for debugging
        Serial.print("Received message: ");
        Serial.println(receivedMessage);

        // Activate the buzzer on receiving any message
        activateBuzzer();
    }
}

// Function to activate the buzzer
void activateBuzzer() {
    // Buzzer ON
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // Buzzer active for 1 second

    // Buzzer OFF
    digitalWrite(buzzerPin, LOW);
}
