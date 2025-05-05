/**
 * Smartwatch class represents a category of the inventory.
 * It extends from Device class.
 */
public class SmartWatch extends Device {
	public SmartWatch() { super(); }

	public SmartWatch(String category, String name, String price, int quantity) { super(category, name, price, quantity); }
}
