from analyzer import Analyzer

test = Analyzer("positive-words.txt", "negative-words.txt")
count = test.analyze("There are too many fun games to play on the Switch, it's turning into a real life survival horror, time management sim")

print("Sentiment score: {}".format(count))