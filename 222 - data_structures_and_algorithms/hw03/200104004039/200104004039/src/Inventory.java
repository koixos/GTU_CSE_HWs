import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.util.*;

/**
 * Inventory class represents an inventory management system for an electronics shop.
 * It allows adding, removing, updating devices, displaying inventory, finding the cheapest product,
 * sorting the inventory, calculating inventory value, restocking devices, and exporting inventory report.
 */
public class Inventory {
    private final static Scanner scanner = new Scanner(System.in);
    private final static String reportFile = "report.txt";

    private ArrayList<TV> TVList;
    private ArrayList<SmartPhone> smartPhoneList;
    private ArrayList<SmartWatch> smartWatchList;
    private ArrayList<Printer> printerList;
    private ArrayList<Laptop> laptopList;
    private LinkedList<ArrayList<? extends Device>> inventoryList;

    /** Default constructor for Inventory */
    public Inventory() {
        TVList = new ArrayList<>();
        smartPhoneList = new ArrayList<>();
        smartWatchList = new ArrayList<>();
        printerList = new ArrayList<>();
        laptopList = new ArrayList<>();
        inventoryList = new LinkedList<>();

        inventoryList.add(TVList);
        inventoryList.add(smartPhoneList);
        inventoryList.add(smartWatchList);
        inventoryList.add(printerList);
        inventoryList.add(laptopList);
    }

    /**
     * Adds a new device to a specific category of the inventory.
     * This method has a time complexity of O(n^2) due to the toInt() method.
     */
    public void addDevice() {
        String[] data = new String[3];

        displayCategories();
        int selectedCategory = toInt(scanner.nextLine());
        if (selectedCategory <= 0 || selectedCategory > 5) {
            System.out.println("\n$ Cancelling...");
            return;
        }

        System.out.println("-------------------------------------------------------");
        System.out.print("-> Enter device name: ");
        data[0] = scanner.nextLine();
        if (doesExist(data[0]) == 1) {
            System.out.println("\n$ There is already a product with this name!\n$ Cancelling...");
            return;
        }

        System.out.print("-> Enter price: ");
        data[1] = scanner.nextLine();
        System.out.print("-> Enter quantity: ");
        data[2] = scanner.nextLine();

        if (selectedCategory == 1) {
            TV tv = new TV("TV", data[0], data[1], toInt(data[2]));
            TVList.add(tv);
            System.out.print("\n$ TV, ");
        } else if (selectedCategory == 2) {
            Laptop laptop = new Laptop("Laptop", data[0], data[1], toInt(data[2]));
            laptopList.add(laptop);
            System.out.print("\n$ Laptop, ");
        } else if (selectedCategory == 3) {
            SmartPhone smartPhone = new SmartPhone("Smart Phone", data[0], data[1], toInt(data[2]));
            smartPhoneList.add(smartPhone);
            System.out.print("\n$ Smart Phone, ");
        } else if (selectedCategory == 4) {
            SmartWatch smartWatch = new SmartWatch("Smart Watch", data[0], data[1], toInt(data[2]));
            smartWatchList.add(smartWatch);
            System.out.print("\n$ Smart Watch, ");
        } else {
            Printer printer = new Printer("Printer", data[0], data[1], toInt(data[2]));
            printerList.add(printer);
            System.out.print("\n$ Printer, ");
        }

        System.out.printf("%s, %s$, qty: %s is successfully added!\n", data[0], data[1], data[2]);
    }

