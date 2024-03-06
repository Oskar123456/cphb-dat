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
    switch (method) { // valid methods are upper-case
      case "GET":
        handleGet(exchange);
        break;
      default:
        throw new IllegalStateException("Unexpected value: " + method);
    }
  }

  private void handleGet(HttpExchange exchange) {
    String resFolder = System.getProperty("user.dir") + "/resources";

    //locate the right folder

    String reqMediaType = "Sec-Fetch-Dest";
    Map<String, List<String>> headers = exchange.getRequestHeaders();
    if (headers.containsKey(reqMediaType)){
      for (String property : headers.get(reqMediaType)){
        if (property.equals("style")){
          //resFolder += "/css";
          break;
        }
        if (property.equals("image")){
          //resFolder += "/images";
          break;
        }
        if (property.equals("document")){
          resFolder += "/docs";
          break;
        }
      }
    }


    int statusCode = 200;
    byte[] data = null;
    System.out.println(" >>> GET RECEIVED...");
    String resPath = resFolder + exchange.getRequestURI().getPath();
    System.out.println(" >>> File requested: " + resPath);
    OutputStream out = exchange.getResponseBody();
    if (exchange.getRequestURI().toString().equals("/")) {
      resPath += "index.html";
    }
    try {
      System.out.println("fetching " + resPath);
      data = Files.readAllBytes(Paths.get(resPath));
    } catch (IOException e) {
      System.out.println("Could not find " + resPath);
      statusCode = 404;
    }
    try {
      sendResponse(exchange, statusCode, data);
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
  }

  private void sendResponse(HttpExchange exchange, int statusCode, byte[] data) throws IOException {
    if (data != null) {
      //System.out.println(">>>Sending:\n" + new String(data));
      exchange.sendResponseHeaders(statusCode, data.length);
      OutputStream out = exchange.getResponseBody();
      out.write(data);
      out.close();
    } else {
      exchange.sendResponseHeaders(statusCode, 0);
    }
  }
}
