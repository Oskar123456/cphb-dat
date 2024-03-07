package entities;

public class trip {
  public int id;
  public String date;
  public String time;
  public int chauffeur;
  public int cap;
  public String src;
  public String dest;

  public trip(int id, String date, String time, int chauffeur, int cap, String src, String dest) {
    this.id = id;
    this.date = date;
    this.time = time;
    this.chauffeur = chauffeur;
    this.cap = cap;
    this.src = src;
    this.dest = dest;
  }
}
