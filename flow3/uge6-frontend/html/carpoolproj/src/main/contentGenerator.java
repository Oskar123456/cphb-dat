import java.io.IOException;
import java.net.URI;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

import entities.*;

public class contentGenerator {
  public static byte[] fetchContent(String file, String queryparams) {
    byte[] data = null;
    String resourcePath = System.getProperty("user.dir") + "/resources";
    if (file.endsWith(".html")) resourcePath += "/docs";
    if (file.equals("/")) {
      resourcePath += "/docs/index.html";
      file = "";
    }
    try {
      data = Files.readAllBytes(Paths.get(resourcePath + file));
    } catch (IOException e) {
      //System.out.println("could not find " + resourcePath + file);
    }
    /*
     * handle queries with params
     * */
    if (queryparams != null) data = modContent(file, queryparams, data);
    return data;
  }

  /*
   * hard-coded rules for different file names
   * */
  private static byte[] modContent(String file, String queryparams, byte[] data) {
    Map<String, String> qparamsMap = new HashMap<>();
    for (String p : queryparams.split("&")) {
      String[] pa = p.split("=");
      if (pa.length > 1) qparamsMap.put(pa[0], pa[1]);
      else qparamsMap.put(pa[0], "");
    }

    if (file.endsWith("result.html")) {
      List<student> studentList = dbConnector.selectStudents("SELECT * FROM public.students");
      boolean studentExists = false;
      for (student st : studentList) {
        if (st.name.equals(qparamsMap.get("fname"))) studentExists = true;
      }
      if (!studentExists) {
        dbConnector.insertStudent(new student(dbConnector.getUniqueStudentId(), qparamsMap.get("fname"), "1234"));
      }
      for (Map.Entry<String, String> s : qparamsMap.entrySet()) {
        //System.out.println("\t>>>" + s.getKey() + " = " + s.getValue());
      }
      if (qparamsMap.containsKey("clienttype")) {
        String clientType = qparamsMap.get("clienttype");
        if (clientType.equals("chauf")) {
          //System.out.println("attempting to insert chauf");
          /* find students ID */
          studentList = dbConnector.selectStudents("SELECT * FROM public.students");
          int studentID = 0;
          for (student st : studentList) {
            if (st.name.equals(qparamsMap.get("fname"))) studentID = st.id;
          }
          /* db insert */
          trip newTrip = new trip(dbConnector.getUniqueTripId(),
            qparamsMap.get("trip-start"),
            qparamsMap.get("departtime"),
            studentID, 4,
            qparamsMap.get("srcsel"),
            qparamsMap.get("destsel"));
          dbConnector.insertTrip(newTrip);

          String goodbye = """
            <div id="titlebox">
            <h1 id="maintitle">
            Rejse booket
            </h1>
            <p id="subtitle">
            God tur
            </p>
            </div>
            """;
          List<trip> newTripList = new ArrayList<>();
          newTripList.add(newTrip);
          data = injectHTML(data, new String(makeHTMLTable(newTripList)));
          data = injectHTML(data, goodbye);
        }
        if (clientType.equals("pass")) {
          studentList = dbConnector.selectStudents("SELECT * FROM public.students");
          student stud = null;
          for (student st : studentList) {
            if (st.name.equals(qparamsMap.get("fname"))) stud = st;
          }
          //TODO: sort trips
          String sql = "SELECT * FROM public.trips WHERE date = '"
            + qparamsMap.get("trip-start") + "'"
            + " AND src = '" + qparamsMap.get("srcsel") + "'"
            + " AND dest = '" + qparamsMap.get("destsel") + "'";
          //System.out.println(sql);
          List<trip> tripList = dbConnector.selectTrips(sql);
          if (!tripList.isEmpty())
            data = injectHTML(data, new String(makeHTMLRadioSelect(tripList, stud)));
          else
            data = injectHTML(data, "<div id=\"titlebox\"> <h1 id=\"maintitle\">Desværre, ingen afgange d. "
              + qparamsMap.get("trip-start") + ", beklager.");
        }
      }
    }
    if (file.endsWith("resultbooked.html")) {
      /* insert new booking */
      List<student> studentList = dbConnector.selectStudents("SELECT * FROM public.students");
      student stud = null;
      for (student st : studentList) {
        if (st.name.equals(qparamsMap.get("fname"))) stud = st;
      }
      dbConnector.insertBooking(new booking(dbConnector.getUniqueBookingId(),
        Integer.parseInt(qparamsMap.get("tripselName")),
        Integer.parseInt(qparamsMap.get("tripsel"))));
      /* display */
      String sql = "SELECT * FROM public.trips WHERE id = " + qparamsMap.get("tripsel");
      //System.out.println(sql);
      List<trip> bookedTrip = dbConnector.selectTrips(sql);
      byte[] htmlTable = makeHTMLTable(bookedTrip);
      data = injectHTML(data, new String(htmlTable));
    }
    return data;
  }

