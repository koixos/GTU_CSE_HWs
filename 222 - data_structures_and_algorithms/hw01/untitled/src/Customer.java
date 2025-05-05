public abstract class Customer extends Person {
    private int                 operatorId;
    private int                 orderCounter;
    private String              customerType;
    private Order[]             orders;
    private static final int    SIZE = 100;

    public Customer() {
        super();
        this.operatorId = -1;
        this.orderCounter = -1;
        this.customerType = "";
        this.orders = null;
    }

    public Customer(String name, String surname, String address, String phone, int id, int operatorId) {
        super(name, surname, address, phone, id);
        this.operatorId = operatorId;
        this.orderCounter = 0;
        this.orders = new Order[SIZE];
    }

    public void printCustomer(boolean flag) {
        if (flag) {
            System.out.println("*** Customer Screen ***");
            System.out.println("------------------------------------------------------------");
        }
        System.out.printf("Name & Surname: %s %s\n", getName(), getSurname());
        System.out.printf("Address: %s\n", getAddress());
        System.out.printf("Phone: %s\n", getPhone());
        System.out.printf("ID: %d\n", getId());
        System.out.printf("Operator ID: %d\n", this.operatorId);
    }

    public void printOrders() {
        if (this.orders[0] == null) {
            System.out.println("This customer doesn't have any orders.");
        } else {
            for (int i = 0; i < this.orderCounter; i++) {
                System.out.printf("Order #%d => ", i+1);
                this.orders[i].printOrder();
            }
        }
        System.out.println("------------------------------------------------------------");
    }

    public void defineOrders(Order[] orders) {
        for (Order order : orders) {
            if (order != null && order.getCustomerId() == this.getId() && order.getCount() > 0) {
                this.orders[this.orderCounter] = order;
                this.orderCounter++;
            }
        }
    }

    //getters & setters
    public int getOperatorId() { return operatorId; }

    public String getCustomerType() { return customerType; }
}
