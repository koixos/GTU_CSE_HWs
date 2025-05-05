import java.util.LinkedList;

/**
 * A class to represent a directory object.
 * Extends from FileSystemElement class.
 */
public class Directory extends FileSystemElement {
	private final LinkedList<FileSystemElement> children;

	/**
	 * Default constructor for a directory object.
	 * @param name name of the object
	 * @param parent parent directory of the object
	 */
	public Directory(String name, FileSystemElement parent) {
		super(name, "DIR", parent);
		children = new LinkedList<>();
	}

	/**
	 * Adds an element to the directory.
	 * @param element FileSystemElement object to be added
	 */
	public void addElement(FileSystemElement element) {
		children.add(element);
	}

	/**
	 * Removes an element from the directory.
	 * @param element FileSystemElement object to be removed
	 */
	public void removeElement(FileSystemElement element) {
		children.remove(element);
		element.setParent(null);
	}

	/**
	 * Prints the name of the directory.
	 * @param prefix indicator for object types
	 */
	@Override
	public void print(String prefix) {
		System.out.println(prefix + " " + getName() + "/");
	}

	public void printDirectory(int tabCount) {
		for (FileSystemElement child : this.getChildren()) {
			for (int i = 0; i < tabCount; i++) {
				System.out.print("\t");
			}
			if (child.getType().equals("DIR")) {
				((Directory) child).print("*");
			} else {
				((File) child).print("");
			}
		}
	}

	/**
	 * Searches for a given file or a directory in the root.
	 * Uses recursion.
	 * @param name name of the FileSystemElement object to be searched
	 * @return absolute path if found, empty string otherwise
	 */
	public String search(String name) {
		if (getName().equals(name)) {
			return getName();
		}

		for (FileSystemElement child : children) {
			if (child.getType().equals("DIR")) {
				String result = ((Directory)child).search(name);
				if (!result.isEmpty()) {
					return getName() + "/" + result;
				}
			} else {
				if (child.getName().equals(name)) {
					return getName() + "/" + child.getName();
				}
			}
		}
		return "";
	}

	/**
	 * Gets the path from root to the current directory recursively.
	 * @return processed path as a string
	 */
	public String getCurrentPath() {
		if (getParent() == null) {
			return getName();
		}
		String nextParents = ((Directory) getParent()).getCurrentPath();
		return nextParents + "/" + getName();
	}

	/**
	 * Getter for the contents of the object.
	 * @return LinkedList
	 */
	public LinkedList<FileSystemElement> getChildren() {
		return children;
	}
}
