public class OrderStatus {
    private static String status;

    public OrderStatus() {
        status = "Undefined";
    }

    public OrderStatus(int id) {
        switch (id) {
            case 0:
                status = "Initialized";
                break;
            case 1:
                status = "Processing";
                break;
            case 2:
                status = "Completed";
                break;
            case 3:
                status = "Cancelled";
                break;
            default:
                status = "Undefined";
                break;
        }
    }

    public String getStatus() { return status; }
}
