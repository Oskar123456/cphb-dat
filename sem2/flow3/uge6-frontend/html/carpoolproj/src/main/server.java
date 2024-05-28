import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class server {
  public static void main(String[] args) throws IOException {
    server s = new server();
    s.serve();
  }

  /*
  * database vars
  * */
  String username = "postgres";
  String pwd = "postgres";
  String url = "jdbc:postgresql://localhost:5432/carpool";

  /*
  * server vars
  * */
  int port = 8088;

  HttpServer httpServer;
  ExecutorService jobQueue;

  public server() throws IOException {
    dbConnector.init(username, pwd, url);

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

