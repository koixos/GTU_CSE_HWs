import random


def generate_random_sentences(ngram_model, n, sentence_length):
	sentence = []
	curr_ngram = random.choice(ngram_model.most_common(5))[0]
	sentence.extend(curr_ngram)

	for _ in range(sentence_length - n):
		possible_ngrams = [ngram for ngram, _ in ngram_model.items() if ngram[:n-1] == tuple(curr_ngram[-(n-1):])]
		if not possible_ngrams:
			break

		next_ngram = random.choice(possible_ngrams[:5])
		sentence.append(next_ngram[-1])
		curr_ngram = next_ngram

	return ' '.join(sentence)
