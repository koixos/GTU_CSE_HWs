import java.util.HashMap;
import java.util.Map;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.Iterator;

public class alphabet {
	private Set<Character> english_alphabet = new LinkedHashSet<Character>();
	private Map<Character, Map<Character, Character>> map = new HashMap<Character,  Map<Character, Character>>();

	/**
	 * default constructor of the alphabet class
	 */
	public alphabet() {
		// do not edit this method
		fill_english_alphabet();
		fill_map();
	}

	/**
	 * creates a set of the English alphabet
	 */
	private void fill_english_alphabet() {
		// do not edit this method
		for(char c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ".toCharArray()) {
		    english_alphabet.add(c);
		}
	}
	
	private void fill_map() {
		// You must use the "english_alphabet" variable in this method, to fill the "map" variable.
		// You can define 1 or 2 iterators to iterate through the set items.
		Iterator<Character> colIterator = english_alphabet.iterator();
		Iterator<Character> rowIterator = english_alphabet.iterator();

		while (colIterator.hasNext()) {
			Map<Character, Character> row = new HashMap<>();

			char charValue = rowIterator.next();    //char on the current row is saved
			char currentChar = colIterator.next();  //char on the current column is saved

			for (int i = 0; i < english_alphabet.size(); i++) {
				row.put(charValue, currentChar);    //saved chars is mapped

				//if iterator is at the end, reset it
				if (!colIterator.hasNext()) {
					colIterator = english_alphabet.iterator();
				}

				//if iterator is at the end, reset it
				if (!rowIterator.hasNext()) {
					rowIterator = english_alphabet.iterator();
				}

				//iterate through all row and column values
				currentChar = colIterator.next();
				charValue = rowIterator.next();
			}

			map.put(currentChar, row);
			rowIterator = english_alphabet.iterator();
		}
	}

	/**
	 * prints the map as in form of a table with its rows and columns
	 */
	public void print_map() {
		// do not edit this method
		System.out.println("*** Viegenere Cipher ***\n\n");
		System.out.println("    " + english_alphabet);
		System.out.print("    ------------------------------------------------------------------------------");
		for(Character k: map.keySet()) {
			System.out.print("\n" + k + " | ");
			System.out.print(map.get(k).values());
		}
		System.out.println("\n");
		
	}

	/**
	 * getter method for map variable
	 * @return map
	 */
	public Map get_map() {
		return map;
	}
}