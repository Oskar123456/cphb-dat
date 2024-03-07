import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import entities.*;

public class dbConnector {
  private static database db;

  //lool
  private static int studentID = 0;
  private static int bookingID = 0;
  private static int tripID = 0;

  synchronized public static int getUniqueStudentId() {
    return studentID++;
  }

  synchronized public static int getUniqueBookingId() {
    return bookingID++;
  }

  synchronized public static int getUniqueTripId() {
    return tripID++;
  }

  /*
   * must be called prior to use of this class
   * */
  synchronized public static void init(String user, String pwd, String url) {
    if (db == null)
      db = new database(user, pwd, url);
    /*
    * find current values for unique IDs in database
    * */
    List<student> studentList = selectStudents("SELECT * FROM public.students");
    List<booking> bookingList = selectBookings("SELECT * FROM public.bookings");
    List<trip> tripList = selectTrips("SELECT * FROM public.trips");
    int max = 0;
    for (student st : studentList){
      max = Math.max(st.id, max);
    }
    studentID = max + 1;
    max = 0;
    for (booking bk : bookingList){
      max = Math.max(bk.id, max);
    }
    bookingID = max + 1;
    max = 0;
    for (trip tr : tripList){
      max = Math.max(tr.id, max);
    }
    tripID = max + 1;
  }

  synchronized public static List<student> selectStudents(String sql) {
    List<student> studentList = new ArrayList<>();
    try (Connection conn = db.connect();) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
          student st = new student(rs.getInt("id"),
            rs.getString("name"),
            rs.getString("password"));
          studentList.add(st);
        }
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
    return studentList;
  }

  synchronized public static void insertStudent(student st) {
    if (st.name == null)
      return;
    String sql = String.format("INSERT INTO public.students(" +
      "id, name, password) VALUES (%d,'%s',%s)",
      st.id, st.name, st.pwd);
    System.out.println(sql);
    try (Connection conn = db.connect()) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        ps.executeUpdate();
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
  }

  synchronized public static List<booking> selectBookings(String sql) {
    List<booking> bookingList = new ArrayList<>();
    try (Connection conn = db.connect();) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
          booking st = new booking(rs.getInt("id"),
            rs.getInt("student"),
            rs.getInt("trip"));
          bookingList.add(st);
        }
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
    return bookingList;
  }


  synchronized public static void insertBooking(booking bk) {
    String sql = String.format("INSERT INTO public.bookings(" +
      "id, student, trip) VALUES (%d,%d,%d)",
      getUniqueBookingId(), bk.student, bk.trip);
    //System.out.println(sql);
    try (Connection conn = db.connect()) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        ps.executeUpdate();
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
  }


  synchronized public static List<trip> selectTrips(String sql) {
    List<trip> tripList = new ArrayList<>();
    try (Connection conn = db.connect();) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
          trip st = new trip(rs.getInt("id"),
            rs.getString("date"),
            rs.getString("time"),
            rs.getInt("chauffeur"),
            rs.getInt("cap"),
            rs.getString("src"),
            rs.getString("dest"));
          tripList.add(st);
        }
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
    return tripList;
  }


  synchronized public static void insertTrip(trip tr) {
    String sql = String.format(
      "INSERT INTO public.trips(id, date, \"time\", chauffeur, cap, src, dest) VALUES (%d,'%s','%s',%d,%d,'%s','%s')",
      getUniqueTripId(), tr.date, tr.time, tr.chauffeur, tr.cap, tr.src, tr.dest);
    //System.out.println(sql);
    try (Connection conn = db.connect()) {
      try (PreparedStatement ps = conn.prepareStatement(sql)) {
        int retval = ps.executeUpdate();
        //System.out.println("insertrip val : " + retval);
      } catch (SQLException e) {
        e.printStackTrace();
        System.out.println("problem in sendQuery");
      }
    } catch (SQLException e) {
      e.printStackTrace();
      System.out.println("problem in sendQuery");
    }
  }

}


