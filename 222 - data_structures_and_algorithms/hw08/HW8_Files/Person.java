import java.util.*;

/**
 * Represents a person in a social network.
 */
public class Person {
    /**
     * The name of the person.
     */
    String name;

    /**
     * The age of the person.
     */
    int age;

    /**
     * The list of hobbies of the person.
     */
    List<String> hobbies;

    /**
     * The timestamp when the person object was created.
     */
    Date timestamp;

    /**
     * Constructs a new person with the specified name, age, and hobbies.
     * @param name    the name of the person
     * @param age     the age of the person
     * @param hobbies the list of hobbies of the person
     */
    public Person(String name, int age, List<String> hobbies) {
        this.name = name;
        this.age = age;
        this.hobbies = new ArrayList<>(hobbies);
        this.timestamp = new Date();
    }

    /**
     * Returns a string representation of the person.
     * @return a string representation of the person
     */
    @Override
    public String toString() {
        return name + " (Age: " + age + ", Hobbies: " + hobbies + ")";
    }
}
