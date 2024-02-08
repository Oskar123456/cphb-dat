package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class exercise1 {
    private static int port = 8080;

    public static void main(String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(port);
        Socket cs = ss.accept();

        httpparser htp = new httpparser();
        htp.readFromSocket(cs);

        System.out.println(htp.getInfo());
    }
}
