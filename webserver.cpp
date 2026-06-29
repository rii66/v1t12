#include "webserver.h"
#include <Update.h>
#include "web.h"
#include "auth.h"

void initWebServer() {

  server.on("/", []() {

    if (!server.authenticate(WEB_USER, WEB_PASS)) {
      return server.requestAuthentication();
    }

    server.send(200, "text/html", index_html);
  });

  server.on("/update", HTTP_POST,

    []() {
      if (!server.authenticate(WEB_USER, WEB_PASS)) {
        return server.requestAuthentication();
      }

      server.send(
        200,
        "text/plain",
        Update.hasError() ? "FAIL" : "OK"
      );

      if (!Update.hasError()) {
        ESP.restart();
      }
    },

    []() {
      if (!server.authenticate(WEB_USER, WEB_PASS)) {
        return server.requestAuthentication();
      }

      HTTPUpload& upload = server.upload();

      if (upload.status == UPLOAD_FILE_START) {
        Update.begin(UPDATE_SIZE_UNKNOWN);
      }

      else if (upload.status == UPLOAD_FILE_WRITE) {
        Update.write(
          upload.buf,
          upload.currentSize
        );
      }

      else if (upload.status == UPLOAD_FILE_END) {
        Update.end(true);
      }
    }
  );

  server.begin();
}