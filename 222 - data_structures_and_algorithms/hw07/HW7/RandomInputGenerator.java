import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.HashSet;
import java.util.Locale;
import java.util.Random;
import java.util.Set;

public class RandomInputGenerator {
	private static final String FILE_NAME = "random_input.txt";
	private static final String LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	private static final int MAX_LENGTH = 3;
	private static final int MAX_COMMAND = 50;

	private static final int MAX_PRICE = 1000;
	private static final int MIN_PRICE = 1;
	private static final int MAX_STOCK = 100000000;

	public static void main(String args[]) {
		try (BufferedWriter writer = new BufferedWriter(new FileWriter(FILE_NAME))) {
			Random rand = new Random();
			Set<String> symbols = new HashSet<>( );
			for (int i = 0; i < MAX_COMMAND; i++) {
				int command = rand.nextInt(4);
				String symbol = generateSymbol();

				double price = MIN_PRICE + (MAX_PRICE - MIN_PRICE) * rand.nextDouble();
				long volume = rand.nextInt(MAX_STOCK);
				long marketCap = rand.nextInt(MAX_STOCK);

				switch (command) {
					case 0: // ADD
						writer.write(String.format(Locale.ROOT, "ADD %s %.2f %d %d\n", symbol, price, volume, marketCap));
						break;
					case 1: // REMOVE
						writer.write(String.format("REMOVE %s\n", symbol));
						break;
					case 2: // SEARCH
						writer.write(String.format("SEARCH %s\n", symbol));
						break;
					case 3: // UPDATE
						String newSymbol = generateSymbol();
						writer.write(String.format(Locale.ROOT, "UPDATE %s %s %.2f %d %d\n", symbol, newSymbol, price, volume, marketCap));
						break;
				}
			}
		} catch (Exception ignored) { }
	}

	private static String generateSymbol() {
		Random rand = new Random();
		String symbol = "";
		for (int j = 0; j < MAX_LENGTH; j++) {
			symbol = symbol.concat(String.valueOf(LETTERS.charAt(rand.nextInt(LETTERS.length()))));
		}
		return symbol;
	}
}