/**
 * Printer class represents a category of the inventory.
 * It extends from Device class.
 */
public class Printer extends Device {
	public Printer() { super(); }

	public Printer(String category, String name, String price, int quantity) { super(category, name, price, quantity); }
}
