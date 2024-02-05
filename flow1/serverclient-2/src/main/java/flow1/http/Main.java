package flow1.http;


import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class Main {
    public static void main(String... args) throws Exception {
        final int port = 8080;
        final ServerSocket server = new ServerSocket( port );
        System.out.println( "Listening for connection on port "+ port + " ...." );
        while ( true ) { // keep listening (as is normal for a server)
            try ( Socket socket = server.accept() ) {
                System.out.println( "-----------------" );
                System.out.println( "Client: " +
                        socket.getInetAddress().getHostName() );
                System.out.println( "-----------------" );
                BufferedReader br = new BufferedReader(
                        new InputStreamReader( socket.getInputStream() ) );
                String line;
                while ( !( ( line = br.readLine() ).isEmpty() ) ) {
                    System.out.println( line );
                }
                String httpResponse = "HTTP/1.1 200 OK\r\n\r\n" + "Hello 2. Sem";
                socket.getOutputStream().write( httpResponse.getBytes( "UTF-8" ) );
            }
        }
    }
}