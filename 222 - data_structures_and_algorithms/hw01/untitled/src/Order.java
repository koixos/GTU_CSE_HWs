public class Order {
    private String  productName;
    private int     count;
    private int     totalPrice;
    private int     status;
    private int     customerId;

    public Order() {
        this.productName = "";
        this.count = 0;
        this.totalPrice = 0;
        this.status = -1;
        this.customerId = -1;
    }

    public Order(String productName, int count, int totalPrice, int status, int customerId) {
        this.productName = productName;
        this.count = count;
        this.totalPrice = totalPrice;
        this.status = status;
        this.customerId = customerId;
    }

    public void printOrder() {
        OrderStatus orderStatus = new OrderStatus(status);
        System.out.printf("Product name: %s - Count: %d - Total price: %d - Status: %s - Customer Id: %d\n",
                getProductName(), getCount(), getTotalPrice(), orderStatus.getStatus(), getCustomerId());
    }

    public String getProductName() { return productName; }

    public int getCount() { return count; }

    public int getTotalPrice() { return totalPrice; }

    public int getCustomerId() { return customerId; }
}
