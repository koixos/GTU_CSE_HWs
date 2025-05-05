import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.Collections;

public class FileSystem {
	private final Directory root;

	/**
	 * Default constructor for the FileSystem class.
	 * Initializes the file system with a root directory.
	 */
	public FileSystem() {
		this.root = new Directory("root", null);
	}

	/**
	 * Creates a new file in the specified directory.
	 * @param name name of the file
	 * @param parent path for the created file
	 */
	public void createFile(String name, Directory parent) {
		if (parent == null) {
			System.out.println("Parent directory does not exist!");
			return;
		}
		parent.addElement(new File(name, parent));
	}

	/**
	 * Creates a new directory inside the specified parent directory.
	 * @param name name of the file
	 * @param parent path for the created directory
	 */
	public void createDirectory(String name, Directory parent) {
		if (parent == null) {
			System.out.println("Parent directory does not exist!");
			return;
		}
		parent.addElement(new Directory(name, parent));
	}

	/**
	 * Deletes a file from the specified directory.
	 * @param element object to be removed
	 */
	public void deleteFile(FileSystemElement element) {
		((Directory) element.getParent()).removeElement(element);
	}

	/**
	 * Deletes a directory, and all its content.
	 * @param element object to be removed
	 */
	public void deleteDirectory(FileSystemElement element) {
		cleanDirectory((Directory) element);
	}

	/**
	 * Moves a file or directory to a new directory.
	 * @param element object to be moved
	 * @param newParent path for the new directory
	 */
	public void moveElement(FileSystemElement element, String[] newParent, Directory parent) {
		Directory directory = changeDirectory(newParent, parent);
		if (directory == null) {
			System.out.println("There is no such directory!");
			return;
		}
		directory.addElement(element);
		element.setParent(directory);
		parent.removeElement(element);
	}

	/**
	 * Searches for files or directories by name.
	 * @param name name of the file system element
	 * @return absolute path if found, empty string otherwise
	 */
	public String search(String name) {
		return root.search(name);
	}

	/**
	 * Prints the entire directory tree starting from the root.
	 */
	public void printDirectoryTree(Directory directory) {
		String absPath = directory.getCurrentPath();
		String[] directories = absPath.split("/", -1);
		root.print("*");
		int tabCount = 1;
		Directory dir = root;
		for (int i = 1; i < directories.length; i++) {
			for (FileSystemElement element : dir.getChildren()) {
				if (element.getType().equals("DIR") && element.getName().equals(directories[i])) {
					for (int j = 0; j < i; j++) {
						System.out.print("\t");
					}
					System.out.println("* " + element.getName() + "/ (Current Directory)");
					++tabCount;
					dir = (Directory) element;
				}
			}
		}
		directory.printDirectory(tabCount);
	}

	/**
	 * Lists all contents of the specified directory.
	 * @param directory directory object to be listed
	 */
	public void listContents(Directory directory) {
		System.out.println("Listing contents:");
		System.out.println("-------------------------------------------------");
		if (directory.getChildren().isEmpty()) {
			System.out.println("Directory is empty...");
			return;
		}
		for (FileSystemElement element : directory.getChildren()) {
			if (element.getType().equals("DIR")) {
				element.print("*");
			}
		}
		for (FileSystemElement element : directory.getChildren()) {
			if (element.getType().equals("FILE")) {
				element.print("");
			}
		}
	}

	/**
	 * Sorts the contents of a directory by creation date.
	 * @param directory path of the directory to be sorted
	 */
	public void sortDirectory(Directory directory) {
		for (FileSystemElement element : directory.getChildren()) {
			if (element.getType().equals("DIR")) {
				System.out.println("* " + element.getName() + "/ (" + element.getDateCreated() + ")");
			} else {
				System.out.println(element.getName() + " (" + element.getDateCreated() + ")");
			}
		}
	}

	/**
	 * Changes the current directory to the one specified by path.
	 * Uses recursion to traverse the contents of the directories.
	 * @param subdirectories an array that includes the given path's subdirectories
	 * @param directory current directory element
	 * @return target directory element
	 */
	public Directory changeDirectory(String[] subdirectories, Directory directory) {
		if (subdirectories[0].equals("root")) {
			Directory dir = root;
			for (int i = 1; i < subdirectories.length; i++) {
				dir = (Directory) doesExist(subdirectories[i], "DIR", dir);
				if (dir == null) {
					return null;
				}
			}
			return dir;
		} else {
			for (String subdirectory : subdirectories) {
				if (subdirectory.equals("..")) {
					if (directory.getParent() == null) {
						directory = root;
					} else {
						directory = (Directory) directory.getParent();
					}
				} else {
					int flag = 0;
					for (FileSystemElement element : directory.getChildren()) {
						if (element.getType().equals("DIR") && element.getName().equals(subdirectory)) {
							directory = (Directory) element;
							flag = 1;
							break;
						}
					}
					if (flag == 0) {
						return null;
					}
				}
			}
		}
		return directory;
	}

	/**
	 * Searches for the object with given name and type in a given directory.
	 * @param name name of the object
	 * @param type type of the object
	 * @param directory target directory
	 * @return object if found, null otherwise
	 */
	public FileSystemElement doesExist(String name, String type, Directory directory) {
		if (type.isEmpty()) {
			for (FileSystemElement element : directory.getChildren()) {
				if (element.getName().equals(name)) {
					return element;
				}
			}
		} else {
			for (FileSystemElement element : directory.getChildren()) {
				if (element.getType().equals(type) && element.getName().equals(name)) {
					return element;
				}
			}
		}
		return null;
	}

	/**
	 * Getter to return the root directory
	 * @return root object
	 */
	public Directory getRoot() {
		return root;
	}

	/**
	 * Cleans the given directory by removing all files.
	 * Uses recursion to traverse the subdirectories.
	 * @param dir directory element to be cleaned
	 */
	private void cleanDirectory(Directory dir) {
		for (FileSystemElement element : dir.getChildren()) {
			if (element.getType().equals("FILE")) {
				dir.removeElement(element);
			} else {
				if (!((Directory) element).getChildren().isEmpty()) {
					cleanDirectory((Directory) element);
				} else {
					Directory parent = (Directory) element.getParent();
					if (parent != null) {
						parent.removeElement(element);
					}
				}
			}
		}

		Directory parent = (Directory) dir.getParent();
		if (parent != null) {
			parent.removeElement(dir);
		}
	}
}
