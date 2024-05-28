package org.example;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;

public class exercise1 {
    private static int port = 8080;
    private static String homePage = "index.html";

    public static void main(String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(port);

        while (true){
            Socket cs = ss.accept();
            OutputStream out_stream = cs.getOutputStream();
            httpparser htp = new httpparser();
            htp.readFromSocket(cs);
            System.out.println(htp.getInfo());
            //write the response to the client
            respond(htp, out_stream);
            //clean up (?)
            cs.close();
        }


    }

    private static void respond(httpparser htp, OutputStream out) throws IOException {
        StringBuilder sb = new StringBuilder();
        if (htp.getMethod().equals("GET")){
            String path = (htp.getPath().isEmpty() || htp.getPath().equals("/")) ?
                    homePage : htp.getPath();
            if (path.startsWith("/"))
                path = path.substring(1);
            byte[] data;
            if (path.equals("addournumbers")){
                int result;
                String operator = "";
                int num1 = Integer.parseInt(htp.getQueryParams().get("firstnumber"));
                int num2 = Integer.parseInt(htp.getQueryParams().get("secondnumber"));
                switch (htp.getQueryParams().get("operator")){
                    case "mul":
                        operator = "*";
                        result = num1 * num2;
                        break;
                    default:
                        operator = "+";
                        result = num1 + num2;
                }
                String htmlbody = "Result of " + num1 + operator + num2 + " : " + result;
                data = generateSimpleHtml(htmlbody);
            }
            else
                data = loadFile(path);

            if (data != null)
                writeResponseOK(data, out);
        }
    }

    private static void writeResponseOK(byte[] data, OutputStream out) throws IOException {
        StringBuilder sb = new StringBuilder();
        sb.append("HTTP/1.1 200 OK").append("\r\n");
        sb.append("Connection: close").append("\r\n");
        sb.append("Content-Length: " + data.length).append("\r\n");
        sb.append("\r\n");
        out.write(sb.toString().getBytes());
        out.write(data);
    }

    private static byte[] loadFile(String res) throws IOException {
        ClassLoader classLoader = exercise1.class.getClassLoader();
        InputStream fileStream = classLoader.getResourceAsStream(res);
        if (fileStream == null)
            return null;
        return fileStream.readAllBytes();
    }

    private static byte[] generateSimpleHtml(String body) throws IOException {
        ClassLoader classLoader = exercise1.class.getClassLoader();
        InputStream fileStream = classLoader.getResourceAsStream("index.html");
        if (fileStream == null)
            return null;
        String html = new String(fileStream.readAllBytes());
        html = html.substring(0, html.indexOf("<body>") + "<body>".length())
                + body
                + html.substring(html.indexOf("</body>"));
        return html.getBytes();
    }
}
