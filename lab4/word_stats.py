
average_len = 0
max_len = 0
longest = ''
num_words = 0

with open("words", 'r') as wfile:
    for word in wfile:
        num_words += 1
        average_len += len(word)
        if len(word) > max_len:
            max_len = len(word)
            longest = word
    average_len /= num_words

print(f"number of words: {num_words}\nlongest({max_len}) word: {longest}\naverage length: {average_len}\n")
