public class CorporateCustomer extends Customer {
    private String          companyName;
    private final String    customerType = "corporate";

    public CorporateCustomer() {
        super();
        this.companyName = "";
    }

    public CorporateCustomer(String name, String surname, String address, String phone, int id, int operatorId, String companyName) {
        super(name, surname, address, phone, id, operatorId);
        this.companyName = companyName;
    }

    public void printCustomer(boolean flag) {
        super.printCustomer(flag);
        System.out.println("Company name: " + this.companyName);
        super.printOrders();
    }

    public String getCustomerType() { return customerType; }
}
