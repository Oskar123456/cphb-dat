package entities;

public class Member {

    private int memberId;
    private String name;
    private String address;
    private int zip;
    private String city;
    private String gender;
    private int year;

    public Member(String name, String address, int zip, String city, String gender, int year) {
        this.name = name;
        this.address = address;
        this.zip = zip;
        this.city = city;
        this.gender = gender;
        this.year = year;
    }

    public Member(int memberId, String name, String address, int zip, String city, String gender, int year) {
        this.memberId = memberId;
        this.name = name;
        this.address = address;
        this.zip = zip;
        this.city = city;
        this.gender = gender;
        this.year = year;
    }

    public int getMemberId() {
        return memberId;
    }

    public void setMemberId(int memberId) {
        this.memberId = memberId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public int getZip() {
        return zip;
    }

    public void setZip(int zip) {
        this.zip = zip;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    @Override
    public String toString() {
        return "Member{" +
                "memberId=" + memberId +
                ", name='" + name + '\'' +
                ", address='" + address + '\'' +
                ", zip=" + zip +
                ", city='" + city + '\'' +
                ", gender='" + gender + '\'' +
                ", year=" + year +
                '}';
    }
}
