/**
 * File class for representing a file object.
 * Extends from FileSystemElement class.
 */
public class File extends FileSystemElement {
	/**
	 * Default constructor for a file object.
	 * @param name name of the file
	 * @param parent parent directory of the file
	 */
	public File(String name, FileSystemElement parent) {
		super(name, "FILE", parent);
	}

	/**
	 * Prints the name of the file object.
	 * @param prefix indicator for object types, empty for file objects
	 */
	@Override
	public void print(String prefix) {
		System.out.println(prefix + getName());
	}
}
