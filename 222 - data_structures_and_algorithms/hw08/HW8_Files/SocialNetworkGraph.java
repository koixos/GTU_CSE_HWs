import java.util.*;

/**
 * Represents a social network graph where people are connected through friendships.
 */
public class SocialNetworkGraph {
    /**
     * A map of people in the network, where the key is the person's name and the value is the Person object.
     */
    Map<String, Person> people = new HashMap<>();

    /**
     * A map of friendships between people in the network, where the key is a Person object and the value is a list of their friends.
     */
    Map<Person, List<Person>> friendships = new HashMap<>();

    /**
     * Adds a new person to the social network.
     * @param name    the name of the person
     * @param age     the age of the person
     * @param hobbies the list of hobbies of the person
     */
    public void addPerson(String name, int age, List<String> hobbies) {
        Person person = new Person(name, age, hobbies);
        people.put(name, person);
        friendships.put(person, new ArrayList<>());
        System.out.println("Person added: " + person + " Timestamp: " + person.timestamp);
    }

    /**
     * Removes a person from the social network.
     * @param name the name of the person to be removed
     */
    public void removePerson(String name) {
        Person person = people.remove(name);
        if (person != null) {
            friendships.remove(person);
            for (List<Person> f : friendships.values()) {
                f.remove(person);
            }
            System.out.println("Person removed: " + person);
        } else {
            System.out.println("Person not found!");
        }
    }

    /**
     * Adds a friendship between two people in the social network.
     * @param name1 the name of the first person
     * @param name2 the name of the second person
     */
    public void addFriendship(String name1, String name2) {
        Person person1 = people.get(name1);
        Person person2 = people.get(name2);
        if (person1 != null && person2 != null) {
            friendships.get(person1).add(person2);
            friendships.get(person2).add(person1);
            System.out.println("Friendship added between " + person1.name + " and " + person2.name);
        } else {
            System.out.println("One or both persons not found in the network.");
        }
    }

    /**
     * Removes a friendship between two people in the social network.
     * @param name1 the name of the first person
     * @param name2 the name of the second person
     */
    public void removeFriendship(String name1, String name2) {
        Person person1 = people.get(name1);
        Person person2 = people.get(name2);
        if (person1 != null && person2 != null) {
            friendships.get(person1).remove(person2);
            friendships.get(person2).remove(person1);
            System.out.println("Friendship removed between " + person1.name + " and " + person2.name);
        } else {
            System.out.println("One or both persons not found in the network.");
        }
    }

    /**
     * Finds the shortest path between two people using BFS.
     * @param startName the name of the starting person
     * @param endName   the name of the ending person
     */
    public void findShortestPath(String startName, String endName) {
        //implement logic here
        Person start = people.get(startName);
        Person end = people.get(endName);

        if (start == null || end == null) {
            System.out.println("One or both person not found in the network!");
            return;
        }

        Queue<Person> queue = new LinkedList<>();
        queue.add(start);

        Map<Person, Integer> distances = new HashMap<>();
        distances.put(start, 0);

        Map<Person, Person> predecessors = new HashMap<>();
        predecessors.put(start, null);

        while (!queue.isEmpty()) {
                Person curr = queue.poll();
                int currDistance = distances.get(curr);
                if (curr.equals(end)) {
                    printPath(start, end, predecessors);
                    return;
                }

                for (Person p : friendships.get(curr)) {
                    if (!distances.containsKey(p)) {
                        distances.put(p, currDistance + 1);
                        predecessors.put(p, curr);
                        queue.add(p);
                    }
                }
        }
        System.out.println("No path found between " + startName + " and " + endName);
    }

    /**
     * Generates suggestions for friends for a given person based on mutual friends and common hobbies.
     * @param name  the name of the person
     * @param count the number of suggestions to generate
     */
    public void suggestFriends(String name, int count) {
        Person person = people.get(name);

        if (person == null) {
            System.out.println("Person not found in the network!");
            return;
        }

        List<Suggestion> suggestions = new ArrayList<>();
        for (Person p : people.values()) {
            if (!p.equals(person) && !friendships.get(person).contains(p)) {
                int mutualFriends = countMutualFriends(p, person);
                int commonHobbies = countCommonHobbies(p, person);
                double score = mutualFriends * 2 + commonHobbies * 1.2;
                suggestions.add(new Suggestion(p, score, mutualFriends, commonHobbies));
            }
        }

        suggestions.sort(Comparator.comparing(Suggestion::getScore).reversed());

        System.out.println("Suggested friends for " + name + ":");
        for (int i = 0; i < count && i < suggestions.size(); i++) {
            Suggestion suggestion = suggestions.get(i);
            System.out.printf("%s (Score: %.1f, %d mutual friends, %d common hobbies)%n",
                    suggestion.person.name, suggestion.score, suggestion.mutualFriends, suggestion.commonHobbies);
        }
    }

    /**
     * Counts the clusters in the social network using BFS.
     */
    public void countClusters() {
        Set<Person> visited = new HashSet<>();
        int counter = 0;
        for (Person person : people.values()) {
            if (!visited.contains(person)) {
                List<Person> cluster = new ArrayList<>();
                bfs(person, visited, cluster);
                ++counter;
                System.out.println("Cluster " + counter + ":");
                for (Person p : cluster) {
                    System.out.println(p.name);
                }
            }
        }
    }

    /**
     * Prints the shortest path between two people in the social network.
     * @param start the starting person of the path
     * @param end   the ending person of the path
     * @param prev  a map containing the predecessor of each person in the path
     */
    private void printPath(Person start, Person end, Map<Person, Person> prev) {
        List<Person> path = new ArrayList<>();
        for (Person at = end; at != null; at = prev.get(at)) {
            path.add(at);
        }
        Collections.reverse(path);
        System.out.print("Shortest path: ");
        for (Person p : path) {
            System.out.print(p.name + " ");
        }
        System.out.println();
    }

    /**
     * Conducts a breadth-first search (BFS) starting from the given person to find a cluster of connected people.
     * @param start    the starting person for the BFS
     * @param visited  a set to keep track of visited people
     * @param cluster  the list representing the cluster of connected people
     */
    private void bfs(Person start, Set<Person> visited, List<Person> cluster) {
        Queue<Person> queue = new LinkedList<>();
        queue.add(start);
        visited.add(start);

        while (!queue.isEmpty()) {
            Person current = queue.poll();
            cluster.add(current);

            for (Person neighbor : friendships.get(current)) {
                if (!visited.contains(neighbor)) {
                    queue.add(neighbor);
                    visited.add(neighbor);
                }
            }
        }
    }

    /**
     * Counts the number of mutual friends between two people.
     * @param p1 the first person
     * @param p2 the second person
     * @return the number of mutual friends between the two people
     */
    private int countMutualFriends(Person p1, Person p2) {
        Set<Person> friends1 = new HashSet<>(friendships.get(p1));
        Set<Person> friends2 = new HashSet<>(friendships.get(p2));
        friends1.retainAll(friends2);
        return friends1.size();
    }

    /**
     * Counts the number of common hobbies between two people.
     * @param p1 the first person
     * @param p2 the second person
     * @return the number of common hobbies between the two people
     */
    private int countCommonHobbies(Person p1, Person p2) {
        Set<String> hobbies1 = new HashSet<>(p1.hobbies);
        Set<String> hobbies2 = new HashSet<>(p2.hobbies);
        hobbies1.retainAll(hobbies2);
        return hobbies1.size();
    }
}
