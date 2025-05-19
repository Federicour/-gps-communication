# -gps-communication_project
📲 What the Code Does
*  Initialization (setup):

Begins serial communication for GSM and GPS modules.

Configures the GSM module to send SMS in text mode.

Sets up input (button) and output (buzzer) pins.

*  Main Loop (loop):

Continuously listens to the GPS module for updated location data.

Monitors the button press.

*  When the Button is Pressed:

GPS location (latitude and longitude) is read.

An SMS is sent to a predefined phone number (phone_no) with:

Latitude and longitude

A Google Maps link showing the location

Buzzer sounds for 1 second after successful SMS.

*  SMS Send Prevention:

A flag (isSMSsent) ensures the SMS is sent only once per press to avoid spamming.

