package app.entities;

import java.sql.Date;
import java.sql.Time;
import java.sql.Timestamp;

public record ChatMsg(String userName, Timestamp time, String content){
}