    /**
     * Removes a device from a specific category of the inventory.
     * This method has a time complexity of O(n^2) due to the toInt() method.
     */
    public void removeDevice() {
        displayCategories();
        int selectedCategory = toInt(scanner.nextLine());
        if (selectedCategory <= 0 || selectedCategory > 5) {
            System.out.println("\n$ Cancelling...");
            return;
        }

        System.out.println("-------------------------------------------------------");
        System.out.print("-> Enter the name of the device to be removed: ");
        String name = scanner.nextLine();

        int isRemoved = 0;
        if (selectedCategory == 1) {
            for (TV tv : TVList) {
                if (tv.getName().equals(name)) {
                    TVList.remove(tv);
                    isRemoved = 1;
                    break;
                }
            }
        } else if (selectedCategory == 2) {
            for (Laptop laptop : laptopList) {
                if (laptop.getName().equals(name)) {
                    laptopList.remove(laptop);
                    isRemoved = 1;
                    break;
                }
            }
        } else if (selectedCategory == 3) {
            for (SmartPhone smartPhone : smartPhoneList) {
                if (smartPhone.getName().equals(name)) {
                    smartPhoneList.remove(smartPhone);
                    isRemoved = 1;
                    break;
                }
            }
        } else if (selectedCategory == 4) {
            for (SmartWatch smartWatch : smartWatchList) {
                if (smartWatch.getName().equals(name)) {
                    smartWatchList.remove(smartWatch);
                    isRemoved = 1;
                    break;
                }
            }
        } else {
            for (Printer printer : printerList) {
                if (printer.getName().equals(name)) {
                    printerList.remove(printer);
                    isRemoved = 1;
                    break;
                }
            }
        }

         if (isRemoved == 1)
             System.out.printf("\n$ %s is successfully removed!\n", name);
         else
             System.out.printf("\n$ There is no device named %s in the selected category!\n", name);
    }

    /**
     * Updates the details of a device in a specific category of the inventory.
     * This method has a time complexity of O(n^2) due to the toInt() method.
     */
    public void updateDevice() {
        Device device = null;

        displayCategories();
        String selection = scanner.nextLine();
        int selectedCategory = toInt(selection);
        if (selectedCategory <= 0 || selectedCategory > 5) {
            System.out.println("\n$ Cancelling...");
            return;
        }

        System.out.println("-------------------------------------------------------");
        System.out.print("-> Enter the name of the device to be updated: ");
        String name = scanner.nextLine();

        int isUpdated = 0;
        if (selectedCategory == 1) {
            for (TV tv : TVList) {
                if (tv.getName().equals(name)) {
                    device = tv;
                    isUpdated = 1;
                    break;
                }
            }
        } else if (selectedCategory == 2) {
            for (Laptop laptop : laptopList) {
                if (laptop.getName().equals(name)) {
                    device = laptop;
                    isUpdated = 1;
                    break;
                }
            }
        } else if (selectedCategory == 3) {
            for (SmartPhone smartPhone : smartPhoneList) {
                if (smartPhone.getName().equals(name)) {
                    device = smartPhone;
                    isUpdated = 1;
                    break;
                }
            }
        } else if (selectedCategory == 4) {
            for (SmartWatch smartWatch : smartWatchList) {
                if (smartWatch.getName().equals(name)) {
                    device = smartWatch;
                    isUpdated = 1;
                    break;
                }
            }
        } else {
            for (Printer printer : printerList) {
                if (printer.getName().equals(name)) {
                    device = printer;
                    isUpdated = 1;
                    break;
                }
            }
        }

        if (isUpdated == 0) {
            System.out.printf("\n$ There is no device named %s in the selected category!\n", name);
            return;
        }

        System.out.print("\n-> Enter new name: ");
        String newName = scanner.nextLine();
        if (doesExist(name) == 1) {
            System.out.println("\n$ There is already a product with this name!\n$ Cancelling...");
            return;
        }
        System.out.print("\n-> Enter new price: ");
        String newPrice = scanner.nextLine();
        if (Double.parseDouble(newPrice) <= 0) {
            System.out.println("\n$ Price cannot be less than or equal to zero!\n$ Cancelling...");
            return;
        }
        System.out.print("\n-> Enter new quantity: ");
        String newQty = scanner.nextLine();
        if (Double.parseDouble(newQty) < 0) {
            System.out.println("\n$ Quantity cannot be less than or equal to zero!\n$ Cancelling...");
            return;
        }

        device.setName(newName);
        device.setPrice(newPrice);
        device.setQuantity(toInt(newQty));
        System.out.printf("\n$ %s is successfully updated: %s$ - Qty: %d\n", newName, newPrice, toInt(newQty));
    }

