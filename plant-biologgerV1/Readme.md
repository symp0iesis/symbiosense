This folder contains the code used to run the first iteration of the symbiosense in the context of plant research, as documented here: https://experiment.com/u/EYOKIw

The codebase consists of a microcontroller board script (run on a adafruit ESP32-s3 2MB RAM via PlatformIO) and an API script stored at hostgador server. The board is then able to send the sensed data directly to the database via wi-fi.

## Procedure to make changes to the database structure

1. Alter the MySQL database structure itself (e.g add/remove columns, change column name, etc)
2. Alter the DataController.php file (on the API side - hosted in the server) to reflect such changes
   - public function set(Request $request, Response $response, $args) {
   - public function create(Request $request, Response $response, $args) {
4. Alter the functions.h file in the PlatformIO scrip to reflect such changes
     - void sendAPI()
           - httpRequestData
5. Alter the var.h in the PlatformIO script to reflect such changes
     - float declarations
6. Alter the main.cpp file to reflect changes
     - varible names and sendAPI call
7. Alter index.html under iot page (on the API side - hosted in the server) to reflect such changes in the monitoring page.
