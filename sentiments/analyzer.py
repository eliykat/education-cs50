import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.poswords = []
        self.negwords = []

        fp = open(positives, "r")
        for line in fp:
            if line[:1] == ";" or line[:1] == "\n":
                continue
            else:
                self.poswords.append(line[:-1]) #note that we use line[:-1] to cut off the \n character at the end of each line
        fp.close()

        fp = open(negatives, "r")
        for line in fp:
            if line[:1] == ";" or line[:1] == "\n":
                continue
            else:
                self.negwords.append(line[:-1])
        fp.close

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        #Use nltk tokenizer to split tweet into individual words
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        count = 0

        for word in tokens:
            if word in self.poswords:
                count += 1
            elif word in self.negwords:
                count -= 1

        return count

