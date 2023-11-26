#include <WebServer.h>
WebServer server(80);
#define HOSTNAME "SinricPro"
void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>" HOSTNAME " Demo </title>" \
                "<meta http-equiv=\"Content-Type\" " \
                    "content=\"text/html;charset=utf-8\">" \
                "</head>" \
                "<body>" \
                  "<h1>Control All IR Devices with " HOSTNAME ".</h1>" \
                      "<h2>RGB Board</h2>" \
                      "<p><a href=\"lm?code=16236607\"> RGB ON</a></p>" \
                      "<p><a href=\"lm?code=16203967\"> OFF</a></p>" \
                "</body>" \
              "</html>");
}




void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}


void requestHandlerarq() {
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      int code = strtoul(server.arg(i).c_str(), NULL, 10);

      String s = server.arg(i);
      Serial.println(s);
      Serial.println(code);
    }
  }
  handleRoot();
}