    /**
     * Displays all devices in the inventory.
     * This method has a time complexity of O(n * m), where n is the total category number,
     * and m is the total number of devices in a category.
     */
    public void displayAll() {
        if (isEmpty() == 0) {
            System.out.println("$ The inventory is empty!");
            return;
        }
        System.out.println("$ Devices in inventory: ");
	    for (ArrayList<? extends Device> categoryList : inventoryList)
		    for (Device device : categoryList)
			    System.out.printf("\t* Category: %s\n\t  Name: %s\n\t  Price: %s\n\t  Quantity: %d\n",
					    device.getCategory(), device.getName(), device.getPrice(), device.getQuantity());
    }

    /**
     * Finds the cheapest product in the inventory.
     * This method has a time complexity of O(n * log n) due to the sort() method.
     */
    public void findCheapest() {
        if (isEmpty() == 0) {
            System.out.println("$ The inventory is empty!");
            return;
        }
        List<Device> sorted = sort();
        System.out.println("$ The cheapest product of the list:");
	    System.out.printf("\t* Category: %s\n\t  Name: %s\n\t  Price: %s\n\t  Quantity: %d\n",
                sorted.get(0).getCategory(), sorted.get(0).getName(), sorted.get(0).getPrice(), sorted.get(0).getQuantity());
    }

    /**
     * Sorts all devices in the inventory.
     * This method has a time complexity of O(n * log n) due to the sort() method.
     */
    public void sortAll() {
        if (isEmpty() == 0) {
            System.out.println("$ The inventory is empty!");
            return;
        }
        List<Device> wholeInv = sort();
        System.out.println("$ The inventory is successfully sorted!\n");
        for (Device device : wholeInv)
            System.out.printf("\t* Category: %s\n\t  Name: %s\n\t  Price: %s\n\t  Quantity: %d\n",
                device.getCategory(), device.getName(), device.getPrice(), device.getQuantity());
    }

    /**
     * Calculates the total value of the inventory.
     * This method has a time complexity of O(n * log(n)) due to the sort() method in the
     * calculateValue() method.
     */
    public void calculateInventoryValue() {
        if (isEmpty() == 0) {
            System.out.println("$ The inventory is empty!");
            return;
        }
        double totalValue = calculateValue();
        System.out.printf("$ Total inventory value: $%.2f.\n", totalValue);
    }

    /**
     * Restocks a device in the inventory by adding or removing stock.
     * This method has a time complexity of O(n^2) due to the toInt() method.
     */
    public void restockDevices() {
        Device device = null;

        displayCategories();
        int selectedCategory = toInt(scanner.nextLine());
        if (selectedCategory <= 0 || selectedCategory > 5) {
            System.out.println("\n$ Cancelling...");
            return;
        }

        System.out.println("-------------------------------------------------------");
        System.out.print("-> Enter the name of the device: ");
        String name = scanner.nextLine();

        int isRestocked = 0;
        if (selectedCategory == 1) {
            for (TV tv : TVList) {
                if (tv.getName().equals(name)) {
                    device = tv;
                    isRestocked = 1;
                    break;
                }
            }
        } else if (selectedCategory == 2) {
            for (Laptop laptop : laptopList) {
                if (laptop.getName().equals(name)) {
                    device = laptop;
                    isRestocked = 1;
                    break;
                }
            }
        } else if (selectedCategory == 3) {
            for (SmartPhone smartPhone : smartPhoneList) {
                if (smartPhone.getName().equals(name)) {
                    device = smartPhone;
                    isRestocked = 1;
                    break;
                }
            }
        } else if (selectedCategory == 4) {
            for (SmartWatch smartWatch : smartWatchList) {
                if (smartWatch.getName().equals(name)) {
                    device = smartWatch;
                    isRestocked = 1;
                    break;
                }
            }
        } else {
            for (Printer printer : printerList) {
                if (printer.getName().equals(name)) {
                    device = printer;
                    isRestocked = 1;
                    break;
                }
            }
        }

        if (isRestocked == 0) {
            System.out.printf("\n$ There is no device named %s in the selected category!\n", name);
            return;
        }

        System.out.print("-> (A)dd Stock / (R)emove Stock: ");
        String selection = scanner.nextLine();
        System.out.printf("-> Enter the quantity to add/remove (current qty: %d): ",
                device.getQuantity());
        int qty = toInt(scanner.nextLine());
        if (selection.equals("R")) {
            if (qty > device.getQuantity()) {
                System.out.println("\n$ Stock amount cannot be less than zero!\n$ Cancelling...");
                return;
            }
            qty *= -1;
        }

        device.setQuantity(device.getQuantity()+qty);
        System.out.printf("\n$ %s's stock amount is successfully updated!\n$ New stock: %d\n",
                name, device.getQuantity());
    }

