void website() {
  WiFiClient client = server.available();   // Listen for incoming clients
  String header;                            // Store the HTTP request
  String doorState = "closed";              
  String enrollState = "null";
  const char* login = "YWRtaW46YWRtaW4="; // base64encoding admin:admin - "YWRtaW46YWRtaW4="

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            
            if (header.indexOf(login) >= 0)   //makes a HTTP authentication before gaining access to the website
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if (header.indexOf("GET /2/open") >= 0) {
                Serial.println("Door Opening");
                doorState = "open";
                validScan();
              }

              if (header.indexOf("GET /16/enroll") >= 0) {
                Serial.println("*** Begining Enrolling ***");
                enrollState = "enroll";
                enroll();
              }
              else{
                getFingerID();
              }

              // Display the HTML web page
              client.println(F(R"=====(
                <!DOCTYPE HTML>
                  <html>
                    <head>
                     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
                      <link rel=\"icon\" href=\"data:,\">
                      <style>
                        .Container{
                          display: flex;
                           flex-direction: column;
                            }
                          .column {
                          display: flex;
                          flex-direction: column;
                          background-color:#116466;
                          align-items: center;
                          }
                          .row {
                          display: flex;
                          flex-direction: row;
                          background-color:#116466;
                            }
                          
                          .livestream {
                          background-color: #D1E8E2;
                          margin: 40px;
                          padding: 40px;
                          width: 40%;
                          height: 200px;
                          }
                           html { 
                          font-family: Helvetica; 
                          display: inline-block; 
                          margin: 0px auto; 
                          text-align: center;
                          }
                          .buttons{
                          background-color: #D1E8E2;
                          margin: 40px;
                          padding 40px;
                          font-size: 50px;
                          width: 300px;
                          text-align: center;
                          align-items: center;
                          cursor: pointer;
                          transition-duration: 0.4s;
                          }
                          .button2:hover {
                          box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
                          }
                      
                          h1{
                          font: bold;
                          font-size: 40px;
                          font-family: Lucida Sans Unicode;
                          color: #D9B08C;
                          }
                      
                          p{
                          font-size: 25px;
                          font-family: Lucida Sans Unicode;
                          color: #2C3531;
                          }
                      </style>
                    </head>
                      <body>
                        <div class="Container">
                        <div class="column">
                          <h1> Home Security System </h1>
                          <div class="livestream">
                            <iframe id="ArduCAM Capture"
                            title="Capture Stream"
                            width="500"
                            height="240"
                            margin-left: auto;
                            margin-right: auto;
                            src="http://192.168.1.15/stream">
                            </iframe>
                          </div>
                          <p> Current Tempreature:  
                          )====="));
              client.print(getTempreature());
              client.println(F(R"=====(
                           *C </p> 
                          <div class="row">
                                  )====="));
              client.println("<p><a href=\"/2/open\"><button class=\"buttons button2\"><p>Open Door</p></button></a></p>");
              client.println("<p><a href=\"/16/enroll\"><button class=\"buttons button2\"><p>Enroll</p></button></a></p>");
              client.println(F(R"=====(
                          </div>
                        </div>
                       </div>
                     </body>
                  </html>
                )====="));
              client.println();
              break;
            }
            else {
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"Secure\"");
              client.println("Content-Type: text/html");
              client.println();
              client.println("<html>Authentication failed</html>");
            }
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
