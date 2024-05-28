package dk.cphbusiness.demo03RequestDataServer;

import dk.cphbusiness.demo01Simple.SimpleServer;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/*
 * Purpose of this demo is to show how to get the data from the request headers etc.
 * This is a simple http server that can handle GET, POST, PUT and PATCH requests with a single client.
 * The server must therefore be restarted for each request.
 * Author: Thomas Hartmann
 */
public class RequestDataServerMultiThread extends SimpleServer {
    public static void main(String[] args) {
        RequestDataServer server = new RequestDataServer();
        server.start(8080);
    }

    @Override
    public void start(int port) {
        int cores = Runtime.getRuntime().availableProcessors();
        ExecutorService jobqueue = Executors.newFixedThreadPool(cores);
        try {
            serverSocket = new ServerSocket(port);
            while (true) { // keep listening untill server is stopped
                clientSocket = serverSocket.accept(); // blocking call
                // Runnable is functional interface ; we can use lambda
                jobqueue.submit(() -> makeResponse(clientSocket));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void makeResponse(Socket clientSocket) {
        try {
            // PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            // Read the request and send it back to the client
            RequestDataServer rqs = new RequestDataServer();
            String response = rqs.generateRequestObject(in).toString();
            clientSocket.getOutputStream().write(response.getBytes());

            // Close the socket
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
