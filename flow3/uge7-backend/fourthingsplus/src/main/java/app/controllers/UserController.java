package app.controllers;

import app.entities.Task;
import app.entities.User;
import app.exceptions.DatabaseException;
import app.persistence.ConnectionPool;
import app.persistence.TaskMapper;
import app.persistence.UserMapper;
import io.javalin.Javalin;
import io.javalin.http.Context;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class UserController {
    public static void addRoutes(Javalin app, ConnectionPool connPool) {
        app.get("index.html", ctx -> ctx.render("index.html"));
        app.post("login", ctx -> login(ctx, connPool));
        app.get("home", ctx -> login(ctx, connPool));
        app.get("logout", ctx -> logout(ctx));
        app.get("createuser", ctx -> ctx.render("createuser.html"));
        app.get("createuser.html", ctx -> ctx.render("createuser.html"));
        app.post("createuser", ctx -> createUser(ctx, connPool));

    }

    private static void createUser(Context ctx, ConnectionPool connPool) {
        String userName = ctx.formParam("username");
        String pwd = ctx.formParam("pwd");
        String pwd1 = ctx.formParam("pwd1");

        if (userName == null || pwd == null || pwd1 == null)
            return;

        if (pwd.equals(pwd1)) {
            try {
                UserMapper.createuser(userName, pwd, connPool);
                ctx.attribute("message", "bruger oprettet");
                ctx.attribute("recentuser", userName);
                ctx.render("index.html");
                return;
            } catch (DatabaseException e) {
                ctx.attribute("message", "bruger findes allerede");
            }
        } else {
            ctx.attribute("message", "kodeord stemmer ikke overens");
        }
        ctx.render("createuser.html");
    }

    private static void logout(Context ctx) {
        System.out.println("logging out ");
        ctx.req().getSession().invalidate();
        ctx.render("index.html");
    }

    public static void login(Context ctx, ConnectionPool connPool) {
        String username = ctx.formParam("username");
        String pwd = ctx.formParam("pwd");

        User user = (username != null && pwd != null) ? null : ctx.sessionAttribute("currentUser");

        try {

            if (user == null)
                user = UserMapper.login(username, pwd, connPool);

            ctx.sessionAttribute("currentUser", user);

            List<Task> taskList = TaskMapper.getAllTasksPerUser(user.id(), connPool);
            List<Task> taskListDone = new ArrayList<>();
            List<Task> taskListUndone = new ArrayList<>();

            for (Task t : taskList){
                if (t.done()){
                    taskListDone.add(t);
                }
                else
                    taskListUndone.add(t);
            }

            ctx.attribute("taskList", taskListUndone);
            ctx.attribute("taskListDone", taskListDone);

            ctx.render("task.html");
        } catch (DatabaseException e) {
            ctx.attribute("message", e.getMessage());
            ctx.render("index.html");
        }


    }
}
