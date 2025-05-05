public class Operator extends Person {
    private int                 wage;
    private int                 customerCounter;
    private Customer[]          customers;
    private static final int    SIZE = 100;

    public Operator() {
        this.wage = 0;
        this.customerCounter = -1;
        this.customers = null;
    }

    public Operator(String name, String surname, String address, String phone, int id, int wage) {
        super(name, surname, address, phone, id);
        this.wage = wage;
        this.customerCounter = 0;
        this.customers = new Customer[SIZE];
    }

    public void printOperator() {
        System.out.println("*** Operator Screen ***");
        System.out.println("------------------------------------------------------------");
        System.out.printf("Name & Surname: %s %s\n", getName(), getSurname());
        System.out.printf("Address: %s\n", getAddress());
        System.out.printf("Phone: %s\n", getPhone());
        System.out.printf("ID: %d\n", getId());
        System.out.printf("Wage: %d\n", this.wage);
        System.out.println("------------------------------------------------------------");
        printCustomers();
    }

    public void printCustomers() {
        if (this.customers[0] == null) {
            System.out.println("This operator does not have any customer.");
            System.out.println("------------------------------------------------------------");
        } else {
            for (int i = 0; i < this.customerCounter; i++) {
                System.out.printf("Customer #%d (a %s customer):\n", i+1, this.customers[i].getCustomerType());
                this.customers[i].printCustomer(false);
            }
        }
    }

    public void defineCustomers(Customer[] customers) {
        for (Customer customer : customers) {
            if (customer != null && customer.getOperatorId() == this.getId()) {
                this.customers[this.customerCounter] = customer;
                this.customerCounter++;
            }
        }
    }
}
