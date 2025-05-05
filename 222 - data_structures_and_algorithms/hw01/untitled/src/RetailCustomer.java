public class RetailCustomer extends Customer {
    private final String customerType = "retail";

    public RetailCustomer() { super(); }

    public RetailCustomer(String name, String surname, String address, String phone, int id, int operatorId) {
        super(name, surname, address, phone, id, operatorId);
    }

    public void printCustomer(boolean flag) {
        super.printCustomer(flag);
        super.printOrders();
    }

    public String getCustomerType() { return customerType; }
}
