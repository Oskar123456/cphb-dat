package flow1.http;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    public static void main(String... args) throws IOException {
        Socket clientsock = new Socket("127.0.0.1", 8080);

        //BufferedReader inputFromServer = new BufferedReader(new InputStreamReader(clientsock.getInputStream()));

        //String messageFromServer = inputFromServer.readLine();
        //System.out.println("from server:\t" + messageFromServer);

        PrintWriter pw = new PrintWriter(clientsock.getOutputStream(), true);

        Scanner userInput = new Scanner(System.in);
        while (userInput.hasNextLine())
                pw.println(userInput.nextLine());

        clientsock.close();
    }
}
