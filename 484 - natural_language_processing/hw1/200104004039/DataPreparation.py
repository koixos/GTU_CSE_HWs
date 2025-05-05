from sklearn.model_selection import train_test_split
import trnlp.helper
from trnlp import *
import re
import random


def load_data(file_path):
	"""Opens the file and returns the content of it."""
	with open(file_path, 'r', encoding='utf-8') as file:
		content = file.read()
	return content


def extract_data(content):
	"""Extracts each article from the given content."""
	content = re.findall(r'<doc id="(.*?)" url="(.*?)" title="(.*?)">(.*?)</doc>', content, re.DOTALL)
	return content


def split(data):
	sample_size = int(0.7 * len(data))
	sample_data = random.sample(data, sample_size)

	train_data, test_data = train_test_split(sample_data, test_size=0.05, random_state=42)
	return train_data, test_data


def tokenize_sentences(data):
	obj = TrnlpToken()
	obj.settext(data)
	return obj.phrasetoken


def tokenize_syllables(word):
	return trnlp.helper.syllabification(word)


def clean(data):
	data = re.sub(r'http\S+', '', data)  # remove URLs
	data = re.sub(r'\\n|\\', '', data)  # remove escape chars
	return data


def preprocess(data):
	syllable_list = []
	char_list = []

	if isinstance(data, list):
		data = " ".join(map(str, data))

	clean_data = clean(data)

	words = word_token(clean_data, numbers=False)
	for word in words:
		syllables = trnlp.helper.syllabification(word)
		syllable_list.extend(syllables)

	normalized_data = trnlp.helper.repc(trnlp.helper.to_lower(clean_data))
	for char in normalized_data:
		char_list.append(char)

	return syllable_list, char_list
