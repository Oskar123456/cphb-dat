package app;

import app.config.ThymeleafConfig;
import app.entities.ChatMsg;
import app.entities.ChatUser;
import app.entities.Student;
import app.persistence.ConnectionPool;
import app.persistence.DBMapper;
import app.persistence.DatabaseException;
import io.javalin.Javalin;
import io.javalin.http.Context;
import io.javalin.http.sse.SseClient;
import io.javalin.rendering.template.JavalinThymeleaf;

import java.util.Collections;
import java.util.HashMap;
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

        // events


        // Routing

        app.get("/", ctx -> formatIndex(ctx));
        app.get("/index.html", ctx -> formatIndex(ctx));
        app.get("/login.html", ctx -> formatLogin(ctx));
    }

    private static void formatIndex(Context ctx) throws DatabaseException {
        ctx.formParamAsClass();

        ChatUser user = ctx.sessionAttribute("userID");

        String loginName = ctx.queryParam("loginboxname");
        String loginPwd = ctx.queryParam("loginboxpwd");

        if (user == null){
            System.out.println("user is null");

            if (loginName != null && loginPwd != null
                    && !loginPwd.isEmpty() && !loginName.isEmpty())
            {
                System.out.println("qparams : " + loginName + " / " + loginPwd);
                HashMap<String, String> userList = DBMapper.getAllChatUsers(connectionPool);

                ChatUser userRequested = new ChatUser(loginName, loginPwd);
                System.out.println("user requested : " + userRequested.name + " / " + userRequested.pwd);
                if (userList.get(loginName) == null)
                    DBMapper.insertChatUser(connectionPool, loginName, loginPwd);
                else {
                    if (!userList.get(loginName).equals(loginPwd)){
                        System.out.println("Incorrect pwd for " + loginName);
                        ctx.render("login.html");
                        return;
                    }
                }
                ctx.sessionAttribute("userID", userRequested);
                user = ctx.sessionAttribute("userID");
                System.out.println("logged in : " + user.name + " / " + user.pwd);
            }

            else {
                ctx.render("login.html");
                return;
            }
        }



        String msgBoxContent = ctx.queryParam("msgboxcontent");
        if (msgBoxContent != null && !msgBoxContent.isEmpty()) {
            int retval = DBMapper.insertChatMsg(connectionPool, msgBoxContent, user.name);
        }

        List<ChatMsg> chatMsgsList = DBMapper.getAllChatMsgs(connectionPool);
        Collections.reverse(chatMsgsList); //to pull scrollbar to the bottom, we reverse here and in CSS

        ctx.attribute("chatMsgs", chatMsgsList);
        ctx.render("index.html");
    }

    private static void formatLogin(Context ctx){
        ctx.render("login.html");
    }
}