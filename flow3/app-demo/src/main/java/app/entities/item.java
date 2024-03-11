package app.entities;

import java.math.BigDecimal;

public class item {
    public int id;
    public int uid;
    public int category;
    public BigDecimal price;
    public String title;
    public String desc;


    public item(int id, int uid, int category, BigDecimal price, String title, String desc) {
        this.id = id;
        this.uid = uid;
        this.category = category;
        this.price = price;
        this.title = title;
        this.desc = desc;
    }

    @Override
    public String toString() {
        return "item{" +
                "id=" + id +
                ", uid=" + uid +
                ", category=" + category +
                ", price=" + price +
                ", title='" + title + '\'' +
                ", desc='" + desc + '\'' +
                '}';
    }
}
