import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import javax.swing.plaf.basic.BasicInternalFrameTitlePane;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class httpMinion implements HttpHandler {

  @Override
  public void handle(HttpExchange exchange) throws IOException {
    /*    OutputStream out = exchange.getResponseBody();*/
/*    String response = "lol";
    exchange.sendResponseHeaders(200, response.getBytes().length);
    out.write(response.getBytes());*/
    String method = exchange.getRequestMethod();
    //System.out.println(" >>> " + method + " RECEIVED...");
    // valid methods are upper-case
    if (method.equals("GET")) {
      handleGet(exchange);
    } else {
      throw new IllegalStateException("Unexpected value: " + method);
    }
  }

  private void handleGet(HttpExchange exchange) {
    int statusCode;

    byte[] responseBody = contentGenerator.fetchContent(
      exchange.getRequestURI().getPath(),
      exchange.getRequestURI().getQuery());
    // TODO: send 404 html page
    if (responseBody == null)
      statusCode = 404;
    else
      statusCode = 200;
    try {
      sendResponse(exchange, statusCode, responseBody);
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
  }

  private void sendResponse(HttpExchange exchange, int statusCode, byte[] data) throws IOException {
    if (data != null) {
      exchange.sendResponseHeaders(statusCode, data.length);
      OutputStream out = exchange.getResponseBody();
      out.write(data);
      out.close();
    } else {
      exchange.sendResponseHeaders(statusCode, 0);
    }
  }
}
