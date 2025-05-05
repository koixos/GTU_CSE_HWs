import java.util.Scanner;

/**
 * CSE 222 - HOMEWORK03
 * This class represents the main entity to execute the whole program of the Inventory Management System.
 * It provides a menu-driven interface for interacting with the inventory.
 * The primary objective of this assignment is to apply the principles of OOP and data structures,
 * specifically ArrayLists and LinkedLists, to develop an inventory management system for an electronics shop.
 * The main features include adding, removing, updating devices, displaying inventory, finding the cheapest product,
 * sorting the inventory, calculating inventory value, restocking devices, and exporting inventory report.
 * Author: Sümeyye Zeynep Gürbüz, 200104004039
 */
public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Inventory inventory = new Inventory();

        System.out.println("-------------------------------------------------------");
        System.out.println("Welcome to the Electronics Inventory Management System!");
        System.out.println("-------------------------------------------------------");
        while (true) {
            System.out.println("# Please select an option:\n");
            System.out.print(
                    "\t1. Add a new device\n" + 
                    "\t2. Remove a device\n" +
                    "\t3. Update device details\n" +
                    "\t4. List all devices\n" +
                    "\t5. Find the cheapest device\n" +
                    "\t6. Sort devices by price\n" + 
                    "\t7. Calculate total inventory value\n" +
                    "\t8. Restock a device\n" +
                    "\t9. Export inventory report\n" +
                    "\t0. Exit\n\n" +
                    "\t-> Selection: ");
            String selection = scanner.nextLine();
            System.out.println("-------------------------------------------------------");
            int selectedOption = toInt(selection);
            if (selectedOption == 0) {
                System.out.println("$ Terminating the program...");
                return;
            }

            if (selectedOption == 1)
                inventory.addDevice();
            else if (selectedOption == 2)
                inventory.removeDevice();
            else if (selectedOption == 3)
                inventory.updateDevice();
            else if (selectedOption == 4)
                inventory.displayAll();
            else if (selectedOption == 5)
                inventory.findCheapest();
            else if (selectedOption == 6)
                inventory.sortAll();
            else if (selectedOption == 7)
                inventory.calculateInventoryValue();
            else if (selectedOption == 8)
                inventory.restockDevices();
            else if (selectedOption == 9)
                inventory.exportInventory();
            else
                System.out.println("$ Invalid selection! Please try again.");
            System.out.println("-------------------------------------------------------");
        }
    }

    /**
     * Parses a given string into its integer value.
     * This method has a time complexity of O(n^2), where n is the length of the string.
     * @param str A given string.
     * @return int
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
}