import java.util.Map;

public class encryptor {
	private Map<Character, Map<Character, Character>> map;
	private String key;
	private String keystream = "";
	private String plain_text;
	private String cipher_text = "";

	/**
	 * default constructor for encryptor objects
	 * @param _map Vigenere table
	 * @param _key given key
	 * @param text given text
	 */
	public encryptor(Map<Character, Map<Character, Character>> _map, String _key, String text) {
		this.map = _map;
		this.key = _key;
		this.plain_text = text;
	}
	
	public void encrypt() {
		// do not edit this method
		generate_keystream();
		generate_cipher_text();
	}

	/**
	 * method to generate the key-stream with the provided key
	 */
	private void generate_keystream() {
		int keyLen = this.key.length();
		int textLen = this.plain_text.length();

		//same algorithm with decrypting class, please check "decrpytor" for the comments
		int i;
		if (keyLen < textLen) {
			int occurrence = textLen / keyLen;
			int remaining = textLen % keyLen;

			for (i = 0; i < occurrence; i++) {
				this.keystream = this.keystream.concat(this.key);
			}

			for (i = 0; i < remaining; i++) {
				this.keystream = this.keystream.concat(String.valueOf(this.key.charAt(i)));
			}
		} else if (keyLen > textLen) {
			for (i = 0; i < textLen; i++) {
				this.keystream = this.keystream.concat(String.valueOf(this.key.charAt(i)));
			}
		} else {
			this.keystream = this.key;
		}
	}

	/**
	 * method to encode the given string with the provided key-stream
	 */
	private void generate_cipher_text() {
		String encodedText = "";
		//for each character of the plain-text, the loop will be executed
		for (int i = 0; i < this.plain_text.length(); i++) {
			for (char c : this.map.keySet()) {
				//if there is a matching between the character and a key of the key-set,
				// take its key-value pair, and add the value to the encoded text
				if (this.plain_text.charAt(i) == c) {
					Map<Character, Character> keyValuePairs = this.map.get(this.plain_text.charAt(i));
					encodedText = encodedText.concat(String.valueOf(keyValuePairs.get(this.keystream.charAt(i))));
				}
			}
		}
		this.cipher_text = encodedText;
	}

	/**
	 * getter method for key stream
	 * @return key stream
	 */
	public String get_keystream() {
		return keystream;
	}

	/**
	 * getter method for cipher-text
	 * @return cipher-text
	 */
	public String get_cipher_text() {
		return cipher_text;
	}
}
