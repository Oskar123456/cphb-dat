package org.example;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class httpparser
{
    private String _request;
    private String _method;
    private String _path;
    private String _httpversion;
    private String _headersstr;
    private Map<String, String> _queryparams;
    private Map<String, String> _headers;
    private String _body;
    private int _bodylength;

    public httpparser(){}
    /**
       Test description
    */
    public void readFromSocket(Socket s) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
        StringBuilder sb = new StringBuilder();
        String inLine = in.readLine();
        _request = inLine;
        System.out.println(_request);
        while ((inLine = in.readLine()) != null && inLine.length() > 0) {
            sb.append(inLine).append(System.lineSeparator());
        }
        _headersstr = sb.toString();
        parse(in);
    }
    private void parse(BufferedReader in) throws IOException {
        // take care of body
        _bodylength = getBodyLength();
        char buf[] = new char[_bodylength];
        in.read(buf, 0, _bodylength);
        _body = new String(buf);
        // format request & headers
        String[] reqfields = _request.split(" ");
        if (reqfields.length < 3)
            return;
        _method = reqfields[0];
        //query params
        if (reqfields[1].contains("?")){
            _path = reqfields[1].split("\\?")[0];
            _queryparams = new HashMap<String, String>();
            for (String qparam : reqfields[1].split("\\?")[1].split("&"))
                _queryparams.put(qparam.split("=")[0], qparam.split("=")[1]);
        } else
            _path = reqfields[1];
        //version
        _httpversion = reqfields[2].split("/")[1];
        //headers
        _headers = new HashMap<String, String>();
        for (String h : _headersstr.split(System.lineSeparator()))
            _headers.put(h.split(":")[0].trim(), h.split(":")[1].trim());
    }
    private int getBodyLength(){
        for (String line : _headersstr.split(System.lineSeparator()))
            if (line.contains("Content-Length:"))
                return Integer.parseInt(line.substring("Content-Length".length() + 1).trim());
        return 0;
    }
    /*
      get/set
    */
    public String getRequest(){return _request;}
    public String getBody(){return _body;}
    public String getMethod(){return _method;}
    public String getHttpVersion(){return _httpversion;}
    public String getHeaders(){return _headersstr;}
    public String getPath(){return _path;}
    public Map<String, String> getQueryParams(){return _queryparams;}

    public String getInfo(){
        StringBuilder sb = new StringBuilder();
        sb.append("\t<<<http request info>>>" + System.lineSeparator() + "method:\t\t");
        if (_method != null)
            sb.append(_method);
        sb.append(System.lineSeparator());
        sb.append("version:\t");
        if (_httpversion != null)
            sb.append(_httpversion);
        sb.append(System.lineSeparator());
        sb.append("path:\t\t");
        if (_path != null)
            sb.append(_path);
        sb.append(System.lineSeparator());
        sb.append("queryparams:\t\t");
        if (_queryparams != null)
            for (Map.Entry<String, String> element : _queryparams.entrySet())
                sb.append(System.lineSeparator()).append("\t\t\t")
                        .append(element.getKey()).append(" = ")
                        .append(element.getValue());
        sb.append(System.lineSeparator());
        sb.append("headers:");
        if (_headers != null)
            for (Map.Entry<String, String> element : _headers.entrySet())
                sb.append(System.lineSeparator()).append("\t\t\t")
                        .append(element.getKey()).append(" = ")
                        .append(element.getValue());
        sb.append(System.lineSeparator());
        if (_bodylength > 0){
            sb.append("body:\t\t");
            sb.append(_body);
            sb.append(System.lineSeparator());
        }
        sb.append("\t<<<http request info>>>");
        sb.append(System.lineSeparator());
        return sb.toString();
    }
}
