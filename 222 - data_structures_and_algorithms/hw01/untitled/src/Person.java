public abstract class Person {
    private String  name;
    private String  surname;
    private String  address;
    private String  phone;
    private int     id;

    public Person() {
        this.name = "";
        this.surname = "";
        this.address = "";
        this.phone = "";
        this.id = -1;
    }

    public Person(String name, String surname, String address, String phone, int id) {
        this.name = name;
        this.surname = surname;
        this.address = address;
        this.phone = phone;
        this.id = id;
    }

    public String getName() { return name; }

    public String getSurname() { return surname; }

    public String getAddress() { return address; }

    public String getPhone() { return phone; }

    public int getId() { return id; }
}
