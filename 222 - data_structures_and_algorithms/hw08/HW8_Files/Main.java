import java.util.*;

/**
 * Main class for the Social Network Analysis program.
 * Allows users to interactively manage a social network by adding/removing people, adding/removing friendships,
 * finding the shortest paths between people, suggesting friends for a person, and counting clusters in the network.
 */
public class Main {
    /**
     * Prints the menu options for the social network analysis program.
     */
    private static void printMenu() {
        System.out.println("===== Social Network Analysis Menu =====");
        System.out.println("1. Add person");
        System.out.println("2. Remove person");
        System.out.println("3. Add friendship");
        System.out.println("4. Remove friendship");
        System.out.println("5. Find shortest path");
        System.out.println("6. Suggest friends");
        System.out.println("7. Count clusters");
        System.out.println("8. Exit");
        System.out.print("Please select an option: ");
    }

    /**
     * Entry point of the program.
     * @param args command-line arguments (not used)
     */
    public static void main(String[] args) {
        SocialNetworkGraph network = new SocialNetworkGraph();
        Scanner scanner = new Scanner(System.in);

        // Adding some people for demonstration
        network.addPerson("zeynep", 25, Arrays.asList("reading", "hiking", "cooking"));
        network.addPerson("duru", 22, Arrays.asList("swimming", "cooking"));
        network.addPerson("talha", 27, Arrays.asList("hiking", "painting"));
        network.addPerson("ahmet", 30, Arrays.asList("reading", "swimming"));
        network.addPerson("arda", 28, Arrays.asList("running", "swimming"));
        network.addPerson("Frank Wilson", 26, Arrays.asList("reading", "hiking"));

        // Adding friendships for demonstration
        network.addFriendship("zeynep", "duru");
        network.addFriendship("zeynep", "talha");
        network.addFriendship("duru", "arda");
        network.addFriendship("talha", "ahmet");
        network.addFriendship("ahmet", "arda");

        // Finding the shortest path for demonstration
        network.findShortestPath("John Doe", "Bob Brown");

        // Counting clusters for demonstration
        network.countClusters();

        while (true) {
            printMenu();
            String input = scanner.nextLine();
            if (input.equals("1")) {
                System.out.print("Enter name & surname: ");
                String name = scanner.nextLine();
                System.out.print("Enter age: ");
                int age = Integer.parseInt(scanner.nextLine());

                System.out.println("Enter two hobbies");
                System.out.println("-----------------------");
                String[] arr = new String[2];
                for (int i = 0; i < 2; i++) {
                    System.out.printf("%d : ", i + 1);
                    arr[i] = scanner.nextLine();
                }
                network.addPerson(name, age, Arrays.asList(arr));
            } else if (input.equals("2")) {
                System.out.print("Person name to be removed: ");
                String name = scanner.nextLine();
                network.removePerson(name);
            } else if (input.equals("3")) {
                System.out.print("First person name: ");
                String name1 = scanner.nextLine();
                System.out.print("Second person name: ");
                String name2 = scanner.nextLine();
                network.addFriendship(name1, name2);
            } else if (input.equals("4")) {
                System.out.print("First person name: ");
                String name1 = scanner.nextLine();
                System.out.print("Second person name: ");
                String name2 = scanner.nextLine();
                network.removeFriendship(name1, name2);
            } else if (input.equals("5")) {
                System.out.print("Start name: ");
                String name1 = scanner.nextLine();
                System.out.print("End name: ");
                String name2 = scanner.nextLine();
                network.findShortestPath(name1, name2);
            } else if (input.equals("6")) {
                System.out.print("Name: ");
                String name = scanner.nextLine();
                System.out.print("Maximum # of friend to suggest: ");
                int suggestCount = Integer.parseInt(scanner.nextLine());
                network.suggestFriends(name, suggestCount);
            } else if (input.equals("7")) {
                network.countClusters();
            } else if (input.equals("8")) {
                System.out.println("Exitting...");
                break;
            } else {
                System.out.println("Unvalid option. Try again!");
            }
        }
            scanner.close();
    }
}
