from nltk import ngrams
from collections import Counter
import math


def create_ngram_tables(tokens):
	unigrams = list(ngrams(tokens, 1))
	bigrams = list(ngrams(tokens, 2))
	trigrams = list(ngrams(tokens, 3))
	return unigrams, bigrams, trigrams


def apply_good_turing_smoothing(frequency_counts):
	freq_of_freqs = Counter(frequency_counts.values())

	smoothed_counts = {}
	for count, freq in freq_of_freqs.items():
		next_count = count + 1
		if next_count in freq_of_freqs:
			smoothed_count = (next_count * freq_of_freqs[next_count]) / freq
			smoothed_counts[count] = smoothed_count
		else:
			smoothed_counts[count] =  count

	zero_freq_prob = freq_of_freqs.get(1, 0) / sum(frequency_counts.values())

	total_smoothed = sum(smoothed_counts.values())
	smoothed_probs = {item: smoothed_counts[count] / total_smoothed for item, count in frequency_counts.items()}
	smoothed_probs["zero_frequency"] = zero_freq_prob

	return smoothed_probs


def calculate_perplexity(test_data, ngram_probs, n):
	log_prob_sum = 0
	N = len(test_data) - n + 1

	for i in range(N):
		ngram = tuple(test_data[i:i + n])
		prob = ngram_probs.get(ngram, 1e-10)

		log_prob_sum += math.log(prob, 2)

		perplexity = 2 ** (-log_prob_sum / N)
		return perplexity
