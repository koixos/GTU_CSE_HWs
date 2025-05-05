/**
 * Device is an abstract class for generalization of
 * the classes which implement it.
 * It implements the IDevice interface.
 */
public abstract class Device implements IDevice {
    private String  category;
    private String  name;
    private String  price;
    private int     quantity;

    /**
     * Default constructor for Device
     */
    public Device() { /* intentionally empty */ }

    /**
     * Initializer constructor for Device
     * @param category category name of the device
     * @param name name of the device
     * @param price price of the device
     * @param quantity quantity of the device
     */
    public Device(String category, String name, String price, int quantity) {
        this.category = category;
        this.name = name;
        this.price = price;
        this.quantity = quantity;
    }

    /**
     * Getter for category.
     * This method has a time complexity of O(1).
     * @return the category of the device
     */
    public String getCategory() { return category; }

    /**
     * Setter for category.
     * This method has a time complexity of O(1).
     * @param category the category to set
     */
    public void setCategory(String category) { this.category = category; }

    /**
     * Getter for name.
     * This method has a time complexity of O(1).
     * @return the name of the device
     */
    public String getName() { return name; }

    /**
     * Setter for name.
     * This method has a time complexity of O(1).
     * @param name the name to set
     */
    public void setName(String name) { this.name = name; }

    /**
     * Getter for price.
     * This method has a time complexity of O(1).
     * @return the price of the device
     */
    public String getPrice() { return price; }

    /**
     * Getter for parsed-to-double price.
     * This method has a time complexity of O(1).
     * @return the parsed price of the device
     */
    public Double getPriceDouble() { return Double.parseDouble(price); }

    /**
     * Setter for price.
     * This method has a time complexity of O(1).
     * @param price the price to set
     */
    public void setPrice(String price) { this.price = price; }

    /**
     * Getter for quantity.
     * This method has a time complexity of O(1).
     * @return the quantity of the device
     */
    public int getQuantity() { return quantity; }

    /**
     * Setter for quantity.
     * This method has a time complexity of O(1).
     * @param quantity the quantity to set
     */
    public void setQuantity(int quantity) { this.quantity = quantity; }
}
