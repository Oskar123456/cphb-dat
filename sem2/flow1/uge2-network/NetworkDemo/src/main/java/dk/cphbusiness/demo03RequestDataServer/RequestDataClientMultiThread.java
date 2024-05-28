package dk.cphbusiness.demo03RequestDataServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import dk.cphbusiness.demo01Simple.SimpleClient;
import dk.cphbusiness.demo04FileServer.RequestFileServerMultiThread;

public class RequestDataClientMultiThread extends SimpleClient {

    private ExecutorService jobqueue;
    private final String httpBaseString =
                    """
                    User-Agent: SimpleWebClient\r
                    Accept: */*\r
                    Content-Type: application/x-www-form-urlencoded\r
                    Connection: close\r
                    """;

    public static void main(String[] args) {
        RequestDataClientMultiThread rdc = new RequestDataClientMultiThread();
        for (int i = 0; i < 10; i++) {
            rdc.sendRequestWithThread(
                    "POST", "localhost/path/to/whereever",
                    8080, "some data");
        }
        rdc.kill(10);
    }

    public void kill(int timeoutSecs){
        jobqueue.shutdown();
        try {
            jobqueue.awaitTermination(timeoutSecs, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    public Future<String> sendRequestWithThread(String method, String path, int port, String data){
        if (jobqueue == null){
            int cores = Runtime.getRuntime().availableProcessors();
            jobqueue = Executors.newFixedThreadPool(cores);
        }
        return jobqueue.submit(() -> sendRequestWrapper(method, path, port, data));
    }
    private String sendRequestWrapper(String method, String url, int port, String data){
        String host = (url.contains("/")) ? url.substring(0, url.indexOf("/")) : url;
        String path = (url.contains("/")) ? url.substring(url.indexOf("/")) : url;
        String httpRequest = method + " " + path + " HTTP/1.1\r\n"
                + "host: " + host + "\r\n" + httpBaseString;
        if (data != null && !data.isEmpty())
            httpRequest += "Content-Length: " + data.length() + "\r\n\r\n" + data;
        return sendRequest(httpRequest, host, port);
    }

    public String sendRequest(String httpRequest, String host, int port) {
        RequestDataClient client = new RequestDataClient();
        client.startConnection(host, port);
        String response = client.sendMessage(httpRequest);
        System.out.println(Thread.currentThread().getName() + " : response: " + response);
        client.stopConnection();
        return response;
    }
}
