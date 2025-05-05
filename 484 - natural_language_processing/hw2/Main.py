'''
pip install datasets
pip install transformers evaluate datasets
pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
pip install wandb
'''
from datasets import load_dataset
from transformers import AutoTokenizer, AutoModelForQuestionAnswering, Trainer, TrainingArguments, DefaultDataCollator
import torch

model_name = "dbmdz/bert-base-turkish-cased"
model = AutoModelForQuestionAnswering.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)


def load_data():
    ds = load_dataset("json", data_files="./data/QAs.json")
    return ds


def preprocess_data(data):
    questions = [q.strip() for q in data["question"]]
    inputs = tokenizer(
        questions,
        data["context"],
        max_length=512,
        truncation="only_second",
        return_offsets_mapping=True,
        padding="max_length",
    )

    offset_mapping = inputs.pop("offset_mapping")
    answers = data["answer"]
    contexts = data["context"]

    start_positions = []
    end_positions = []

    for i, offset in enumerate(offset_mapping):
        answer = answers[i]
        start_char = contexts[i].find(answer)
        end_char = start_char + len(answer)
        sequence_ids = inputs.sequence_ids(i)

        # find the start & end of the context
        idx = 0
        while sequence_ids[idx] != 1:
            idx += 1
        context_start = idx
        while sequence_ids[idx] == 1:
            idx += 1
        context_end = idx - 1

        # if the answer not fully inside the context, label it (0, 0)
        if offset[context_start][0] > end_char or offset[context_end][1] < start_char:
            start_positions.append(0)
            end_positions.append(0)
        else:
            # otherwise it is the start & end token positions
            idx = context_start
            while idx <= context_end and offset[idx][0] <= start_char:
                idx += 1
            start_positions.append(idx - 1)

            idx = context_end
            while idx >= context_start and offset[idx][1] >= end_char:
                idx -= 1
            end_positions.append(idx + 1)

    inputs["start_positions"] = start_positions
    inputs["end_positions"] = end_positions

    return inputs


def get_answer(_question, context):
    # Soru ve bağlamı birleştir
    inputs = tokenizer(
        _question,
        context,
        add_special_tokens=True,
        return_tensors="pt",
        max_length=512,
        truncation=True,
        padding="max_length",
        return_offsets_mapping=True
    )

    offset_mapping = inputs.pop("offset_mapping")

    # Model tahminlerini al
    with torch.no_grad():
        outputs = model(**inputs)

    # En olası başlangıç ve bitiş pozisyonlarını bul
    start_logits = outputs.start_logits[0].cpu().numpy()
    end_logits = outputs.end_logits[0].cpu().numpy()

    # Özel tokenleri ([CLS], [SEP] vb.) hariç tut
    undesired_tokens = (
        tokenizer.get_special_tokens_mask(inputs["input_ids"][0].cpu().tolist(), already_has_special_tokens=True)
    )

    # Özel token pozisyonlarını maskeleme
    start_logits[undesired_tokens] = -float("inf")
    end_logits[undesired_tokens] = -float("inf")

    # En yüksek olasılıklı başlangıç ve bitiş pozisyonlarını bul
    start_index = start_logits.argmax()
    end_index = end_logits[start_index:].argmax() + start_index

    # Token dizisini al
    tokens = tokenizer.convert_ids_to_tokens(inputs["input_ids"][0].cpu())
    _answer = tokens[start_index:end_index + 1]

    # Alt kelimeleri birleştir ve temizle
    _answer = tokenizer.convert_tokens_to_string(_answer)

    # Özel tokenleri temizle
    _answer = _answer.replace("[CLS]", "").replace("[SEP]", "").strip()

    return _answer


dataset = load_data()
tokenized_dataset = dataset.map(preprocess_data, batched=True)
train_test_dataset = tokenized_dataset["train"].train_test_split(test_size=0.2)
tokenized_dataset = {
    "train": train_test_dataset["train"].map(preprocess_data, batched=True),
    "test": train_test_dataset["test"].map(preprocess_data, batched=True)
}

training_args = TrainingArguments(
    output_dir="./out",
    evaluation_strategy="epoch",
    learning_rate=2e-5,
    per_device_train_batch_size=16,
    per_device_eval_batch_size=16,
    num_train_epochs=50,
    weight_decay=0.01,
    save_strategy="epoch",
    load_best_model_at_end=True,
    logging_dir="./out",
    logging_steps=10,
    push_to_hub=False,
)

trainer = Trainer(
    model=model,
    args=training_args,
    train_dataset=tokenized_dataset["train"],
    eval_dataset=tokenized_dataset["test"],
    tokenizer=tokenizer,
    data_collator=DefaultDataCollator(),
    # compute_metrics=compute_metrics,
)

trainer.train()

# Test soruları
test_context = """
Mahkeme kararına dayalı nüfus kaydı değişikliği, diplomanın arkasına şerh düşülüp tarih konularak, Gebze Teknik Üniversitesi Öğrenci İşleri Daire Başkanı imzası ile imzalanır ve ıslak mühür vurulur.
Diploma eki, Öğrenci İşleri Dairesi Başkanı tarafından imzalanır.
Zorunlu ve isteğe bağlı yabancı dil hazırlık sınıflarında geçen öğretim süresi, yukarıda belirtilen sürelere dahil değildir.
"""

test_questions = [
  "Mahkeme kararına dayalı nüfus kaydı değişikliği için hangi belgeler gereklidir?",
  "Diploma ekini kim imzalar?",
  "Zorunlu ve isteğe bağlı yabancı dil hazırlık sınıflarında geçen süre hangi hesaplamalara dahil edilmez?"
]

# Soruları test et
print("\nTest Sonuçları:")
print("-" * 50)
for question in test_questions:
    answer = get_answer(question, test_context)
    print(f"\nSoru: {question}")
    print(f"Cevap: {answer}")

'''
if __name__ == '__main__':
    dataset = load_data()
    tokenized_dataset = dataset.map(preprocess_data, batched=True)
    train_test_dataset = tokenized_dataset["train"].train_test_split(test_size=0.2)
    tokenized_dataset = {
        "train": train_test_dataset["train"].map(preprocess_data, batched=True),
        "test": train_test_dataset["test"].map(preprocess_data, batched=True)
    }

    training_args = TrainingArguments(
        output_dir="/content",
        evaluation_strategy="epoch",
        learning_rate=2e-5,
        per_device_train_batch_size=16,
        per_device_eval_batch_size=16,
        num_train_epochs=50,
        weight_decay=0.01,
        save_strategy="epoch",
        load_best_model_at_end=True,
        logging_dir="/content",
        logging_steps=10,
        push_to_hub=False,
    )

    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=tokenized_dataset["train"],
        eval_dataset=tokenized_dataset["test"],
        tokenizer=tokenizer,
        data_collator=DefaultDataCollator(),
        # compute_metrics=compute_metrics,
    )

    trainer.train()
    trainer.save_model("./out/trained_model")

    test_question = "Hazırlık sınıfı zorunlu mu?"
    inps = tokenizer(test_question, return_tensors="pt")
    outs = model(**inps)

    print("Guessed answer: ", tokenizer.decode(outs.logits.argmax(-1)))'''