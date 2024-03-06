import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class server {
  public static void main(String[] args) throws IOException {
    server s = new server();
    s.serve();
  }

  int port = 8080;

  HttpServer httpServer;
  ExecutorService jobQueue;

  public server() throws IOException {
    int nCores = Runtime.getRuntime().availableProcessors();
    jobQueue = Executors.newFixedThreadPool(nCores);

    httpServer = HttpServer.create(new InetSocketAddress(port), 0);
    httpServer.createContext("/", new httpMinion());
    httpServer.setExecutor(null);

    System.out.println("server initialized...");
  }

  public void serve() {
    System.out.println("server started, listening on " + httpServer.getAddress().toString() + "...");
    httpServer.start();
  }
}

