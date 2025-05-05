import java.io.File;
import java.util.Scanner;

public final class FileOps {
    private static final int SIZE = 100;

    public static String[] read(String fileName) {
        String[] fileContent = new String[SIZE];
        try {
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            int ind = 0;
            while (scanner.hasNextLine()) {
                fileContent[ind] = scanner.nextLine();
                ++ind;
            }
            scanner.close();
        } catch (Exception e) {
            System.out.println("An error occurred.");
            System.out.printf("Error: %s\n", e);
        }
        return fileContent;
    }

    public static String[] splitLine(String delimiter, String line) {
        String[] words = line.split(delimiter);
        return words;
    }
}