    /**
     * Exports the inventory to a report file.
     * This method has a time complexity of O(n * m), where n is the total category number,
     * and m is the total number of devices in a category.
     */
    public void exportInventory() {
        try {
            Writer fw = new OutputStreamWriter(new FileOutputStream(reportFile, false), StandardCharsets.UTF_8);
            Date timeStamp = new Date();
            fw.write("Electronics Shop Inventory Report\n");
            fw.write("Generated on: " + timeStamp + "\n\n");
            fw.write("----------------------------------------------------------------------------------------\n");
            if (isEmpty() == 0) {
                fw.write("$ The inventory is empty!\n");
            } else {
                int counter = 0;
                fw.write("| No | Category | Name "
                        + " | Price | Quantity |\n");
                fw.write("----------------------------------------------------------------------------------------\n");
                for (ArrayList<? extends Device> categoryList : inventoryList)
                    for (Device device : categoryList)
                        fw.write("| " + ++counter + " | " + device.getCategory() + " | "
                                + device.getName() + " | $" + device.getPrice() + " | "
                                + device.getQuantity() + " |\n");

                double totalValue = calculateValue();
                fw.write("\nSummary:\n- Total number of devices: " + counter + "\n- Total inventory value: $" + totalValue + "\n");
            }
            fw.write("----------------------------------------------------------------------------------------\n");
            fw.close();
            System.out.printf("$ The inventory report is successfully created.\n$ Output File: %s\n", reportFile);
        } catch (Exception e) {
            System.out.println("$ There is an error occurred while file operations!\n$ The operation is cancelled.");
        }
    }

    /**
     * Displays all categories.
     * This method has a time complexity of O(1).
     */
    private static void displayCategories() {
        System.out.println("# Select a category to proceed:\n");
        System.out.print(
                    "\t1. TV\n" +
                    "\t2. Laptop\n" +
                    "\t3. Smart Phone\n" +
                    "\t4. Smart Watch\n" +
                    "\t5. Printer\n" +
                    "\t0. Exit\n\n" +
                    "\t-> Selection: ");
    }

    /**
     * Parses a given string into its integer value.
     * This method has a time complexity of O(n^2), where n is the length of the string.
     * @param str A given string.
     * @return integer value of the string
     */
    private static int toInt(String str) {
        int number = 0;
        for (int i = 0; i < str.length(); i++) {
            int pow = 1;
            for (int j = i; j < str.length() - 1; j++)
                pow *= 10;
            number += (str.charAt(i) - 48) * pow;
        }
        return number;
    }

    /**
     * Merges all devices in each category, and sorts them according to their prices in ascending order.
     * This method has a time complexity of O(n * log(n)), where n is the total number of categories in the inventory,
     * due to Java's sort() method.
     * @return a device list
     */
    private List<Device> sort() {
        List<Device> wholeInv = new ArrayList<>();
        for (ArrayList<? extends Device> categoryList : inventoryList)
            wholeInv.addAll(categoryList);
        wholeInv.sort(Comparator.comparing(Device::getPriceDouble));
        return wholeInv;
    }


