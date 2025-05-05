from DataPreparation import *
from NGramCalculation import *
from RandomSentenceGeneration import *

if __name__ == "__main__":
	content = load_data('wiki_00')
	extracted_content = extract_data(content)

	train_data, test_data = split(extracted_content)

	train_syllables, train_chars = preprocess(train_data)
	test_syllables, test_chars = preprocess(test_data)
	# preprocess(train_data)

	# this step creates frequency tables of 1-gram, 2-gram, & 3-gram
	syll_unigrams, syll_bigrams, syll_trigrams = (
		create_ngram_tables(train_syllables)
	)

	char_unigrams, char_bigrams, char_trigrams = (
		create_ngram_tables(train_chars)
	)

	syll_uni_counts = Counter(syll_unigrams)
	syll_bi_counts = Counter(syll_bigrams)
	syll_tri_counts = Counter(syll_trigrams)

	char_uni_counts = Counter(char_unigrams)
	char_bi_counts = Counter(char_bigrams)
	char_tri_counts = Counter(char_trigrams)

	syll_uni_gtp = apply_good_turing_smoothing(syll_uni_counts)
	syll_bi_gtp = apply_good_turing_smoothing(syll_bi_counts)
	syll_tri_gtp = apply_good_turing_smoothing(syll_tri_counts)

	char_uni_gtp = apply_good_turing_smoothing(char_uni_counts)
	char_bi_gtp = apply_good_turing_smoothing(char_bi_counts)
	char_tri_gtp = apply_good_turing_smoothing(char_tri_counts)

	syll_uni_perplexity = calculate_perplexity(test_syllables, syll_uni_gtp, 1)
	syll_bi_perplexity = calculate_perplexity(test_syllables, syll_bi_gtp, 2)
	syll_tri_perplexity = calculate_perplexity(test_syllables, syll_tri_gtp, 3)

	char_uni_perplexity = calculate_perplexity(test_chars, char_uni_gtp, 1)
	char_bi_perplexity = calculate_perplexity(test_chars, char_bi_gtp, 2)
	char_tri_perplexity = calculate_perplexity(test_chars, char_tri_gtp, 3)

	print("\nSyllable-Based Model Perplexities")
	print("------------------------------------------")
	print("1-gram perplexity: ", syll_uni_perplexity)
	print("2-gram perplexity: ", syll_bi_perplexity)
	print("3-gram perplexity: ", syll_tri_perplexity)

	print("\nCharacter-Based Model Perplexities")
	print("------------------------------------------")
	print("1-gram perplexity: ", char_uni_perplexity)
	print("2-gram perplexity: ", char_bi_perplexity)
	print("3-gram perplexity: ", char_tri_perplexity)

	print("\nGenerating Sentences with Syllable-Based Model")
	print("------------------------------------------")
	print(generate_random_sentences(syll_uni_counts, 1, 20))
	print(generate_random_sentences(syll_bi_counts, 2, 20))
	print(generate_random_sentences(syll_tri_counts, 3, 20))

	print("\nGenerating Sentences with Character-Based Model")
	print("------------------------------------------")
	print(generate_random_sentences(char_uni_counts, 1, 20))
	print(generate_random_sentences(char_bi_counts, 2, 20))
	print(generate_random_sentences(char_tri_counts, 3, 20))