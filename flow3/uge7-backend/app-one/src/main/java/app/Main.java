package app;

import app.config.ThymeleafConfig;
import app.entities.ChatMsg;
import app.entities.Student;
import app.persistence.ConnectionPool;
import app.persistence.DBMapper;
import app.persistence.DatabaseException;
import io.javalin.Javalin;
import io.javalin.http.Context;
import io.javalin.rendering.template.JavalinThymeleaf;

import java.util.Collections;
import java.util.List;
import java.util.Objects;

public class Main {

    private static final String USER = "postgres";
    private static final String PASSWORD = "postgres";
    private static final String URL = "jdbc:postgresql://localhost:5432/%s?currentSchema=public";
    private static final String DB = "chatsite";

    private static final ConnectionPool connectionPool = ConnectionPool.getInstance(USER, PASSWORD, URL, DB);

    public static String fixedLengthString(String string, int length) {
        return String.format("%1$" + length + "s", string);
    }

    public static void main(String[] args) throws DatabaseException {
        // Initializing Javalin and Jetty webserver

        Javalin app = Javalin.create(config -> {
            config.staticFiles.add("/public");
            config.fileRenderer(new JavalinThymeleaf(ThymeleafConfig.templateEngine()));
        }).start(7070);

        // Routing

        app.get("/", ctx -> formatIndex(ctx));
        app.get("/index.html", ctx -> formatIndex(ctx));



        /*int elmLen = 10;*/
/*
        System.out.println(fixedLengthString("id", elmLen) + "|" +
                fixedLengthString("name", elmLen) + "|" +
                fixedLengthString("pwd", elmLen));
        for (Student s : studentList){
            String id = fixedLengthString(String.valueOf(s.getId()), elmLen);
            String name = fixedLengthString(s.getName(), elmLen);
            String pwd = fixedLengthString(s.getPwd(), elmLen);
            System.out.println(id + "|" + name + "|" + pwd);
        }*/
    }

    private static void formatIndex(Context ctx) throws DatabaseException {
        String msgBoxContent = ctx.queryParam("msgboxcontent");
        System.out.println(msgBoxContent);
        if (msgBoxContent != null && !msgBoxContent.isEmpty()) {
            int retval = DBMapper.insertChatMsg(connectionPool, msgBoxContent, "niels1");
            System.out.println(retval);
        }

        List<ChatMsg> chatMsgsList = DBMapper.getAllChatMsgs(connectionPool);
        Collections.reverse(chatMsgsList);
        for (ChatMsg msg : chatMsgsList) {
            System.out.println(msg.content());
        }
        ctx.attribute("chatMsgs", chatMsgsList);
        ctx.render("index.html");
    }
}