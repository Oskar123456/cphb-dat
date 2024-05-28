package app.controllers;

import app.entities.Task;
import app.entities.User;
import app.exceptions.DatabaseException;
import app.persistence.ConnectionPool;
import app.persistence.TaskMapper;
import io.javalin.Javalin;
import io.javalin.http.Context;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

public class TaskController {
    public static void addRoutes(Javalin app, ConnectionPool connPool) {
        app.post("addtask", ctx -> addTask(ctx, connPool));
        app.post("taskdone", ctx -> taskDone(ctx, connPool));
        app.post("taskundone", ctx -> taskUndone(ctx, connPool));
        app.post("taskdelete", ctx -> taskDelete(ctx, connPool));
        app.post("taskedit", ctx -> taskEdit(ctx, connPool));
        app.post("taskeditdone", ctx -> taskEditDone(ctx, connPool));
    }

    private static void taskEditDone(Context ctx, ConnectionPool connPool) {
        User user = ctx.sessionAttribute("currentUser");
        int taskSelectedId = Integer.parseInt(ctx.formParam("taskselected"));
        String taskName = ctx.formParam("taskname");

        try {
            if (taskName.length() < 1)
                ctx.attribute("message", "opgaven skal have et navn");
            else if (user != null) {
                TaskMapper.update(taskSelectedId, taskName, connPool);
            }
            ctx.redirect("home");
        }
        catch (DatabaseException e)
        {
            ctx.attribute("message", "kunne ikke tilføje opgave");
        }
    }

    private static void taskEdit(Context ctx, ConnectionPool connPool) {
        String taskSelectedId = ctx.formParam("taskselected");
        ctx.attribute("taskselectedforedit", taskSelectedId);
        ctx.render("edittask.html");
    }

    private static void taskDelete(Context ctx, ConnectionPool connPool) {
        int taskSelectedId = Integer.parseInt(ctx.formParam("taskselected"));
        User user = ctx.attribute("currentUser");

        try{
            TaskMapper.delete(taskSelectedId, connPool);
            ctx.redirect("home");
        }
        catch (DatabaseException e) {
            ctx.attribute("message", "kunne ikke slette opgaven");
        }
    }

    private static void taskUndone(Context ctx, ConnectionPool connPool) {
        int taskSelectedId = Integer.parseInt(ctx.formParam("taskselected"));
        User user = ctx.attribute("currentUser");

        try{
            TaskMapper.setDoneTo(false, taskSelectedId, connPool);
            ctx.redirect("home");
        }
        catch (DatabaseException e) {
            ctx.attribute("message", "kunne ikke slette opgaven");
        }
    }

    private static void taskDone(Context ctx, ConnectionPool connPool) {
        int taskSelectedId = Integer.parseInt(ctx.formParam("taskselected"));
        User user = ctx.attribute("currentUser");

        try{
            TaskMapper.setDoneTo(true, taskSelectedId, connPool);
            ctx.redirect("home");
        }
        catch (DatabaseException e) {
            ctx.attribute("message", "kunne ikke slette opgaven");
        }
    }

    private static void addTask(@NotNull Context ctx, ConnectionPool connPool) {
        String taskName = ctx.formParam("taskname");
        User user = ctx.sessionAttribute("currentUser");

        if (taskName.length() < 1)
            ctx.attribute("message", "kunne ikke tilføje opgaven " + taskName);
        else if (!(taskName == null) && !(user == null)) {
            try {
                TaskMapper.addTask(user, taskName, connPool);
            } catch (DatabaseException e) {
                ctx.attribute("message", "kunne ikke tilføje opgaven " + taskName);
            }
        }
        List<Task> taskList = null;
        List<Task> taskListDone = new ArrayList<>();
        List<Task> taskListUndone = new ArrayList<>();

        try {
            taskList = TaskMapper.getAllTasksPerUser(user.id(), connPool);
        } catch (DatabaseException e) {
            ctx.attribute("message", "databasefejl (task)");
        }
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
    }

}
