/**
 * Laptop class represents a category of the inventory.
 * It extends from Device class.
 */
public class Laptop extends Device {
	public Laptop() { super(); }

	public Laptop(String category, String name, String price, int quantity) { super(category, name, price, quantity); }
}