    /**
     * Calculates the total value of the inventory.
     * This method has a time complexity of O(n * log(n)) due to sort() method.
     * @return total inventory value
     */
    private double calculateValue() {
        double totalValue = 0;
        List<Device> sorted = sort();
        for (Device device : sorted)
            totalValue += device.getPriceDouble();
        return totalValue;
    }

    /**
     * Checks if there is any device in the inventory which registered already with the given name.
     * @param name given name
     * @return 1 if there is a device with the given name, 0 otherwise.
     */
    private int doesExist(String name) {
        List<Device> wholeInv = new ArrayList<>();
        for (ArrayList<? extends Device> categoryList : inventoryList)
            wholeInv.addAll(categoryList);
        for (Device device : wholeInv)
            if (device.getName().equals(name))
                return 1;
        return 0;
    }

    /**
     * Checks if any inventory category is empty or not
     * Returns 1 if not empty, 0 otherwise
     * This method has a time complexity of O(n), where n is the total number of categories in the inventory.
     * @return a flag to decide if it is empty
     */
    private int isEmpty() {
        for (ArrayList<? extends Device> categoryList : inventoryList)
            if (!categoryList.isEmpty())
                return 1;
        return 0;
    }

    /**
     * Getter for TV category.
     * This method has a time complexity of O(1).
     * @return the TV category
     */
    public ArrayList<TV> getTVList() { return TVList; }

    /**
     * Setter for TV category.
     * This method has a time complexity of O(1).
     * @param TVList the TV category to set
     */
    public void setTVList(ArrayList<TV> TVList) { this.TVList = TVList; }

    /**
     * Getter for Smartphone category.
     * This method has a time complexity of O(1).
     * @return the Smartphone category
     */
    public ArrayList<SmartPhone> getSmartPhoneList() { return smartPhoneList; }

    /**
     * Setter for Smartphone category.
     * This method has a time complexity of O(1).
     * @param smartPhoneList the Smartphone category to set
     */
    public void setSmartPhoneList(ArrayList<SmartPhone> smartPhoneList) { this.smartPhoneList = smartPhoneList; }

    /**
     * Getter for Smartwatch category.
     * This method has a time complexity of O(1).
     * @return the Smartwatch category
     */
    public ArrayList<SmartWatch> getSmartWatchList() { return smartWatchList; }

    /**
     * Setter for Smartwatch category.
     * This method has a time complexity of O(1).
     * @param smartWatchList the Smartwatch category to set
     */
    public void setSmartWatchList(ArrayList<SmartWatch> smartWatchList) { this.smartWatchList = smartWatchList; }

    /**
     * Getter for Printer category.
     * This method has a time complexity of O(1).
     * @return the Printer category
     */
    public ArrayList<Printer> getPrinterList() { return printerList; }

    /**
     * Setter for Printer category.
     * This method has a time complexity of O(1).
     * @param printerList the Printer category to set
     */
    public void setPrinterList(ArrayList<Printer> printerList) { this.printerList = printerList; }

    /**
     * Getter for Laptop category.
     * This method has a time complexity of O(1).
     * @return the Laptop category
     */
    public ArrayList<Laptop> getLaptopList() { return laptopList; }

    /**
     * Setter for Laptop category.
     * This method has a time complexity of O(1).
     * @param laptopList the Laptop category to set
     */
    public void setLaptopList(ArrayList<Laptop> laptopList) { this.laptopList = laptopList; }

    /**
     * Getter for the inventory.
     * This method has a time complexity of O(1).
     * @return the inventory
     */
    public LinkedList<ArrayList<? extends Device>> getInventoryList() { return inventoryList; }

    /**
     * Setter for the inventory.
     * This method has a time complexity of O(1).
     * @param inventoryList the inventory to set
     */
    public void setInventoryList(LinkedList<ArrayList<? extends Device>> inventoryList) { this.inventoryList = inventoryList; }
}
