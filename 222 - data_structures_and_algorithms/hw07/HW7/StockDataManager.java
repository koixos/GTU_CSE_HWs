/**
 * A class to manage stock data using an AVL tree for efficient storage and retrieval.
 */
public class StockDataManager {
    private AVLTree avlTree;

    /**
     * Constructs a new StockDataManager with an empty AVL tree.
     */
    public StockDataManager() {
        avlTree = new AVLTree();
    }

    /**
     * Adds a new stock to the AVL tree or updates the existing stock with the given symbol.
     * @param symbol The symbol of the stock.
     * @param price The price of the stock.
     * @param volume The volume of the stock.
     * @param marketCap The market capitalization of the stock.
     */
    public void addOrUpdateStock(String symbol, double price, long volume, long marketCap) {
        Stock existingStock = avlTree.search(symbol);
        if (existingStock != null) {
            existingStock.setPrice(price);
            existingStock.setVolume(volume);
            existingStock.setMarketCap(marketCap);
            System.out.printf("Symbol %s already exists! Updated: ", symbol);
        } else {
            Stock newStock = new Stock(symbol, price, volume, marketCap);
            avlTree.insert(newStock);
            System.out.printf("Symbol %s added!\n", symbol);
        }
    }

    /**
     * Removes the stock with the given symbol from the AVL tree.
     * @param symbol The symbol of the stock to remove.
     */
    public void removeStock(String symbol) {
        if (searchStock(symbol) == null) {
            System.out.printf("Symbol %s not found!\n", symbol);
            return;
        }
        avlTree.delete(symbol);
    }

    /**
     * Searches for a stock with the given symbol in the AVL tree.
     * @param symbol The symbol of the stock to search for.
     * @return The stock with the given symbol, or null if not found.
     */
    public Stock searchStock(String symbol) {
        return avlTree.search(symbol);
    }

    /**
     * Updates the details of a stock with the given symbol.
     * @param symbol The symbol of the stock to update.
     * @param newSymbol The new symbol of the stock.
     * @param newPrice The new price of the stock.
     * @param newVolume The new volume of the stock.
     * @param newMarketCap The new market capitalization of the stock.
     */
    public void updateStock(String symbol, String newSymbol, double newPrice, long newVolume, long newMarketCap) {
        Stock stock = avlTree.search(symbol);
        if (stock != null) {
            stock.setSymbol(newSymbol);
            stock.setPrice(newPrice);
            stock.setVolume(newVolume);
            stock.setMarketCap(newMarketCap);
            System.out.printf("Symbol %s updated - new symbol: %s\n", symbol, newSymbol);
        } else {
            System.out.printf("Symbol %s not found!\n", symbol);
        }
    }

    /**
     * Main method for testing the StockDataManager functionality.
     * @param args Command line arguments.
     */
    public static void main(String[] args) {
        StockDataManager manager = new StockDataManager();
        manager.addOrUpdateStock("AAPL", 150.0, 1000000, 2500000000L);
        manager.addOrUpdateStock("GOOGL", 2800.0, 500000, 1500000000L);
        System.out.println(manager.searchStock("AAPL"));
        manager.removeStock("AAPL");
        System.out.println(manager.searchStock("AAPL"));
    }
}