  private static byte[] injectHTML(byte[] html, String injection) {
    String injPoint = "<!--INJECTION_START-->";
    String htmlStr = new String(html);
    String[] htmlStrSplit = htmlStr.split(injPoint);
    String retStr = htmlStrSplit[0] + injPoint + injection + htmlStrSplit[1];
    return retStr.getBytes();
  }

  private static byte[] makeHTMLTable(List<trip> trips) {
    StringBuilder sb = new StringBuilder();
    sb.append("<table>");
    sb.append("<tr>");
    sb.append("<th>" + "fra" + "</th>" +
      "<th>" + "til" + "</th>" +
      "<th>" + "dato" + "</th>" +
      "<th>" + "tidspunkt" + "</th>" +
      "<th>" + "chauffeur" + "</th>");
    sb.append("</tr>");
    for (trip t : trips) {
      List<student> studentList = dbConnector.selectStudents("SELECT * FROM public.students");
      String name = "";
      for (student s : studentList) {
        if (s.id == t.chauffeur) name = s.name;
      }
      sb.append("<tr>");
      sb.append("<td>" + t.src + "</td>" +
        "<td>" + t.dest + "</td>" +
        "<td>" + t.date + "</td>" +
        "<td>" + t.time + "</td>" +
        "<td>" + name + "</td>");
      sb.append("</tr>");
    }
    sb.append("</table>");
    //System.out.println(sb.toString());
    return sb.toString().getBytes();
  }


  private static byte[] makeHTMLRadioSelect(List<trip> trips, student stud) {
    StringBuilder sb = new StringBuilder();
    String start = """
      <form action="resultbooked.html">

      <div id="tripsel">
      <div>
      <fieldset>
      <legend>Vælg afgang:</legend>
      """;
    String mid = """
      </fieldset>
      </div>
      </div>
      """;
    String end = """
      <div id="confirmtrip" class="confbutton">
      <a href="result.html">
      <input type="submit" value="Bekræft">
      </a>
      </div>
      </form>
      """;
    sb.append(start);
    String firstChecked = "checked";
    for (trip t : trips) {
      List<student> studentList = dbConnector.selectStudents("SELECT * FROM public.students");
      String name = "";
      for (student s : studentList) {
        if (s.id == t.chauffeur) name = s.name;
      }
      sb.append("<div> <input type=\"radio\" id=\"t\" name=\"tripsel\" value=\"" + t.id + "\"" + firstChecked + "/>");
      sb.append("<label for=\"t\">d. " + t.date + " kl. " + t.time + " fra " + t.src + " til " + t.dest + " med " + name + "</label>");
      sb.append("</div>");
      firstChecked = "";
    }
    sb.append(mid);

    if (stud != null) {
      sb.append("<div> <input type=\"radio\" id=\"s\" name=\"tripselName\" value=\""
        + stud.id + "\"" + "checked" + "/>");
      sb.append("<label for=\"s\">Passager: " + stud.name + "</label>");
      sb.append("</div>");
    }

    sb.append(end);

    //System.out.println(sb);
    return sb.toString().getBytes();

  }
}
