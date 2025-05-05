import java.util.Scanner;

/**
 * Main class for the project.
 */
public class Main {
	private static final Scanner scanner = new Scanner(System.in);
	private static final FileSystem fs = new FileSystem();
	private static Directory currentDirectory;

	public static void main(String[] args) {
		currentDirectory = fs.getRoot();

		while (true) {
			System.out.println("==== File System Management Menu ====\n");
			System.out.println("1. Change directory");
			System.out.println("2. List directory");
			System.out.println("3. Create file/directory");
			System.out.println("4. Delete file/directory");
			System.out.println("5. Move file/directory");
			System.out.println("6. Search file/directory");
			System.out.println("7. Print directory tree");
			System.out.println("8. Sort contents");
			System.out.println("9. Exit");
			System.out.print("\n-> Select an option: ");
			int choice = scanner.nextInt();
			scanner.nextLine();
			System.out.println("-------------------------------------------------");

			System.out.println("Current directory: " + getCurrentPath() + "\n");
			switch (choice) {
				case 1:
					changeDirectory();
					break;
				case 2:
					listContents();
					break;
				case 3:
					create();
					break;
				case 4:
					delete();
					break;
				case 5:
					move();
					break;
				case 6:
					search();
					break;
				case 7:
					printDirectoryTree();
					break;
				case 8:
					sort();
					break;
				case 9:
					System.out.println("Exiting...");
					scanner.close();
					System.exit(0);
					break;
				default:
					System.out.println("Invalid option! Please try again.");
			}
			System.out.print("Press enter to proceed...");
			scanner.nextLine();
			System.out.println("-------------------------------------------------");
		}
	}

	/**
	 * Helper function for changing directory.
	 */
	private static void changeDirectory() {
		System.out.print("Enter new directory path: ");
		String path = scanner.nextLine();
		String[] splitPath = path.split("/", -1);
		Directory targetDir = fs.changeDirectory(splitPath, currentDirectory);
		if (targetDir == null) {
			System.out.println("No such directory!");
			return;
		}
		currentDirectory = targetDir;
	}

	/**
	 * Helper function for listing contents of the current directory.
	 */
	private static void listContents() {
		fs.listContents(currentDirectory);
	}

	/**
	 * Helper function for creating a file or directory.
	 */
	private static void create() {
		System.out.print("Create a file or directory (f/d): ");
		String selection = scanner.nextLine();
		if (selection.equals("f")) {
			System.out.print("Enter a file name: ");
			String name = scanner.nextLine();
			if (fs.doesExist(name, "FILE", currentDirectory) != null) {
				System.out.println("There is already a file with this name!");
				System.out.println("Please try again.");
				return;
			}
			fs.createFile(name, currentDirectory);
			System.out.println("File successfully created: " + name);
		} else if (selection.equals("d")) {
			System.out.print("Enter a directory name: ");
			String name = scanner.nextLine();
			if (fs.doesExist(name, "DIR", currentDirectory) != null) {
				System.out.println("There is already a directory with this name!");
				System.out.println("Please try again.");
				return;
			}
			fs.createDirectory(name, currentDirectory);
			System.out.println("Directory successfully created: " + name + "/");
		} else {
			System.out.println("Invalid selection! Please try again.");
		}
	}

	/**
	 * Helper function for deleting a file or directory.
	 */
	private static void delete() {
		System.out.print("Delete a file or directory (f/d): ");
		String selection = scanner.nextLine();
		if (selection.equals("f")) {
			System.out.print("Enter the file name: ");
			String name = scanner.nextLine();
			FileSystemElement element = fs.doesExist(name, "FILE", currentDirectory);
			if (element == null) {
				System.out.println("No such file!");
				return;
			}
			fs.deleteFile(element);
		} else if (selection.equals("d")) {
			System.out.print("Enter the directory name: ");
			String name = scanner.nextLine();
			FileSystemElement element = fs.doesExist(name, "DIR", currentDirectory);
			if (element == null) {
				System.out.println("No such directory!");
				return;
			}
			fs.deleteDirectory(element);
		} else {
			System.out.println("Invalid selection! Please try again.");
		}
	}

	/**
	 * Helper function for moving an element to a new directory.
	 */
	private static void move() {
		System.out.print("Enter the name of the file/directory: ");
		String name = scanner.nextLine();
		FileSystemElement element = fs.doesExist(name, "", currentDirectory);
		if (element == null) {
			System.out.println("No such file or directory!");
			return;
		}
		System.out.print("Enter the full path of the directory: ");
		String path = scanner.nextLine();
		String[] directories = path.split("/", -1);
		fs.moveElement(element, directories, currentDirectory);
	}

	/**
	 * Helper function for searching a file or directory.
	 */
	private static void search() {
		System.out.print("Enter search query: ");
		String query = scanner.nextLine();
		System.out.println("Searching from root...");
		String absPth = fs.search(query);
		if (absPth.isEmpty()) {
			System.out.println("Not Found!");
		} else {
			System.out.println("Found: " + absPth);
		}
	}

	/**
	 * Helper function for printing the directory tree.
	 */
	private static void printDirectoryTree() {
		fs.printDirectoryTree(currentDirectory);
	}

	/**
	 * Helper function for sorting contents of a directory.
	 */
	private static void sort() {
		fs.sortDirectory(currentDirectory);
	}

	/**
	 * Helper function to print the current path.
	 * @return current path
	 */
	private static String getCurrentPath() {
		return currentDirectory.getCurrentPath();
	}
}
