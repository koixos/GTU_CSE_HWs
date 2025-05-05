/**
 * A class representing an AVL tree, a self-balancing binary search tree.
 * It supports insertion, deletion, and search operations, ensuring that the tree remains balanced.
 */
public class AVLTree {
    private class Node {
        Stock stock;
        Node left, right;
        int height;

        /**
         * Constructs a new node with the given stock data.
         * @param stock The stock data to store in the node.
         */
        Node(Stock stock) {
            this.stock = stock;
            this.height = 1;
        }
    }

    private Node root;

    /**
     * Inserts a new stock into the AVL tree.
     * @param stock The stock to insert.
     */    public void insert(Stock stock) {
        root = insert(root, stock);
    }

    /**
     * Inserts a new stock into the AVL tree, starting from the given node.
     * @param node The node to start insertion from.
     * @param stock The stock to insert.
     * @return The updated node after insertion.
     */
    private Node insert(Node node, Stock stock) {
        // Implementation of AVL Tree insertion logic
        if (node == null) {
            return new Node(stock);
        }

        if (node.stock.getSymbol().compareTo(stock.getSymbol()) < 0) {
            //if given stock symbol is alphabetically smaller than the current one
            //insert it to the left
            node.left = insert(node.left, stock);
        } else if (node.stock.getSymbol().compareTo(stock.getSymbol()) > 0) {
            //if it is greater than the current one
            //insert it to the right
            node.right = insert(node.right, stock);
        } else {
            //if they are equal, update the data
            node.stock.setPrice(stock.getPrice());
            node.stock.setVolume(stock.getVolume());
            node.stock.setMarketCap(stock.getMarketCap());
            return node;
        }

        node.height = maxHeight(node.left, node.right) + 1;

        int balanceAmount = getBalance(node);
        if (balanceAmount > 1) {
            if (node.left.stock.getSymbol().compareTo(stock.getSymbol()) < 0) {
                return rotateRight(node);
            } else if (node.left.stock.getSymbol().compareTo(stock.getSymbol()) > 0) {
                node.left = rotateLeft(node.left);
                return rotateRight(node);
            }
        } else if (balanceAmount < -1) {
            if (node.right.stock.getSymbol().compareTo(stock.getSymbol()) > 0) {
                return rotateLeft(node);
            } else if (node.right.stock.getSymbol().compareTo(stock.getSymbol()) < 0) {
                node.right = rotateRight(node.right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    /**
     * Deletes a stock with the given symbol from the AVL tree.
     * @param symbol The symbol of the stock to delete.
     */
    public void delete(String symbol) {
        root = delete(root, symbol);
    }

    /**
     * Deletes a stock with the given symbol from the AVL tree, starting from the given node.
     * @param node The node to start deletion from.
     * @param symbol The symbol of the stock to delete.
     * @return The updated node after deletion.
     */
    private Node delete(Node node, String symbol) {
        // Implementation of AVL Tree deletion logic
        if (node == null) {
            return null;
        }

        if (node.stock.getSymbol().compareTo(symbol) < 0) {
            node.left = delete(node.left, symbol);
        } else if (node.stock.getSymbol().compareTo(symbol) > 0) {
            node.right = delete(node.right, symbol);
        } else {
            //node with only one child or no child
            if (node.left == null || node.right == null) {
                Node tempNode = null;
                if (node.left == null) {
                    tempNode = node.right;
                } else {
                    tempNode = node.left;
                }

                //no child case
                if (tempNode == null) {
                    tempNode = node;
                    node = null;
                } else {
                    //one child case
                    node = tempNode;
                }
            } else {
                //node with two children
                Node tempNode = minValueNode(node.right);
                node.stock.setSymbol(tempNode.stock.getSymbol());
                node.right = delete(node.right, tempNode.stock.getSymbol());
            }
        }

        //if tree has only one node, return
        if (node == null) {
            return node;
        }

        node.height = maxHeight(node.left, node.right) + 1;

        int balanceAmount = getBalance(node);
        if (balanceAmount > 1) {
            if (getBalance(node.left) >= 0) {
                //left-left case
                return rotateRight(node);
            } else if (getBalance(node.left) < 0) {
                //left-right case
                node.left = rotateLeft(node.left);
                return rotateRight(node);
            }
        } else if (balanceAmount < -1) {
            if (getBalance(node.right) <= 0) {
                //right-right case
                return rotateLeft(node);
            } else if (getBalance(node.right) > 0) {
                //right-left case
                node.right = rotateRight(node.right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    /**
     * Searches for a stock with the given symbol in the AVL tree.
     * @param symbol The symbol of the stock to search for.
     * @return The stock with the given symbol, or null if not found.
     */
    public Stock search(String symbol) {
        Node result = search(root, symbol);
        return (result != null) ? result.stock : null;
    }

    /**
     * Searches for a stock with the given symbol in the AVL tree, starting from the given node.
     * @param node The node to start search from.
     * @param symbol The symbol of the stock to search for.
     * @return The node containing the stock with the given symbol, or null if not found.
     */
    private Node search(Node node, String symbol) {
        // Implementation of AVL Tree search logic
        while (node != null) {
            if (node.stock.getSymbol().compareTo(symbol) == 0) {
                return node;
            } else if (node.stock.getSymbol().compareTo(symbol) < 0) {
                node = node.left;
            } else {
                node = node.right;
            }
        }
        return null;
    }

    /**
     * Returns the height of the given node.
     * @param node The node whose height to return.
     * @return The height of the node.
     */
    private int height(Node node) {
        if (node == null) {
            return 0;
        } else {
            return node.height;
        }
    }

    /**
     * Returns the balance factor of the given node.
     * @param node The node whose balance factor to return.
     * @return The balance factor of the node.
     */
    private int getBalance(Node node) {
        if (node == null) {
            return 0;
        } else {
            return height(node.left) - height(node.right);
        }
    }

    /**
     * Returns the maximum height of the two given nodes.
     * @param left The left node.
     * @param right The right node.
     * @return The maximum height of the two nodes.
     */
    private int maxHeight(Node left, Node right) {
        return height(left) > height(right) ? height(left) : height(right);
    }

    /**
     * Returns the node with the minimum key value found in the given subtree.
     * @param node The root of the subtree.
     * @return The node with the minimum key value.
     */
    private Node minValueNode(Node node) {
        Node curr = node;
        while (curr.left != null) {
            curr = curr.left;
        }
        return curr;
    }

    /**
     * Performs a right rotation on the given node.
     * @param node The node to rotate.
     * @return The new root of the subtree after rotation.
     */
    private Node rotateRight(Node node) {
        Node left = node.left;
        Node tempNode = left.right;

        left.right = node;
        node.left = tempNode;

        int maxHeight = height(node.left);
        if (height(node.right) > maxHeight) {
            maxHeight = height(node.right);
        }
        node.height = maxHeight + 1;

        maxHeight = height(left.left);
        if (height(left.right) > maxHeight) {
            maxHeight = height(left.right);
        }
        left.height = maxHeight + 1;

        return left;
    }

    /**
     * Performs a left rotation on the given node.
     * @param node The node to rotate.
     * @return The new root of the subtree after rotation.
     */
    private Node rotateLeft(Node node) {
        Node right = node.right;
        Node tempNode = right.left;

        right.left = node;
        node.right = tempNode;

        int maxHeight = height(node.left);
        if (height(node.right) > maxHeight) {
            maxHeight = height(node.right);
        }
        node.height = maxHeight + 1;

        maxHeight = height(right.left);
        if (height(right.right) > maxHeight) {
            maxHeight = height(right.right);
        }
        right.height = maxHeight + 1;

        return right;
    }

    /**
     * Performs an in-order traversal of the AVL tree.
     */
    public void inOrderTraversal() {
        inOrderTraversal(root);
    }

    /**
     * Performs a pre-order traversal of the AVL tree.
     */
    public void preOrderTraversal() {
        preOrderTraversal(root);
    }

    /**
     * Performs a post-order traversal of the AVL tree.
     */
    public void postOrderTraversal() {
        postOrderTraversal(root);
    }

    /**
     * Performs an in-order traversal of the AVL tree, starting from the given node.
     * @param node The node to start traversal from.
     */
    private void inOrderTraversal(Node node) {
        if (node != null) {
            inOrderTraversal(node.left);
            System.out.println(node.stock);
            inOrderTraversal(node.right);
        }
    }

    /**
     * Performs a pre-order traversal of the AVL tree, starting from the given node.
     * @param node The node to start traversal from.
     */
    private void preOrderTraversal(Node node) {
        if (node != null) {
            System.out.println(node.stock);
            preOrderTraversal(node.left);
            preOrderTraversal(node.right);
        }
    }

    /**
     * Performs a post-order traversal of the AVL tree, starting from the given node.
     * @param node The node to start traversal from.
     */
    private void postOrderTraversal(Node node) {
        if (node != null) {
            postOrderTraversal(node.left);
            postOrderTraversal(node.right);
            System.out.println(node.stock);
        }
    }
}
