import java.io.File;
import java.sql.Timestamp;

public abstract class FileSystemElement {
	protected String name;
	protected String type;
	protected Timestamp dateCreated;
	protected FileSystemElement parent;

	public FileSystemElement(String name, String type, FileSystemElement parent) {
		this.name = name;
		this.type = type;
		this.parent = parent;
		this.dateCreated = new Timestamp(System.currentTimeMillis());
	}

	public abstract void print(String prefix);

	public String getName() {
		return name;
	}

	public String getType() {
		return type;
	}

	public Timestamp getDateCreated() {
		return dateCreated;
	}

	public FileSystemElement getParent() {
		return parent;
	}

	public void setParent(Directory parent) {
		this.parent = parent;
	}
}
