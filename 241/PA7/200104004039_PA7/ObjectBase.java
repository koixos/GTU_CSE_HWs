public class ObjectBase {
    public ObjectBase() { }

    public ObjectBase(String name, String info, String type) {
        this.name = name;
        this.info = info;
        this.type = type;
    }

    public void info() {
        System.out.println("Type: " + this.type);
        System.out.println("Name: " + this.name);
        System.out.println("Info: " + this.info);
    }

    public String getType() { return type; };

    public void setType(String type) { this.type = type; }

    public String getName() { return name; }

    public void setName(String name) { this.name = name; }

    public String getInfo() { return info; }

    public void setInfo(String info) { this.info = info; }

    String name = "";
    String info = "";
    String type = "";
}
