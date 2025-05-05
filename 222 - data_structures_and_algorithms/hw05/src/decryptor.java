import java.util.Map;
import java.util.Iterator;

public class decryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text = "";
	private String cipher_text;

	/**
	 * default constructor for decryptor objects
	 * @param _map Vigenere table
	 * @param _key given key
	 * @param text given text
	 */
	public decryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		this.map = _map;
		this.key = _key;
		this.cipher_text = text;
	}

	public void decrypt() {
		// do not edit this method
		generate_keystream();
		generate_plain_text();
	}

	/**
	 * method to generate the key-stream with the provided key
	 */
	private void generate_keystream() {
		int keyLen = this.key.length();
		int textLen = this.cipher_text.length();

		int i;
		if (keyLen < textLen) {
			int occurrence = textLen / keyLen;  //calculating how many times the key will be copied
			int remaining = textLen % keyLen;   //calculating which characters will be repeated

			for (i = 0; i < occurrence; i++) {
				this.keystream = this.keystream.concat(this.key);
			}

			for (i = 0; i < remaining; i++) {
				this.keystream = this.keystream.concat(String.valueOf(this.key.charAt(i)));
			}
		} else if (keyLen > textLen) {
			//if key length is bigger than the text's, just take as the amount of text length
			for (i = 0; i < textLen; i++) {
				this.keystream = this.keystream.concat(String.valueOf(this.key.charAt(i)));
			}
		} else {
			this.keystream = this.key;
		}
	}

	/**
	 * method to decode the given string with the provided key-stream
	 */
	private void generate_plain_text() {
		// You must use map.get(x).keySet() with an iterator in this method
		String decodedStr = "";
		for (int i = 0; i < this.keystream.length(); i++) {
			//for each character of the key-stream, this loop will be executed
			//each key-stream character represents a column in the table
			char c = this.keystream.charAt(i);
			//the key-set function gives the column values of each character
			//to find the plain-text character, we must iterate this set as if it is a row
			//when we found that the key of the row and
			// the character on the current index of the cipher-text is mapped (as a value),
			// we add that key to the plain-text
			Iterator<Character> rowIterator = this.map.get(c).keySet().iterator();
			while (rowIterator.hasNext()) {
				char key = rowIterator.next();
				if (this.map.get(c).get(key) == this.cipher_text.charAt(i)) {
					decodedStr = decodedStr.concat(String.valueOf(key));
				}
			}
		}
		this.plain_text = decodedStr;
	}

	/**
	 * getter method for key stream
	 * @return key stream
	 */
	public String get_keystream() {
		return keystream;
	}

	/**
	 * getter method for plain-text
	 * @return plain-text
	 */
	public String get_plain_text() {
		return plain_text;
	}
}
