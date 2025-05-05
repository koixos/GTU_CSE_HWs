import javax.swing.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * The main class for managing and analyzing stock data.
 * This class reads commands from an input file, processes them, and performs a performance analysis on stock data operations.
 */
public class Main {
    /**
     * The main method which serves as the entry point of the application.
     * @param args Command line arguments. Expects a single argument specifying the input file path.
     */
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java Main <input_file>");
            return;
        }

        String inputFile = args[0];
        StockDataManager manager = new StockDataManager();

        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                processCommand(line, manager);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        // Perform & visualize a simple performance analysis
        performPerformanceAnalysis(manager, 1000);
    }

    /**
     * Processes a single command from the input file and performs the corresponding operation on the stock data manager.
     * @param line The command line to process.
     * @param manager The StockDataManager instance to operate on.
     */
    private static void processCommand(String line, StockDataManager manager) {
        String[] tokens = line.split(" ");
        String command = tokens[0];

        switch (command) {
            case "ADD":
                manager.addOrUpdateStock(tokens[1], Double.parseDouble(tokens[2]), Long.parseLong(tokens[3]), Long.parseLong(tokens[4]));
                break;
            case "REMOVE":
                manager.removeStock(tokens[1]);
                break;
            case "SEARCH":
                Stock stock = manager.searchStock(tokens[1]);
                if (stock != null) {
                    System.out.println(stock);
                } else {
                    System.out.println("Stock not found: " + tokens[1]);
                }
                break;
            case "UPDATE":
                manager.updateStock(tokens[1], tokens[2], Double.parseDouble(tokens[3]), Long.parseLong(tokens[4]), Long.parseLong(tokens[5]));
                break;
            default:
                System.out.println("Unknown command: " + command);
                break;
        }
    }

    /**
     * Performs a simple performance analysis on the stock data manager and visualizes the results.
     * @param manager The StockDataManager instance to analyze.
     * @param size The number of operations to perform for each test.
     */
    private static void performPerformanceAnalysis(StockDataManager manager, int size) {
        long startTime, endTime;

        // Measure time for ADD operation
        startTime = System.nanoTime();
        for (int i = 0; i < size; i++) {
            manager.addOrUpdateStock("SYM" + i, Math.random() * 100, (long) (Math.random() * 1000000), (long) (Math.random() * 1000000000));
        }
        endTime = System.nanoTime();
        long addAvrg = (endTime - startTime) / size;

        // Measure time for SEARCH operation
        startTime = System.nanoTime();
        for (int i = 0; i < size; i++) {
            Stock stock = manager.searchStock("SYM" + i);
            if (stock != null) {
                System.out.println(stock);
            } else {
                System.out.println("Stock not found: SYM" + i);
            }
        }
        endTime = System.nanoTime();
        long searchAvrg = (endTime - startTime) / size;

        // Measure time for UPDATE operation
        startTime = System.nanoTime();
        for (int i = 0; i < size; i++) {
            manager.updateStock(("SYM" + i), ("SYM" + (i+2)), 15, 12, 23);
        }
        endTime = System.nanoTime();
        long updateAvrg = (endTime - startTime) / size;

        // Measure time for REMOVE operation
        startTime = System.nanoTime();
        for (int i = 0; i < size; i++) {
            manager.removeStock("SYM" + i);
        }
        endTime = System.nanoTime();
        long removeAvrg = (endTime - startTime) / size;

        System.out.println("Average ADD time: " + addAvrg + " ns");
        System.out.println("Average SEARCH time: " + searchAvrg + " ns");
        System.out.println("Average SEARCH time: " + updateAvrg + " ns");
        System.out.println("Average REMOVE time: " + removeAvrg + " ns");

        //visualize the analysis
        visualize("scatter", "ADD", size, addAvrg);
        visualize("scatter", "SEARCH", size, searchAvrg);
        visualize("scatter", "UPDATE", size, searchAvrg);
        visualize("scatter", "REMOVE", size, removeAvrg);
    }

    /**
     * Visualizes the performance analysis results using a GUI.
     * @param plotType The type of plot to create (e.g., "scatter").
     * @param title The title of the plot.
     * @param numberTrials The number of trials performed.
     * @param averageTime The average time taken for the operation.
     */
    private static void visualize(String plotType, String title, int numberTrials, long averageTime) {
        SwingUtilities.invokeLater(() -> {
            GUIVisualization frame = new GUIVisualization(plotType, title, numberTrials, averageTime); // Create a new instance of GUIVisualization
            frame.setVisible(true); // Make the frame visible
        });
    }
}
