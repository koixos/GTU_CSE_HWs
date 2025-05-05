import java.util.Scanner;

public class Main {
    private static final int SIZE = 100;

    private static int toInt(String word) {
        int number = 0;
        for (int i = 0; i < word.length(); i++) {
            int pow = 1;
            for (int j = i; j < word.length() - 1; j++)
                pow *= 10;
            number += (word.charAt(i) - 48) * pow;
        }
        return number;
    }

    private static boolean validateInput(String[] words) {
        for (String word : words)
            if (word.isEmpty())
                return false;
        return true;
    }

    private static boolean validateId(int id,  CorporateCustomer[] corporateCustomers, RetailCustomer[] retailCustomers, Operator[] operators) {
        if (!validateInt(id))
            return false;

        for (Operator operator : operators)
            if (operator != null && operator.getId() == id)
                return false;

        for (CorporateCustomer corporateCustomer : corporateCustomers)
            if (corporateCustomer != null && corporateCustomer.getId() == id)
                return false;

        for (RetailCustomer retailCustomer : retailCustomers)
            if (retailCustomer != null && retailCustomer.getId() == id)
                return false;

        return true;
    }

    private static boolean validateInt(int number) {
        return number > 0 && number < 2147483647;
    }

    private static boolean searchAndFind(int id, CorporateCustomer[] corporateCustomers, RetailCustomer[] retailCustomers, Operator[] operators) {
        if (id <= 0)
            return false;

        for (Operator operator : operators) {
            if (operator != null && operator.getId() == id) {
                operator.printOperator();
                return true;
            }
        }

        for (RetailCustomer retailCustomer : retailCustomers) {
            if (retailCustomer != null && retailCustomer.getId() == id) {
                retailCustomer.printCustomer(true);
                return true;
            }
        }

        for (CorporateCustomer corporateCustomer : corporateCustomers) {
            if (corporateCustomer != null && corporateCustomer.getId() == id) {
                corporateCustomer.printCustomer(true);
                return true;
            }
        }

        return false;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String[] content = FileOps.read("content.txt");

        Order[] orders = new Order[SIZE];
        Operator[] operators = new Operator[SIZE];
        RetailCustomer[] retailCustomers = new RetailCustomer[SIZE];
        CorporateCustomer[] corporateCustomers = new CorporateCustomer[SIZE];

        int ordCtr = 0,
            opCtr = 0,
            rCustCtr = 0,
            cCustCtr = 0;

        for (String line : content) {
            if (line != null) {
                String[] words = FileOps.splitLine(";", line);
                boolean isValid = validateInput(words);
                if (isValid) {
                    switch (words[0]) {
                        case "order" -> {
                            if (words.length == 6) {
                                int count = toInt(words[2]),
                                    totalPrice = toInt(words[3]),
                                    status = toInt(words[4]),
                                    customerId = toInt(words[5]);
                                if (!validateInt(count) || !validateInt(totalPrice) || !validateInt(customerId))
                                    break;
                                orders[ordCtr] = new Order(words[1], count, totalPrice, status, customerId);
                                ordCtr++;
                            }
                        }
                        case "operator" -> {
                            if (words.length == 7) {
                                int id = toInt(words[5]),
                                    wage = toInt(words[6]);
                                if (!validateInt(id) || !validateInt(wage))
                                    break;
                                if (!validateId(id, corporateCustomers, retailCustomers, operators))
                                    break;
                                operators[opCtr] = new Operator(words[1], words[2], words[3], words[4], id, wage);
                                opCtr++;
                            }
                        }
                        case "retail_customer" -> {
                            if (words.length == 7) {
                                int id = toInt(words[5]),
                                    opId = toInt(words[6]);
                                if (!validateInt(id) || !validateInt(opId))
                                    break;
                                if (!validateId(id, corporateCustomers, retailCustomers, operators))
                                    break;
                                retailCustomers[rCustCtr] = new RetailCustomer(words[1], words[2], words[3], words[4], id, opId);
                                rCustCtr++;
                            }
                        }
                        case "corporate_customer" -> {
                            if (words.length == 8) {
                                int id = toInt(words[5]),
                                    opId = toInt(words[6]);
                                if (!validateInt(id) || !validateInt(opId))
                                    break;
                                if (!validateId(id, corporateCustomers, retailCustomers, operators))
                                    break;
                                corporateCustomers[cCustCtr] = new CorporateCustomer(words[1], words[2], words[3], words[4], toInt(words[5]), toInt(words[6]), words[7]);
                                cCustCtr++;
                            }
                        }
                    }
                }
            }
        }

        for (RetailCustomer retailCustomer : retailCustomers)
            if (retailCustomer != null)
                retailCustomer.defineOrders(orders);

        for (CorporateCustomer corporateCustomer : corporateCustomers)
            if (corporateCustomer != null)
                corporateCustomer.defineOrders(orders);

        for (Operator operator : operators) {
            if (operator != null) {
                operator.defineCustomers(retailCustomers);
                operator.defineCustomers(corporateCustomers);
            }
        }

        System.out.print("Please enter your ID: ");
        String id = scanner.nextLine();

        boolean isFound = searchAndFind(toInt(id), corporateCustomers, retailCustomers, operators);

        if (!isFound) {
            System.out.printf("There is no customer/operator with ID %s.\nPlease try again.\n", id);
            return;
        }
    }
}