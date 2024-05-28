package dk.cphbusiness.demo05ThreadServer.demo04FileServer;

import dk.cphbusiness.demo01Simple.SimpleClient;

public class ThreadServerClient extends SimpleClient {

    public String sendRequest(String httpRequest, String host, int port){
        ThreadServerClient client = new ThreadServerClient();
        client.startConnection(host, port);
        String response = client.sendMessage(httpRequest);
        client.stopConnection();
        return response;
    }
}
