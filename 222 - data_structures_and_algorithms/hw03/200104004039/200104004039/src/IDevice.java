/**
 * IDevice interface defines the contract for device objects.
 */
public interface IDevice {
    String getCategory();

    void setCategory(String category);

    String getName();

    void setName(String name);

    String getPrice();

    Double getPriceDouble();

    void setPrice(String price);

    int getQuantity();

    void setQuantity(int quantity);
}
