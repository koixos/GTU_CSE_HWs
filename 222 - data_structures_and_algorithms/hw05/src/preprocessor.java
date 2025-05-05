public class preprocessor {
	private String initial_string;
	private String preprocessed_string;

	/**
	 * default constructor for preprocessor objects
	 * @param str initial string
	 */
	public preprocessor(String str) {
		this.initial_string = str;
	}

	public void preprocess() {
		// do not edit this method
		capitalize();
		clean();
	}

	/**
	 * method to capitalize the initial string
	 * assuming the key-set is Turkish
	 */
	private void capitalize() {
		final String accented = "ıüğşöç";   //these letters are excepted from the alphabet
		String capitalizedStr = "";
		for (char c : this.initial_string.toCharArray()) {
			//if the current character is a non-capitalized & not-accented character, capitalize it
			if (accented.indexOf(c) == -1 && c < 65 || c > 90) {
				c -= 32;
			}
			capitalizedStr = capitalizedStr.concat(String.valueOf(c));
		}
		this.initial_string = capitalizedStr;
	}

	/**
	 * method to clean the invalid characters in the initial string
	 */
	private void clean() {
		String cleanedStr = "";
		for (char c : this.initial_string.toCharArray()) {
			//if the current character is not a letter, do not add it to the cleaned string
			if (c >= 65 && c <= 90) {
				cleanedStr = cleanedStr.concat(String.valueOf(c));
			}
		}
		this.preprocessed_string = cleanedStr;
	}

	/**
	 * getter method for preprocessed string
	 * @return preprocessed string
	 */
	public String get_preprocessed_string() {
		return preprocessed_string;
	}
}